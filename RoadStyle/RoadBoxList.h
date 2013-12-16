#pragma once

#include "RoadBox.h"
#include "ui_RoadBoxList.h"

class RoadStyle;

class RoadBoxList : public QWidget {
Q_OBJECT

public:
	Ui::RoadBoxList ui;
	RoadStyle* mainWin;
	std::vector<RoadBox*> references;

public:
	RoadBoxList(RoadStyle* mainWin);
	~RoadBoxList();

public slots:

};

