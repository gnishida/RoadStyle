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

RoadGraph::RoadGraph() {
	selectedEdge = NULL;
	widthPerLane = 2.0f;
}

RoadGraph::~RoadGraph() {
}

void RoadGraph::generateMesh(bool showHighways, bool showAvenues, bool showStreets) {
	int count = 0;
	vertices.clear();

	Vertex v;

	// road edge
	RoadEdgeIter ei, eend;
	for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei) {
		RoadEdge* edge = graph[*ei];
		int num = edge->getPolyLine().size();
		if (num <= 1) continue;

		// filter the hidden roads
		if (!showHighways) {
			if (edge->type == 3) continue;
		}
		if (!showAvenues) {
			if (edge->type == 2) continue;
		}
		if (!showStreets) {
			if (edge->type == 1) continue;
		}

		// draw the border with darker color
		for (int i = 0; i < num - 1; ++i) {
			QVector2D pt1 = edge->polyLine[i];
			QVector2D pt2 = edge->polyLine[i + 1];
			QVector2D vec = pt2 - pt1;
			vec = QVector2D(-vec.y(), vec.x());
			vec.normalize();

			QVector2D p0 = pt1 + vec * edge->getWidth(widthPerLane) * 0.6f;
			QVector2D p1 = pt1 - vec * edge->getWidth(widthPerLane) * 0.6f;
			QVector2D p2 = pt2 - vec * edge->getWidth(widthPerLane) * 0.6f;
			QVector2D p3 = pt2 + vec * edge->getWidth(widthPerLane) * 0.6f;

			if (i > 0) {
				QVector2D pt0 = edge->polyLine[i - 1];
				QVector2D vec0 = pt1- pt0;
				vec0 = QVector2D(-vec0.y(), vec0.x());
				vec0.normalize();

				QVector2D q0 = pt0 + vec0 * edge->getWidth(widthPerLane) * 0.6f;
				QVector2D q1 = pt0 - vec0 * edge->getWidth(widthPerLane) * 0.6f;
				QVector2D q2 = pt1 - vec0 * edge->getWidth(widthPerLane) * 0.6f;
				QVector2D q3 = pt1 + vec0 * edge->getWidth(widthPerLane) * 0.6f;

				float tab, tcd;
				Util::segmentSegmentIntersectXY(q1, q2, p1, p2, &tab, &tcd, false, p1);
				Util::segmentSegmentIntersectXY(q0, q3, p0, p3, &tab, &tcd, false, p0);
			}

			if (i < num - 2) {
				QVector2D pt3 = edge->polyLine[i + 2];
				QVector2D vec2 = pt3 - pt2;
				vec2 = QVector2D(-vec2.y(), vec2.x());
				vec2.normalize();

				QVector2D q0 = pt2 + vec2 * edge->getWidth(widthPerLane) * 0.6f;
				QVector2D q1 = pt2 - vec2 * edge->getWidth(widthPerLane) * 0.6f;
				QVector2D q2 = pt3 - vec2 * edge->getWidth(widthPerLane) * 0.6f;
				QVector2D q3 = pt3 + vec2 * edge->getWidth(widthPerLane) * 0.6f;

				float tab, tcd;
				Util::segmentSegmentIntersectXY(p1, p2, q1, q2, &tab, &tcd, false, p2);
				Util::segmentSegmentIntersectXY(p0, p3, q0, q3, &tab, &tcd, false, p3);				
			}

			switch (edge->type) {
			case 3:	// high way
				v.color[0] = 0.89803f;
				v.color[1] = 0.6;
				v.color[2] = 0.08235;
				v.location[2] = 0.0f;
				break;
			case 2: // avenue
				v.color[0] = 0.83921f;
				v.color[1] = 0.80784f;
				v.color[2] = 0.77647f;
				v.location[2] = 0.0f;
				break;
			case 1: // street
				v.color[0] = 0.83921f;
				v.color[1] = 0.80784f;
				v.color[2] = 0.77647f;
				v.location[2] = 0.0f;
				break;
			default:
				v.color[0] = 1.0f;
				v.color[1] = 1.0f;
				v.color[2] = 1.0f;
				v.location[2] = 0.0f;
				break;
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

		for (int i = 0; i < num - 1; ++i) {
			QVector2D pt1 = edge->polyLine[i];
			QVector2D pt2 = edge->polyLine[i + 1];
			QVector2D vec = pt2 - pt1;
			vec = QVector2D(-vec.y(), vec.x());
			vec.normalize();

			QVector2D p0 = pt1 + vec * edge->getWidth(widthPerLane) * 0.5f;
			QVector2D p1 = pt1 - vec * edge->getWidth(widthPerLane) * 0.5f;
			QVector2D p2 = pt2 - vec * edge->getWidth(widthPerLane) * 0.5f;
			QVector2D p3 = pt2 + vec * edge->getWidth(widthPerLane) * 0.5f;

			if (i > 0) {
				QVector2D pt0 = edge->polyLine[i - 1];
				QVector2D vec0 = pt1- pt0;
				vec0 = QVector2D(-vec0.y(), vec0.x());
				vec0.normalize();

				QVector2D q0 = pt0 + vec0 * edge->getWidth(widthPerLane) * 0.5f;
				QVector2D q1 = pt0 - vec0 * edge->getWidth(widthPerLane) * 0.5f;
				QVector2D q2 = pt1 - vec0 * edge->getWidth(widthPerLane) * 0.5f;
				QVector2D q3 = pt1 + vec0 * edge->getWidth(widthPerLane) * 0.5f;

				float tab, tcd;
				Util::segmentSegmentIntersectXY(q1, q2, p1, p2, &tab, &tcd, false, p1);
				Util::segmentSegmentIntersectXY(q0, q3, p0, p3, &tab, &tcd, false, p0);
			}

			if (i < num - 2) {
				QVector2D pt3 = edge->polyLine[i + 2];
				QVector2D vec2 = pt3 - pt2;
				vec2 = QVector2D(-vec2.y(), vec2.x());
				vec2.normalize();

				QVector2D q0 = pt2 + vec2 * edge->getWidth(widthPerLane) * 0.5f;
				QVector2D q1 = pt2 - vec2 * edge->getWidth(widthPerLane) * 0.5f;
				QVector2D q2 = pt3 - vec2 * edge->getWidth(widthPerLane) * 0.5f;
				QVector2D q3 = pt3 + vec2 * edge->getWidth(widthPerLane) * 0.5f;

				float tab, tcd;
				Util::segmentSegmentIntersectXY(p1, p2, q1, q2, &tab, &tcd, false, p2);
				Util::segmentSegmentIntersectXY(p0, p3, q0, q3, &tab, &tcd, false, p3);				
			}

			switch (edge->type) {
			case 3:	// high way
				v.color[0] = 1.0f;
				v.color[1] = 0.88235f;
				v.color[2] = 0.40784f;
				v.location[2] = 0.3f;
				break;
			case 2: // avenue
				v.color[0] = 1.0f;
				v.color[1] = 1.0f;
				v.color[2] = 1.0f;
				v.location[2] = 0.2f;
				break;
			case 1: // street
				v.color[0] = 1.0f;
				v.color[1] = 1.0f;
				v.color[2] = 1.0f;
				v.location[2] = 0.2f;
				break;
			default:
				v.color[0] = 1.0f;
				v.color[1] = 1.0f;
				v.color[2] = 1.0f;
				v.location[2] = 0.2f;
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

void RoadGraph::setWidthPerLane(float widthPerLane) {
	if (this->widthPerLane != widthPerLane) {
		this->widthPerLane = widthPerLane;
		modified = true;
	}
}

/**
 * Importanceに基づいて、エッジを並べて返却する。
 */
QList<RoadEdgeDesc> RoadGraph::getOrderedEdgesByImportance() {
	std::vector<RoadEdgeDesc> data;
	//std::vector<int> data;

	RoadEdgeIter ei, eend;
	int count = 0;
	for (boost::tie(ei, eend) = boost::edges(graph); ei != eend; ++ei) {
		if (!graph[*ei]->valid) continue;

		data.push_back(*ei);
		//data.push_back(count);
		count++;
	}

	std::sort(data.begin(), data.end(), MoreImportantEdge(this));

	QList<RoadEdgeDesc> ret;
	for (int i = 0; i < data.size(); i++) {
		//ret.push_back(GraphUtil::getEdge(this, data[i]));
		ret.push_back(data[i]);
	}

	return ret;
}

LessWeight::LessWeight(RoadGraph* roads) {
	this->roads = roads;
}

bool LessWeight::operator()(const RoadEdgeDesc& left, const RoadEdgeDesc& right) const {
	return roads->graph[left]->lanes < roads->graph[right]->lanes;
}

MoreImportantEdge::MoreImportantEdge(RoadGraph* roads) {
	this->roads = roads;
}

//bool MoreImportantEdge::operator()(const int& left, const int& right) const {
bool MoreImportantEdge::operator()(const RoadEdgeDesc& left, const RoadEdgeDesc& right) const {
	RoadEdge* e1 = roads->graph[left];
	RoadEdge* e2 = roads->graph[right];

	return e1->importance > e2->importance;
}

/**
 * Select a road segment based on the specified position.
 *
 * @param pos		the specified position
 * @return			the selected road edge
 */
RoadEdge* RoadGraph::select(const QVector2D &pos) {
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

void RoadGraph::load(FILE* fp, int roadType) {
	clear();

	QMap<uint, RoadVertexDesc> idToDesc;

	// Read the number of vertices
	unsigned int nVertices;
	fread(&nVertices, sizeof(unsigned int), 1, fp);

	// Read each vertex's information: desc, x, and y.
	for (int i = 0; i < nVertices; i++) {
		RoadVertexDesc id;
		float x, y;
		fread(&id, sizeof(RoadVertexDesc), 1, fp);
		fread(&x, sizeof(float), 1, fp);
		fread(&y, sizeof(float), 1, fp);

		RoadVertex* vertex = new RoadVertex(QVector2D(x, y));

		RoadVertexDesc desc = boost::add_vertex(graph);
		graph[desc] = vertex;

		idToDesc[id] = desc;
	}

	// Read the number of edges
	unsigned int nEdges;
	fread(&nEdges, sizeof(unsigned int), 1, fp);

	// Read each edge's information: the descs of two vertices, road type, the number of lanes, the number of points along the polyline, and the coordinate of each point along the polyline.
	for (int i = 0; i < nEdges; i++) {
		RoadEdge* edge = new RoadEdge(1, 1, false);

		RoadVertexDesc id1, id2;
		fread(&id1, sizeof(RoadVertexDesc), 1, fp);
		fread(&id2, sizeof(RoadVertexDesc), 1, fp);

		RoadVertexDesc src = idToDesc[id1];
		RoadVertexDesc tgt = idToDesc[id2];

		fread(&edge->type, sizeof(unsigned int), 1, fp);
		fread(&edge->lanes, sizeof(unsigned int), 1, fp);
		fread(&edge->oneWay, sizeof(unsigned int), 1, fp);

		unsigned int nPoints;
		fread(&nPoints, sizeof(unsigned int), 1, fp);

		for (int j = 0; j < nPoints; j++) {
			float x, y;
			fread(&x, sizeof(float), 1, fp);
			fread(&y, sizeof(float), 1, fp);

			edge->addPoint(QVector2D(x, y));
		}

		// 指定されたタイプの道路エッジのみを読み込む
		if (((int)powf(2, (edge->type - 1)) & roadType)) {
			std::pair<RoadEdgeDesc, bool> edge_pair = boost::add_edge(src, tgt, graph);
			graph[edge_pair.first] = edge;
		} else {
			delete edge;
		}
	}

	modified = true;
}