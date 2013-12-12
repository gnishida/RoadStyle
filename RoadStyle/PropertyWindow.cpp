#include "PropertyWindow.h"
#include "RoadStyle.h"
#include "GraphUtil.h"

PropertyWindow::PropertyWindow(RoadStyle* mainWin) : QDockWidget("Property", (QWidget*)mainWin) {
	this->mainWin = mainWin;
	ui.setupUi(this);

	ui.lineEditSimplifyThreshold->setText("10");
	ui.pushButtonModeView->setChecked(true);

	ui.pushButtonModeView->setCheckable(true);
	ui.pushButtonModeSketch->setCheckable(true);
	ui.pushButtonModeSelect->setCheckable(true);

	connect(ui.pushButtonModeView, SIGNAL(clicked(bool)), this, SLOT(modeView(bool)));
	connect(ui.pushButtonModeSketch, SIGNAL(clicked(bool)), this, SLOT(modeSketch(bool)));
	connect(ui.pushButtonModeSelect, SIGNAL(clicked(bool)), this, SLOT(modeSelect(bool)));
	connect(ui.pushButtonClear, SIGNAL(clicked()), this, SLOT(clear()));
	connect(ui.pushButtonMakeRoad, SIGNAL(clicked()), this, SLOT(makeRoad()));
	connect(ui.pushButtonSimplify, SIGNAL(clicked()), this, SLOT(simplify()));

	hide();
}

void PropertyWindow::setRoadEdge(RoadEdge* selectedEdge) {
	QString type("");
	QString numLanes("");
	QString oneWay("");

	if (selectedEdge != NULL) {
		type.setNum(selectedEdge->type);
		numLanes.setNum(selectedEdge->lanes);
		if (selectedEdge->oneWay) {
			oneWay = "Yes";
		} else {
			oneWay = "No";
		}
	}

	ui.lineEditType->setText(type);
	ui.lineEditLanes->setText(numLanes);
	ui.lineEditOneWay->setText(oneWay);
}

/**
 * Update the up/down appearance of buttons according to the current mode.
 */
void PropertyWindow::updateModeButtons() {
	ui.pushButtonModeView->setChecked(false);
	ui.pushButtonModeSketch->setChecked(false);
	ui.pushButtonModeSelect->setChecked(false);

	switch (mainWin->mode) {
	case RoadStyle::MODE_VIEW:
		ui.pushButtonModeView->setChecked(true);
		break;
	case RoadStyle::MODE_SKETCH:
		ui.pushButtonModeSketch->setChecked(true);
		break;
	case RoadStyle::MODE_SELECT:
		ui.pushButtonModeSelect->setChecked(true);
		break;
	}
}

/**
 * Change the mode to "View" mode.
 */
void PropertyWindow::modeView(bool flag) {
	mainWin->mode = RoadStyle::MODE_VIEW;
	updateModeButtons();
}

/**
 * Change the mode to "Sketch" mode.
 */
void PropertyWindow::modeSketch(bool flag) {
	mainWin->mode = RoadStyle::MODE_SKETCH;
	updateModeButtons();
}

/**
 * Change the mode to "Select" mode.
 */
void PropertyWindow::modeSelect(bool flag) {
	mainWin->mode = RoadStyle::MODE_SELECT;
	updateModeButtons();
}

/**
 * Clear the screen.
 */
void PropertyWindow::clear() {
	mainWin->glWidget->sketch->clear();
	mainWin->glWidget->updateGL();
}

/**
 * Make the road graph from the sketch 
 */
void PropertyWindow::makeRoad() {
	mainWin->glWidget->makeRoadsFromSketch();
}

/**
 * Event handler for button [simplify]
 */
void PropertyWindow::simplify() {
	float threshold = ui.lineEditSimplifyThreshold->text().toFloat();

	GraphUtil::simplify(mainWin->glWidget->roads, threshold);
	GraphUtil::clean(mainWin->glWidget->roads);
	mainWin->glWidget->updateGL();
}
