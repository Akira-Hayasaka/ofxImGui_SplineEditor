#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "ofxCurvesTool.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

private:
    
    float linear(float sig);
    float sRGB(float sig);
    float bt709(float sig);
    double st2084_pq(double L);
    
    ofxImGui::Gui gui;
    
    ofxCurvesTool curvesTool;
    
    const int bin = 1024;
    const int mappedBin = 512;
    vector<ofVec2f> curve;
    vector<ImVec2> mappedCurve;
};
