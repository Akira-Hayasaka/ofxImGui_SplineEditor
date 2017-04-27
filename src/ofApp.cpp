#include "ofApp.h"

void ofApp::setup()
{
    ofSetVerticalSync(true);
    ofBackground(ofColor::black);
    ofSetFrameRate(60);
    
    Global::ui = make_shared<UI>();
    
    product.load("/Users/Akira/Project/20170213GrandCore/UIDesign/product.png");
    product.resize(product.getWidth() * 0.15, product.getHeight() * 0.15);
    
    vid.load("/Users/Akira/Project/20170213GrandCore/sirokan_small_mov/sirokan_small.mov");
    ofLog() << "num frame = " << vid.getTotalNumFrames();
    ofLog() << "dur = " << vid.getDuration();
    
    testIdx = 0;
}

void ofApp::exit()
{
    Global::ui.reset();
}

void ofApp::update()
{
//    if (testIdx < preview.getNumFile())
//    {
//        preview.setPosition(testIdx);
//        testIdx++;
//    }
    vid.update();
    preview.update();
}

void ofApp::draw()
{
//    preview.draw();
    vid.draw(0, 0);
    
    Global::ui->draw();
    
    ofDrawBitmapStringHighlight("fps:" + ofToString(ofGetFrameRate(), 2), 10, 10);
    
//    ofSetRectMode(OF_RECTMODE_CENTER);
//    product.draw(ofGetWidth()/2, ofGetHeight()/2);
//    ofSetRectMode(OF_RECTMODE_CORNER);
}

void ofApp::keyPressed(int key)
{
    if (!vid.isPlaying())
    {
        vid.setPaused(false);
        vid.play();
    }
    else
        vid.setPaused(true);
}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y )
{
    
}
void ofApp::mouseDragged(int x, int y, int button)
{
    vid.setFrame(ofMap(x, 0, ofGetScreenWidth(), 0, preview.getNumFile(), true));
//    preview.setPosition(ofMap(x, 0, ofGetScreenWidth(), 0, preview.getNumFile(), true));
}
void ofApp::mousePressed(int x, int y, int button)
{
    
}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}
