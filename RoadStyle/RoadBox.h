#pragma once

#include "RoadView.h"
#include "ui_RoadBox.h"
#include <qwidget.h>

class RoadStyle;

class RoadBox : public QWidget {
Q_OBJECT

public:
	Ui::RoadBox ui;
	RoadStyle* mainWin;
	RoadView* view;
	QPushButton* pushButtonSelect;

public:
	RoadBox(RoadStyle* parent, const char* filename);
	~RoadBox();

public slots:
	void select();
};

