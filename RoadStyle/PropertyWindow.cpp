#include "PropertyWindow.h"
#include "RoadStyle.h"
#include "GraphUtil.h"

PropertyWindow::PropertyWindow(RoadStyle* mainWin) : QDockWidget("Property", (QWidget*)mainWin) {
	this->mainWin = mainWin;
	ui.setupUi(this);

	ui.lineEditSimplifyThreshold->setText("10");
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
 * Event handler for button [simplify]
 */
void PropertyWindow::simplify() {
	float threshold = ui.lineEditSimplifyThreshold->text().toFloat();

	GraphUtil::simplify(mainWin->glWidget->roads, threshold);
	GraphUtil::clean(mainWin->glWidget->roads);
	mainWin->glWidget->updateGL();
}