#pragma once
#include "stdafx.h"
#include <windows.h>


// my own
//#include "TreeEdge.h" 
//#include "TreeNode.h"
#include "MST.h"

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
	// find directory
	/*if (argc < 2)
	{
		cout << _T("Enter the dir from which to find the stereo files.") << endl;
		return 1;
	}
	
	// directories from user input
	string dataFolder = argv[1];

	// max length of directory
	char path[MAX_PATH];
	if (dataFolder.length() > MAX_PATH - 11)
	{
		cerr << _T("Your path is too long.") << endl;
		return -1;
	}

	WIN32_FIND_DATAA FindFileData;

	string firstLeftImage = dataFolder + "\\left\\" +"*";
	HANDLE hFind = FindFirstFileA(firstLeftImage.c_str(), &FindFileData);

	vector<string> leftImageNames;
	if (hFind== INVALID_HANDLE_VALUE) {
		cout<<"No png or jpg files found in left folder"<<endl;
		return 0;
	}
	else {
		while (FindNextFileA(hFind, &FindFileData)) {
			string ws(FindFileData.cFileName);

			if (ws.find(".jpg") != std::string::npos || ws.find(".png") != std::string::npos) {
				leftImageNames.push_back(ws);
			}
		}
	}
	FindClose(hFind);

	string firstRightImage = dataFolder + "\\right\\" + "*";
	hFind = FindFirstFileA(firstRightImage.c_str(), &FindFileData);
	vector<string> rightImageNames;
	if (hFind == INVALID_HANDLE_VALUE) {
		cout << "No png or jpg files found in right folder" << endl;
		return 0;
	}
	else {
		while (FindNextFileA(hFind, &FindFileData)) {
			string ws(FindFileData.cFileName);

			if (ws.find(".jpg") != std::string::npos|| ws.find(".png") != std::string::npos) {
				rightImageNames.push_back(ws);
			}
		}
	}
	FindClose(hFind);

	cout << endl << leftImageNames.size() << " Left Images"<< endl;
	cout << rightImageNames.size() << " Right Images" << endl << endl;*/

	/*Mat image1;
	Mat image2;
	//cout << dataFolder + "\\right\\" + rightImageNames[0] << endl;
	image1 = imread(dataFolder + "\\right\\" + rightImageNames[0], CV_LOAD_IMAGE_GRAYSCALE);
	image2 = imread(dataFolder + "\\left\\" + leftImageNames[0], CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("Right", WINDOW_AUTOSIZE);
	namedWindow("Left", WINDOW_AUTOSIZE);
	imshow("Right", image1);
	imshow("Left", image2);
	waitKey(0);*/

	

	








	
	// TEMP CREATING OWN MAT///
	 /*Mat imgLeft(5, 3, CV_8UC1);;
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
	 imgLeft.at<uchar>(4, 2) = 6;*/
	 // TEMP

	Mat imgLeft, imgEdgeLeftx;
	Mat imgRight, imgEdgeRight;
	imgLeft = imread("C:/Users/Rob McCormack/Documents/Visual Studio 2017/Projects/Bilateral Filter/EdgeDetection/x64/Release/image_L.png", IMREAD_GRAYSCALE);	// only inputting string directly for testing purposes
	imgRight = imread("C:/Users/Rob McCormack/Documents/Visual Studio 2017/Projects/Bilateral Filter/EdgeDetection/x64/Release/image_R.png", IMREAD_GRAYSCALE);
	if (!imgLeft.data || !imgRight.data)	// error check for the image files
	{
		printf(" Not enough image data \n ");
		return -1;
	}

	// Make minspantree object to access relevent functions
	MST mst;
	int squareDimension = 3;

	//////////////////////// LEFT ////////////////////////
	vector<TreeEdge> sortedEdgesLeft = mst.shortestSidesOfImage(imgLeft);
	sort(sortedEdgesLeft.begin(), sortedEdgesLeft.end());
	cout << "1" << endl;
	// min span tree of image
	vector<MST::nodeStructure> nodeMSTLeft = mst.makeMST(imgLeft.rows, imgLeft.cols,sortedEdgesLeft);
	cout << "2" << endl;
	// filtered image for local costs
	vector<vector<double>> localFilterLeft = mst.imageLocallyFiltered(imgLeft.rows, imgLeft.cols, imgLeft, nodeMSTLeft);
	cout << "3" << endl;
	// filtered image for non-local costs
	vector<vector<double>> nonLocalFilterLeft = mst.imageNonLocalFilter(imgLeft.rows, imgLeft.cols, nodeMSTLeft, squareDimension,imgLeft);
	cout << "4" << endl;


	//////////////////////// Right ////////////////////////
	vector<TreeEdge> sortedEdgesRight = mst.shortestSidesOfImage(imgRight);
	sort(sortedEdgesRight.begin(), sortedEdgesRight.end());
	vector<MST::nodeStructure> nodeMSTRight = mst.makeMST(imgRight.rows, imgRight.cols, sortedEdgesRight);
	vector<vector<double>> localFilterRight = mst.imageLocallyFiltered(imgRight.rows, imgRight.cols, imgRight, nodeMSTRight);
	vector<vector<double>> nonLocalFilterRight = mst.imageNonLocalFilter(imgRight.rows, imgRight.cols, nodeMSTRight, squareDimension, imgRight);



	// a=1 , b=0 currently
	//vector<vector<vector<double>>> abConstants = mst.abMappingCostants(imgLeft, squareDimension);

	// final disparity map
	//vector<vector<double>> disparityMap = mst.findDisparityMap(imgLeft,imgRight);

	cout << "FINISHED " << endl;

	system("PAUSE");
	return 0;
}

