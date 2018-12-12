#include "HoughTrans.h"


void HoughTrans::loadPtCloudData()
{
	this->dir.allowExt("txt");
	this->dir.listDir("");
	std::vector<ofFile> listFile = this->dir.getFiles();
	ofBuffer buffer = ofBufferFromFile("rotation.txt");

	ofstream transformData;
	transformData.open("data\\transformData.txt", ios::app);

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

	float tX = 0, tY = 0, tZ = 0, rX = 0, rY = 0, rZ = 0;

	for (int index = 0; index < listFile.size(); ++index) {
		std::string fileName = listFile.at(index).getFileName();
		std::vector<std::string> linesOfTheFile;
		ofVec3f point;
		ofColor color;
		ofBuffer buffer = ofBufferFromFile(fileName);
		std::vector<ofVec3f> tempPointClouds;
		std::vector<ofFloatColor> tempPointCloudsColor;
		bool start = false;

		cout << "rotData.at(index) : " << rotData.at(index) << endl;
		cout << "transData.at(index) : " << transData.at(index) << endl;

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
				ofFloatColor color = ofFloatColor(ofToFloat(oneLine.at(3))/255.0, ofToFloat(oneLine.at(4)) / 255.0, ofToFloat(oneLine.at(5)) / 255.0, ofToFloat(oneLine.at(6)) / 255.0);
				tempPointClouds.push_back(point);
				tempPointCloudsColor.push_back(color);

				point.x = point.x + transData.at(index).x + tX;
				point.y = point.y + transData.at(index).y + tY;
				point.z = point.z + transData.at(index).z + tZ;

				//cv::Mat rotateX = cv::Mat::ones(3, 3, CV_32FC1);
				//cv::Mat rotateY = cv::Mat::ones(3, 3, CV_32FC1);
				//cv::Mat rotateZ = cv::Mat::ones(3, 3, CV_32FC1);
				//float rotateX = [1, 0, 0; 0, cos a, -sin a; 0, sin a, cos a];
				//float rotateY = [cos a, 0, sin a; 0, 1, 0; -sin a, 0, cos a];
				//float rotateZ = [cos a, -sin a, 0; sin a, cos a, 0; 0, 0, 1];
				
				//cv::Mat rotateX = (cv::Mat_<float>(3, 3) <<
				//	1, 0, 0,
				//	0, cos(x), -sin(x),
				//	0, sin(x), cos(x));

				//cv::Mat rotateY = (cv::Mat_<float>(3, 3) <<
				//	cos(y), 0, sin(y),
				//	0, 1, 0,
				//	-sin(y), 0, cos(y));

				//cv::Mat rotateZ = (cv::Mat_<float>(3, 3) <<
				//	cos(z), -sin(z), 0,
				//	sin(z), cos(z), 0,
				//	0, 0, 1);

				//cv::Mat pointMat = (cv::Mat_<float>(3, 1) <<
				//	point.x, point.y, point.z);

				//pointMat = pointMat * rotateX * rotateY * rotateZ;
				//pointMat = rotateX * pointMat;

				//point.x = pointMat.at<float>(0, 0);
				//point.y = pointMat.at<float>(1, 0);
				//point.z = pointMat.at<float>(2, 0);

				//ofRotateX(this->rotData.at(index).x + rX);
				//ofRotateY(this->rotData.at(index).y + rY);
				//ofRotateZ(this->rotData.at(index).z + rZ);
				
				float a = this->rotData.at(index).x + rX;
				float b = this->rotData.at(index).y + rY;
				float c = this->rotData.at(index).z + rZ;

				ofVec3f vec(point.x, point.y, point.z);
				ofVec3f rotX(1, 0, 0);
				ofVec3f rotY(0, 1, 0);
				ofVec3f rotZ(0, 0, 1);

				vec = vec.getRotated(a, rotX);
				vec = vec.getRotated(b, rotY);
				vec = vec.getRotated(c, rotZ);

				//a = a * PI / 180.0f;
				//b = b * PI / 180.0f;
				//c = c * PI / 180.0f;

				////point.x = point.x;
				//point.y = cos(a) * point.y + -sin(a) * point.z;
				//point.z = sin(a) * point.y + cos(a) * point.z;

				//point.x = cos(b) * point.x + -sin(b) * point.z;
				////point.y = point.y;
				//point.z = sin(b) * point.x + cos(b) * point.z;

				//point.x = cos(c) * point.x + -sin(c) * point.y;
				//point.y = sin(c) * point.x + cos(c) * point.y;
				////point.z = point.z;

				point = vec;

				transformData << point.x << " " << point.y << " " << point.z << " " <<
					color.r * 255.0 << " " << color.g * 255.0 << " " << color.b * 255.0 << " " << color.a * 255.0 << endl;
			}
		}

		tX += this->transData.at(index).x;
		tY += this->transData.at(index).y;
		tZ += this->transData.at(index).z;
		rX += this->rotData.at(index).x;
		rY += this->rotData.at(index).y;
		rZ += this->rotData.at(index).z;

		this->pointClouds.push_back(tempPointClouds);
		this->pointCloudsColor.push_back(tempPointCloudsColor);
	}

	/*
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

	*/

	transformData.close();
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
