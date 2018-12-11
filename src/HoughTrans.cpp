#include "HoughTrans.h"


void HoughTrans::loadPtCloudData()
{
	this->dir.allowExt("txt");
	this->dir.listDir("");
	std::vector<ofFile> listFile = this->dir.getFiles();
	ofBuffer buffer = ofBufferFromFile("rotation.txt");
	for (auto line : buffer.getLines()) {
		std::vector<std::string> oneLine = ofSplitString(line, " ");
		std::cout << line << std::endl;
		if (line.empty())
			continue;
		ofVec3f rot = ofVec3f(ofToFloat(oneLine.at(0)), ofToFloat(oneLine.at(1)), ofToFloat(oneLine.at(2)));
		ofVec3f trans = ofVec3f(ofToFloat(oneLine.at(3)), ofToFloat(oneLine.at(4)), ofToFloat(oneLine.at(5)));
		this->rotData.push_back(rot);
		this->transData.push_back(trans);
	}
	for (int index = 0; index < listFile.size(); ++index) {
		std::string fileName = listFile.at(index).getFileName();
		std::vector<std::string> linesOfTheFile;
		ofVec3f point;
		ofColor color;
		ofBuffer buffer = ofBufferFromFile(fileName);
		std::vector<ofVec3f> tempPointClouds;
		std::vector<ofFloatColor> tempPointCloudsColor;
		bool start = false;
		for (auto line : buffer.getLines()) {
			if (line.find("end") == 0)
			{
				start = true;
				continue;
			}
			if (start) {
				linesOfTheFile.push_back(line);
//				std::cout << line << std::endl;
				std::vector<std::string> oneLine = ofSplitString(line, " ");
				if (oneLine.at(0) == "-inf" || oneLine.at(0).empty())
					continue;
				point.x = ofToFloat(oneLine.at(0));
				point.y = ofToFloat(oneLine.at(1));
				point.z = ofToFloat(oneLine.at(2));
				ofFloatColor color = ofFloatColor(ofToFloat(oneLine.at(3))/255.0, ofToFloat(oneLine.at(4)) / 255.0, ofToFloat(oneLine.at(5)) / 255.0, ofToFloat(oneLine.at(6)));
				tempPointClouds.push_back(point);
				tempPointCloudsColor.push_back(color);
			}
		}
		this->pointClouds.push_back(tempPointClouds);
		this->pointCloudsColor.push_back(tempPointCloudsColor);
	}
	this->drawMesh();
}

void HoughTrans::drawMesh()
{
	for (int index = 0; index < this->pointClouds.size(); ++index) {
		ofMesh mesh;
		mesh.setMode(OF_PRIMITIVE_POINTS);
		mesh.enableColors();
		mesh.addVertices(this->pointClouds.at(index));
		mesh.addColors(this->pointCloudsColor.at(index));
		this->meshs.push_back(mesh);
	}
}

void HoughTrans::draw()
{
	ofSetBackgroundColor(ofColor(0, 0, 0));
	cam.begin();

	ofPushMatrix();
	ofRotateZ(90);
	ofDrawGridPlane(300);
	ofPopMatrix();

	double tX = 0, tY = 0, tZ = 0, rX = 0, rY = 0, rZ = 0;
	for (int index = 0; index < this->meshs.size(); ++index) {
		ofPushMatrix();
		ofTranslate(this->transData.at(index).x + tX, this->transData.at(index).y + tY, this->transData.at(index).z + tZ);
		ofRotateX(this->rotData.at(index).x + rX);
		ofRotateY(this->rotData.at(index).y + rY);
		ofRotateZ(this->rotData.at(index).z + rZ);
		this->meshs.at(index).draw();
		ofPopMatrix();
		tX += this->transData.at(index).x;
		tY += this->transData.at(index).y;
		tZ += this->transData.at(index).z;
		rX += this->rotData.at(index).x;
		rY += this->rotData.at(index).y;
		rZ += this->rotData.at(index).z;
	}

	cam.end();



}
