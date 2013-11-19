#include "RoadGraph.h"
#include "RoadStyle.h"
#include "Util.h"
#include <qfile.h>
#include <qdom.h>
#include <qset.h>
#include <qmap.h>
#include <QGLWidget>

#define _USE_MATH_DEFINES
#include <math.h>

#define SQR(x)		((x) * (x))

RoadGraph::RoadGraph(RoadStyle* mainWin) {
	this->mainWin = mainWin;

	selectedEdge = NULL;
}

RoadGraph::~RoadGraph() {
}

BGLGraph& RoadGraph::getGraph() {
	return graph;
}

void RoadGraph::setRange(MapRange ranges) {
	this->ranges = ranges;
}

void RoadGraph::generateMesh() {
	int count = 0;
	vertices.clear();

	Vertex v;

	// map area
	BBox2D bbox = ranges.getBBox();
	QVector2D minCorner = Util::projLatLonToMeter(bbox.minCorner(), bbox.getCenter());
	QVector2D maxCorner = Util::projLatLonToMeter(bbox.maxCorner(), bbox.getCenter());

	float cellLength = 1000.0f;

	int cntX = 0;
	for (float x = minCorner.x(); x <= maxCorner.x() - cellLength; x += cellLength, cntX++) {
		float x2 = x + cellLength;
		if (x2 > maxCorner.x()) x2 = maxCorner.x();

		int cntY = 0;
		for (float y = minCorner.y(); y <= maxCorner.y() - cellLength; y += cellLength, cntY++) {
			float y2 = y + cellLength;
			if (y2 > maxCorner.y()) y2 = maxCorner.y();

			if ((cntX + cntY) % 2 == 0) {
				v.color[0] = 0.0f;
				v.color[1] = 0.0f;
				v.color[2] = 0.0f;
			} else {
				v.color[0] = 0.2f;
				v.color[1] = 0.2f;
				v.color[2] = 0.2f;
			}

			BBox2D area(QVector2D(x, y), QVector2D(x2, y2));

			for (int i = 0; i < ranges.size(); i++) {
				QVector2D c0 = Util::projLatLonToMeter(ranges.get(i).minCorner(), ranges.getBBox().getCenter());
				QVector2D c1 = Util::projLatLonToMeter(ranges.get(i).maxCorner(), ranges.getBBox().getCenter());
				BBox2D range(c0, c1);

				BBox2D b = area.intersect(range);

				v.location[0] = b.minCorner().x();
				v.location[1] = b.minCorner().y();
				v.location[2] = -100.0f;
				v.normal[0] = 0.0f;
				v.normal[1] = 0.0f;
				v.normal[2] = 1.0f;
				vertices.push_back(v);

				v.location[0] = b.maxCorner().x();
				vertices.push_back(v);

				v.location[1] = b.maxCorner().y();
				vertices.push_back(v);

				v.location[0] = b.minCorner().x();
				v.location[1] = b.minCorner().y();
				vertices.push_back(v);

				v.location[0] = b.maxCorner().x();
				v.location[1] = b.maxCorner().y();
				vertices.push_back(v);

				v.location[0] = b.minCorner().x();
				vertices.push_back(v);
			}
		}
	}

	// road vertex
	/*
	//v.color[0] = 1.0f;
	//v.color[1] = 0.0f;
	//v.color[2] = 0.0f;
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi) {
		RoadVertex* vertex = graph[*vi];

		v.color[0] = (float)(rand() % 255) / 255.0f;
		v.color[1] = (float)(rand() % 255) / 255.0f;
		v.color[2] = (float)(rand() % 255) / 255.0f;

		v.location[0] = vertex->getPt().x() - 3.0f;
		v.location[1] = vertex->getPt().y() - 3.0f;
		v.location[2] = 10.0f;
		vertices.push_back(v);

		v.location[0] = vertex->getPt().x() + 3.0f;
		v.location[1] = vertex->getPt().y() - 3.0f;
		vertices.push_back(v);

		v.location[0] = vertex->getPt().x() + 3.0f;
		v.location[1] = vertex->getPt().y() + 3.0f;
		vertices.push_back(v);

		v.location[0] = vertex->getPt().x() - 3.0f;
		v.location[1] = vertex->getPt().y() - 3.0f;
		vertices.push_back(v);

		v.location[0] = vertex->getPt().x() + 3.0f;
		v.location[1] = vertex->getPt().y() + 3.0f;
		vertices.push_back(v);

		v.location[0] = vertex->getPt().x() - 3.0f;
		v.location[1] = vertex->getPt().y() + 3.0f;
		vertices.push_back(v);
	}
	*/

	// road edge
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei) {
		RoadEdge* edge = graph[*ei];
		int num = edge->getPolyLine().size();
		if (num <= 1) continue;

		// filter the hidden roads
		if (!mainWin->getAttributes()->getBool("showHighways")) {
			if (edge->getType() == 3) continue;
		}
		if (!mainWin->getAttributes()->getBool("showAvenues")) {
			if (edge->getType() == 2) continue;
		}
		if (!mainWin->getAttributes()->getBool("showStreets")) {
			if (edge->getType() == 1) continue;
		}

		for (int i = 0; i < num - 1; ++i) {
			QVector3D pt1 = edge->getPolyLine()[i];
			QVector3D pt2 = edge->getPolyLine()[i + 1];
			QVector3D vec = pt2 - pt1;
			vec = QVector3D(-vec.y(), vec.x(), 0.0f);
			vec.normalize();

			QVector3D p0 = pt1 + vec * edge->getWidth() / 2.0f;
			QVector3D p1 = pt1 - vec * edge->getWidth() / 2.0f;
			QVector3D p2 = pt2 - vec * edge->getWidth() / 2.0f;
			QVector3D p3 = pt2 + vec * edge->getWidth() / 2.0f;
			//QVector3D normal = ucore::Util::calculateNormal(p0, p1, p2);

			switch (edge->getType()) {
			case 3:
				v.color[0] = 0.53725f;
				v.color[1] = 0.64313f;
				v.color[2] = 0.79607f;
				v.location[2] = mainWin->getAttributes()->getFloat("HighwayHeight");
				break;
			case 2: // avenue
				v.color[0] = 0.86274f;
				v.color[1] = 0.61960f;
				v.color[2] = 0.61960f;
				v.location[2] = mainWin->getAttributes()->getFloat("AvenueHeight");
				break;
			case 1: // street
				v.color[0] = 1.0f;
				v.color[1] = 1.0f;
				v.color[2] = 1.0f;
				v.location[2] = mainWin->getAttributes()->getFloat("StreetHeight");
				break;
			default:
				v.color[0] = 1.0f;
				v.color[1] = 1.0f;
				v.color[2] = 1.0f;
				break;
			}

			if (edge == selectedEdge) {
				v.color[0] = 1.0f;
				v.color[1] = 0.0f;
				v.color[2] = 0.0f;
			}

			v.location[0] = p0.x();
			v.location[1] = p0.y();
			vertices.push_back(v);

			v.location[0] = p1.x();
			v.location[1] = p1.y();
			vertices.push_back(v);

			v.location[0] = p2.x();
			v.location[1] = p2.y();
			vertices.push_back(v);

			v.location[0] = p0.x();
			v.location[1] = p0.y();
			vertices.push_back(v);

			v.location[0] = p2.x();
			v.location[1] = p2.y();
			vertices.push_back(v);

			v.location[0] = p3.x();
			v.location[1] = p3.y();
			vertices.push_back(v);
		}

		//if (++count == 8) break;
	}

	modified = false;
}

