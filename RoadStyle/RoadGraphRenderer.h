#pragma once

#include "RoadGraph.h"
#include "Renderable.h"

class RoadStyle;

class RoadGraphRenderer {
private:
	unsigned int dispList;

public:
	RoadGraphRenderer();

	void render(std::vector<Renderable>& renderables);
	void _render(std::vector<Renderable>& renderables);
};

