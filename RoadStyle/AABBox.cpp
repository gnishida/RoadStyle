#include "AABBox.h"
#include <limits>

AABBox::AABBox() {
	clear();
}

AABBox::AABBox(const QVector3D &pt0, const QVector3D &pt1) {
	corner0 = pt0;
	corner1 = pt1;
}

void AABBox::clear() {
	corner0.setX(std::numeric_limits<float>::max());
	corner0.setY(std::numeric_limits<float>::max());
	corner0.setZ(std::numeric_limits<float>::max());

	corner1.setX(-std::numeric_limits<float>::max());
	corner1.setY(-std::numeric_limits<float>::max());
	corner1.setZ(-std::numeric_limits<float>::max());
}

void AABBox::addPoint(const QVector3D &pt) {
	if (pt.x() < corner0.x()) {
		corner0.setX(pt.x());
	}
	if (pt.y() < corner0.y()) {
		corner0.setY(pt.y());
	}
	if (pt.z() < corner0.z()) {
		corner0.setZ(pt.z());
	}

	if (pt.x() > corner1.x()) {
		corner1.setX(pt.x());
	}
	if (pt.y() > corner1.y()) {
		corner1.setY(pt.y());
	}
	if (pt.z() > corner1.z()) {
		corner1.setZ(pt.z());
	}
}

const QVector3D& AABBox::minCorner() const {
	return corner0;
}

const QVector3D& AABBox::maxCorner() const {
	return corner1;
}

QVector3D AABBox::getCentroid() {
	return (corner0 + corner1) / 2.0f;
}

QVector3D AABBox::size() const {
	return corner1 - corner0;
}

bool AABBox::contains(const QVector3D &pt) const {
	if (pt.x() < corner0.x()) return false;
	if (pt.y() < corner0.y()) return false;
	if (pt.z() < corner0.z()) return false;

	if (pt.x() > corner1.x()) return false;
	if (pt.y() > corner1.y()) return false;
	if (pt.z() > corner1.z()) return false;

	return true;
}

bool AABBox::containsXY(const QVector3D &pt) const {
	if (pt.x() < corner0.x()) return false;
	if (pt.y() < corner0.y()) return false;

	if (pt.x() > corner1.x()) return false;
	if (pt.y() > corner1.y()) return false;

	return true;
}