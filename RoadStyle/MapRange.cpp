#include "MapRange.h"

MapRange::MapRange() {
}

void MapRange::addBox(const BBox2D &box) {
	ranges.push_back(box);
}

bool MapRange::contains(const QVector2D &p) {
	for (int i = 0; i < ranges.size(); i++) {
		if (ranges[i].contains(p)) return true;
	}

	return false;
}

int MapRange::size() const {
	return ranges.size();
}

BBox2D& MapRange::get(int index) {
	return ranges[index];
}

BBox2D MapRange::getBBox() const {
	BBox2D box;

	for (int i = 0; i < ranges.size(); i++) {
		box.addPoint(ranges[i].minCorner());
		box.addPoint(ranges[i].maxCorner());
	}

	return box;
}