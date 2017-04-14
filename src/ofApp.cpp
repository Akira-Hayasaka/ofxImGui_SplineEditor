#include "ofApp.h"

void ofApp::setup()
{
    ofSetVerticalSync(true);
    
    float max = 1.0;
    ofPolyline lutLine;
    for (float i = 0; i < max; i += max/bin)
    {
        float e = st2084_pq(i);
        curve.push_back(ofVec2f(ofMap(i, 0.0, max, 0, bin),
                                ofMap(e, 0.0, max, 0, bin)));
        lutLine.addVertex(curve.back());
        
    }
    
//    for (float i = 0; i < max; i += max / 10)
//    {
//        float e = st2084_pq(i);
//        mappedCurve.push_back(ImVec2(i, e));
//    }
    
    lutLine = lutLine.getResampledByCount(30);
    vector<ofVec2f> simplified;
    for (auto v3 : lutLine.getVertices())
    {
        simplified.push_back(ofVec2f(v3));
        mappedCurve.push_back(ImVec2(ofMap(simplified.back().x, 0.0, bin, 0.0, mappedBin, true),
                                     ofMap(simplified.back().y, 0.0, bin, mappedBin, 0.0, true)));
    }
    curvesTool.setup(bin, mappedBin, simplified);
    
    controlPoints = mappedCurve;
    
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
    
//    ImGui::Text("Hello, world!");
//    
//    ImGui::Begin("Another Window");
//    if (ImGui::Curve("curve editor", ImVec2(512, 512), 30, &mappedCurve[0]))
//    {
//        
//    }
//    ImGui::End();
    
    
    ImGui::Text("Hello, world!");
    
    ImGui::Begin("Spline Editor");
    ImGui::CurveEditor editor = ImGui::BeginCurveEditor("", mappedBin);
    if (editor.valid)
    {
        ImGui::SplineCurve(mappedBin, controlPoints, editor);
        ImGui::EndCurveEditor(editor);
    }
    ImGui::End();
    
//    ImGui::Begin("Another Window");
//    ImGui::Text("Curve Editor (based on the code by https://github.com/nem0/LumixEngine):");
//    ImVec2 editor_size;
//    ImGui::CurveEditor editor = ImGui::BeginCurveEditor("Curve Editor Child");
//    if (editor.valid)
//    {
//        editor_size = ImVec2(ImGui::CalcItemWidth(), ImGui::GetItemRectSize().y);
//        static ImVec2 point1[3] = {ImVec2(0.10f, 0.0f), ImVec2(0.0f, 0.0f), ImVec2(.0f, .10f)};
//        static ImVec2 point2[3] = {ImVec2(0.0f, 0.0f), ImVec2(0.0f, 0.0f), ImVec2(.0f, .10f)};
////        static ImVec2 point3[3] = {ImVec2(0.0f, 0.0f), ImVec2(0.0f, 0.0f), ImVec2(.0f, .0f)};
////        static ImVec2 point4[3] = {ImVec2(0.10f, 0.0f), ImVec2(0.0f, 0.0f), ImVec2(.0f, .10f)};
//        if (ImGui::CurveSegment(point1, editor))
//        {
//            //	changed = true;
//        }
//        ImGui::CurveSegment(point2, editor);
////        ImGui::CurveSegment(point3, editor);
////        ImGui::CurveSegment(point4, editor);
//        ImGui::EndCurveEditor(editor);
//    }
//    ImGui::End();
    
    gui.end();
}

float ofApp::linear(float sig)
{
    return sig;
}

float ofApp::sRGB(float sig)
{
    float gamma = 2.2;
    return pow(sig, 1.0/gamma);
}

float ofApp::bt709(float sig)
{
    float gamma = 2.4;
    return pow(sig, 1.0/gamma);
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
