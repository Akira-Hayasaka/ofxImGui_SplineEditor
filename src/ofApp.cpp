#include "ofApp.h"

void ofApp::setup()
{
    ofSetVerticalSync(true);
    ofBackground(ofColor::black);
    
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
//        controlPoints.push_back(ImVec2(ofMap(simplified.back().x, 0.0, bin, 0.0, 512, true),
//                                     ofMap(simplified.back().y, 0.0, bin, 512, 0.0, true)));
    }
    controlPoints.push_back(ImVec2(0, 200));
    controlPoints.push_back(ImVec2(200, 0));
    gui.setup();
    
    bg.load("/Users/Akira/Project/20170213GrandCore/UIDesign/background.png");
    product.load("/Users/Akira/Project/20170213GrandCore/UIDesign/product.png");
    product.resize(product.getWidth() * 0.15, product.getHeight() * 0.15);
}

void ofApp::update(){}

void ofApp::draw()
{
    bg.draw(0, 0, 7680 * 0.25, 2160 * 0.25);
    
    gui.begin();
    
    bool p_open = true;
    ImGui::Begin("Selected Item", &p_open, ImVec2(200, 300), 0.5f,
                 ImGuiWindowFlags_NoResize |
//                 ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoCollapse);
    ImGui::PushItemWidth(-1);
    static int item = 1;
    ImGui::Combo("", &item, "movie\0sequece\0still\0\0");   // Combo using
    static bool check = true;
//    ImGui::Checkbox("drop credit image", &check);
    ImGui::Dummy(ImVec2(200, 210));
    ImGui::Button("Play"); ImGui::SameLine();
    static float pct = 0.0;
    ImGui::SliderFloat("pct", &pct, 0.0f, 1.0f, "pct = %.3f");
    ImGui::PopItemWidth();
    ImGui::End();
    
    ImGui::SetNextWindowSize(ImVec2(250, 520), ImGuiSetCond_Always);
    ImGui::Begin("Tweaker");
    if (ImGui::CollapsingHeader("Gamma Curve",
                                ImGuiTreeNodeFlags_CollapsingHeader |
                                ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::CurveEditor editor = ImGui::BeginCurveEditor("", 200);
        if (editor.valid)
        {
            ImGui::SplineCurve(200, controlPoints, editor);
            ImGui::EndCurveEditor(editor);
        }
    }
    if (ImGui::CollapsingHeader("Color",
                                ImGuiTreeNodeFlags_CollapsingHeader |
                                ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::PushItemWidth(-1);
        static float bri = 0.5, sat = 0.5, cont = 0.5, hue = 0.5;
        ImGui::SliderFloat("brightness", &bri, 0.0f, 1.0f, "brightness = %.3f");
        ImGui::SliderFloat("saturation", &sat, 0.0f, 1.0f, "saturation = %.3f");
        ImGui::SliderFloat("contrast", &cont, 0.0f, 1.0f, "contrast = %.3f");
        ImGui::SliderFloat("hue", &hue, 0.0f, 1.0f, "hue = %.3f");
        ImGui::PopItemWidth();
    }
    if (ImGui::CollapsingHeader("Translation",
                                ImGuiTreeNodeFlags_CollapsingHeader |
                                ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::PushItemWidth(-1);
        static int xpos = 0, ypos = 0, rot = 0;
        static float scale = 1.0;
        ImGui::SliderInt("xpos", &xpos, -500, 500, "xPos = %.0f%");
        ImGui::SliderInt("ypos", &ypos, -500, 500, "yPos = %.0f%");
        ImGui::SliderFloat("scale", &scale, 0.5f, 2.0f, "scale = %.3f");
        ImGui::SliderInt("rot", &rot, 0, 360, "rotation = %.0f%");
        ImGui::PopItemWidth();
    }
    ImGui::End();
    

    ImGui::Begin("Global UI", &p_open, ImVec2(600, 35), 0.5f,
                 ImGuiWindowFlags_NoTitleBar |
                 ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoCollapse);
    ImGui::PushItemWidth(-1);
    ImGui::Button("Play"); ImGui::SameLine();
    ImGui::Button("Render");ImGui::SameLine();
    static float pct2 = 0.0;
    ImGui::SliderFloat("pct2", &pct2, 0.0f, 1.0f, "preview pos = %.3f");
    ImGui::PopItemWidth();
    ImGui::End();
    
    gui.end();
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    product.draw(ofGetWidth()/2, ofGetHeight()/2);
    ofSetRectMode(OF_RECTMODE_CORNER);
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

void ofApp::keyPressed(int key)
{
//    ImVec2 front = controlPoints.
}
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
