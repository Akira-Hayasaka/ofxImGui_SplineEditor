//
//  Preview.hpp
//  ImGUI
//
//  Created by Akira Hayasaka on 2017/04/25.
//
//

#ifndef Preview_hpp
#define Preview_hpp

#include "ofMain.h"
#include "ofxXmlSettings.h"

class Preview : public ofThread
{
public:
    
    Preview();
    ~Preview();
    
    void draw();
    
protected:
    
    void threadedFunction() override;
    
private:
    
    enum STATE
    {
        AVAILABLE,
        LOADING,
        PIXELREADY
    };
    STATE state;
    
    static const int NUM_LOAD = 10;
    
    ofThreadChannel<string> loadCh;
    ofThreadChannel<bool> waitCh;
    
    vector<string> seqPath;
    deque<ofTexture> texs;
    ofTexture* curTex;
};

#endif /* Preview_hpp */
