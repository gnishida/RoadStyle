/************************************************************************************************
*
*		MTC Project - Geometry Project - RoadGraph Class
*
*
*		@desc Class to create road graph from OSM
*		@author igarciad
*
************************************************************************************************/

#include "roadGraphOSM.h"
#include "Geometry/client_geometry.h"
#include "client_main_traffic.h"
//#include "..\PM\pmVegetation.h"

#define WIDTH_WATER 4000.0f

namespace LC {

	extern ClientGeometry *cgPtrTraff;
	extern ClientMainTraffic *clientMainTraffic;

	/*LatLonToWorldMercator
	// gis.stackexchange.com/questions/15269/how-to-convert-lat-long-to-meters-using-mercator-projection-in-c
	Converts a latitude/longitude pair to x and y coordinates in the
	World Mercator projection.

	Inputs:
	lat   - Latitude of the point.
	lon   - Longitude of the point.
	isDeg - Whether the given latitude and longitude are in degrees. If False 
	(default) it is assumed they are in radians.

	Returns:
	x,y - A 2-element tuple with the World Mercator x and y values.
	*/

	static QVector3D projLatLonToWorldMercator(float lat,float lon,bool isDeg=false){
		// Ellipsoid model constants (actual values here are for WGS84) 
		const float sm_a = 6378137.0f; 
		const float sm_b = 6356752.314f;

		QVector3D result;
		float lon0 = 0;
		if (isDeg==true){
			lat = lat/180.0f * M_PI;
			lon = lon/180.0f * M_PI;
		}
		result.setX(sm_a*(lon-lon0));
		result.setY(sm_a*log((sin(lat)+1)/cos(lat)));

		return  result; 
	}//

	struct roadEdge{
		uint source;
		uint destination;
		std::vector<uint> nodes;

		uint numLanes;
		uint roadType;
		bool oneWay;
	};

	

	void updateMinMax(QVector3D& newPoint,QVector3D& minBox,QVector3D& maxBox){
		if(newPoint.x()<minBox.x()){
			minBox.setX(newPoint.x());
		}
		if(newPoint.y()<minBox.y()){
			minBox.setY(newPoint.y());
		}
		if(newPoint.x()>maxBox.x()){
			maxBox.setX(newPoint.x());
		}
		if(newPoint.y()>maxBox.y()){
			maxBox.setY(newPoint.y());
		}
	}//

	void renderNonConvexOSM(LC::RoadGraphOSM::polygon& contour,
			QVector3D& myNormal,QVector3D& color,float height)
		{
			//const float height=0.0f;
			glColor3ub(color.x(),color.y(),color.z());
			//Render inside fill			
			if((contour.outer()).size() == 3){
				glBegin(GL_TRIANGLES);	
				for(size_t i=0; i<(contour.outer()).size(); i++){	
					glNormal3f(myNormal.x(), myNormal.y(), myNormal.z());
					glVertex3f((contour.outer())[i].x(), (contour.outer())[i].y(), height);//(contour.outer())[i].z());			
				}
				glEnd();
			} else if((contour.outer()).size() == 4){
				glBegin(GL_QUADS);	
				for(int i=0; i<(contour.outer()).size(); i++){	
					glNormal3f(myNormal.x(), myNormal.y(), myNormal.z());
					glVertex3f((contour.outer())[i].x(), (contour.outer())[i].y(),height);// (contour.outer())[i].z());			
				}
				glEnd();
			} else {

				// create tessellator
				GLUtesselator *tess = gluNewTess();

				//double *vtxData = new double[3*(contour.outer()).size()];
				int numVertex=0;
				for(int nI=0;nI<contour.inners().size();nI++){
					numVertex+=contour.inners()[nI].size();
				}
				double *vtxData = new double[3*((contour.outer()).size()+numVertex)];
				for(size_t i=0; i<(contour.outer()).size(); i++){
					vtxData[3*i]=(contour.outer())[i].x();
					vtxData[3*i+1]=(contour.outer())[i].y();
					vtxData[3*i+2]=height;//(contour.outer())[i].z();
				}
				int index=(contour.outer()).size();
				for(int nI=0;nI<contour.inners().size();nI++){
					for(int pI=contour.inners()[nI].size()-1;pI>=0/*<contour.inners()[nI].size()*/;/*pI++*/pI--){
						vtxData[index]=(contour.inners()[nI])[pI].x();
						vtxData[index+1]=(contour.inners()[nI])[pI].y();
						vtxData[index+2]=height;//(contour.outer())[i].z();
						index+=3;
					}

				}

				// register callback functions
				gluTessCallback(tess, GLU_TESS_BEGIN, 
					(void (__stdcall *)(void))glBegin);
				gluTessCallback(tess, GLU_TESS_VERTEX,
					(void (__stdcall *)(void))glVertex3dv);
				gluTessCallback(tess, GLU_TESS_END, glEnd);

				// describe non-convex polygon
				gluTessBeginPolygon(tess, NULL);

				// contour
				gluTessBeginContour(tess);

				for(size_t i=0; i<(contour.outer()).size()+numVertex; i++){
					//HACK
					glNormal3f(myNormal.x(), myNormal.y(), fabs(myNormal.z()));
					gluTessVertex(tess, &vtxData[3*i], &vtxData[3*i]);
				}
				gluTessEndContour(tess);
				gluTessEndPolygon(tess);

				// delete tessellator after processing
				gluDeleteTess(tess);

				delete [] vtxData;
			}
			
			glColor4f(0,0,0,1.0f);//color.x()/255.0f,color.y()/255.0f,color.z()/255.0f,1.0f);
			glBegin(GL_LINE_LOOP);
			for(size_t i=0; i<(contour.outer()).size(); i++){
				glVertex3f((contour.outer())[i].x(),(contour.outer())[i].y(),height);
			}
			glEnd();
		}//

	void RoadGraphOSM::readRoadNetworkFromOSM(RoadGraph& inRoadGraph,QString fileName){
		printf(">>readRoadNetworkFromOSM\n");
		QFile* file = new QFile(fileName);//"data/map.osm");
		//QFile* file = new QFile("data/madrid.osm");
		// 1. Check if file exists
		if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
			/*QMessageBox::critical(this,
			"QXSRExample::parseXML",
			"Couldn't open example.xml",
			QMessageBox::Ok);*/
			printf("ERROR: file does not exit %s\n",fileName.toAscii().constData());
			return;
		}
		// 2. Create dom document
		QDomDocument doc("mydocument"); 
		if (!doc.setContent(file)){         
			printf("Error: Reading document\n");
			return;      
		} 

