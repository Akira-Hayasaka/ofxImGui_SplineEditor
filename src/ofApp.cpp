#include "ofApp.h"

void ofApp::setup()
{
    gui.setup();
}

void ofApp::update(){}

void ofApp::draw()
{
    gui.begin();
//    ImGui::Begin("Another Window");
//    ImGui::Text("Hello");
//    ImGui::End();
    
    ImGui::Text("Hello, world!");
    
    ImGui::Begin("Another Window");
    ImGui::Text("Curve Editor (based on the code by https://github.com/nem0/LumixEngine):");
    ImVec2 editor_size;
    ImGui::CurveEditor editor = ImGui::BeginCurveEditor("Curve Editor Child");
    if (editor.valid)
    {
        editor_size = ImVec2(ImGui::CalcItemWidth(), ImGui::GetItemRectSize().y);
        static ImVec2 point1[3] = {ImVec2(0.10f, 0.0f), ImVec2(0.0f, 0.0f), ImVec2(.0f, .10f)};
        static ImVec2 point2[3] = {ImVec2(0.0f, 0.0f), ImVec2(0.0f, 0.0f), ImVec2(.0f, .10f)};
//        static ImVec2 point3[3] = {ImVec2(0.0f, 0.0f), ImVec2(0.0f, 0.0f), ImVec2(.0f, .0f)};
//        static ImVec2 point4[3] = {ImVec2(0.10f, 0.0f), ImVec2(0.0f, 0.0f), ImVec2(.0f, .10f)};
        if (ImGui::CurveSegment(point1, editor))
        {
            //	changed = true;
        }
        ImGui::CurveSegment(point2, editor);
//        ImGui::CurveSegment(point3, editor);
//        ImGui::CurveSegment(point4, editor);
        ImGui::EndCurveEditor(editor);
    }
    ImGui::End();
                 
    gui.end();
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
