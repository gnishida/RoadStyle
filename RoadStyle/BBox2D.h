#pragma once

#include <qvector2d.h>

class BBox2D {
private:
	QVector2D corner0;
	QVector2D corner1;

public:
	BBox2D();
	BBox2D(const QVector2D &corner0, const QVector2D &corner1);

	void addPoint(const QVector2D &p);
	bool contains(const QVector2D &p);

	QVector2D minCorner() const;
	QVector2D maxCorner() const;

	QVector2D size() const;
	QVector2D getCenter() const;

	BBox2D intersect(const BBox2D &box) const;
};

