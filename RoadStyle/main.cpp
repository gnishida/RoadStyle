#include "RoadStyle.h"
#include "Util.h"
#include <QtGui/QApplication>
#include <assert.h>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	RoadStyle w;
	w.show();
	return a.exec();
}
