#pragma once
#include "opencv.hpp"
#include "ofMain.h"

class HoughTrans {
private :
	ofEasyCam cam;
	std::vector<ofMesh> meshs;
	ofDirectory dir;
	std::vector<std::vector<ofVec3f>> pointClouds;
	std::vector<std::vector<ofFloatColor>> pointCloudsColor;
	std::vector<ofVec3f> rotData, transData;

public:
	void loadPtCloudData();
	void drawMesh();
	void draw();
};