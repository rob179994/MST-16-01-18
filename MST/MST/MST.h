using namespace std;

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <cmath>
#include <math.h> 

#include "TreeEdge.h" 
#include "TreeNode.h"

using namespace cv;

class MST {
public:

	// A structure to represent a subset for union-find
	struct nodeStructure
	{
		int parent;
		int root;
		int rank;
		int index;
		vector<int> children;
		int xCoord;
		int yCoord;

		bool operator< (const nodeStructure &other) const {
			return rank < other.rank;
		}

	};

	void Union(vector<nodeStructure> &allNodes, int rootOne, int rootTwo, int nodeOne, int nodeTwo) {

		// Attach smaller rank tree under root of high 
		// rank tree (Union by Rank)
		if (allNodes[rootOne].rank < allNodes[rootTwo].rank) {
			if (allNodes[nodeOne].root !=nodeOne) { // check subtree is in correct order
				reverseSubTree(allNodes,nodeOne, allNodes[allNodes[nodeOne].root].rank, nodeOne, nodeOne);
				rootOne = nodeOne;
			}

			allNodes[rootOne].root = rootTwo;
			setRootForChildren(allNodes,rootTwo,nodeOne);
			allNodes[nodeOne].parent = nodeTwo;
			allNodes[nodeTwo].children.push_back(nodeOne);
			setRankAbove(allNodes,nodeOne,allNodes[nodeOne].rank);
		}
		else if (allNodes[rootOne].rank > allNodes[rootTwo].rank) {

			if (allNodes[nodeTwo].root != nodeTwo) { // check subtree is in correct order
				reverseSubTree(allNodes, nodeTwo, allNodes[allNodes[nodeTwo].root].rank, nodeTwo, nodeTwo);
				rootTwo = nodeTwo;
			}

			allNodes[rootTwo].root = rootOne;
			setRootForChildren(allNodes, rootOne, nodeTwo);
			allNodes[nodeTwo].parent = nodeOne;
			allNodes[nodeOne].children.push_back(nodeTwo);
			setRankAbove(allNodes, nodeTwo, allNodes[nodeTwo].rank);
		}
		// If ranks are same, then make one as root and 
		// increment its rank by one
		else
		{

			if (allNodes[nodeTwo].root != nodeTwo) { // check subtree is in correct order
				reverseSubTree(allNodes, nodeTwo, allNodes[allNodes[nodeTwo].root].rank, nodeTwo, nodeTwo);
				rootTwo = nodeTwo;
			}

			allNodes[rootTwo].root = rootOne;
			setRootForChildren(allNodes, rootOne, nodeTwo);
			allNodes[nodeTwo].parent = nodeOne;
			allNodes[nodeOne].children.push_back(nodeTwo);
			allNodes[rootOne].rank++;
			setRankAbove(allNodes, nodeTwo, allNodes[nodeTwo].rank);
		}

		setRankDueToChildren(allNodes,allNodes[nodeOne].root);

	}

	void setRootForChildren(vector<nodeStructure> &allNodes,int rootToSet, int nodeToChangeRootBelow) {
		for (int i = 0; i < allNodes[nodeToChangeRootBelow].children.size();i++) {
			allNodes[allNodes[nodeToChangeRootBelow].children[i]].root = rootToSet;
			setRootForChildren(allNodes,rootToSet, allNodes[nodeToChangeRootBelow].children[i]);
		}
	}

	void setRankAbove(vector<nodeStructure> &allNodes, int index,int rankToSet) {
		if (rankToSet>=allNodes[index].rank) {
			allNodes[index].rank = rankToSet;
			if (allNodes[index].root != index) {
				setRankAbove(allNodes, allNodes[index].parent, rankToSet + 1);
			}			
		}
	}

