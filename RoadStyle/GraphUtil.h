﻿#pragma once

#include "RoadGraph.h"
#include "BBox.h"
#include "AbstractForest.h"
#include <vector>
#include <opencv/cv.h>
#include <opencv/highgui.h>

class BFSForest;

class EdgePair {
public:
	RoadEdgeDesc edge1;
	RoadEdgeDesc edge2;

public:
	EdgePair(RoadEdgeDesc edge1, RoadEdgeDesc edge2);
};

class EdgePairComparison {
public:
	RoadGraph* roads1;
	RoadGraph* roads2;

public:
	EdgePairComparison(RoadGraph* roads1, RoadGraph* roads2);
	bool operator()(const EdgePair& left, const EdgePair& right) const;
};

class GraphUtil {
protected:
	GraphUtil() {}

public:
	// Vertex related functions
	static int getNumVertices(RoadGraph* roads, bool onlyValidVertex = true);
	static int getNumConnectedVertices(RoadGraph* roads, RoadVertexDesc start, bool onlyValidVertex = true);
	static RoadVertexDesc getVertex(RoadGraph* roads, int index, bool onlyValidVertex = true);
	static RoadVertexDesc getVertex(RoadGraph* roads, QVector2D pt, bool onlyValidVertex = true);
	static bool getVertex(RoadGraph* roads, QVector2D pos, float threshold, RoadVertexDesc& desc, bool onlyValidVertex = true);
	static bool getVertex(RoadGraph* roads, QVector2D pos, float threshold, RoadVertexDesc ignore, RoadVertexDesc& desc, bool onlyValidVertex = true);
	static int getVertexIndex(RoadGraph* roads, RoadVertexDesc desc, bool onlyValidVertex = true);
	static RoadVertexDesc addVertex(RoadGraph* roads, RoadVertex* v);
	static void moveVertex(RoadGraph* roads, RoadVertexDesc v, QVector2D pt);
	static void collapseVertex(RoadGraph* roads, RoadVertexDesc v1, RoadVertexDesc v2);
	static int getDegree(RoadGraph* roads, RoadVertexDesc v, bool onlyValidEdge = true);
	static std::vector<RoadVertexDesc> getVertices(RoadGraph* roads, bool onlyValidVertex = true);
	static void removeIsolatedVertices(RoadGraph* roads, bool onlyValidVertex = true);
	static void snapVertex(RoadGraph* roads, RoadVertexDesc v1, RoadVertexDesc v2);
	static RoadVertexDesc getCentralVertex(RoadGraph* roads);

	// Edge related functions
	static RoadEdgeDesc getEdge(RoadGraph* roads, int index, bool onlyValidEdge = true);
	static float getTotalEdgeLength(RoadGraph* roads, RoadVertexDesc v);
	static void collapseEdge(RoadGraph* roads, RoadEdgeDesc e);
	static int getNumEdges(RoadGraph* roads, bool onlyValidEdge = true);
	static RoadEdgeDesc addEdge(RoadGraph* roads, RoadVertexDesc src, RoadVertexDesc tgt, unsigned int lanes, unsigned int type, bool oneWay = false);
	static RoadEdgeDesc addEdge(RoadGraph* roads, RoadVertexDesc src, RoadVertexDesc tgt, RoadEdge* ref_edge);
	static bool hasEdge(RoadGraph* roads, RoadVertexDesc desc1, RoadVertexDesc desc2, bool onlyValidEdge = true);
	static RoadEdgeDesc getEdge(RoadGraph* roads, RoadVertexDesc src, RoadVertexDesc tgt, bool onlyValidEdge = true);
	static std::vector<QVector2D> getOrderedPolyLine(RoadGraph* roads, RoadEdgeDesc e);
	static void orderPolyLine(RoadGraph* roads, RoadEdgeDesc e, RoadVertexDesc src);
	static void moveEdge(RoadGraph* roads, RoadEdgeDesc e, QVector2D& src_pos, QVector2D& tgt_pos);
	static std::vector<RoadEdgeDesc> getMajorEdges(RoadGraph* roads, int num);
	static bool removeDeadEnd(RoadGraph* roads);
	static std::vector<QVector2D> interpolateEdges(RoadGraph* roads1, RoadEdgeDesc e1, RoadVertexDesc src1, RoadGraph* roads2, RoadEdgeDesc e2, RoadVertexDesc src2, float t);
	static void computeImportanceOfEdges(RoadGraph* roads, float w_length, float w_valence, float w_lanes);
	static float computeDissimilarityOfEdges(RoadGraph* roads1, RoadEdgeDesc e1, RoadGraph* roads2, RoadEdgeDesc e2);
	static void removeIsolatedEdges(RoadGraph* roads, bool onlyValidEdge = true);

	// The entire graph related functions
	static RoadGraph* copyRoads(RoadGraph* roads);
	static void copyRoads(RoadGraph* roads1, RoadGraph* roads2);
	static void mergeRoads(RoadGraph* roads1, RoadGraph* roads2);
	static BBox getAABoundingBox(RoadGraph* roads);
	static BBox getBoudingBox(RoadGraph* roads, float theta1, float theta2, float theta_step = 0.087f);
	static RoadGraph* extractMajorRoad(RoadGraph* roads, bool remove = true);
	static float extractMajorRoad(RoadGraph* roads, RoadEdgeDesc root, QList<RoadEdgeDesc>& path);