RoadVertexDesc RoadGraph::addVertex(RoadVertex* vertex) {
	RoadVertexDesc desc = boost::add_vertex(graph);
	graph[desc] = vertex;
	//bbox.addPoint(vertex->getPt());

	modified = true;

	return desc;
}

RoadVertex* RoadGraph::getVertex(RoadVertexDesc desc) {
	return graph[desc];
}

void RoadGraph::removeVertex(RoadVertexDesc desc) {
	RoadOutEdgeIter ei, ei_end;
	for (boost::tie(ei, ei_end) = out_edges(desc, graph); ei != ei_end; ++ei) {
		boost::remove_edge(*ei, graph);
	}

	boost::remove_vertex(desc, graph);
}

bool RoadGraph::reduceVertex(RoadVertexDesc desc) {
	int count = 0;
	RoadVertexDesc vd[2];
	RoadEdgeDesc ed[2];
	RoadEdge* edges[2];

	RoadOutEdgeIter ei, ei_end;
	for (boost::tie(ei, ei_end) = out_edges(desc, graph); ei != ei_end; ++ei, ++count) {
		vd[count] = boost::target(*ei, graph);
		ed[count] = *ei;
		edges[count] = graph[*ei];
	}

	if (count != 2) {
		int k = 0;
	}

	if (edges[0]->getType() != edges[1]->getType()) return false;

	RoadEdge* new_edge = new RoadEdge(edges[0]->isOneWay(), edges[0]->getNumLanes(), edges[0]->getType());
	edges[0]->startFrom(graph[vd[0]]->getPt());
	edges[1]->startFrom(graph[desc]->getPt());
	
	for (int i = 0; i < edges[0]->getPolyLine().size(); i++) {
		new_edge->addPoint(edges[0]->getPolyLine()[i]);
	}
	for (int i = 1; i < edges[1]->getPolyLine().size(); i++) {
		new_edge->addPoint(edges[1]->getPolyLine()[i]);
	}
	addEdge(vd[0], vd[1], new_edge);

	boost::remove_edge(vd[0], desc, graph);
	boost::remove_edge(vd[1], desc, graph);
	delete edges[0];
	if (edges[1] != edges[0]) delete edges[1];

	RoadVertex* vertex = graph[desc];
	boost::remove_vertex(desc, graph);
	delete vertex;

	return true;
}

