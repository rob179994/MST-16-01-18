#pragma once
#include "stdafx.h"


// my own
//#include "TreeEdge.h" 
//#include "TreeNode.h"
#include "MST.h"

using namespace cv;
using namespace std;

int main()
{
	
	// TEMP CREATING OWN MAT///
	 Mat imgLeft(5, 3, CV_8UC1);;
	 imgLeft.at<uchar>(0, 0) = 2;
	 imgLeft.at<uchar>(0, 1) = 1;
	 imgLeft.at<uchar>(0, 2) = 5;
	 imgLeft.at<uchar>(1, 0) = 2;
	 imgLeft.at<uchar>(1, 1) = 3;
	 imgLeft.at<uchar>(1, 2) = 3;
	 imgLeft.at<uchar>(2, 0) = 8;
	 imgLeft.at<uchar>(2, 1) = 10;
	 imgLeft.at<uchar>(2, 2) = 8;

	 imgLeft.at<uchar>(3, 0) = 8;
	 imgLeft.at<uchar>(3, 1) = 14;
	 imgLeft.at<uchar>(3, 2) = 6;
	 imgLeft.at<uchar>(4, 0) = 10;
	 imgLeft.at<uchar>(4, 1) = 12;
	 imgLeft.at<uchar>(4, 2) = 6;
	 // TEMP

	//Mat imgLeft, imgEdgeLeftx;
	Mat imgRight, imgEdgeRight;
	//imgLeft = imread("C:/Users/Rob McCormack/Documents/Visual Studio 2017/Projects/Diss_Algorithm/x64/Debug/image_L.png", IMREAD_GRAYSCALE);	// only inputting string directly for testing purposes
	imgRight = imread("C:/Users/Rob McCormack/Documents/Visual Studio 2017/Projects/old&trial/MST/x64/Debug/image_R.png", IMREAD_GRAYSCALE);
	if (!imgLeft.data || !imgRight.data)	// error check for the image files
	{
		printf(" Not enough image data \n ");
		return -1;
	}

	// Make minspantree object to access relevent functions
	MST minSpanTree;

	vector<TreeEdge> sortedEdges = minSpanTree.shortestSidesOfImage(imgLeft);
	sort(sortedEdges.begin(), sortedEdges.end());

	// min span tree of image
	vector<MST::nodeStructure> nodeMST = minSpanTree.makeMST(imgLeft.rows, imgLeft.cols,sortedEdges);
	// filtered image for local costs
	vector<vector<double>> localFilter = minSpanTree.imageLocallyFiltered(imgLeft.rows, imgLeft.cols, imgLeft, nodeMST);

	int squareDimension = 3;

	vector<vector<vector<double>>> abConstants = minSpanTree.abMappingCostants(imgLeft,squareDimension);

	cout << "FINISHED " << endl;

	system("PAUSE");
	return 0;
}

