#pragma once

#include "Camera.h"
#include "RoadGraph.h"
#include "RoadGraphRenderer.h"
#include <QGLWidget>
#include <qstring.h>
#include <qvector3d.h>

class RoadStyle;

class GLWidget : public QGLWidget {
public:
	RoadStyle* mainWin;
	Camera* camera;
	RoadGraph* roads;
	RoadGraphRenderer* roadsRenderer;
	RoadGraphRenderer* sketchRenderer;
	QPoint lastPos;

public:
	GLWidget(QWidget *parent);
	~GLWidget();

	void drawScene();
	void loadOSM(QString filename);

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

private:
	void mouseTo2D(int x, int y, QVector2D *result);
};

