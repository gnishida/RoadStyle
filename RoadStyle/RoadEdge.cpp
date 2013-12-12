#include "RoadEdge.h"
#include "Util.h"

RoadEdge::RoadEdge(unsigned int lanes, unsigned int type, bool oneWay) {
	this->lanes = lanes;
	this->type = type;
	this->oneWay = oneWay;
	this->weight = 0.0f;
	this->valid = true;
	this->importance = 0.0f;
	this->group = 0;
	this->seed = false;
	this->fullyPaired = false;
}

RoadEdge::~RoadEdge() {
}

float RoadEdge::getLength() {
	float length = 0.0f;
	for (int i = 0; i < polyLine.size() - 1; i++) {
		length += (polyLine[i + 1] - polyLine[i]).length();
	}

	return length;
}

std::vector<QVector2D> RoadEdge::getPolyLine() {
	return polyLine;
}

/**
 * Add a point to the polyline of the road segment.
 *
 * @param pt		new point to be added
 */
void RoadEdge::addPoint(const QVector2D &pt) {
	polyLine.push_back(pt);
}

float RoadEdge::getWidth(float widthPerLane) {
	if (type == 1) { // local street
		return widthPerLane;
	} else if (type == 2) { // avenue
		return widthPerLane * 1.5f;
	} else if (type == 3) { // high way
		return widthPerLane * 2.0f;
	}
}

/**
 * Check whether the point resides in this road segment.
 * Return true if so, false otherwise.
 *
 * @param pos		the point
 * @return			true if the point is inside the road segment, false otherwise
 */
bool RoadEdge::containsPoint(const QVector2D &pos) {
	for (int i = 0; i < polyLine.size() - 1; i++) {
		QVector2D p0 = polyLine[i];
		QVector2D p1 = polyLine[i + 1];
		if (Util::pointSegmentDistanceXY(p0, p1, pos) <= getWidth(3.5f)) return true;
	}

	return false;
}