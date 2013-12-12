#pragma once

#include "RoadGraph.h"

class EditableRoadGraph : public RoadGraph {
public:
	RoadEdge* selectedEdge;
	RoadVertex* selectedVertex;

public:
	EditableRoadGraph();
	~EditableRoadGraph();

	RoadEdge* select(const QVector2D &pos);
};

