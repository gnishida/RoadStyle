#pragma once

#include "RoadVertex.h"
#include "RoadEdge.h"
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
public:
	BGLGraph graph;
	bool modified;
	std::vector<Vertex> vertices;
	RoadEdge* selectedEdge;

public:
	RoadGraph();
	~RoadGraph();

	void generateMesh(bool showHighways, bool showAvenues, bool showStreets);
	bool getModified();
	void setModified();
	void clear();

	QList<RoadEdgeDesc> getOrderedEdgesByImportance();

	RoadEdge* select(const QVector2D &pos);

	Vertex getMesh(unsigned int index);
	unsigned int getNumMeshes();

	void load(FILE* fp, int roadType);
};

class LessWeight {
private:
	RoadGraph* roads;

public:
	LessWeight(RoadGraph* roads);

	bool operator()(const RoadEdgeDesc& left, const RoadEdgeDesc& right) const;
};

class MoreImportantEdge {
private:
	RoadGraph* roads;

public:
	MoreImportantEdge(RoadGraph* roads);

	bool operator()(const RoadEdgeDesc& left, const RoadEdgeDesc& right) const;
};
