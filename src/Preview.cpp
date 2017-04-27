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
    dir.sort();
    for (int i = 0; i < dir.size(); i++)
        seqPath.push_back(dir.getFile(i).getAbsolutePath());
    dir.close();
    xml.clear();
    
    startThread();
    
    setPosition(0);
}

Preview::~Preview()
{
    loadCh.close();
    waitCh.close();
    waitForThread(true);
}

void Preview::draw()
{
    if (drawer.isAllocated())
        drawer.draw(0, 0);
}

void Preview::update()
{    
    bool ready;
    if (waitCh.tryReceive(ready))
    {
        if (idx_pxs.count(curPosIdx) != 0)
        {
            drawer.getPixels() = idx_pxs[curPosIdx];
            drawer.update();
        }
    }
}

void Preview::setPosition(const int idx)
{
    curPosIdx = idx;
    load(idx);
}

void Preview::threadedFunction()
{
    idx_binded<string> idx_path;
    while (loadCh.receive(idx_path))
    {
        for (auto e : idx_path)
        {
            if (idx_pxs.count(e.first) == 0)
            {
                ofLoadImage(idx_pxs[e.first], e.second);
                ofLog() << "loaded " << e.first;
            }
        }
        waitCh.send(true);
    }
}

void Preview::load(const int idx)
{
    int i = 0;
//    for (int i = 0; i < NUM_LOAD; i++)
    {
//        ofLog() << seqPath.at(idx + i);
        idx_binded<string> idx_path = {{idx + i, seqPath.at(idx + i)}};
        loadCh.send(idx_path);
    }
}
