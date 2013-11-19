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
	roadGraph = new RoadGraph(mainWin);
	renderer = new RoadGraphRenderer();
}

GLWidget::~GLWidget() {
}

void GLWidget::drawScene() {
	renderer->render(roadGraph);
}

void GLWidget::loadOSM(QString filename) {
	roadGraph->clear();

	/*OSMRoadsParser* parser = new OSMRoadsParser(roadGraph);

    QXmlSimpleReader reader;
    reader.setContentHandler(parser);
	QFile file(filename);
	QXmlInputSource source(&file);
	reader.parse(source);

	roadGraph->reduce();*/
	FILE* fp = fopen(filename.toUtf8().data(), "rb");
	roadGraph->load(fp);

	camera->setLookAt(0.0f, 0.0f, 0.0f);
	//camera->setTranslation(0.0f, 0.0f, roadGraph->getRanges().getBBox().size().y());
	camera->setTranslation(0.0f, 0.0f, 100.0f);
	updateGL();
}

RoadGraph* GLWidget::getRoadGraph() {
	return roadGraph;
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
	this->setFocus();

	lastPos = event->pos();

	if (event->buttons() & Qt::LeftButton) {
		QVector3D pos;
		if (mouseTo3D(event->x(), event->y(), &pos)) {
			mainWin->ui.statusBar->showMessage(QString("clicked (%1, %2)").arg(pos.x()).arg(pos.y()));
			RoadEdge* selectedEdge = roadGraph->select(pos);
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

	if (event->buttons() & Qt::LeftButton) {	// Rotate
		setCursor(Qt::SizeVerCursor);

		camera->changeXRotation(dy * 3);
		camera->changeZRotation(-dx * 3);
		lastPos = event->pos();
	} else if (event->buttons() & Qt::MidButton) {
		camera->changeXYZTranslation(-dx * 10, dy * 10, 0);
		lastPos = event->pos();
	} else if (event->buttons() & Qt::RightButton) {	// Zoom
		setCursor(Qt::SizeVerCursor);

		camera->changeXYZTranslation(0, 0, -dy * 10);
		lastPos = event->pos();
	}
	updateGL();
}

void GLWidget::initializeGL() {
	qglClearColor(QColor(128, 128, 128));
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

bool GLWidget::mouseTo3D(int x,int y,QVector3D *result) {
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
	result->setZ(posZ);
	return true;
}