		//Get the root element
		QDomElement docElem = doc.documentElement(); 

		// you could check the root tag name here if it matters
		QString rootTag = docElem.tagName(); // == persons
		printf("rootTag %s\n",rootTag.toAscii().constData());

		// get the node's interested in, this time only caring about person's
		QDomNodeList nodeList = docElem.elementsByTagName("way");

		/////////////////////////////
		// 3. Process each road
		int numStreets=0;
		std::vector<roadEdge> roadsOSM;
		QSet<uint> nodesOSM;
		QSet<uint> nodesMiddleOSM;
		for(int ii = 0;ii < nodeList.count(); ii++){
			// get the current one as QDomElement
			QDomElement el = nodeList.at(ii).toElement();
			bool isStreet=false;
			bool oneWay=false;
			uint numLanes=1;
			uint typeRoad=1;//0 pedestrian 1 ground 2 secondary 3 primary 4 motor
			QDomNodeList nodeListTag = el.elementsByTagName("tag");
			for(int iT = 0;iT < nodeListTag.count(); iT++){
				QDomElement elTag = nodeListTag.at(iT).toElement();
				//QString tagNam="k";
				QString attTag=elTag.attribute("k");
				// road
				if(attTag=="highway"){
					isStreet=true;
					QString attV=elTag.attribute("v");
					if(attV=="motorway"||attV=="motorway_link"||attV=="trunk"||attV=="trunk"){
						typeRoad=4;
					}
					if(attV=="primary"||attV=="primary_link"){
						typeRoad=3;
					}
					if(attV=="secondary"||attV=="secondary_link"){
						typeRoad=2;
					}
					if(attV=="residential"||attV=="tertiary"||attV=="tertiary_link"||attV=="living_street"||attV=="unclassified"||attV=="living_street"){
						typeRoad=1;
					}
					if(attV=="pedestrian"||attV=="track"||attV=="footway"){
						typeRoad=0;
					}
				}
				// one way
				if(attTag=="oneway"){
					QString attV=elTag.attribute("v");
					if(attV=="yes"){
						oneWay=true;
						//printf("One way\n");
					}
				}
				// num lanes
				if(attTag=="lanes"){
					QString attV=elTag.attribute("v");
					numLanes=attV.toInt();
				}
			}
			// it is a street
			if(isStreet==true&&typeRoad!=0){
				numStreets++;
				roadEdge tempRoad;
				tempRoad.oneWay=oneWay;
				tempRoad.numLanes=numLanes;
				tempRoad.roadType=typeRoad;
				
				/*QDomNodeList nodeListNode = el.elementsByTagName("nd");
				for(int iN = 0;iN < nodeListNode.count(); iN++){
					QDomElement elNd= nodeListTag.at(iN).toElement();
					//QString tagNam="k";
					uint attTag=elNd.attribute("ref").toUInt();
				}*/
				// take the first and last node
				QDomNodeList nodeListNode = el.elementsByTagName("nd");
				tempRoad.source=nodeListNode.at(0).toElement().attribute("ref").toUInt();
				tempRoad.destination=nodeListNode.at(nodeListNode.size()-1).toElement().attribute("ref").toUInt();
				////
				for(int iN = 0;iN < nodeListNode.count(); iN++){//all
					uint midId=nodeListNode.at(iN).toElement().attribute("ref").toUInt();
					tempRoad.nodes.push_back(midId);
					nodesMiddleOSM.insert(midId);
				}
				//
				nodesOSM.insert(tempRoad.source);
				nodesOSM.insert(tempRoad.destination);
				roadsOSM.push_back(tempRoad);
			}
		}
		printf("numStreetsOSM %d\n",numStreets);
		/////////////////////////////
		// 4. Process each node
		std::vector<QVector3D> nodesPos;
		QMap<uint,uint> nodeIdToVertexId;//from OSM Id to Vertex Id
		QHash<uint,QVector3D> nodeMidIdToPos3D;//from OSM Id to Position 3D
		uint vertexId=0;
		QVector3D accPoint;

		QDomNodeList nodeListNodes = docElem.elementsByTagName("node");
		for(int iN = 0;iN < nodeListNodes.count(); iN++){
			// actual nodes
			uint id=nodeListNodes.at(iN).toElement().attribute("id").toUInt();
			if(nodesOSM.find(id)!=nodesOSM.end()){//node in roadOSM
				float lat=nodeListNodes.at(iN).toElement().attribute("lat").toFloat();
				float lon=nodeListNodes.at(iN).toElement().attribute("lon").toFloat();
				QVector3D nodePos=projLatLonToWorldMercator(lat,lon,true);
				nodePos.setZ(0);
				// check if very close to other nodes
				uint nN;
				bool veryClose=false;
				for(nN=0;nN<nodesPos.size();nN++){
					if((nodesPos[nN]-nodePos).lengthSquared()<4.0f){
						veryClose=true;
						break;
					}
				}
				if(veryClose==true){
					nodeIdToVertexId.insert(id,nN);
				}else{
					nodeIdToVertexId.insert(id,vertexId);
					nodeMidIdToPos3D[id]=nodePos;
					nodesPos.push_back(nodePos);
					vertexId++;
					//updateMinMax(nodePos,minBox,maxBox);
					accPoint+=nodePos;
				}
			}
			// mid nodes
			if(nodesMiddleOSM.find(id)!=nodesMiddleOSM.end()){//node in nodesMiddleOSM
				float lat=nodeListNodes.at(iN).toElement().attribute("lat").toFloat();
				float lon=nodeListNodes.at(iN).toElement().attribute("lon").toFloat();
				QVector3D nodePos=projLatLonToWorldMercator(lat,lon,true);
				// check if very close to other nodes
				nodeMidIdToPos3D[id]=nodePos;
				//updateMinMax(nodePos,minBox,maxBox);
			}
		}
		accPoint/=nodesPos.size();
		printf("Num of Nodes Before %d After %u %d\n",nodesOSM.size(),vertexId,nodesPos.size());
		printf("Avg point %f %f\n",accPoint.x(),accPoint.y());
		
