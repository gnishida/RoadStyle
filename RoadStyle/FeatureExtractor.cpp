#include "FeatureExtractor.h"
#include <qfile.h>
#include <qtextstream.h>

FeatureExtractor::FeatureExtractor(RoadGraph* roadGraph) {
	this->roadGraph = roadGraph;
}

void FeatureExtractor::extractFeatures() {
	// average distance between intersection

}

bool FeatureExtractor::saveFeatures(QString filename) {
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		printf("%s\n", file.errorString());
		return false;
	}

	QTextStream out(&file);
	for (int i = 0; i < features.size(); i++) {
		out << features[i] << "\t";
	}
	out << endl;

	return true;
}