	void reverseSubTree(vector<nodeStructure> &allNodes, int index, int rankToSet, int parentToBe, int root) {
		if (allNodes[index].root != index) {
			allNodes[index].children.push_back(allNodes[index].parent);

			findAndRemoveChild(allNodes,index, allNodes[index].parent);

			reverseSubTree(allNodes, allNodes[index].parent, rankToSet - 1, index, root);
		}
		else {
			findAndRemoveChild(allNodes, index, allNodes[index].parent);
		}

		allNodes[index].parent = parentToBe;

		if (index == root) {	// set all ranks and roots
			setRankDueToChildren(allNodes,index);
			setRootOfAllChildrenBelow(allNodes,index,root);
		}
	}

	void findAndRemoveChild(vector<nodeStructure> &allNodes, int indexToRemove, int parent) {
		for (int i = 0; i < allNodes[parent].children.size();i++) {
			if (allNodes[parent].children[i]==indexToRemove) {
				allNodes[parent].children.erase(allNodes[parent].children.begin()+i);
				return;
			}
		}
	}

	int setRankDueToChildren(vector<nodeStructure> &allNodes,int index) {
		int topRankOfChildren=0;
		for (int i = 0; i < allNodes[index].children.size();i++) {
			int topRankOfThisChild = setRankDueToChildren(allNodes, allNodes[index].children[i]);
			if (topRankOfThisChild>topRankOfChildren) {
				topRankOfChildren = topRankOfThisChild;
			}
		}

		if (allNodes[index].children.size()>0) {
			allNodes[index].rank = topRankOfChildren + 1;
		}
		else {
			allNodes[index].rank = 0;
		}
		
		return allNodes[index].rank;
	}

	void setRootOfAllChildrenBelow(vector<nodeStructure> &allNodes, int index,int root) {
		for (int i = 0; i < allNodes[index].children.size();i++) {
			setRootOfAllChildrenBelow(allNodes, allNodes[index].children[i],root);
		}
		allNodes[index].root = root;
	}


	vector<nodeStructure> makeMST(int yPixels, int xPixels, vector<TreeEdge> &sortedEdges) {
		vector<MST::nodeStructure> nodeMST(yPixels*xPixels);
		for (int v = 0; v < yPixels*xPixels; v++)
		{
			nodeMST[v].parent = v;
			nodeMST[v].root = v;
			nodeMST[v].rank = 0;
			nodeMST[v].index = v;
			nodeMST[v].xCoord = v % xPixels;
			nodeMST[v].yCoord = v / xPixels;
		}

		int i = 0;
		int sidesPicked = 0;
		while (sidesPicked < yPixels*xPixels - 1)
		{
			// Pick the smallest edge. And increment 
			// the index for next iteration
			TreeEdge smallestEdge = sortedEdges[i];

			int rootOne = nodeMST[smallestEdge.getSourceNodeIndex()].root;
			int rootTwo = nodeMST[smallestEdge.getDestinationNodeIndex()].root;

			if (rootOne != rootTwo) {
				Union(nodeMST, rootOne, rootTwo, smallestEdge.getSourceNodeIndex(), smallestEdge.getDestinationNodeIndex());
				sidesPicked++;
			}

			i++;
		}

		return nodeMST;
	}

