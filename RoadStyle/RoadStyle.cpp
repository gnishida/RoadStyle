#include "RoadStyle.h"
#include <qdockwidget.h>
#include <qfiledialog.h>

RoadStyle::RoadStyle(QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
	ui.setupUi(this);

	// setup the docking widgets
	propertyWindow = new PropertyWindow(this);

	dockRoadBoxList = new QDockWidget(tr("Reference Roads"), this);
	roadBoxList = new RoadBoxList(this);//dockRoadBoxList);
	dockRoadBoxList->setWidget(roadBoxList);
	dockRoadBoxList->hide();

	// register the menu handlers
	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(newRoad()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openRoad()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionHighways, SIGNAL(triggered(bool)), this, SLOT(showHighways(bool)));
	connect(ui.actionAvenues, SIGNAL(triggered(bool)), this, SLOT(showAvenues(bool)));
	connect(ui.actionStreets, SIGNAL(triggered(bool)), this, SLOT(showStreets(bool)));
	connect(ui.actionProperty, SIGNAL(triggered()), this, SLOT(showProperty()));
	connect(ui.actionReferenceRoads, SIGNAL(triggered()), this, SLOT(showReferenceRoads()));

	attributes = new Attributes();
	attributes->set("showHighways", true);
	attributes->set("showAvenues", true);
	attributes->set("showStreets", true);
	attributes->set("HighwayHeight", 10.0f);
	attributes->set("AvenueHeight", 0.5f);
	attributes->set("StreetHeight", 0.0f);

	mode = MODE_VIEW;

	// setup the GL widget
	glWidget = new GLWidget(this);
	setCentralWidget(glWidget);
}

RoadStyle::~RoadStyle() {
}

Attributes* RoadStyle::getAttributes() {
	return attributes;
}

void RoadStyle::newRoad() {
	glWidget->newRoad();
}

void RoadStyle::openRoad() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open GenStreetMap file..."), "", tr("GenStreetMap Files (*.gsm)"));

	if (filename.isEmpty()) {
		printf("Unable to open file\n");
		return;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);
	glWidget->openRoad(filename.toAscii().constData());
	QApplication::restoreOverrideCursor();
}

void RoadStyle::showHighways(bool flag) {
	attributes->set("showHighways", flag);
	glWidget->roads->setModified();
	glWidget->updateGL();
}

void RoadStyle::showAvenues(bool flag) {
	attributes->set("showAvenues", flag);
	glWidget->roads->setModified();
	glWidget->updateGL();
}

void RoadStyle::showStreets(bool flag) {
	attributes->set("showStreets", flag);
	glWidget->roads->setModified();
	glWidget->updateGL();
}

void RoadStyle::showProperty() {
	propertyWindow->show();
	addDockWidget(Qt::LeftDockWidgetArea, propertyWindow);
}

void RoadStyle::showReferenceRoads() {
	dockRoadBoxList->show();
	addDockWidget(Qt::RightDockWidgetArea, dockRoadBoxList);
}
