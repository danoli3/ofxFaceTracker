#include "Expression.h"

using namespace cv;

#define it at<int>
#define db at<double>

vector<int> Expression::consecutive(int start, int end) {
	int n = end - start;
	vector<int> result(n);
	for(int i = 0; i < n; i++) {
		result[i] = start + i;
	}
	return result;
}

vector<int> Expression::getFeatureIndices(Feature feature) {
	switch(feature) {
		case LEFT_JAW: return consecutive(0, 9);
		case RIGHT_JAW: return consecutive(8, 17);
		case JAW: return consecutive(0, 17);
		case LEFT_EYEBROW: return consecutive(17, 22);
		case RIGHT_EYEBROW: return consecutive(22, 27);
		case LEFT_EYE: return consecutive(36, 42);
		case RIGHT_EYE: return consecutive(42, 48);
		case OUTER_MOUTH: return consecutive(48, 60);
		case INNER_MOUTH:
			static int innerMouth[] = {48,60,61,62,54,63,64,65};
			return vector<int>(innerMouth, innerMouth + 8);
		case NOSE_BRIDGE: return consecutive(27, 31);
		case NOSE_BASE: return consecutive(31, 36);
		case FACE_OUTLINE:
			static int faceOutline[] = {17,18,19,20,21,22,23,24,25,26, 16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0};
			return vector<int>(faceOutline, faceOutline + 27);
		case ALL_FEATURES: return consecutive(0, 66);
	}
}

Expression::Expression(string description) {
	this->description = description;
	
	
	string triFile = ofToDataPath("model/face.tri");
	
	tri = FACETRACKER::IO::LoadTri(triFile.c_str());
}

void Expression::setDescription(string description) {
	this->description = description;
}

string Expression::getDescription() const {
	return description;
}

void Expression::addSample(const Mat& sample) {
	samples.push_back(sample.clone());
}

Mat& Expression::getExample(unsigned int i) {
	return samples[i];
}

void Expression::draw(unsigned int ex) {
	
	ofMesh mesh = getMesh(getImagePoints(ex), ex);
	mesh.draw();
	/**
	ofPushStyle();
	ofNoFill();
	getImageFeature(LEFT_EYE, ex).draw();
	getImageFeature(RIGHT_EYE,ex).draw();
	getImageFeature(LEFT_EYEBROW,ex).draw();
	getImageFeature(RIGHT_EYEBROW,ex).draw();
	getImageFeature(NOSE_BRIDGE,ex).draw();
	getImageFeature(NOSE_BASE,ex).draw();
	getImageFeature(INNER_MOUTH,ex).draw();
	getImageFeature(OUTER_MOUTH,ex).draw();
	getImageFeature(JAW,ex).draw();
	ofPopStyle();
	 
	 */
	
}

ofPolyline Expression::getImageFeature(Feature feature, unsigned ex) const {
	return getFeature(feature, getImagePoints(ex));
}

void Expression::addTriangleIndices(ofMesh& mesh) const {
	int in = tri.rows;
	for(int i = 0; i < tri.rows; i++) {
		int i0 = tri.it(i, 0), i1 = tri.it(i, 1), i2 = tri.it(i, 2);
		//bool visible = getVisibility(i0) && getVisibility(i1) && getVisibility(i2);
		//if(useInvisible || visible) {
			mesh.addIndex(i0);
			mesh.addIndex(i1);
			mesh.addIndex(i2);
		//}
	}
}


ofVec2f Expression::getImagePoint(int i, unsigned int ex) const {
	const Mat& shape = samples[ex];
	int n = shape.rows / 2;
	return ofVec2f(shape.db(i, 0), shape.db(i + n, 0)) / 1;
}

vector<ofVec2f> Expression::getImagePoints(unsigned ex) const {
	int n = exsize(ex);
	vector<ofVec2f> imagePoints(n);
	for(int i = 0; i < n; i++) {
		imagePoints[i] = getImagePoint(i, ex);
	}
	return imagePoints;
}

int Expression::exsize(unsigned ex) const {
	return samples[ex].rows / 2;
}

unsigned int Expression::size() const {
	return samples.size();
}

void Expression::updateObjectPoints(unsigned ex) {
	//const Mat& mean = samples[ex]._clm._pdm._M;
	//const Mat& variation = tracker._clm._pdm._V;
	//const Mat& weights = tracker._clm._plocal;
	//objectPoints = mean + variation * weights;
}

ofMesh Expression::getObjectMesh(unsigned ex) const {
	//return getMesh(getObjectPoints(ex));
}

vector<ofVec3f> Expression::getObjectPoints(unsigned ex) const {
	int n = exsize(ex);
	vector<ofVec3f> objectPoints(n);
	for(int i = 0; i < n; i++) {
		objectPoints[i] = getObjectPoint(i, ex);
	}
	return objectPoints;
}

ofVec3f Expression::getObjectPoint(int i, unsigned ex) const {
	//int n = objectPoints.rows / 3;
	//return ofVec3f(objectPoints.db(i,0), objectPoints.db(i+n,0), objectPoints.db(i+n+n,0));
}

void Expression::reset() {
	samples.clear();
}

void Expression::save(string filename) const {
	FileStorage fs(ofToDataPath(filename), FileStorage::WRITE);
	fs <<	"description" << description <<
	"samples" << "[";
	for(int i = 0; i < size(); i++) {
		fs << samples[i];
	}
	fs << "]";
}

void Expression::load(string filename) {
	FileStorage fs(ofToDataPath(filename), FileStorage::READ);
	description = (string) fs["description"];
	FileNode samplesNode = fs["samples"];
	int n = samplesNode.size();
	samples.resize(n);
	for(int i = 0; i < n; i++) {
		samplesNode[i] >> samples[i];
	}
}