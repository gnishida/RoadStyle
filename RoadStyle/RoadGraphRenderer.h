#pragma once

#include "RoadGraph.h"

class RoadStyle;

class RoadGraphRenderer {
private:
	unsigned int dispList;

public:
	RoadGraphRenderer();

	void render(RoadGraph* object, RoadStyle* mainWin);
	void _render(RoadGraph* object);
};

