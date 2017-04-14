#pragma once

namespace ImGui
{
    
    
//    class Spline
//    {
//    public:
//        Spline();
//        void setup
//    };
    
    IMGUI_API int Curve(const char *label, const ImVec2& size, int maxpoints, ImVec2 *points);
    IMGUI_API float CurveValue(float p, int maxpoints, const ImVec2 *points);
    IMGUI_API float CurveValueSmooth(float p, int maxpoints, const ImVec2 *points);
    
    struct CurveEditor
    {
        bool valid;
        ImVec2 beg_pos;
        ImVec2 graph_size;
        static const float GRAPH_MARGIN;
        static const float HEIGHT;
        ImVec2 inner_bb_min;
        ImVec2 inner_bb_max;
        int point_idx;
    };
    
    IMGUI_API CurveEditor BeginCurveEditor(const char* label, const int bin);
    IMGUI_API bool CurveSegment(ImVec2* point, CurveEditor& editor);
    IMGUI_API void SplineCurve(const int n, vector<ImVec2> curve, CurveEditor& editor);
    IMGUI_API void EndCurveEditor(const CurveEditor& editor);

} // namespace ImGui