	// Connectivity related functions
	static std::vector<RoadVertexDesc> getNeighbors(RoadGraph* roads, RoadVertexDesc v, bool onlyValidVertex = true);
	static bool isNeighbor(RoadGraph* roads, RoadVertexDesc v1, RoadVertexDesc v2);
	static bool isConnected(RoadGraph* roads, RoadVertexDesc desc1, RoadVertexDesc desc2, bool onlyValidEdge = true);
	static RoadVertexDesc findConnectedNearestNeighbor(RoadGraph* roads, const QVector2D &pt, RoadVertexDesc v);
	static bool getEdge(RoadGraph* roads, const QVector2D &pt, float threshold, RoadEdgeDesc& e, bool onlyValidEdge = true);
	static RoadEdgeDesc findNearestEdge(RoadGraph* roads, RoadVertexDesc v, float& dist, QVector2D& closestPt, bool onlyValidEdge = true);

	// The road graph modification functions
	static void clean(RoadGraph* roads);
	static void reduce(RoadGraph* roads);
	static bool reduce(RoadGraph* roads, RoadVertexDesc desc);
	static void simplify(RoadGraph* roads, float dist_threshold);
	static void normalize(RoadGraph* roads);
	static void singlify(RoadGraph* roads);
	static void planarify(RoadGraph* roads);
	static bool planarifyOne(RoadGraph* roads);
	static void skeltonize(RoadGraph* roads);
	static void rotate(RoadGraph* roads, float theta);
	static void translate(RoadGraph* roads, QVector2D offset);
	static RoadGraph* convertToGridNetwork(RoadGraph* roads, RoadVertexDesc start);
	static RoadGraph* approximateToGridNetwork(RoadGraph* roads, float cellLength, QVector2D orig);
	static void scaleToBBox(RoadGraph* roads, BBox& area);
	static void normalizeBySpring(RoadGraph* roads, BBox& area);
	static bool removeDuplicateEdges(RoadGraph* roads);
	static void snapDeadendEdges(RoadGraph* roads, float threshold);
	static void snapDeadendEdges2(RoadGraph* roads, int degree, float threshold);
	static void removeShortDeadend(RoadGraph* roads, float threshold);

	// Others
	static float computeMinDiffAngle(std::vector<float> *data1, std::vector<float> *data2);
	static float normalizeAngle(float angle);
	static float diffAngle(const QVector2D& dir1, const QVector2D& dir2);
	static float diffAngle(float angle1, float angle2);

	// Compute similarity
	static float computeDissimilarity(RoadGraph* roads1, QMap<RoadVertexDesc, RoadVertexDesc>& map1, RoadGraph* roads2, QMap<RoadVertexDesc, RoadVertexDesc>& map2, float w_connectivity, float w_split, float w_angle, float w_distance);
	static float computeDissimilarity2(RoadGraph* roads1, QMap<RoadVertexDesc, RoadVertexDesc>& map1, RoadGraph* roads2, QMap<RoadVertexDesc, RoadVertexDesc>& map2, float w_matching, float w_split, float w_angle, float w_distance);
	static float computeSimilarity(RoadGraph* roads1, QMap<RoadVertexDesc, RoadVertexDesc>& map1, RoadGraph* roads2, QMap<RoadVertexDesc, RoadVertexDesc>& map2, float w_connectivity, float w_angle);
	static void findCorrespondenceByNearestNeighbor(RoadGraph* roads1, RoadGraph* roads2, QMap<RoadVertexDesc, RoadVertexDesc>& map1, QMap<RoadVertexDesc, RoadVertexDesc>& map2);
	static QMap<RoadVertexDesc, RoadVertexDesc> findCorrespondentEdges(RoadGraph* roads1, RoadVertexDesc parent1, std::vector<RoadVertexDesc> children1, RoadGraph* roads2, RoadVertexDesc parent2, std::vector<RoadVertexDesc> children2);
	static QMap<RoadVertexDesc, RoadVertexDesc> findApproximateCorrespondentEdges(RoadGraph* roads1, RoadVertexDesc parent1, std::vector<RoadVertexDesc> children1, RoadGraph* roads2, RoadVertexDesc parent2, std::vector<RoadVertexDesc> children2);
	static void findCorrespondence(RoadGraph* roads1, AbstractForest* forest1, RoadGraph* roads2, AbstractForest* forest2, bool findAllMatching, float threshold_angle, QMap<RoadVertexDesc, RoadVertexDesc>& map1, QMap<RoadVertexDesc, RoadVertexDesc>& map2);
	static bool forceMatching(RoadGraph* roads1, RoadVertexDesc parent1, AbstractForest* forest1, QMap<RoadVertexDesc, RoadVertexDesc>& map1, RoadGraph* roads2, RoadVertexDesc parent2, AbstractForest* forest2, QMap<RoadVertexDesc, RoadVertexDesc>& map2, RoadVertexDesc& child1, RoadVertexDesc& child2);

	static bool nextSequence(std::vector<int>& seq, int N);

	// Statistics
	static float computeAvgEdgeLength(RoadGraph* roads);
	static QList<EdgePair> getClosestEdgePairs(RoadGraph* roads1, RoadGraph* roads2, int num);

	// ICP
	static void rigidICP(RoadGraph* roads1, RoadGraph* roads2, QList<EdgePair>& pairs);
	static cv::Mat convertVerticesToCVMatrix(RoadGraph* roads, bool onlyValidVertex = true);
	static cv::Mat convertEdgesToCVMatrix(RoadGraph* roads, bool onlyValidVertex = true);

	// Sample road networks
	static RoadGraph* createGridNetwork(float size, int num);
	static RoadGraph* createCurvyNetwork(float size, int num, float angle);
	static RoadGraph* createRadialNetwork(float size, int num, int degree);

	// For debugging
	static void printStatistics(RoadGraph* roads);
};

