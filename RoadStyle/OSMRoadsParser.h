#pragma once

#include "MapRange.h"
#include "RoadGraph.h"
#include "RoadVertex.h"
#include "RoadEdge.h"
#include <QtXml/qxml.h>
#include <qstring.h>
#include <qvector2d.h>

class OSMRoadsParser : public QXmlDefaultHandler {
private:
	RoadGraph* roads;
	//QVector2D minLatLon;
	//QVector2D maxLatLon;
	QVector2D centerLatLon;
	MapRange ranges;

	QString parentNodeName;
	bool isStreet;
	bool oneWay;
	uint numLanes;
	uint typeRoad;
	std::vector<uint> nds;

	QMap<uint, RoadVertexDesc> idToDesc;
	QMap<uint, uint> idToActualId;
	QMap<uint, RoadVertex> vertices;


public:
	OSMRoadsParser(RoadGraph *roads);

	bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
	bool characters(const QString &ch_in);
	bool endElement(const QString&, const QString& localName, const QString& qName);

private:
	void handleNode(const QXmlAttributes &atts);
	void handleWay(const QXmlAttributes &atts);
	void handleNd(const QXmlAttributes &atts);
	void handleTag(const QXmlAttributes &atts);
	void createRoadEdge();
};

