#pragma once

#include "RoadVertex.h"
#include "RoadEdge.h"
#include "AABBox.h"
#include "MapRange.h"
#include <stdio.h>
#include <qvector3d.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/graph_utility.hpp>

using namespace boost;


typedef adjacency_list<vecS, vecS, undirectedS, RoadVertex*, RoadEdge*> BGLGraph;
typedef graph_traits<BGLGraph>::vertex_descriptor RoadVertexDesc;
typedef graph_traits<BGLGraph>::edge_descriptor RoadEdgeDesc;
typedef graph_traits<BGLGraph>::vertex_iterator RoadVertexIter;
typedef graph_traits<BGLGraph>::edge_iterator RoadEdgeIter;
typedef graph_traits<BGLGraph>::out_edge_iterator RoadOutEdgeIter;
typedef graph_traits<BGLGraph>::in_edge_iterator RoadInEdgeIter;

typedef struct {
	float location[3];
	float tex[2];
	float normal[3];
	float color[4];
	char padding[16];
} Vertex;

class RoadStyle;

class RoadGraph {
private:
	RoadStyle* mainWin;
	BGLGraph graph;
	//AABBox bbox;
	MapRange ranges;
	bool modified;
	std::vector<Vertex> vertices;
	RoadEdge* selectedEdge;

public:
	RoadGraph(RoadStyle* mainWin);
	~RoadGraph();

	BGLGraph& getGraph();
	//void setBBox(AABBox bbox);
	void setRange(MapRange ranges);
	void generateMesh();
	RoadVertexDesc addVertex(RoadVertex* vertex);
	RoadVertex* getVertex(RoadVertexDesc desc);
	void removeVertex(RoadVertexDesc desc);
	bool reduceVertex(RoadVertexDesc desc);
	RoadEdgeDesc addEdge(RoadVertexDesc v1, RoadVertexDesc v2, RoadEdge* edge);
	//AABBox& getBBox();
	MapRange& getRanges();
	bool getModified();
	void setModified();
	void clear();
	void reduce();

	std::vector<std::vector<float> > extractFeatures(QString filename);
	std::vector<float> extractFeatures(AABBox area);
	RoadEdge* select(const QVector3D &pos);

	Vertex getMesh(unsigned int index);
	unsigned int getNumMeshes();

	void load(FILE* fp);
};

