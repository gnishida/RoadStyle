#include "PropertyWindow.h"

PropertyWindow::PropertyWindow(RoadStyle* mainWin) : QDockWidget("Property", (QWidget*)mainWin) {
	ui.setupUi(this);
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

	ui.labelType->setText(type);
	ui.labelNumLanes->setText(numLanes);
	ui.labelOneWay->setText(oneWay);
}