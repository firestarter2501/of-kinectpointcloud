#include "ofApp.h"

void ofApp::setup()
{
    ofSetLogLevel(OF_LOG_VERBOSE);

    ofBackground(0);
    ofEnableDepthTest();
    ofEnableSmoothing();

    ofxKinectV2 tmp;
    std::vector <ofxKinectV2::KinectDeviceInfo> deviceList = tmp.getDeviceList();
    
    kinects.resize(deviceList.size());
    texDepth.resize(kinects.size());
    texRGB.resize(kinects.size());
    texRGBRegistered.resize(kinects.size());
    texIR.resize(kinects.size());
    
    ofxKinectV2::Settings ksettings;
    ksettings.enableRGB = true;
    ksettings.enableIR = true;
    ksettings.enableDepth = true;
    ksettings.enableRGBRegistration = true;
    ksettings.enableDepthRegistration = true;
    ksettings.config.MinDepth = 0.5;
    ksettings.config.MaxDepth = 1.0;
    
    for(int d = 0; d < kinects.size(); d++) {
        kinects[d] = std::make_shared<ofxKinectV2>();
        kinects[d]->open(deviceList[d].serial, ksettings);
    }
}


void ofApp::update()
{
    for (int d = 0; d < kinects.size(); d++)
    {
        kinects[d]->update();
        
        if (kinects[d]->isFrameNew())
        {
            if( kinects[d]->isRGBEnabled()) texRGB[d].loadData(kinects[d]->getPixels());
            if(kinects[d]->getRegisteredPixels().getWidth() > 10) texRGBRegistered[d].loadData(kinects[d]->getRegisteredPixels());
            if(kinects[d]->isIREnabled()) texIR[d].loadData(kinects[d]->getIRPixels());
            if(kinects[d]->isDepthEnabled() ) texDepth[d].loadData(kinects[d]->getDepthPixels());
            
            pointCloud.clear();
            for (std::size_t x = 0; x < texRGBRegistered[d].getWidth(); x++)
            {
                for (std::size_t y = 0; y < texRGBRegistered[d].getHeight(); y++)
                {
                    pointCloud.addVertex(kinects[d]->getWorldCoordinateAt(x, y));
                    pointCloud.addColor(kinects[d]->getRegisteredPixels().getColor(x, y));
                }
            }
        }
    }
}


void ofApp::draw()
{
    cam.begin();
    ofPushMatrix();
    ofScale(5000, -5000, -2500);
    glPointSize(2.5);
    //glEnable(GL_DYNAMIC_DRAW);
    pointCloud.drawVertices();
    ofPopMatrix();
    cam.end();
    
    if( kinects.size() < 1 ) {
        ofDrawBitmapStringHighlight( "No Kinects Detected", 40, 40 );
    }
}


void ofApp::keyPressed(int key)
{
    if (key == ' ')
    {
        currentKinect = (currentKinect + 1) % kinects.size();
    }
}