	vector<TreeEdge> shortestSidesOfImage(Mat &inputImage) {
		int yPixels = inputImage.rows;
		int xPixels = inputImage.cols;

		// create 2D array for tree
		vector<TreeNode> treeRows(xPixels);
		vector<vector<TreeNode>> treeNodeMatrix(yPixels, treeRows);

		// edge array, vertical and horizontal
		// Note: need 1 less row/column in that dimension
		vector<TreeEdge> imageTreeHorizontalEdgesRows(xPixels - 1);
		vector<TreeEdge> imageTreeVerticalEdgesRows(xPixels);

		vector<vector<TreeEdge>> imageTreeHorizontalEdges(yPixels, imageTreeHorizontalEdgesRows);
		vector<vector<TreeEdge>> imageTreeVerticalEdges(yPixels - 1, imageTreeVerticalEdgesRows);


		// list of lowest weight edges
		vector<TreeEdge> sortedEdges;
		// list of all nodes
		vector<TreeNode> allNodes;

		int nodeNumber = 0;
		for (int j = 0; j < yPixels; j++) {	// iterate rows
			for (int i = 0; i < xPixels; i++) {	//iterate columns
				TreeNode *thisNode = new TreeNode(i, j);
				TreeEdge *eastEdge = new TreeEdge(i, j);
				TreeEdge *southEdge = new TreeEdge(i, j);

				eastEdge->setSourceNodeIndex(nodeNumber);
				southEdge->setSourceNodeIndex(nodeNumber);

				if (i<xPixels - 1) {
					imageTreeHorizontalEdges[j][i] = *eastEdge;
				}
				if (j < yPixels - 1) {
					imageTreeVerticalEdges[j][i] = *southEdge;
				}

				// get pixel intensity at pixel
				Scalar pixel1Intensity = inputImage.at<uchar>(j, i);

				// set weight of westEdge
				if (i>0) {
					// get pixel intensity of one to left
					Scalar pixel2Intensity = inputImage.at<uchar>(j, i - 1);
					// as defined in MST Yang 2015 paper
					double difference = *pixel1Intensity.val - *pixel2Intensity.val;
					double intensityDifference = pow((*pixel1Intensity.val - *pixel2Intensity.val), 2.0);
					// set weight and add to edge list
					imageTreeHorizontalEdges[j][i - 1].setWeight(intensityDifference);
					// set that the edge is horizontal
					imageTreeHorizontalEdges[j][i - 1].setIsHorizontal(true);
					// set the nodedestination
					imageTreeHorizontalEdges[j][i - 1].setDestinationNodeIndex(nodeNumber);

					sortedEdges.push_back(imageTreeHorizontalEdges[j][i - 1]);
				}


				// same for northEdge
				if (j>0) {
					Scalar pixel2Intensity = inputImage.at<uchar>(j - 1, i);
					double intensityDifference = pow((*pixel1Intensity.val - *pixel2Intensity.val), 2.0);
					imageTreeVerticalEdges[j - 1][i].setWeight(intensityDifference);
					imageTreeVerticalEdges[j - 1][i].setIsHorizontal(false);
					imageTreeVerticalEdges[j - 1][i].setDestinationNodeIndex(nodeNumber);

					sortedEdges.push_back(imageTreeVerticalEdges[j - 1][i]);
				}
				treeNodeMatrix[j][i] = *thisNode;
				//allNodes[nodeNumber] = *thisNode;
				nodeNumber++;
			}
		}

		return sortedEdges;
	}

	vector<vector<double>> imageLocallyFiltered(int inputRows, int inputCols, Mat &inputImage, vector<nodeStructure> &nodeMST) {
		vector<vector<double>> stepOneLocal = forwardsAggregation(inputRows,inputCols,inputImage,nodeMST);
		return backwardsAggregation(inputRows, inputCols, stepOneLocal, nodeMST);
	}

	vector<vector<double>> forwardsAggregation(int inputRows, int inputCols, Mat &inputImage, vector<nodeStructure> &nodeMST) {
		
		// create 2D array of local costs
		vector<double> localOneDim(inputCols);
		vector<vector<double>> localCostsLeafToRoot(inputRows, localOneDim);

		aggregateStepOne(nodeMST, nodeMST[0].root, inputImage, localCostsLeafToRoot);

		return localCostsLeafToRoot;
	}

	void aggregateStepOne(vector<nodeStructure> &nodeMST,int parentIndex, Mat &inputImage, vector<vector<double>> &localCostsLeafToRoot) {
		for (int i = 0; i < nodeMST[parentIndex].children.size(); i++) {
			aggregateStepOne(nodeMST, nodeMST[parentIndex].children[i],inputImage,localCostsLeafToRoot);
			double childSimilarity = localCostsLeafToRoot[nodeMST[nodeMST[parentIndex].children[i]].yCoord][nodeMST[nodeMST[parentIndex].children[i]].xCoord] * exp((double)(-0.5) / (double)2.3);
			localCostsLeafToRoot[nodeMST[parentIndex].yCoord][nodeMST[parentIndex].xCoord] += childSimilarity;
		}
		// add own intensity 
		Scalar pixelIntensityScalar = inputImage.at<uchar>(nodeMST[parentIndex].yCoord, nodeMST[parentIndex].xCoord);
		double pixelintensity = *pixelIntensityScalar.val;
		localCostsLeafToRoot[nodeMST[parentIndex].yCoord][nodeMST[parentIndex].xCoord] += pixelintensity;
	}

