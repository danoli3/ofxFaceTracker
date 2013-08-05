#ifndef _Expression_H_
#define _Expression_H_

#include "ofxCv.h"

#include "FaceTracker.h"
#include "IO.h"

enum Feature {
	LEFT_EYEBROW, RIGHT_EYEBROW,
	LEFT_EYE, RIGHT_EYE,
	LEFT_JAW, RIGHT_JAW, JAW,
	OUTER_MOUTH, INNER_MOUTH,
	NOSE_BRIDGE, NOSE_BASE,
	FACE_OUTLINE, ALL_FEATURES
};

class Expression {
public:
	Expression(string description = "");
	void setDescription(string description);
	string getDescription() const;
	void addSample(const cv::Mat& sample);
	cv::Mat& getExample(unsigned int i);
	unsigned int size() const;
	void reset();
	void save(string filename) const;
	void load(string filename);
	
	ofMesh getObjectMesh(unsigned ex) const;
	vector<ofVec3f> getObjectPoints(unsigned ex) const;
	ofVec3f getObjectPoint(int i, unsigned ex) const;
	void updateObjectPoints(unsigned ex);
	
	void draw(unsigned int i);
	ofPolyline getImageFeature(Feature feature, unsigned ex) const;
	
	static vector<int> consecutive(int start, int end);
	
	ofVec2f getImagePoint(int i, unsigned int ex) const;
	vector<ofVec2f> getImagePoints(unsigned ex) const;
	
	static vector<int> getFeatureIndices(Feature feature);
	template <class T> ofPolyline getFeature(Feature feature, vector<T> points) const;
	template <class T> ofMesh getMesh(vector<T> points, unsigned ex) const;
	
	void addTriangleIndices(ofMesh& mesh) const;
	
	int exsize(unsigned ex) const;
protected:
	string description;
	vector<cv::Mat> samples;
	
	cv::Mat tri;
};

template <class T>
ofPolyline Expression::getFeature(Feature feature, vector<T> points) const {
	ofPolyline polyline;
	
		vector<int> indices = getFeatureIndices(feature);
		for(int i = 0; i < indices.size(); i++) {
			int cur = indices[i];
			//if(useInvisible || getVisibility(cur)) {
				polyline.addVertex(points[cur]);
			//}
		}
		switch(feature) {
			case LEFT_EYE:
			case RIGHT_EYE:
			case OUTER_MOUTH:
			case INNER_MOUTH:
			case FACE_OUTLINE:
				polyline.close();
		}
	return polyline;
}

template <class T>
ofMesh Expression::getMesh(vector<T> points, unsigned ex) const {
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	
		int n = size();
		for(int i = 0; i < n; i++) {
			mesh.addVertex(points[i]);
			mesh.addTexCoord(getImagePoint(i, ex));
		}
		addTriangleIndices(mesh);
	return mesh;
}

#endif