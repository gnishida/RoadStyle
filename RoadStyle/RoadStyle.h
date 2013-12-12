#ifndef ROADSTYLE_H
#define ROADSTYLE_H

#include <QtGui/QMainWindow>
#include "ui_RoadStyle.h"
#include "PropertyWindow.h"
#include "GLWidget.h"
#include "Attributes.h"

class RoadStyle : public QMainWindow {
Q_OBJECT

public:
	static enum { MODE_VIEW = 0, MODE_SKETCH, MODE_SELECT };

public:
	Ui::RoadStyleClass ui;
	PropertyWindow* propertyWindow;
	GLWidget* glWidget;
	Attributes* attributes;
	int mode;

public:
	RoadStyle(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RoadStyle();

	Attributes* getAttributes();

public slots:
	void newRoad();
	void openRoad();
	void showHighways(bool flag);
	void showAvenues(bool flag);
	void showStreets(bool flag);
	void showProperty();
};

#endif // ROADSTYLE_H
