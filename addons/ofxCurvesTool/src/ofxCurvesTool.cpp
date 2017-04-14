#include "ofxCurvesTool.h"

ofxCurvesTool::ofxCurvesTool()
:mouseX(0)
,mouseY(0)
,hoverState(false)
,dragState(false)
,curHover(0)
,focus(false)
,drawing(false)
,drawn(false) {
	ofAddListener(ofEvents().draw, this, &ofxCurvesTool::drawEvent);
}

GLdouble modelviewMatrix[16], projectionMatrix[16];
GLint viewport[4];
void updateProjectionState() {
	glGetDoublev(GL_MODELVIEW_MATRIX, modelviewMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
	glGetIntegerv(GL_VIEWPORT, viewport);
}

ofVec3f worldToScreen(ofVec3f world) {
	updateProjectionState();
	GLdouble x, y, z;
	gluProject(world.x, world.y, world.z, modelviewMatrix, projectionMatrix, viewport, &x, &y, &z);
	ofVec3f screen(x, y, z);
	screen.y = ofGetHeight() - screen.y;
	return screen;
}

void ofxCurvesTool::setup(const int bin, const int mappedBin, vector<ofVec2f> pLut)
{
    this->bin = bin;
    this->n = mappedBin;
    lut.resize(mappedBin);
    for (auto ent : pLut)
    {
        add(ofVec2f(ofMap(ent.x, 0.0, bin, 0.0, mappedBin, true),
                    ofMap(ent.y, 0.0, bin, 0.0, mappedBin, true)));
    }
    update();
}
               
void ofxCurvesTool::draw()
{
	drawn = true; // we've made a call to draw
	
	ofPushStyle();
	
	drawPosition = worldToScreen(ofVec2f(0, 0));
	
	ofPushMatrix();
	ofTranslate(0, n);
	ofScale(1, -1);
	
	// grid
	ofSetColor(50);
	for(int i = 0; i < n; i += 64) {
		ofDrawLine(0, i, n, i);
		ofDrawLine(i, 0, i, n);
	}
	
	// diagonal, crosshairs
	ofSetColor(100);
	ofVec2f cur;
	if(hoverState || dragState) {
		cur = controlPoints[curHover];
	} else {
		cur = ofVec2f(mouseX, lut[(int) mouseX]);
	}
	ofDrawLine(0, 0, n, n);
	if(focus) {
		ofDrawLine(0, cur.y, n, cur.y);
		ofDrawLine(cur.x, 0, cur.x, n);
	}
	
	// outline
	ofSetColor(ofColor::white);
	ofNoFill();
	ofDrawRectangle(.5, .5, n - 1, n - 1);
	
	// curve
	ofNoFill();
	ofBeginShape();
	for(int x = 0; x < n; x++) {
		ofVertex(x, lut[x]);
	}
	ofEndShape();
	
	// control points
	int m = controlPoints.size();
	for(int i = 0; i < m; i++) {
		ofPushMatrix();
		ofVec2f& cur = controlPoints[i];
		ofTranslate((int) cur.x, (int) cur.y);
		if(hoverState && curHover == i) {
			if(dragState) {
				ofFill();
			} else {
				ofNoFill();
			}
			ofDrawCircle(0, 0, 3);
		} else {
			ofFill();
			ofDrawCircle(0, 0, 2);
		}
		ofPopMatrix();
	}
	ofPopMatrix();
	
	// info for current point
	if(focus) {
		string label = ofToString((int) cur.x) + ", " + ofToString((int) cur.y);
		ofDrawBitmapString(label, 4, 18);
	}
	ofPopStyle();
}

void ofxCurvesTool::updateMouse(ofMouseEventArgs& args)
{
	mouseX = args.x - drawPosition.x;
	mouseY = n - (args.y - drawPosition.y);
	focus = dragState;
	if(ofRectangle(0, 0, n, n).inside(mouseX, mouseY)) {
		focus = true;
	} else {
		hoverState = false;
	}
	mouseX = ofClamp(mouseX, 0, n - 1);
	mouseY = ofClamp(mouseY, 0, n - 1);
	int m = controlPoints.size();
	if(focus && !dragState) {
		hoverState = false;
		for(int i = 0; i < m; i++) {
			ofVec2f& cur = controlPoints[i];
			if(abs(cur.x - mouseX) < minDistance) {
				curHover = i;
				hoverState = true;
			}
		}
	}
}

void ofxCurvesTool::mouseMoved(ofMouseEventArgs& args) {
	updateMouse(args);
}

void ofxCurvesTool::mousePressed(ofMouseEventArgs& args) {
	updateMouse(args);
	if(focus) {
		if(!hoverState) {
			add(ofVec2f(mouseX, mouseY));
			updateMouse(args);
		}
		dragState = true;
		mouseDragged(args);
	}
}

void ofxCurvesTool::mouseDragged(ofMouseEventArgs& args) {
	updateMouse(args);
	if(dragState) {
		set(curHover, ofVec2f(mouseX, mouseY));
	} else {
		focus = false;
	}
}

void ofxCurvesTool::mouseReleased(ofMouseEventArgs& args) {
	updateMouse(args);
	dragState = false;
}

void ofxCurvesTool::keyPressed(ofKeyEventArgs& args) {
	if((args.key == OF_KEY_DEL || args.key == OF_KEY_BACKSPACE) && hoverState) {
		remove(curHover);
		hoverState = false;
		dragState = false;
	}
}

/*
 this part controls whether events get through to the object or not. if the
 object is not drawn using draw(), we know here because draw() never set the
 drawn flag. in that case, we unregister events. if it changes from being off
 to on, then we register the events again.
 */
void ofxCurvesTool::drawEvent(ofEventArgs& args) {
	bool prevDrawing = drawing;
	drawing = drawn;
	if(drawing != prevDrawing) {
		if(drawing) {
            ofAddListener(ofEvents().mouseDragged,this,&ofxCurvesTool::mouseDragged);
            ofAddListener(ofEvents().mouseMoved,this,&ofxCurvesTool::mouseMoved);
            ofAddListener(ofEvents().mousePressed,this,&ofxCurvesTool::mousePressed);
            ofAddListener(ofEvents().mouseReleased,this,&ofxCurvesTool::mouseReleased);
			ofRegisterKeyEvents(this);
		} else {
            ofRemoveListener(ofEvents().mouseDragged,this,&ofxCurvesTool::mouseDragged);
            ofRemoveListener(ofEvents().mouseMoved,this,&ofxCurvesTool::mouseMoved);
            ofRemoveListener(ofEvents().mousePressed,this,&ofxCurvesTool::mousePressed);
            ofRemoveListener(ofEvents().mouseReleased,this,&ofxCurvesTool::mouseReleased);
			ofUnregisterKeyEvents(this);
		}
	}
	drawn = false; // turn the drawn flag off, for draw() to turn back on
}
