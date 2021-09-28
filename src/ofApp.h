#pragma once


#include "ofMain.h"
#include "ofxKinectV2.h"


class ofApp: public ofBaseApp
{
public:
    void setup() override;
    void update() override;
    void draw() override;

    void keyPressed(int key) override;
    
    std::vector<std::shared_ptr<ofxKinectV2>> kinects;

    std::vector<ofTexture> texRGB;
    std::vector<ofTexture> texRGBRegistered;
    std::vector<ofTexture> texIR;
    std::vector<ofTexture> texDepth;
    
    std::size_t currentKinect = 0;

    ofEasyCam cam;
    ofMesh pointCloud;
};