		/////////////////////////////
		// 5. Convert to new Graph
		inRoadGraph.myRoadGraph.clear();
		inRoadGraph.myRoadGraph_BI.clear();
	
		// 5.1 Add vertex
		for(int nN=0;nN<nodesPos.size();nN++){
			nodesPos[nN]-=accPoint;//less accPoint
			RoadGraph::roadGraphVertexDesc vDesc0=boost::add_vertex(inRoadGraph.myRoadGraph_BI);
			inRoadGraph.myRoadGraph_BI[vDesc0].pt=nodesPos[nN];//QVector3D(0,0,15.0f);
			//printf("Pos %f %f %f\n",inRoadGraph.myRoadGraph[vDesc0].pt.x(),inRoadGraph.myRoadGraph[vDesc0].pt.y(),inRoadGraph.myRoadGraph[vDesc0].pt.z());
		}
		numStreets=0;
		float totalRoadLength=0;
		for(int nR=0;nR<roadsOSM.size();nR++){
			// start and end
			uint sourceId=nodeIdToVertexId[roadsOSM[nR].source];
			//uint destinationId=nodeIdToVertexId[roadsOSM[nR].destination];
			
			float totalEdgeLength=0;
			std::vector<QVector3D> tempRoadGeometry;

			for(int iN=0;iN<roadsOSM[nR].nodes.size();iN++){
			
				QVector3D pos=nodeMidIdToPos3D[roadsOSM[nR].nodes[iN]];
				//pos-=minBox;
				//pos.setZ(0.0f);
				//pos+=QVector3D(-(maxBox.x()-minBox.x())/2.0f,-(maxBox.y()-minBox.y())/2.0f,0.0f);
				pos-=accPoint;
				tempRoadGeometry.push_back(pos);
				
				if(tempRoadGeometry.size()>1){
					//// check if pos is a vertex
					bool pointIsNode=false;
					int nN;
					for(nN=0;nN<nodesPos.size();nN++){
						//if((nodesPos[nN].x()-pos.x()).lengthSquared()<2.0f){
						if(((nodesPos[nN].x()-pos.x())*(nodesPos[nN].x()-pos.x())+(nodesPos[nN].y()-pos.y())*(nodesPos[nN].y()-pos.y()))<2.0f){
							pointIsNode=true;
							break;
						}
					}
					totalEdgeLength+=(tempRoadGeometry[tempRoadGeometry.size()-1]-tempRoadGeometry[tempRoadGeometry.size()-2]).length();
					if(pointIsNode==true){
						if(sourceId==nN){//just same nodes
							totalEdgeLength=0;
							continue;
						}
						std::pair<RoadGraph::roadGraphEdgeDesc_BI, bool> e0_pair = boost::add_edge(  sourceId, nN, inRoadGraph.myRoadGraph_BI );
						inRoadGraph.myRoadGraph_BI[e0_pair.first].numberOfLanes=roadsOSM[nR].numLanes;
						//printf("[%d] numLanes %d type %u\n",nR,roadsOSM[nR].numLanes,roadsOSM[nR].roadType);
						//segment geometry
						inRoadGraph.myRoadGraph_BI[e0_pair.first].edgeLength =totalEdgeLength;
						totalRoadLength+=(totalEdgeLength*roadsOSM[nR].numLanes);
						inRoadGraph.myRoadGraph_BI[e0_pair.first].roadSegmentGeometry=tempRoadGeometry;
						inRoadGraph.myRoadGraph_BI[e0_pair.first].maxSpeedMperSec =LC::misctools::Global::global()->cuda_arterial_edges_speed_ms;
						switch(roadsOSM[nR].roadType){
						case 0:
							//inRoadGraph.myRoadGraph_BI[e0_pair.first].numberOfLanes=0;//0 if it is walk path
							inRoadGraph.myRoadGraph_BI[e0_pair.first].maxSpeedMperSec=0.01f;
							break;
						case 1:
							inRoadGraph.myRoadGraph_BI[e0_pair.first].maxSpeedMperSec=LC::misctools::Global::global()->cuda_arterial_edges_speed_ms/2.0f;
							break;
						case 2:
							inRoadGraph.myRoadGraph_BI[e0_pair.first].maxSpeedMperSec=LC::misctools::Global::global()->cuda_arterial_edges_speed_ms;
							break;
						case 3:
							inRoadGraph.myRoadGraph_BI[e0_pair.first].maxSpeedMperSec=1.5f*LC::misctools::Global::global()->cuda_arterial_edges_speed_ms;
							break;
						case 4:
							inRoadGraph.myRoadGraph_BI[e0_pair.first].maxSpeedMperSec=2.0f*LC::misctools::Global::global()->cuda_arterial_edges_speed_ms;
							break;
						}
						if(roadsOSM[nR].oneWay==false){
							std::pair<RoadGraph::roadGraphEdgeDesc_BI, bool> e1_pair = boost::add_edge(  nN,sourceId, inRoadGraph.myRoadGraph_BI );
							inRoadGraph.myRoadGraph_BI[e1_pair.first].numberOfLanes=inRoadGraph.myRoadGraph_BI[e0_pair.first].numberOfLanes;
							inRoadGraph.myRoadGraph_BI[e1_pair.first].edgeLength=inRoadGraph.myRoadGraph_BI[e0_pair.first].edgeLength;
							inRoadGraph.myRoadGraph_BI[e1_pair.first].roadSegmentGeometry=inRoadGraph.myRoadGraph_BI[e0_pair.first].roadSegmentGeometry;
							inRoadGraph.myRoadGraph_BI[e1_pair.first].maxSpeedMperSec=inRoadGraph.myRoadGraph_BI[e0_pair.first].maxSpeedMperSec;
						}
						totalEdgeLength=0;
						sourceId=nN;//new sourceId is the last one
						numStreets++;
					}
				}
			}
		}
		printf("numStreets %d Nodes %d Length %f\n",numStreets,nodeIdToVertexId.size()+nodeMidIdToPos3D.size(),totalRoadLength);
		////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////
		/*// 6. Process buildings
		int numBuildings=0;
		std::vector<buildingInfoStruct> buildingsOSM;
		QSet<uint> nodesBuildingOSM;
		QDomNodeList nodeListBuilding = docElem.elementsByTagName("way");
		for(int ii = 0;ii < nodeListBuilding.count(); ii++){
			// get the current one as QDomElement
			QDomElement el = nodeListBuilding.at(ii).toElement();
			bool isBuilding=false;
			QDomNodeList nodeListTag = el.elementsByTagName("tag");
			for(int iT = 0;iT < nodeListTag.count(); iT++){
				QDomElement elTag = nodeListTag.at(iT).toElement();
				//QString tagNam="k";
				QString attTag=elTag.attribute("k");
				if(attTag=="building"){
					QString attV=elTag.attribute("v");
					if(attV=="yes"){
						isBuilding=true;
						break;
					}
				}
			}
			// it is a building
			if(isBuilding==true){
				numBuildings++;
				buildingInfoStruct tempBldg;
				QDomNodeList nodeListNode = el.elementsByTagName("nd");
				for(int iN = 0;iN < nodeListNode.count(); iN++){//all
					uint midId=nodeListNode.at(iN).toElement().attribute("ref").toUInt();
					tempBldg.nodes.push_back(midId);
					nodesBuildingOSM.insert(midId);
				}
				buildingsOSM.push_back(tempBldg);
				//
			}
		}
		printf("numBuildings %d\n",numBuildings);
		////////////////////////////////////
		// 6.2 Convert ids to positions
		QHash<uint,QVector3D> nodeBldgIdToPos3D;
		QDomNodeList nodeListNodesBuilding = docElem.elementsByTagName("node");
		for(int iN = 0;iN < nodeListNodesBuilding.count(); iN++){
			// actual nodes
			uint id=nodeListNodesBuilding.at(iN).toElement().attribute("id").toUInt();
			if(nodesBuildingOSM.find(id)!=nodesOSM.end()){//nodes in nodesBuildingOSM
				float lat=nodeListNodes.at(iN).toElement().attribute("lat").toFloat();
				float lon=nodeListNodes.at(iN).toElement().attribute("lon").toFloat();
				QVector3D nodePos=projLatLonToWorldMercator(lat,lon,true);
				//nodePos-=minBox;
				//nodePos.setZ(0.0);
				//nodePos+=QVector3D(-(maxBox.x()-minBox.x())/2.0f,-(maxBox.y()-minBox.y())/2.0f,0.0f);
				nodePos.setZ(0);
				nodePos-=accPoint;
				nodeBldgIdToPos3D.insert(id,nodePos);
			}
		}
		////////////////////////////////////
		// 6.3 Create buildings
		cgPtrTraff->geoZone.clear();
		cgPtrTraff->geoVegetation.resetMe();
		// 6.3.1 Create buildings
		osmBuildings.resize(buildingsOSM.size());
		osmBuildingsAtt.resize(buildingsOSM.size());
		for(int bN=0;bN<buildingsOSM.size();bN++){
			osmBuildings[bN].buildingFootprint.contour.clear();
			// create geometry
			for(int iN=0;iN<buildingsOSM[bN].nodes.size();iN++){
				QVector3D pos=nodeBldgIdToPos3D[buildingsOSM[bN].nodes[iN]];
				osmBuildings[bN].buildingFootprint.contour.push_back(pos);
				//qDebug()<<pos;
			}
			//osmBuildings[bN].buildingFootprint.contour.push_back(osmBuildings[bN].buildingFootprint.contour[0]);//close polygon
			//LC::misctools::Polygon3D::cleanLoop(pContourCpy, inParcel.parcelContour.contour, 1.0f);
			LC::misctools::Polygon3D::reorientFace(osmBuildings[bN].buildingFootprint.contour);

			float buildingHeight=5.0f+((float)qrand()/RAND_MAX)*25.0f+osmBuildings[bN].buildingFootprint.contour[0].z();
			LC::ProceduralBuildingsMachine::generateBuildingGeometry(osmBuildings[bN].buildingFootprint,
					buildingHeight, osmBuildings[bN].buildingFaces);
			// create att
			int colorF=qrand()%100+20;
			int colorR=colorF+qrand()%30+10;
			osmBuildingsAtt[bN].att_facade_color=QColor(colorF,colorF,colorF);
			osmBuildingsAtt[bN].att_roof_color=QColor(colorR,colorR,colorR);
			//break;////!!!!!!!
		}*/
		////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////
		// 7. Process parks
		int numParks=0;
		std::vector<buildingInfoStruct> parksOSM;
		QSet<uint> nodesParksOSM;
		QDomNodeList nodeListParks = docElem.elementsByTagName("way");
		for(int ii = 0;ii < nodeListParks.count(); ii++){
			// get the current one as QDomElement
			QDomElement el = nodeListParks.at(ii).toElement();
			bool isPark=false;
			QDomNodeList nodeListTag = el.elementsByTagName("tag");
			for(int iT = 0;iT < nodeListTag.count(); iT++){
				QDomElement elTag = nodeListTag.at(iT).toElement();
				//QString tagNam="k";
				QString attTag=elTag.attribute("k");
				if(attTag=="leisure"){
					QString attV=elTag.attribute("v");
					if(attV=="park"){
						isPark=true;
						break;
					}
				}
			}
			// it is a building
			if(isPark==true){
				numParks++;
				buildingInfoStruct tempBldg;
				QDomNodeList nodeListNode = el.elementsByTagName("nd");
				for(int iN = 0;iN < nodeListNode.count(); iN++){//all
					uint midId=nodeListNode.at(iN).toElement().attribute("ref").toUInt();
					tempBldg.nodes.push_back(midId);
					nodesParksOSM.insert(midId);
				}
				parksOSM.push_back(tempBldg);
				//
			}
		}
		printf("numBuildings %d\n",numParks);
		////////////////////////////////////
		// 6.2 Convert ids to positions
		{
			QHash<uint,QVector3D> nodeBldgIdToPos3D;
			QDomNodeList nodeListNodesBuilding = docElem.elementsByTagName("node");
			for(int iN = 0;iN < nodeListNodesBuilding.count(); iN++){
				// actual nodes
				uint id=nodeListNodesBuilding.at(iN).toElement().attribute("id").toUInt();
				if(nodesParksOSM.find(id)!=nodesOSM.end()){//nodes in nodesBuildingOSM
					float lat=nodeListNodes.at(iN).toElement().attribute("lat").toFloat();
					float lon=nodeListNodes.at(iN).toElement().attribute("lon").toFloat();
					QVector3D nodePos=projLatLonToWorldMercator(lat,lon,true);
					nodePos.setZ(0);
					nodePos-=accPoint;
					nodeBldgIdToPos3D.insert(id,nodePos);
				}
			}
			////////////////////////////////////
			// 6.3 Create parks
			createParksFromOSM(parksOSM,nodeBldgIdToPos3D);
			///////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////
			// 7. Terrain

			polygon tPoly;
			for(int nN=0;nN<nodesPos.size();nN++){
				boost::geometry::append(tPoly,mPoint (nodesPos[nN].x(),nodesPos[nN].y()) );
			}
			
			polygon hull;
			polygonP hullP;
			boost::geometry::convex_hull(tPoly, hull);
			std::vector<pointP> hullPPoints;
			float zVal=LC::misctools::Global::global()->boundingPolygon[0].z();
			LC::misctools::Global::global()->boundingPolygon.clear();
			for(int hN=0;hN<hull.outer().size();hN++){
				QVector3D pHu=QVector3D(hull.outer()[hN].x(),hull.outer()[hN].y(),zVal);
				LC::misctools::Global::global()->boundingPolygon.push_back(pHu);
				pointP hP=boost::polygon::construct<pointP>(hull.outer()[hN].x(),hull.outer()[hN].y());
				hullPPoints.push_back(hP);
			}
			boost::polygon::set_points(hullP,hullPPoints.begin(),hullPPoints.end());
			using namespace boost::polygon::operators;
			blockSet+=hullP;
			///////////////////////////////////////////////////////////////////////////////////////
			//////////////////
			// 7. Blocks
			bool blockUsingBoost=false;
			bool blockUsinRender=true;
			if(blockUsingBoost==true){
				generateBlocksUsingBoost(inRoadGraph);

			}
			if(blockUsinRender==true){
				generateBlocksUsingRender();
			}
			printf("numBlocks %d\n",blockVec.size());
		}

