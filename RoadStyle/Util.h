#pragma once

#include <qvector3d.h>
#include <qvector2d.h>

class Util {
protected:
	Util();

public:
	static float pointSegmentDistanceXY(const QVector3D &a, const QVector3D &b, const QVector3D &c);
	//static QVector3D projLatLonToMeter(const QVector3D &latLon, const QVector3D &centerLatLon);
	static QVector2D projLatLonToMeter(const QVector2D &latLon, const QVector2D &centerLatLon);
};

