#include "GLWidget.h"
#include "RoadStyle.h"
#include "OSMRoadsParser.h"
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
	renderer = new RoadGraphRenderer();
}

GLWidget::~GLWidget() {
}

void GLWidget::drawScene() {
	renderer->render(roads, mainWin);
}

void GLWidget::loadOSM(QString filename) {
	roads->clear();

	FILE* fp = fopen(filename.toUtf8().data(), "rb");
	roads->load(fp, 7);

	camera->setLookAt(0.0f, 0.0f, 0.0f);
	camera->setTranslation(0.0f, 0.0f, 150.0f);
	roads->widthPerLane = 2.0f;

	updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
	this->setFocus();

	lastPos = event->pos();

	if (event->buttons() & Qt::LeftButton) {
		QVector2D pos;
		if (mouseTo2D(event->x(), event->y(), &pos)) {
			mainWin->ui.statusBar->showMessage(QString("clicked (%1, %2)").arg(pos.x()).arg(pos.y()));
			RoadEdge* selectedEdge = roads->select(pos);
			mainWin->propertyWindow->setRoadEdge(selectedEdge);
			updateGL();
		}
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

bool GLWidget::mouseTo2D(int x,int y, QVector2D *result) {
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	if(posZ<-10000.0f){//if it -10000 means we are not in the scene{
		// 2. ray test
		float zNear=1.0f,zFar= 10000.0f;
		GLdouble posXFar, posYFar, posZFar;
		gluUnProject( winX, winY, zNear, modelview, projection, viewport, &posX, &posY, &posZ);
		gluUnProject( winX, winY, zFar, modelview, projection, viewport, &posXFar, &posYFar, &posZFar);
		QVector3D rayStar(posX,posY,posZ);
		QVector3D rayEnd(posXFar,posYFar,posZFar);
		double t;
		QVector3D q1(0,0,1.0f);
		QVector3D q2(0,0,0);
		//if(planeIntersectWithLine(rayStar,rayEnd,q1,q2,t,*result)!=0)	return true;
		return false;
	}

	result->setX(posX);
	result->setY(posY);

	return true;
}