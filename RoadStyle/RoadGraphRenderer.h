#pragma once

#include "RoadGraph.h"

class RoadGraphRenderer {
private:
	unsigned int dispList;

public:
	RoadGraphRenderer();

	void render(RoadGraph* object);
	void _render(RoadGraph* object);
};

