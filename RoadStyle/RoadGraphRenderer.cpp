#include "RoadGraphRenderer.h"
#include "RoadStyle.h"
#include <QtOpenGL>

RoadGraphRenderer::RoadGraphRenderer() {
}

void RoadGraphRenderer::render(std::vector<Renderable>& renderables) {
	/*if (!object->getModified()) {
		glCallList(dispList);
	} else {
		object->generateMesh(mainWin->getAttributes()->getBool("showHighways"), mainWin->getAttributes()->getBool("showAvenues"), mainWin->getAttributes()->getBool("showStreets"));
		*/
		//glDeleteLists(dispList, 1);
		//dispList = glGenLists(1);
		//glNewList(dispList, GL_COMPILE_AND_EXECUTE);

		_render(renderables);

		//glEndList();
	//}
}

void RoadGraphRenderer::_render(std::vector<Renderable>& renderables) {
	for (int i = 0; i < renderables.size(); i++) {
		if (renderables[i].type == GL_LINES || renderables[i].type == GL_LINE_STRIP) {
			glLineWidth(renderables[i].size);
		}

		if (renderables[i].type == GL_POINTS) {
			glPointSize(renderables[i].size);
		}

		glBegin(renderables[i].type);
		for (int j = 0; j < renderables[i].vertices.size(); ++j) {
			glColor3f(renderables[i].vertices[j].color[0], renderables[i].vertices[j].color[1], renderables[i].vertices[j].color[2]);
			glNormal3f(renderables[i].vertices[j].normal[0], renderables[i].vertices[j].normal[1], renderables[i].vertices[j].normal[2]);
			glVertex3f(renderables[i].vertices[j].location[0], renderables[i].vertices[j].location[1], renderables[i].vertices[j].location[2]);
		}
		glEnd();
	}
}