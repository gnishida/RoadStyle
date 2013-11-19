#include "RoadVertex.h"

RoadVertex::RoadVertex() {
	this->pt = QVector3D(0.0f, 0.0f, 0.0f);
}

RoadVertex::RoadVertex(const QVector3D &pt) {
	this->pt = pt;
}

const QVector3D& RoadVertex::getPt() const {
	return pt;
}