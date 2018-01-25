#pragma once
#include "TreeEdge.h"
//#include "TreeEdge.h" 
#include "TreeNode.h"

class TreeNode {

	TreeEdge northEdge;
	TreeEdge eastEdge;
	TreeEdge southEdge;
	TreeEdge westEdge;
	int xCoord;
	int yCoord;
	float forwardSum;
	float backwardSum;

public:

	// Default constructor
	TreeNode() {
		forwardSum = 0;
		backwardSum = 0;
	}

	//constructor 2
	TreeNode(int xInput, int yInput) {
		xCoord = xInput;
		yCoord = yInput;
	}

	void setEastEdge(TreeEdge east) {
		eastEdge = east;
	}

	void setNorthEdge(TreeEdge north) {
		northEdge = north;
	}

	void setSouthEdge(TreeEdge south) {
		southEdge = south;
	}

	void setWestEdge(TreeEdge west) {
		westEdge = west;
	}

	void setEastSouthEdges(TreeEdge east, TreeEdge south) {
		eastEdge = east;
		southEdge = south;
	}

	void setAllTreeEdges(TreeEdge north, TreeEdge east, TreeEdge south, TreeEdge west) {
		northEdge = north;
		eastEdge = east;
		southEdge = south;
		westEdge = west;
	}

	int getUniqueIndex() {
		return xCoord*yCoord + xCoord;
	}

	//int getNodeNumberOfEdgeEnd() {
	//
	//	return;
	//}
};