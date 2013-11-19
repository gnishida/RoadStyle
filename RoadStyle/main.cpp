#include "RoadStyle.h"
#include "Util.h"
#include <QtGui/QApplication>
#include <assert.h>

int main(int argc, char *argv[]) {
	QVector3D p0(0, 0, 0);
	QVector3D p1(10, 0, 0);
	QVector3D p2(5, 5, 0);
	assert(Util::pointSegmentDistanceXY(p0, p1, p2) == 5.0f);
	p2 = QVector3D(-5, 5, 0);
	assert(Util::pointSegmentDistanceXY(p0, p1, p2) - 5.0f * sqrtf(2.0f) < 0.01f);

	QApplication a(argc, argv);
	RoadStyle w;
	w.show();
	return a.exec();
}
