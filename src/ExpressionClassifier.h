#ifndef _Expression_Classifier_H_
#define _Expression_Classifier_H_

#include "ofxCv.h"
#include "Expression.h"

#include "ofxFaceTracker.h"

class ofxFaceTracker;

class ExpressionClassifier {
public:
	ExpressionClassifier();
	void save(string directory) const;
    void save(string mainDir, string directory) const;
	void load(string directory);
	unsigned int classify(const ofxFaceTracker& tracker);
	unsigned int getPrimaryExpression() const;
	unsigned int getSecondaryExpression() const;
	double getProbability(unsigned int i) const;
	string getDescription(unsigned int i) const;
	Expression& getExpression(unsigned int i);
	void setSigma(double sigma);
	double getSigma() const;
	unsigned int size() const;
	void addExpression(string description = "");
	void addExpression(Expression& expression);
	void addSample(const ofxFaceTracker& tracker);
    void addSample(const ofxFaceTracker& tracker, int id);
	void reset();
	void drawPrimary();
protected:
	vector<Expression> expressions;
	vector<double> probability;
	float sigma;
	
	int test;
};

#endif
