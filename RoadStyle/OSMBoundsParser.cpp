#include "OSMBoundsParser.h"

OSMBoundsParser::OSMBoundsParser() {
}

bool OSMBoundsParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts) {
	if (localName == "bounds") {
		minLatLon.setX(atts.value("minlon").toFloat());
		minLatLon.setY(atts.value("minlat").toFloat());
		maxLatLon.setX(atts.value("maxlon").toFloat());
		maxLatLon.setY(atts.value("maxlat").toFloat());
	}

	return true;
}

bool OSMBoundsParser::characters(const QString &ch_in) {
	return true;
}

bool OSMBoundsParser::endElement(const QString& namespaceURI, const QString& localName, const QString& qName) {
	return true;
}