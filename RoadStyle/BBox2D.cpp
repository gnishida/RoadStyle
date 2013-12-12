#include "BBox2D.h"
#include <limits>

BBox2D::BBox2D() {
	corner0.setX(std::numeric_limits<float>::max());
	corner0.setY(std::numeric_limits<float>::max());

	corner1.setX(-std::numeric_limits<float>::max());
	corner1.setY(-std::numeric_limits<float>::max());
}

BBox2D::BBox2D(const QVector2D &corner0, const QVector2D &corner1) {
	this->corner0 = corner0;
	this->corner1 = corner1;
}

void BBox2D::addPoint(const QVector2D &p) {
	if (p.x() < corner0.x()) {
		corner0.setX(p.x());
	}
	if (p.y() < corner0.y()) {
		corner0.setY(p.y());
	}

	if (p.x() > corner1.x()) {
		corner1.setX(p.x());
	}
	if (p.y() > corner1.y()) {
		corner1.setY(p.y());
	}
}

bool BBox2D::contains(const QVector2D &p) {
	if (p.x() < corner0.x()) return false;
	if (p.y() < corner0.y()) return false;

	if (p.x() > corner1.x()) return false;
	if (p.y() > corner1.y()) return false;

	return true;
}

QVector2D BBox2D::minCorner() const {
	return corner0;
}

QVector2D BBox2D::maxCorner() const {
	return corner1;
}

QVector2D BBox2D::size() const {
	return corner1 - corner0;
}

QVector2D BBox2D::getCenter() const {
	return (corner0 + corner1) / 2.0f;
}

BBox2D BBox2D::intersect(const BBox2D &box) const {
	float x0, x1;
	if (corner0.x() < box.minCorner().x()) {
		if (corner1.x() > box.minCorner().x()) {
			x0 = box.minCorner().x();
			if (box.maxCorner().x() > corner1.x()) {
				x1 = corner1.x();
			} else {
				x1 = box.maxCorner().x();
			}
		} else {
			return BBox2D(QVector2D(0.0f, 0.0f), QVector2D(0.0f, 0.0f));
		}
	} else {
		if (box.maxCorner().x() > corner0.x()) {
			x0 = corner0.x();
			if (corner1.x() > box.maxCorner().x()) {
				x1 = box.maxCorner().x();
			} else {
				x1 = corner1.x();
			}
		} else {
			return BBox2D(QVector2D(0.0f, 0.0f), QVector2D(0.0f, 0.0f));
		}
	}

	float y0, y1;
	if (corner0.y() < box.minCorner().y()) {
		if (corner1.y() > box.minCorner().y()) {
			y0 = box.minCorner().y();
			if (box.maxCorner().y() > corner1.y()) {
				y1 = corner1.y();
			} else {
				y1 = box.maxCorner().y();
			}
		} else {
			return BBox2D(QVector2D(0.0f, 0.0f), QVector2D(0.0f, 0.0f));
		}
	} else {
		if (box.maxCorner().y() > corner0.y()) {
			y0 = corner0.y();
			if (corner1.y() > box.maxCorner().y()) {
				y1 = box.maxCorner().y();
			} else {
				y1 = corner1.y();
			}
		} else {
			return BBox2D(QVector2D(0.0f, 0.0f), QVector2D(0.0f, 0.0f));
		}
	}

	return BBox2D(QVector2D(x0, y0), QVector2D(x1, y1));
}