	vector<vector<double>> backwardsAggregation(int inputRows, int inputCols, vector<vector<double>> &localCostsStepOne, vector<nodeStructure> &nodeMST) {
		// create 2D array of local costs step 2
		vector<double> localOneDim(inputCols);
		vector<vector<double>> localCostsRootToLeaf(inputRows, localOneDim);

		// factors defined in paper
		double nodeFactor = (1 - exp(-1 / ((double)2.3*(double)2.3)));
		double parentFactor = exp((double)(-0.5) / ((double)2.3*(double)2.3));

		aggregateStepTwo(nodeMST, nodeMST[0].root, localCostsStepOne, localCostsRootToLeaf,parentFactor,nodeFactor);

		return localCostsRootToLeaf;
	}

	void aggregateStepTwo(vector<nodeStructure> &nodeMST, int parentIndex, vector<vector<double>> localCostsStepOne, vector<vector<double>> &localCostsLeafToRoot, double parentFactor, double nodeFactor) {
		// add value from step 1
		localCostsLeafToRoot[nodeMST[parentIndex].yCoord][nodeMST[parentIndex].xCoord] += (nodeFactor*localCostsStepOne[nodeMST[parentIndex].yCoord][nodeMST[parentIndex].xCoord]);
		// add value from parent
		if (parentIndex != nodeMST[parentIndex].parent) {
			localCostsLeafToRoot[nodeMST[parentIndex].yCoord][nodeMST[parentIndex].xCoord] += localCostsLeafToRoot[nodeMST[nodeMST[parentIndex].parent].yCoord][nodeMST[nodeMST[parentIndex].parent].xCoord] *parentFactor;
		}
		// call same function for all children
		for (int i = 0; i < nodeMST[parentIndex].children.size(); i++) {
			aggregateStepTwo(nodeMST, nodeMST[parentIndex].children[i], localCostsStepOne, localCostsLeafToRoot, parentFactor, nodeFactor);
		}
	}

	vector<vector<vector<double>>> abMappingCostants(Mat &inputImage, int squareDimension) {
		
		vector<vector<vector<double>>> abConstants;
		int xmax = inputImage.cols - 1;
		int ymax = inputImage.rows - 1;
		
		// get a and b constants for each pixel
		for (int j = 0; j < ymax;j++) {
			for (int i = 0; i < xmax; i++) {
				abConstants[j][i] = abMappingCostantAtPixel(inputImage,j,i,squareDimension);
			}
		}

		return abConstants;
	}


	vector<double> abMappingCostantAtPixel(Mat &inputImage, int y, int x, int squareDimension) {

		vector<double> abConstants;
		vector<double> pixelIntensities;
		int xmax = inputImage.cols-1;
		int ymax = inputImage.rows-1;

		double sumOfPixelValues = 0;
		int numberEitherside = (squareDimension - 1) / 2;
		// go through surrounding area and collect pixel intensities
		for (int j = y - numberEitherside; j < y + numberEitherside +1; j++) {
			for (int i = x -  numberEitherside; i < x + numberEitherside + 1; i++) {
				if (j<0||i<0||j>ymax||i>xmax) {
					continue;
				}
				else {
					Scalar pixelIntensityScalar = inputImage.at<uchar>(j,i);
					pixelIntensities[j*squareDimension + i] = *pixelIntensityScalar.val;
					sumOfPixelValues += *pixelIntensityScalar.val;
				}
			}
		}

		double mean = sumOfPixelValues/ pixelIntensities.size();

		double residual = 0;
		for (int i = 0; i < pixelIntensities.size(); i++)
		{
			residual += (pixelIntensities[i] - mean) * (pixelIntensities[i] - mean);
		}
		double variance = residual / pixelIntensities.size();




		//????????????????????????????????????????
		double a = 1;
		double b = 1;

		return abConstants;
	}

};