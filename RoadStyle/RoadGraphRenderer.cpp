#include "RoadGraphRenderer.h"
#include "RoadStyle.h"
#include <QtOpenGL>

RoadGraphRenderer::RoadGraphRenderer() {
}

void RoadGraphRenderer::render(RoadGraph* object, RoadStyle* mainWin) {
	if (!object->getModified()) {
		glCallList(dispList);
	} else {
		object->generateMesh(mainWin->getAttributes()->getBool("showHighways"), mainWin->getAttributes()->getBool("showAvenues"), mainWin->getAttributes()->getBool("showStreets"));

		glDeleteLists(dispList, 1);
		dispList = glGenLists(1);
		glNewList(dispList, GL_COMPILE_AND_EXECUTE);

		_render(object);

		glEndList();
	}
}

void RoadGraphRenderer::_render(RoadGraph* object) {
	glBegin(GL_TRIANGLES);
	for (int j = 0; j < object->getNumMeshes(); ++j) {
		glColor3f(object->getMesh(j).color[0], object->getMesh(j).color[1], object->getMesh(j).color[2]);
		glNormal3f(object->getMesh(j).normal[0], object->getMesh(j).normal[1], object->getMesh(j).normal[2]);
		glVertex3f(object->getMesh(j).location[0], object->getMesh(j).location[1], object->getMesh(j).location[2]);
	}
	glEnd();
}