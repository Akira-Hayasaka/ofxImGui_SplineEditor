//
//  GUI.hpp
//  ImGUI
//
//  Created by Akira Hayasaka on 2017/04/25.
//
//

#ifndef GUI_hpp
#define GUI_hpp

#include "ofMain.h"
#include "ofxImGui.h"

class GUI
{
public:
    
    void setup();
    void draw();
    
private:
    
    ofxImGui::Gui gui;
    
    vector<ImVec2> controlPoints;
};

#endif /* GUI_hpp */
