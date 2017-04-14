#pragma once

#include "CurveLut.h"
#include "ofMain.h"

class ofxCurvesTool : public CurveLut {
public:
    void setup(const int bin, const int mappedBin, vector<ofVec2f> curve);
	void draw();
	
	// these are only used internally
	ofxCurvesTool();
	void mouseMoved(ofMouseEventArgs& args);
	void mousePressed(ofMouseEventArgs& args);
	void mouseDragged(ofMouseEventArgs& args);
	void mouseReleased(ofMouseEventArgs& args);
	void keyPressed(ofKeyEventArgs& args);
	void keyReleased(ofKeyEventArgs& args) {}
	void drawEvent(ofEventArgs& args);
    void mouseScrolled(int x, int y, float scrollX, float scrollY ){}
    void mouseEntered(int x, int y){}
    void mouseExited(int x, int y){}
	
protected:
	ofVec2f drawPosition;
	
	void updateMouse(ofMouseEventArgs& args);
	int mouseX, mouseY;
	bool focus;
	
	static const int minDistance = 10;
	bool hoverState;
	bool dragState;
	int curHover;
	bool drawing, drawn;
    
    int bin;
    int mappedBin;
};
