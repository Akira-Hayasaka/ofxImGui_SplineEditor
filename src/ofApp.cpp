#include "ofApp.h"

void ofApp::setup()
{
    ofSetVerticalSync(true);
    
    float max = 1.0;
    ofPolyline lutLine;
    for (float i = 0; i < max; i += max/bin)
    {
        float e = st2084_pq(i);
        lutLine.addVertex(ofVec2f(ofMap(i, 0.0, max, 0, bin),
                                  ofMap(e, 0.0, max, 0, bin)));
    }
    
    lutLine = lutLine.getResampledByCount(30);
    vector<ofVec2f> simplified;
    for (auto v3 : lutLine.getVertices())
    {
        simplified.push_back(ofVec2f(v3));
        controlPoints.push_back(ImVec2(ofMap(simplified.back().x, 0.0, bin, 0.0, mappedBin, true),
                                     ofMap(simplified.back().y, 0.0, bin, mappedBin, 0.0, true)));
    }
    
    gui.setup();
}

void ofApp::update(){}

void ofApp::draw()
{
    ofBackground(0);
    
    gui.begin();
    
    ImGui::Begin("Spline Editor");
    ImGui::CurveEditor editor = ImGui::BeginCurveEditor("", mappedBin);
    if (editor.valid)
    {
        ImGui::SplineCurve(mappedBin, controlPoints, editor);
        ImGui::EndCurveEditor(editor);
    }
    ImGui::End();
    
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
