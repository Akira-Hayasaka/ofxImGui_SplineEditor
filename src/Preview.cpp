//
//  Preview.cpp
//  ImGUI
//
//  Created by Akira Hayasaka on 2017/04/25.
//
//

#include "Preview.hpp"

Preview::Preview()
{
    ofxXmlSettings xml;
    xml.load("settings/setting.xml");
    ofDirectory dir;
    dir.open(xml.getValue("sirokan_dir_for_preview", ""));
    dir.listDir();
    for (int i = 0; i < dir.size(); i++)
        seqPath.push_back(dir.getFile(i).getAbsolutePath());
    dir.close();
    xml.clear();
    
    texs.resize(NUM_LOAD);
    for (int i = 0; i < texs.size(); i++)
        ofLoadImage(texs.at(i), seqPath.at(i));
    curTex = &texs.front();
    
    state = AVAILABLE;
    startThread();
}

Preview::~Preview()
{
    loadCh.close();
    waitCh.close();
    waitForThread(true);
}

void Preview::draw()
{
    if (curTex != NULL)
        curTex->draw(0, 0);
}

void Preview::threadedFunction()
{
    
}
