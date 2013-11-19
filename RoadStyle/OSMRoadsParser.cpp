#include "OSMRoadsParser.h"
#include "BBox2D.h"
#include "Util.h"

OSMRoadsParser::OSMRoadsParser(RoadGraph *roads) {
	this->roads = roads;

	parentNodeName = "osm";
	vertices.clear();
}

bool OSMRoadsParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts) {
	if (localName == "bounds") {
		BBox2D box;
		QVector2D minLatLon, maxLatLon;
		
		minLatLon.setX(atts.value("minlon").toFloat());
		minLatLon.setY(atts.value("minlat").toFloat());
		maxLatLon.setX(atts.value("maxlon").toFloat());
		maxLatLon.setY(atts.value("maxlat").toFloat());

		box.addPoint(minLatLon);
		box.addPoint(maxLatLon);
		ranges.addBox(box);

		centerLatLon = ranges.getBBox().getCenter();

		roads->setRange(ranges);

		/*
		centerLatLon = (minLatLon + maxLatLon) / 2.0f;

		QVector2D minCorner = Util::projLatLonToMeter(minLatLon, centerLatLon);
		QVector2D maxCorner = Util::projLatLonToMeter(maxLatLon, centerLatLon);

		BBox2D box;
		box.addPoint(minCorner);
		box.addPoint(maxCorner);

		ranges.addBox(box);

		roads->setRange(ranges);
		*/
	} else if (localName == "node") {
		handleNode(atts);
	} else if (localName == "way") {
		parentNodeName = "way";
		handleWay(atts);
	} else if (localName == "nd") {
		if (parentNodeName == "way") {
			handleNd(atts);
		}
	} else if (localName == "tag") {
		if (parentNodeName == "way") {
			handleTag(atts);
		}
	}

	return true;
}

bool OSMRoadsParser::characters(const QString &ch_in) {
	return true;
}

bool OSMRoadsParser::endElement(const QString& namespaceURI, const QString& localName, const QString& qName) {
	if (localName == "way" && parentNodeName == "way") {
		parentNodeName = "osm";

		createRoadEdge();
	}

	return true;
}

void OSMRoadsParser::handleNode(const QXmlAttributes &atts) {
	uint id = atts.value("id").toUInt();
	QVector2D latlon;
	latlon.setX(atts.value("lon").toFloat());
	latlon.setY(atts.value("lat").toFloat());
	QVector2D pos = Util::projLatLonToMeter(latlon, centerLatLon) - Util::projLatLonToMeter(centerLatLon, centerLatLon);

	bool duplicated = false;
	QMap<uint, RoadVertex>::iterator j;
	for (j = vertices.begin(); j != vertices.end(); j++) {
		if ((j.value().getPt() - pos).lengthSquared() < 25.0f) {
			idToActualId.insert(id, j.key());
			duplicated = true;
			break;
		}
	}

	if (!duplicated) {
		idToActualId.insert(id, id);
		vertices.insert(id, RoadVertex(pos));
	}
}

void OSMRoadsParser::handleWay(const QXmlAttributes &atts) {
	isStreet = false;
	oneWay = false;
	numLanes = 1;
	typeRoad = 1;
	nds.clear();
}

void OSMRoadsParser::handleNd(const QXmlAttributes &atts) {
	uint ref = atts.value("ref").toUInt();
	nds.push_back(ref);
}

void OSMRoadsParser::handleTag(const QXmlAttributes &atts) {
	QString key = atts.value("k");
	if (key == "highway") {
		QString value = atts.value("v");
		isStreet = true;
		if (value=="motorway"||value=="motorway_link"||value=="trunk"||value=="trunk") {
			typeRoad=5;
		}
		if (value=="primary"||value=="primary_link") {
			typeRoad=4;
		}
		if (value=="secondary"||value=="secondary_link") {
			typeRoad=3;
		}
		if (value=="tertiary"||value=="tertiary_link") {
			typeRoad = 2;
		}
		if (value=="residential"||value=="living_street"||value=="unclassified") {
			typeRoad=1;
		}
		if (value=="pedestrian"||value=="track"||value=="footway") {
			typeRoad=0;
		}
	} else if (key == "oneway") {
		QString value = atts.value("v");
		if (value == "yes") {
			oneWay = true;
		}
	} else if (key == "lanes") {
		numLanes = atts.value("v").toInt();
	}
}

void OSMRoadsParser::createRoadEdge() {
	if (!isStreet || typeRoad == 0) return;

	for (int k = 0; k < nds.size() - 1; k++) {
		uint id = nds[k];
		uint next = nds[k + 1];

		//if (!idToActualId.contains(id)) continue;
		//if (!idToActualId.contains(next)) continue;

		RoadVertexDesc sourceDesc;
		if (idToDesc.contains(idToActualId[id])) {
			sourceDesc = idToDesc[idToActualId[id]];
		} else {
			RoadVertex* v = new RoadVertex(vertices[idToActualId[id]].getPt());
			sourceDesc = roads->addVertex(v);
			idToDesc.insert(idToActualId[id], sourceDesc);
		}

		RoadVertexDesc destDesc;
		if (idToDesc.contains(idToActualId[next])) {
			destDesc = idToDesc[idToActualId[next]];
		} else {
			RoadVertex* v = new RoadVertex(vertices[idToActualId[next]].getPt());
			destDesc = roads->addVertex(v);
			idToDesc.insert(idToActualId[next], destDesc);
		}

		bool duplicated = false;
		RoadEdgeIter ei, eend;
		for (boost::tie(ei, eend) = boost::edges(roads->getGraph()); ei != eend; ++ei) {
			if (boost::source(*ei, roads->getGraph()) == sourceDesc && boost::target(*ei, roads->getGraph()) == destDesc) {
				duplicated = true;
				break;
			}
			if (boost::source(*ei, roads->getGraph()) == destDesc && boost::target(*ei, roads->getGraph()) == sourceDesc) {
				duplicated = true;
				break;
			}
		}

		if (!duplicated) {
			RoadEdge* e = new RoadEdge(oneWay, numLanes, typeRoad);
			e->addPoint(vertices[idToActualId[id]].getPt());
			e->addPoint(vertices[idToActualId[next]].getPt());
			roads->addEdge(sourceDesc, destDesc, e);
		}
	}
}