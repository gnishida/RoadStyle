#include "Util.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define SQR(x)		((x) * (x))

/**
 * Return the sistance from segment ab to point c.
 * If the 
 */
float Util::pointSegmentDistanceXY(const QVector3D &a, const QVector3D &b, const QVector3D &c) {
	float dist;		

	float r_numerator = (c.x()-a.x())*(b.x()-a.x()) + (c.y()-a.y())*(b.y()-a.y());
	float r_denomenator = (b.x()-a.x())*(b.x()-a.x()) + (b.y()-a.y())*(b.y()-a.y());
	float r = r_numerator / r_denomenator;

	if (r >= 0 && r <= 1) {
		return abs((a.y()-c.y())*(b.x()-a.x())-(a.x()-c.x())*(b.y()-a.y())) / sqrt(r_denomenator);
	} else {
		float dist1 = SQR(c.x() - a.x()) + SQR(c.y() - a.y());
		float dist2 = SQR(c.x() - b.x()) + SQR(c.y() - b.y());
		if (dist1 < dist2) {	
			return sqrt(dist1);
		} else {
			return sqrt(dist2);
		}
	}
}

/**
 * Project latitude/longitude coordinate to world coordinate.
 * Mercator projection cannot be used for this purpose, becuase
 * it deforms the area especially in the high latitude regions.
 * Hubeny's theorum should be used for this purpose, but not yet implemented yet.
 *
 * @param lat		latitude
 * @param lon		longitude
 * @param centerLat	latitude of the center of the map
 * @param centerLon	longitude of the center of the map
 * @return			the world coordinate (Z coordinate is dummy.)
 */
QVector2D Util::projLatLonToMeter(const QVector2D &latLon, const QVector2D &centerLatLon) {
	QVector2D result;

	float y = latLon.y() / 180.0f * M_PI;
	float dx = (latLon.x() - centerLatLon.x()) / 180.0f * M_PI;
	float dy = (latLon.y() - centerLatLon.y()) / 180.0f * M_PI;

	float radius = 6378137.0f;;

	result.setX(radius * cos(y) * dx);
	result.setY(radius * dy);

	return  result; 
}

/*
QVector3D Util::projLatLonToMeter(const QVector3D &latLon, const QVector3D &centerLatLon) {
	QVector3D result;

	float y = latLon.y() / 180.0f * M_PI;
	float dx = (latLon.x() - centerLatLon.x()) / 180.0f * M_PI;
	float dy = (latLon.y() - centerLatLon.y()) / 180.0f * M_PI;

	float radius = 6378137.0f;;

	result.setX(radius * cos(y) * dx);
	result.setY(radius * dy);
	result.setZ(0.0f);

	return  result; 
}
*/