RoadEdgeDesc RoadGraph::addEdge(RoadVertexDesc v1, RoadVertexDesc v2, RoadEdge* edge) {
	std::pair<RoadEdgeDesc, bool> edge_pair = boost::add_edge(v1, v2, graph);
	graph[edge_pair.first] = edge;
	modified = true;

	return edge_pair.first;
}

/*
AABBox& RoadGraph::getBBox() {
	return bbox;
}
*/

MapRange& RoadGraph::getRanges() {
	return ranges;
}

bool RoadGraph::getModified() {
	return modified;
}

void RoadGraph::setModified() {
	modified = true;
}

void RoadGraph::clear() {
	RoadVertexIter vi, vend;
	for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi) {
		RoadVertex* v = graph[*vi];
		delete v;
	}

	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei) {
		RoadEdge* edge = graph[*ei];
		delete edge;
	}

	graph.clear();
	//bbox.clear();

	modified = true;
}

void RoadGraph::reduce() {
	// reduce the graph by removing the vertices which have two outing edges.
	RoadVertexIter vi, vend;
	bool deleted = false;
	do {
		deleted = false;
		for (boost::tie(vi, vend) = boost::vertices(graph); vi != vend; ++vi) {
			RoadVertex* v = graph[*vi];

			int degree = boost::degree(*vi, graph);
			if (degree == 2) {
				if (reduceVertex(*vi)) {
					deleted = true;
					break;
				}
			}
		}
	} while (deleted);
}

/**
 * Select a road segment based on the specified position.
 *
 * @param pos		the specified position
 * @return			the selected road edge
 */
RoadEdge* RoadGraph::select(const QVector3D &pos) {
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei) {
		if (graph[*ei]->containsPoint(pos)) {
			selectedEdge = graph[*ei];
			modified = true;
			return selectedEdge;
		}
	}

	return NULL;
}

Vertex RoadGraph::getMesh(unsigned int index) {
	return vertices[index];
}

unsigned int RoadGraph::getNumMeshes() {
	return vertices.size();
}

void RoadGraph::load(FILE* fp) {
	clear();

	QMap<uint, RoadVertexDesc> idToDesc;

	unsigned int nVertices;
	fread(&nVertices, sizeof(unsigned int), 1, fp);

	// 頂点情報を読み込む
	for (int i = 0; i < nVertices; i++) {
		RoadVertexDesc id;
		float x, y;
		fread(&id, sizeof(RoadVertexDesc), 1, fp);
		fread(&x, sizeof(float), 1, fp);
		fread(&y, sizeof(float), 1, fp);

		RoadVertex* vertex = new RoadVertex(QVector3D(x, y, 0.0f));
		//vertex->pt.setX(x);
		//vertex->pt.setY(y);

		RoadVertexDesc desc = boost::add_vertex(graph);
		graph[desc] = vertex;

		idToDesc[id] = desc;
	}

	unsigned int nEdges;
	fread(&nEdges, sizeof(unsigned int), 1, fp);

	// エッジ情報を読み込む
	for (int i = 0; i < nEdges; i++) {
		RoadEdge* edge = new RoadEdge();

		RoadVertexDesc id1, id2;
		fread(&id1, sizeof(RoadVertexDesc), 1, fp);
		fread(&id2, sizeof(RoadVertexDesc), 1, fp);

		RoadVertexDesc src = idToDesc[id1];
		RoadVertexDesc tgt = idToDesc[id2];

		fread(&edge->type, sizeof(unsigned int), 1, fp);
		fread(&edge->numLanes, sizeof(unsigned int), 1, fp);
		fread(&edge->oneWay, sizeof(unsigned int), 1, fp);

		unsigned int nPoints;
		fread(&nPoints, sizeof(unsigned int), 1, fp);

		for (int j = 0; j < nPoints; j++) {
			float x, y;
			fread(&x, sizeof(float), 1, fp);
			fread(&y, sizeof(float), 1, fp);
			edge->addPoint(QVector3D(x, y, 0.0f));
		}

		std::pair<RoadEdgeDesc, bool> edge_pair = boost::add_edge(src, tgt, graph);
		graph[edge_pair.first] = edge;
	}

	modified = true;
}