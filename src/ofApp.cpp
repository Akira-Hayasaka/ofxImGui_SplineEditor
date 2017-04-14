#include "ofApp.h"

void ofApp::setup()
{
    ofSetVerticalSync(true);
    
    int bin = 1024;
    float max = 1.0;
    for (float i = 0; i < max; i += max/bin)
    {
        float e = st2084_pq(i);
        lut.push_back(e);
        pLut.push_back(ofVec2f(ofMap(i, 0.0, max, 0, bin),
                               ofMap(lut.back(), 0.0, max, 0, bin)));
        lutLine.addVertex(pLut.back());
    }
    
    lutLine = lutLine.getResampledByCount(30);
    vector<ofVec2f> simplified;
    for (auto v3 : lutLine.getVertices())
        simplified.push_back(ofVec2f(v3));
    curvesTool.setup(bin, 512, simplified);
    
    gui.setup();
}

void ofApp::update(){}

void ofApp::draw()
{
    ofBackground(0);
    
    ofPushMatrix();
    ofTranslate(16, 16);
    curvesTool.draw();
    ofPopMatrix();
    
    
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

double ofApp::st2084_pq(double L)
{
    double m1 = 0.1593017578125;
    double m2 = 78.84375;
    double c2 = 18.8515625;
    double c3 = 18.6875;
    double c1 = c3 - c2 + 1.0;
    
    // 0.0 <= L <= 1.0
    // L = 1.0 = 10000 cd/m^2
    return pow((c1 + c2 * pow(L, m1)) / (1.0 + c3 * pow(L, m1)), m2);
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
