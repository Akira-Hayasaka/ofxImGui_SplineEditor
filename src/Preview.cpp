//
//  Preview.cpp
//  ImGUI
//
//  Created by Akira Hayasaka on 2017/04/25.
//
//

#include "Preview.hpp"

Preview::Preview() :
sirokan_mov_path("/Users/Akira/Project/20170213GrandCore/sirokan_small_mov/sirokan_small.mov"),
last_selected_frame(0)
{
    vid.load(sirokan_mov_path);
    ofLog() << "num frame = " << vid.getTotalNumFrames();
    ofLog() << "dur = " << vid.getDuration();
    
    ofAddListener(Global::ui->preview_frame_change_event, this, &Preview::setFrame);
}

Preview::~Preview()
{
    
}

void Preview::update()
{
    vid.update();
}

void Preview::draw()
{
    vid.draw(0, 0, APP_W, APP_H);
}
