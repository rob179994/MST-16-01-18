#pragma once


class TreeEdge {

	double weight;
	int coords[4];
	bool isHorizontal;
	int sourceNodeIndex;
	int destinationNodeIndex;

public:

	TreeEdge() {
		weight = 0;
	}

	TreeEdge(int firstXCoord, int firstYCoord) {
		coords[0] = firstXCoord;
		coords[1] = firstYCoord;
		weight = 0;
	}

	void setWeight(double inputWeight) {
		weight = inputWeight;
	}

	double getWeight() {
		return weight;
	}

	void setStartCoords(int xCoord, int yCoord) {
		coords[0] = xCoord;
		coords[1] = yCoord;
	}

	int * getCoords() {
		return coords;
	}

	// easier to access relevent nodes
	int getSourceNodeIndex() {
		return sourceNodeIndex;
	}

	void setSourceNodeIndex(int input) {
		sourceNodeIndex = input;
	}

	int getDestinationNodeIndex() {
		return destinationNodeIndex;
	}

	void setDestinationNodeIndex(int input) {
		destinationNodeIndex = input;
	}

	/*int getSourceNode() {
		if (isHorizontal) {
			return;
		}
		else {

		}
	}

	int getDestinationNode() {

	}*/

	void setEndCoords(int xCoord, int yCoord) {
		coords[2] = xCoord;
		coords[3] = yCoord;
	}

	void setIsHorizontal(bool input) {
		isHorizontal = input;
	}

	bool getIsHorizontal() {
		isHorizontal = false;
	}

	bool operator< (const TreeEdge &other) const {
		return weight < other.weight;
	}

	int getOtherEnd(int inputEnd){
		if (inputEnd == sourceNodeIndex) {
			return destinationNodeIndex;
		}
		return sourceNodeIndex;
	}
};