		printf("<<readRoadNetworkFromOSM\n");
	}//

	void RoadGraphOSM::renderOSMBuildings(){
		//printf(">>renderOSMBuildings\n");
		glEnable(GL_LIGHTING);

		// BUILDINGS

		if(myParametricBuildings.size()>=0&&LC::misctools::Global::global()->selectedParametricBuildings==true){

			for(int bN=0;bN<myParametricBuildings.size();bN++){
				if(abs(myParametricBuildings[bN]->buildingX)<3.0f||abs(myParametricBuildings[bN]->buildingY)<3.0f) continue;
				glEnable(GL_LIGHTING);
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				glColor3f(1.0f,1.0f,1.0f);

				myParametricBuildings[bN]->render();
			}

		}else{
			//BOX BUILDINGS
			for(int bN=0;bN<osmBuildings.size();bN++){
				//printf("bN %d\n",bN);
				osmBuildings[bN].renderMyGeometry( &osmBuildingsAtt[bN] );
				//osmBuildings[bN].renderFloor();
			}
		}

		glDisable(GL_LIGHTING);

		for(int blN=0;blN<blockVec.size();blN++){
			renderNonConvexOSM(blockVec[blN],QVector3D(0,0,1.0f),QVector3D(255.0f,0,0),35.0f);
		}

		/*std::vector<polygonP> allP;
		boost::polygon::get_trapezoids(allP,blockSet);
		
		for(int pN=0;pN<allP.size();pN++){
			polygon tPoly;
			boost::polygon::polygon_with_holes_data<int>::iterator_type itPoly=allP[pN].begin();
			while(itPoly!=allP[pN].end()){
				pointP cP=*itPoly;
				boost::geometry::append(tPoly,mPoint (cP.x(),cP.y()) );
				itPoly++;
			}
			renderNonConvexOSM(tPoly,QVector3D(0,0,1.0f),QVector3D(0,qrand()%255,0),50.0f);
		}*/
		for(int pN=0;pN<blockSet.size();pN++){
			polygon tPoly;
			for(int poN=0;poN<blockSet[pN].coords_.size();poN++){
				boost::geometry::append(tPoly,mPoint (blockSet[pN].coords_[poN].x(),blockSet[pN].coords_[poN].y()) );
			}
			renderNonConvexOSM(tPoly,QVector3D(0,0,1.0f),QVector3D(0,qrand()%255,0),50.0f);
		}
		//printf("<<renderOSMBuildings\n");
	}//


	void RoadGraphOSM::createParksFromOSM(std::vector<buildingInfoStruct>& parksOSM,QHash<uint,QVector3D>& nodeBldgIdToPos3D){
		cgPtrTraff->geoZone.clear();
		cgPtrTraff->geoVegetation.resetMe();
		// 6.3.1 Create parks
		osmParks.resize(parksOSM.size());
		for(int bN=0;bN<parksOSM.size();bN++){
			osmParks[bN].buildingFootprint.contour.clear();
			// create geometry
			for(int iN=0;iN<parksOSM[bN].nodes.size();iN++){
				QVector3D pos=nodeBldgIdToPos3D[parksOSM[bN].nodes[iN]];
				osmParks[bN].buildingFootprint.contour.push_back(pos);
				//qDebug()<<pos;
			}
		}
		// 6.4 add trees to parks
		float treesPerSqMeter = 0.002f; //used for trees in parcels
		float distanceBetweenTrees = 15.0f; //used for trees along streets
		float minTreeHeight = 10.0f;
		float maxTreeHeight = 20.0f;
		float minTreeRadToHeightRatio = 0.3f;
		float maxTreeRadToHeightRatio = 0.5f;
		float Theight,rad;
		for(int plN=0;plN<osmParks.size();plN++){
			QVector3D minPt,maxPt;
			osmParks[plN].buildingFootprint.getBBox(minPt, maxPt);
			int numTrees=treesPerSqMeter*osmParks[plN].buildingFootprint.computeArea();
			QVector3D pos;
			for(int i=0; i<numTrees; ++i){					
				pos.setX( LC::misctools::genRand(minPt.x(), maxPt.x()) );
				pos.setY( LC::misctools::genRand(minPt.y(), maxPt.y()) );
				pos.setZ( 0.0f );


				if(LC::misctools::isPointWithinLoop(osmParks[plN].buildingFootprint.contour, pos)){
					Theight = LC::misctools::genRand(minTreeHeight, maxTreeHeight);
					rad    = Theight * LC::misctools::genRand(minTreeRadToHeightRatio, maxTreeRadToHeightRatio);
					cgPtrTraff->geoVegetation.addTreeAtLocation( pos, rad, Theight );
				}
			}
		}
	}//

	void RoadGraphOSM::generateBlocksUsingRender(){
		// 7. BLOCKS WITH RENDER
		blockSet.clear();
		// 7.1 set up
		if(clientMainTraffic->mGLWidget_3D->infoLayers.layers.size()<=0){
			clientMainTraffic->mGLWidget_3D->infoLayers.initInfoLayer();
		}

		int initW=clientMainTraffic->mGLWidget_3D->width();
		int initH=clientMainTraffic->mGLWidget_3D->height();
		//int imageSize=WIDTH_WATER/2.0f;
		clientMainTraffic->mGLWidget_3D->resize(WIDTH_WATER/2.0f,WIDTH_WATER/2.0f);
		glMatrixMode(GL_PROJECTION);
		GLdouble proj[16];
		glGetDoublev( GL_PROJECTION_MATRIX, proj );
		glLoadIdentity();
		//gluPerspective(90.0f, 1.0f,1.0f, 10.0f*1000000.0f);
		glOrtho(-WIDTH_WATER/2.0f,WIDTH_WATER/2.0f,-WIDTH_WATER/2.0f,WIDTH_WATER/2.0f,1.0f, 10.0f*1000000.0f);//-WIDTH_WATER/2.0f, WIDTH_WATER/2.0f,-WIDTH_WATER/2.0f, WIDTH_WATER/2.0f);
		////////////
		clientMainTraffic->mGLWidget_3D->myCam->dz=WIDTH_WATER/2.0f;
		clientMainTraffic->mGLWidget_3D->myCam->xrot=360.0f;
		clientMainTraffic->mGLWidget_3D->myCam->yrot=0.0f;
		clientMainTraffic->mGLWidget_3D->myCam->zrot=0.0f;
		clientMainTraffic->mGLWidget_3D->myCam->dx=0;
		clientMainTraffic->mGLWidget_3D->myCam->dy=0;

		bool temp[]={LC::misctools::Global::global()->view_place_types_centept_render,LC::misctools::Global::global()->view_arterials_edges_render,LC::misctools::Global::global()->view_arterials_vertices_render};
		LC::misctools::Global::global()->view_place_types_centept_render=false;
		LC::misctools::Global::global()->view_arterials_edges_render=false;
		LC::misctools::Global::global()->view_arterials_vertices_render=false;
		{// 7.2 capture
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);						// Enables Depth Testing
			glDepthFunc(GL_LESS);							// The Type Of Depth Testing To Do
			glDisable(GL_TEXTURE_2D);

			glDisable(0x809D);//GL_MULTISAMPLE
			glShadeModel (GL_FLAT);
			//worldview transform
			glMatrixMode(GL_MODELVIEW);
			clientMainTraffic->mGLWidget_3D->myCam->applyCamTransform();
			clientMainTraffic->cg.roadGraph.renderRoadGraph();

			{/// render parks
				for(int plN=0;plN<osmParks.size();plN++){
					//renderNonConvexOSM(blockVec[blN],QVector3D(0,0,1.0f),QVector3D(255.0f,0,0),35.0f);
					glColor3f(1.0f,1.0f,1.0f);
					osmParks[plN].buildingFootprint.renderNonConvex(false);
				}
			}
			///
			clientMainTraffic->mGLWidget_3D->grabFrameBuffer().save("test/im1.png");

			glEnable(0x809D);//GL_MULTISAMPLE
			glShadeModel (GL_SMOOTH);
		}
		////
		clientMainTraffic->mGLWidget_3D->resize(initW,initH);
		glLoadIdentity();//perspective matrix
		glMultMatrixd(proj);
		glMatrixMode(GL_MODELVIEW);

		//LC::misctools::Global::global()->view_place_types_centept_render=temp[0];
		LC::misctools::Global::global()->view_arterials_edges_render=temp[1];
		//LC::misctools::Global::global()->view_arterials_vertices_render=temp[2];

		clientMainTraffic->mGLWidget_3D->updateGL();

		//7.3 Get blocks
		cv::Mat im=cv::imread("test/im1.png",0);
		if(im.type()==CV_8UC1){
			printf("CV_8UC1\n");
		}
		printf("im.type() %d\n",im.type());
		cv::imwrite("test/im2.png",im);
		// THESHOLD (binary)
		cv::threshold(im,im,120,255,cv::THRESH_BINARY);
		cv::imwrite("test/im3.png",im);
		// DILATE
		int dilation_size=2;
		cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,
			cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
			cv::Point( dilation_size, dilation_size ) );
		cv::dilate(im,im,element);
		cv::imwrite("test/im4.png",im);
		// FLOOD BORDERS
		for(int r=0;r<im.rows;r++){
			im.at<uchar>(r,0)=0;
			im.at<uchar>(r,im.cols-1)=0;
		}
		for(int c=0;c<im.cols;c++){
			im.at<uchar>(0,c)=0;
			im.at<uchar>(im.rows-1,c)=0;
		}
		cv::floodFill(im,cv::Mat(),cv::Point(0,0),cv::Scalar(255));
		cv::imwrite("test/im4_2.png",im);
		cv::threshold(im,im,120,255,cv::THRESH_BINARY_INV);//!!INVER
		cv::imwrite("test/im4_3.png",im);
		// FIND COUNTOURS
		cv::Mat canny_output;
		std::vector<std::vector<cv::Point> > contours;
		std::vector<cv::Vec4i> hierarchy;
		cv::findContours( im, contours, hierarchy, /*CV_RETR_TREE*/CV_RETR_EXTERNAL , CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

		/// Draw contours
		cv::Mat drawing = cv::Mat::zeros( im.size(), CV_8UC3 );
		for( int i = 0; i< contours.size(); i++ )
		{
			cv::Scalar color = cv::Scalar( qrand()%255, qrand()%255, qrand()%255 );
			drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, cv::Point() );
		}
		cv::imwrite("test/im5.png",drawing);
		/////////////////////////////////////////////
		// Create buildings
		osmBuildings.clear();
		osmBuildingsAtt.clear();
		for( int i = 0; i< contours.size(); i++ ){
			Building osmBuilding;
			// create geometry
			LC::misctools::Polygon3D parcelP;
			for(int iN=0;iN<contours[i].size();iN++){
				// from 0-imageWidth-->-WIDTH_WATER/2.0f--->WIDTH_WATER/2.0f
				QVector3D pos=QVector3D(2*contours[i][iN].x-WIDTH_WATER/2.0f,-1*(2*contours[i][iN].y-WIDTH_WATER/2.0f),0);
				/*if(osmBuilding.buildingFootprint.contour.size()>1){
				int lastInd=osmBuilding.buildingFootprint.contour.size()-1;
				QVector3D antepenAndlastDir=(osmBuilding.buildingFootprint.contour[lastInd]-osmBuilding.buildingFootprint.contour[lastInd-1]).normalized();
				QVector3D antepenNewDir=(pos-osmBuilding.buildingFootprint.contour[lastInd-1]).normalized();
				if(abs(acos(QVector3D::dotProduct(antepenAndlastDir,antepenNewDir)))<0.174532925f){//10 deg
				osmBuilding.buildingFootprint.contour[lastInd]=pos;//skip last
				continue;
				}
				}*/
				parcelP.contour.push_back(pos);

				//qDebug()<<pos;
			}
			//osmBuildings[bN].buildingFootprint.contour.push_back(osmBuildings[bN].buildingFootprint.contour[0]);//close polygon
			//LC::misctools::Polygon3D::cleanLoop(pContourCpy, inParcel.parcelContour.contour, 1.0f);
			/*if(LC::misctools::Polygon3D::reorientFace(parcelP.contour)){
			std::reverse(parcelP.contour.begin(), parcelP.contour.end() - 1);
			}*/



			//printf("area %d: %f\n",i,osmBuilding.buildingFootprint.computeArea());
			if(parcelP.computeArea()<(200.0f*0.8))//average footprint-->210m2 hamilton-co.org/hcrpc/partner/uc_fs/Characteristics/Charts/NBD/Footprint.htm
				continue;
			//LC::misctools::Polygon3D::cleanLoop(osmBuilding.buildingFootprint.contour,osmBuilding.buildingFootprint.contour,2.0f);
			//parcelP.computeInset(4.0f, osmBuilding.buildingFootprint.contour, false);
			osmBuilding.buildingFootprint.contour=parcelP.contour;

			float buildingHeight=5.0f+((float)qrand()/RAND_MAX)*25.0f+osmBuilding.buildingFootprint.contour[0].z();
			if(LC::misctools::Global::global()->cuda_bldg_use_density==1&&LC::misctools::Global::global()->peopleDist!=0){
				// NACHO Use distribution to define heigh
				QVector3D bldPos=osmBuilding.buildingFootprint.contour[0];
				int xP=((bldPos.x()/WIDTH_WATER)+0.5f)*LC::misctools::Global::global()->peopleDist->cols;
				int yP=((bldPos.y()/WIDTH_WATER)+0.5f)*LC::misctools::Global::global()->peopleDist->rows;

				float peoplDis=LC::misctools::Global::global()->peopleDist->at<float>(yP,xP);//rc
				float jobDis=LC::misctools::Global::global()->jobDist->at<float>(yP,xP);//rc
				float bldgNumStFLOAT=(peoplDis+jobDis)/LC::misctools::MEAN_BUILDING_FLOOR_HEIGHT;
				bldgNumStFLOAT*=(LC::misctools::Global::global()->cuda_bldg_use_density_factor/10.0f);
				bldgNumStFLOAT+=2;
				buildingHeight=bldgNumStFLOAT*LC::misctools::MEAN_BUILDING_FLOOR_HEIGHT;
			}

			// BOXY BUILDING
			LC::ProceduralBuildingsMachine::generateBuildingGeometry(osmBuilding.buildingFootprint,
				buildingHeight, osmBuilding.buildingFaces);
			// create att
			int colorF=qrand()%100+20;
			int colorR=colorF+qrand()%30+10;
			ParcelBuildingAttributes osmBuildingsAttO;
			osmBuildingsAttO.att_facade_color=QColor(colorF,colorF,colorF);
			osmBuildingsAttO.att_roof_color=QColor(colorR,colorR,colorR);

			osmBuildings.push_back(osmBuilding);
			osmBuildingsAtt.push_back(osmBuildingsAttO);

			/*// PARAMETRIC BUILDING
			MTC::geometry::Parcel parcel;

			//.contour
			QVector3D minBox(FLT_MAX,FLT_MAX,0);
			QVector3D maxBox(-FLT_MAX,-FLT_MAX,0);
			for(int pC=0;pC<osmBuilding.buildingFootprint.contour.size();pC++){
				//parcel.parcelContour.contour.push_back(osmBuilding.buildingFootprint.contour[pC]);
				updateMinMax(osmBuilding.buildingFootprint.contour[pC], minBox, maxBox);
			}
			parcel.parcelContour.contour.push_back(QVector3D(minBox.x(),minBox.y(),0));
			parcel.parcelContour.contour.push_back(QVector3D(maxBox.x(),minBox.y(),0));
			parcel.parcelContour.contour.push_back(QVector3D(maxBox.x(),maxBox.y(),0));
			parcel.parcelContour.contour.push_back(QVector3D(minBox.x(),maxBox.y(),0));
			parcel.parcelContour.contour.push_back(QVector3D(minBox.x(),minBox.y(),0));
			//parcel.parcelContour.contour.push_back(osmBuilding.buildingFootprint.contour[osmBuilding.buildingFootprint.contour.size()-1]);//last

			parcel.parcelBuildingAttributes.att_building_type= (qrand()%15)+1;//1-16
			MTC::geometry::Building parBldg;
			parBldg.setParcel(&parcel);
			parBldg.att_stories=10;//!!!!

			MTC::geometry::BuildingRenderer* parBldPtr=BuildingRendererFactory::Instance()->CreateBuildingRenderer(&parBldg);

			if(parBldPtr!=NULL){
				//load tex
				parBldPtr->loadRoofTextures(clientMainTraffic->mGLWidget_3D);
				parBldPtr->initializeRenderer(0,clientMainTraffic->mGLWidget_3D);
				// put in vector
				myParametricBuildings.push_back(parBldPtr);
			}

			*/
		}
		//////////////
		// Improved hull
		polygon tPoly2;
		for(int nN=0;nN<osmBuildings.size();nN++){
			for(int i=0;i<osmBuildings[nN].buildingFootprint.contour.size();i++){
				boost::geometry::append(tPoly2,mPoint (osmBuildings[nN].buildingFootprint.contour[i].x(),osmBuildings[nN].buildingFootprint.contour[i].y()) );
			}
		}
		boost::geometry::append(tPoly2,mPoint (-3000,-3000) );//!!!!!!
			boost::geometry::append(tPoly2,mPoint (-3000,3000) );//!!!!!!
			boost::geometry::append(tPoly2,mPoint (3000,3000) );//!!!!!!
			boost::geometry::append(tPoly2,mPoint (3000,-3000) );//!!!!!!

		polygon hull2;
		boost::geometry::convex_hull(tPoly2, hull2);

		float zVal=LC::misctools::Global::global()->boundingPolygon[0].z();
		LC::misctools::Global::global()->boundingPolygon.clear();
		for(int hN=0;hN<hull2.outer().size();hN++){
			QVector3D pHu=QVector3D(hull2.outer()[hN].x(),hull2.outer()[hN].y(),zVal);
			LC::misctools::Global::global()->boundingPolygon.push_back(pHu);
		}
	}//

	void RoadGraphOSM::generateBlocksUsingBoost(RoadGraph& inRoadGraph){
		using namespace boost::polygon::operators;

		QTime timer;
		timer.start();
		float width=LC::misctools::Global::global()->roadLaneWidth*1.5f;
		RoadGraph::roadGraphEdgeIter_BI ei, eiEnd;

		int edge=0;
		for(boost::tie(ei, eiEnd) = boost::edges(inRoadGraph.myRoadGraph_BI);
			ei != eiEnd; ++ei)
		{
			printf("edge %d\n",edge);
			if(edge%10==0){
				boost::polygon::simplify(blockSet,2.0f);
			}
			edge++;
			if(inRoadGraph.myRoadGraph_BI[*ei].roadSegmentGeometry.size()>0){
				///////////////////////////////////////////
				// Squares
				QVector3D p0;
				QVector3D p1;
				QVector3D p2;
				for(int sN=0;sN<inRoadGraph.myRoadGraph_BI[*ei].roadSegmentGeometry.size()-1;sN++){
					//std::vector<polygon> tempPolys;
					polygonP tempPolyP;
					p0=inRoadGraph.myRoadGraph_BI[*ei].roadSegmentGeometry[sN];
					p1=inRoadGraph.myRoadGraph_BI[*ei].roadSegmentGeometry[sN+1];
					QVector3D dir=(p1-p0).normalized();
					QVector3D per=(QVector3D::crossProduct(dir,QVector3D(0,0,1.0f)).normalized());
					pointP vP[4];
					vP[0]=boost::polygon::construct<pointP>(p0.x()+per.x()*width, p0.y()+per.y()*width);
					vP[1]=boost::polygon::construct<pointP>(p1.x()+per.x()*width, p1.y()+per.y()*width);

					vP[2]=boost::polygon::construct<pointP>(p1.x()-per.x()*width, p1.y()-per.y()*width);
					vP[3]=boost::polygon::construct<pointP>(p0.x()-per.x()*width, p0.y()-per.y()*width);

					boost::polygon::set_points(tempPolyP,vP,vP+4);


					blockSet-=tempPolyP;

				}
			}
		}
		printf("7. Blocks %d ms\n",timer.elapsed());
		{
			QFile blocksText("block.txt");
			if(!blocksText.open(QIODevice::WriteOnly)) {
				std::cout << "ERROR: Cannot open temporary file";
				return;
			}
			QTextStream blockStream(&blocksText);
			std::vector<polygonP> allP;
			boost::polygon::get_trapezoids(allP,blockSet);
			blockStream<<"Num Poly:"<<allP.size()<<"\n";
			for(int pN=0;pN<allP.size();pN++){
				blockStream<<"P["<<pN<<"]:"<<allP[pN].size()<<"\n";
				boost::polygon::polygon_with_holes_data<int>::iterator_type itPoly=allP[pN].begin();
				int vertN=0;
				while(itPoly!=allP[pN].end()){
					pointP cP=*itPoly;
					blockStream<<"P["<<pN<<"]["<<vertN<<"]X:"<<cP.x()<<"\n";
					blockStream<<"P["<<pN<<"]["<<vertN<<"]Y:"<<cP.y()<<"\n";
					itPoly++;
					vertN++;
				}
			}
			blocksText.close();
		}
	}//

	void RoadGraphOSM::recalculateHeightFromDistribution(){

		for(int bN=0;bN<osmBuildings.size();bN++){
			float buildingHeight=5.0f+((float)qrand()/RAND_MAX)*25.0f+osmBuildings[bN].buildingFootprint.contour[0].z();
			if(LC::misctools::Global::global()->cuda_bldg_use_density==1&&LC::misctools::Global::global()->peopleDist!=0){
				// NACHO Use distribution to define heigh
				QVector3D bldPos=osmBuildings[bN].buildingFootprint.contour[0];
				int xP=((bldPos.x()/WIDTH_WATER)+0.5f)*LC::misctools::Global::global()->peopleDist->cols;
				int yP=((bldPos.y()/WIDTH_WATER)+0.5f)*LC::misctools::Global::global()->peopleDist->rows;

				float peoplDis=LC::misctools::Global::global()->peopleDist->at<float>(yP,xP);//rc
				float jobDis=LC::misctools::Global::global()->jobDist->at<float>(yP,xP);//rc
				float bldgNumStFLOAT=(peoplDis+jobDis)/LC::misctools::MEAN_BUILDING_FLOOR_HEIGHT;
				bldgNumStFLOAT*=(LC::misctools::Global::global()->cuda_bldg_use_density_factor/10.0f);
				bldgNumStFLOAT+=2;
				bldgNumStFLOAT*=(((float)qrand()/RAND_MAX)+0.5f);//random +-50%
				buildingHeight=bldgNumStFLOAT*LC::misctools::MEAN_BUILDING_FLOOR_HEIGHT;
			}

			// BOXY BUILDING
			osmBuildings[bN].buildingFaces.clear();
			LC::ProceduralBuildingsMachine::generateBuildingGeometry(osmBuildings[bN].buildingFootprint,
				buildingHeight, osmBuildings[bN].buildingFaces);
		}
	}//

}
