#pragma once

#include "RoadGraph.h"

class FeatureExtractor {
private:
	RoadGraph* roadGraph;
	std::vector<float> features;

public:
	FeatureExtractor(RoadGraph* roadGraph);

	void extractFeatures();
	bool saveFeatures(QString filename);
};

