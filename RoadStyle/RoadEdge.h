#pragma once

#include <qvector3d.h>
#include <vector>

class RoadEdge {
public:
	bool oneWay;
	int numLanes;
	float length;
	int type;
	std::vector<QVector3D> polyLine;

public:
	RoadEdge();
	RoadEdge(bool oneWay, int numLanes, int typeRoad);
	~RoadEdge();
	
	bool isOneWay();
	int getNumLanes();
	float getLength();
	int getType();
	std::vector<QVector3D> getPolyLine();

	void addPoint(const QVector3D &pt);
	float getWidth();

	void startFrom(QVector3D start);
	bool containsPoint(const QVector3D &pos);
};

