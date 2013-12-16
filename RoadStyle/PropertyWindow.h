#pragma once

#include <qdockwidget.h>
#include "ui_PropertyWindow.h"
#include "RoadEdge.h"

class RoadStyle;

class PropertyWindow : public QDockWidget {
Q_OBJECT

private:
	Ui::PropertyWindow ui;
	RoadStyle* mainWin;

public:
	PropertyWindow(RoadStyle* mainWin);

	void setRoadEdge(RoadEdge* selectedEdge);
	void updateModeButtons();

public slots:
	void modeView(bool flag);
	void modeSketch(bool flag);
	void modeSelect(bool flag);
	void search();
	void clear();
	void makeRoad();
	void simplify();
};

