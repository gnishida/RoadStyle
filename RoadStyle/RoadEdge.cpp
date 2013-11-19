#include "RoadEdge.h"
#include "Util.h"

RoadEdge::RoadEdge() {
	this->oneWay = false;
	this->numLanes = 1;
	this->type = 1;
}

RoadEdge::RoadEdge(bool oneWay, int numLanes, int type) {
	this->oneWay = oneWay;
	this->numLanes = numLanes;
	this->type = type;
}

RoadEdge::~RoadEdge() {
}

bool RoadEdge::isOneWay() {
	return oneWay;
}

int RoadEdge::getNumLanes() {
	if (type >= 3 && numLanes == 1) return 2;
	else return numLanes;
}

float RoadEdge::getLength() {
	return length;
}

int RoadEdge::getType() {
	return type;
}

std::vector<QVector3D> RoadEdge::getPolyLine() {
	return polyLine;
}

/**
 * Add a point to the polyline of the road segment.
 *
 * @param pt		new point to be added
 */
void RoadEdge::addPoint(const QVector3D &pt) {
	if (polyLine.size() > 0) {
		length += (pt - polyLine[polyLine.size() - 1]).length();
	}
	polyLine.push_back(pt);
}

float RoadEdge::getWidth() {
	if (type >= 2) {
		return numLanes * (oneWay ? 1 : 2) * 3.5f;
	} else {
		return numLanes * (oneWay ? 1 : 2) * 3.0f;
	}
}

/**
 * Order the poly line of the road segment by starting from the point that is close to the specified point.
 *
 * @param start			the specified point to start the poly line
 */
void RoadEdge::startFrom(QVector3D start) {
	if (polyLine.size() == 0) return;

	if ((polyLine[0] - start).lengthSquared() > (polyLine[polyLine.size() - 1] - start).lengthSquared()) {
		std::reverse(polyLine.begin(), polyLine.end());
	}
}

/**
 * Check whether the point resides in this road segment.
 * Return true if so, false otherwise.
 *
 * @param pos		the point
 * @return			true if the point is inside the road segment, false otherwise
 */
bool RoadEdge::containsPoint(const QVector3D &pos) {
	for (int i = 0; i < polyLine.size() - 1; i++) {
		QVector3D p0 = polyLine[i];
		QVector3D p1 = polyLine[i + 1];
		if (Util::pointSegmentDistanceXY(p0, p1, pos) <= getWidth()) return true;
	}

	return false;
}