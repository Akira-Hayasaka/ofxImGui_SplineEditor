//
//  UI.hpp
//  ImGUI
//
//  Created by Akira Hayasaka on 2017/04/25.
//
//

#ifndef UI_hpp
#define UI_hpp

#include "ofMain.h"
#include "ofxImGui.h"

class UI
{
public:
    
    UI();
    ~UI(){};
    void setup();
    void draw();
    
private:
    
    ofxImGui::Gui gui;
    
    vector<ImVec2> controlPoints;
};

#endif /* UI_hpp */
