#pragma once

#include "RoadVertex.h"
#include "RoadEdge.h"
#include "Renderable.h"
#include <stdio.h>
#include <qvector2d.h>
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

class RoadStyle;

class RoadGraph {
public:
	BGLGraph graph;
	bool modified;
	std::vector<Renderable> renderables;

	RoadVertex* selectedVertex;
	RoadEdge* selectedEdge;
	int selectedEdgeIndex;
	float widthPerLane;

public:
	RoadGraph();
	~RoadGraph();

	void generateMesh(bool showHighways, bool showAvenues, bool showStreets, float highwayHeight, float avenueHeight, float curbRatio, bool drawLocalStreets);
	void RoadGraph::addMeshFromEdge(Renderable* renderable, RoadEdge* edge, float widthFactor, QColor color, float height);

	bool getModified();
	void setModified();
	void clear();
	void setWidthPerLane(float widthperLane);

	QList<RoadEdgeDesc> getOrderedEdgesByImportance();

	RoadEdge* selectEdge(const QVector2D &pos);
	RoadVertex* selectVertex(const QVector2D &pos);

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
