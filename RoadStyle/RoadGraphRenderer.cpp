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
	for (int i = 0; i < object->renderables.size(); i++) {
		if (object->renderables[i].type == GL_LINES || object->renderables[i].type == GL_LINE_STRIP) {
			glLineWidth(object->renderables[i].size);
		}

		if (object->renderables[i].type == GL_POINTS) {
			glPointSize(object->renderables[i].size);
		}

		glBegin(object->renderables[i].type);
		for (int j = 0; j < object->renderables[i].vertices.size(); ++j) {
			glColor3f(object->renderables[i].vertices[j].color[0], object->renderables[i].vertices[j].color[1], object->renderables[i].vertices[j].color[2]);
			glNormal3f(object->renderables[i].vertices[j].normal[0], object->renderables[i].vertices[j].normal[1], object->renderables[i].vertices[j].normal[2]);
			glVertex3f(object->renderables[i].vertices[j].location[0], object->renderables[i].vertices[j].location[1], object->renderables[i].vertices[j].location[2]);
		}
		glEnd();
	}
}