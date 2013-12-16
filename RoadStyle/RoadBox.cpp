#include "RoadBox.h"
#include "RoadStyle.h"
#include "GraphUtil.h"
#include <qlayout.h>
#include <qpushbutton.h>

RoadBox::RoadBox(RoadStyle* parent, const char* filename) : QWidget((QWidget*)parent) {
	mainWin = parent;
	this->setMinimumSize(250, 250);

	QVBoxLayout* layout = new QVBoxLayout();
	this->setLayout(layout);

	view = new RoadView();
	view->load(filename);
	pushButtonSelect = new QPushButton(tr("Select"));

	layout->addWidget(view);
	layout->addWidget(pushButtonSelect);

	// register the event handlers
	connect(pushButtonSelect, SIGNAL(clicked()), this, SLOT(select()));
}

RoadBox::~RoadBox() {
}

void RoadBox::select() {
	mainWin->glWidget->ref_roads = GraphUtil::copyRoads(view->roads);
	mainWin->glWidget->updateGL();
}
