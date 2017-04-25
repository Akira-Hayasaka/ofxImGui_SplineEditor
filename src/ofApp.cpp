#include "ofApp.h"

void ofApp::setup()
{
    ofSetVerticalSync(true);
    ofBackground(ofColor::black);
    
    Global::ui = make_shared<UI>();
    
    bg.load("/Users/Akira/Project/20170213GrandCore/UIDesign/background.png");
    product.load("/Users/Akira/Project/20170213GrandCore/UIDesign/product.png");
    product.resize(product.getWidth() * 0.15, product.getHeight() * 0.15);
}

void ofApp::exit()
{
    Global::ui.reset();
}

void ofApp::update(){}

void ofApp::draw()
{
//    bg.draw(0, 0, 7680 * 0.25, 2160 * 0.25);
    preview.draw();
    
    Global::ui->draw();
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    product.draw(ofGetWidth()/2, ofGetHeight()/2);
    ofSetRectMode(OF_RECTMODE_CORNER);
}

void ofApp::keyPressed(int key){}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}
