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
	Ui::RoadStyleClass ui;
	PropertyWindow* propertyWindow;
	GLWidget* glWidget;
	Attributes* attributes;

public:
	RoadStyle(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RoadStyle();

	Attributes* getAttributes();

public slots:
	void open();
	void showHighways(bool flag);
	void showAvenues(bool flag);
	void showStreets(bool flag);
	void showProperty();
};

#endif // ROADSTYLE_H
