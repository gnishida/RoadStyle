#include "GLWidget.h"
#include "RoadStyle.h"
#include "OSMRoadsParser.h"
#include "GraphUtil.h"
#include <gl/GLU.h>
#include <qvector3d.h>
#include <qfile.h>
#include <qset.h>
#include <qmap.h>
#include <qdom.h>
#include <vector>

GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
	mainWin = (RoadStyle*)parent;
	camera = new Camera();
	//camera->setTranslation(-11840.0f, 354100.0f, 100.0f);
	//camera->setLookAt(-11840.0f, 354100.0f, 0.0f);
	roads = new RoadGraph();
	roadsRenderer = new RoadGraphRenderer();
	sketchRenderer = new RoadGraphRenderer();

	// set up the camera
	camera->setLookAt(0.0f, 0.0f, 0.0f);
	camera->setTranslation(0.0f, 0.0f, 150.0f);

	// initialize the width per lane
	roads->widthPerLane = 2.0f;
}

GLWidget::~GLWidget() {
}

void GLWidget::drawScene() {
	roads->generateMesh(mainWin->getAttributes()->getBool("showHighways"), mainWin->getAttributes()->getBool("showAvenues"), mainWin->getAttributes()->getBool("showStreets"));
	roadsRenderer->render(roads->renderables);
}

void GLWidget::loadOSM(QString filename) {
	roads->clear();

	FILE* fp = fopen(filename.toUtf8().data(), "rb");
	roads->load(fp, 7);

	updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
	this->setFocus();

	lastPos = event->pos();

	if (event->buttons() & Qt::LeftButton) {
		QVector2D pos;
		mouseTo2D(event->x(), event->y(), &pos);
		mainWin->ui.statusBar->showMessage(QString("clicked (%1, %2)").arg(pos.x()).arg(pos.y()));

		if (mainWin->mode == RoadStyle::MODE_VIEW) {
			// select the edge close to the point
			RoadEdge* selectedEdge = roads->select(pos);
			mainWin->propertyWindow->setRoadEdge(selectedEdge);
		} else if (mainWin->mode == RoadStyle::MODE_SKETCH) {
			// put the vertex of a line

		}

		updateGL();
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
	event->ignore();

	setCursor(Qt::ArrowCursor);
	updateGL();

	return;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
	float dx = (float)(event->x() - lastPos.x());
	float dy = (float)(event->y() - lastPos.y());
	float camElevation = camera->getCamElevation();

	if (event->buttons() & Qt::LeftButton) {			// Shift
		camera->changeXYZTranslation(-dx * camera->dz * 0.001f, dy * camera->dz * 0.001f, 0);
		lastPos = event->pos();
	} else if (event->buttons() & Qt::RightButton) {	// Zoom
		setCursor(Qt::SizeVerCursor);

		camera->changeXYZTranslation(0, 0, -dy * 10);
		if (camera->dz < 150.0f) camera->dz = 150.0f;

		// define the width per lane according to the z coordinate of the camera
		if (camera->dz < 350.0f) {
			roads->setWidthPerLane(2.0f);
		} else if (camera->dz < 700.0f) {
			roads->setWidthPerLane(4.0f);
		} else if (camera->dz < 1300.0f) {
			roads->setWidthPerLane(7.0f);
		} else if (camera->dz < 2500.0f) {
			roads->setWidthPerLane(10.0f);
		} else {
			roads->setWidthPerLane(16.0f);
		}

		lastPos = event->pos();
	}
	updateGL();
}

void GLWidget::initializeGL() {
	qglClearColor(QColor(233, 229, 220));
	glClearDepth(1.0f);

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//---- lighting ----
	glDisable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	static GLfloat lightPosition[4] = { 0.0f, 0.0f, 100.0f, 0.0f };
	static GLfloat lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static GLfloat lightDiffuse[4] = { 0.75f, 0.75f, 0.5f, 1.0f };

	glEnable(GL_COLOR_MATERIAL);	
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);	
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	static GLfloat materialSpecular[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static GLfloat materialEmission[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialEmission);
}

void GLWidget::resizeGL(int width, int height) {
	height = height ? height : 1;

	glViewport(0, 0, (GLint)width, (GLint)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLfloat)width/(GLfloat)height, 5.0f, 90000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera->applyCamTransform();
}

void GLWidget::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	camera->applyCamTransform();	

	drawScene();	
}

/**
 * Convert the screen space coordinate (x, y) to the model space coordinate.
 */
void GLWidget::mouseTo2D(int x,int y, QVector2D *result) {
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];

	// retrieve the matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	// retrieve the projected z-buffer of the origin
	GLdouble origX, origY, origZ;
	gluProject(0, 0, 0, modelview, projection, viewport, &origX, &origY, &origZ);

	// set up the projected point
	GLfloat winX = (float)x;
	GLfloat winY = (float)viewport[3] - (float)y;
	GLfloat winZ = origZ;
	
	// unproject the image plane coordinate to the model space
	GLdouble posX, posY, posZ;
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	result->setX(posX);
	result->setY(posY);
}