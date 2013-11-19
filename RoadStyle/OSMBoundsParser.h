#pragma once

#include <QtXml/qxml.h>
#include <qstring.h>
#include <qvector3d.h>

class OSMBoundsParser : public QXmlDefaultHandler{
public:
	//QVector3D minLatLon;
	//QVector3D maxLatLon;
	QVector3D centerLatLon;

public:
	OSMBoundsParser();

	bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
	bool characters(const QString &ch_in);
	bool endElement(const QString&, const QString& localName, const QString& qName);
};

