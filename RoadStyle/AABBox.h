#pragma once

#include <qvector3d.h>

class AABBox {
private:
	QVector3D corner0;
	QVector3D corner1;

public:
	AABBox();
	AABBox(const QVector3D &pt0, const QVector3D &pt1);

	void clear();
	void addPoint(const QVector3D &pt);
	const QVector3D& minCorner() const;
	const QVector3D& maxCorner() const;
	QVector3D getCentroid();
	QVector3D size() const;
	bool contains(const QVector3D &pt) const;
	bool containsXY(const QVector3D &pt) const;
};

