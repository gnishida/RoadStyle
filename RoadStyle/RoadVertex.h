#pragma once

#include <qvector3d.h>

class RoadVertex {
public:
	QVector3D pt;

public:
	RoadVertex();
	RoadVertex(const QVector3D &pt);

	const QVector3D& getPt() const;
};

