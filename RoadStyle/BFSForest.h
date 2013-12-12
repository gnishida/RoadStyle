#pragma once

#include "AbstractForest.h"
#include "RoadGraph.h"
#include <vector>

class BFSForest : public AbstractForest {
public:
	BFSForest(RoadGraph* roads, QList<RoadVertexDesc> roots);
	~BFSForest();
	
	void buildForest();
};

