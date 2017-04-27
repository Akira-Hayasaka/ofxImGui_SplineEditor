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
    void update();
    void setPosition(const int idx);
    int getNumFile() { return seqPath.size(); }
    int getPosition() { return curPosIdx; }
    
protected:
    
    void threadedFunction() override;
    
private:
    
    void load(const int idx);
    
    static const int NUM_LOAD = 10;
    template<class T>
    using idx_binded = map<int, T>;
    
    ofThreadChannel<idx_binded<string>> loadCh;
    ofThreadChannel<bool> waitCh;
    
    int curPosIdx;
    
    vector<string> seqPath;    
    idx_binded<ofPixels> idx_pxs;
    ofImage drawer;
};

#endif /* Preview_hpp */
