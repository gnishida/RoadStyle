#include "RoadBoxList.h"
#include "RoadStyle.h"

RoadBoxList::RoadBoxList(RoadStyle* mainWin) : QWidget((QWidget*)mainWin) {
	this->mainWin = mainWin;
	//ui.setupUi(this);

	QVBoxLayout* layout = new QVBoxLayout();
	this->setLayout(layout);

	// Add three views for the example road graphs
	const char* filename[3] = {"osm\\3x3\\beijing.gsm", "osm\\3x3\\canberra.gsm", "osm\\3x3\\london.gsm"};
	for (int i = 0; i < 3; i++) {
		RoadBox* box = new RoadBox(mainWin, filename[i]);
		references.push_back(box);
		
		layout->addWidget(box);
	}
}

RoadBoxList::~RoadBoxList() {
}
