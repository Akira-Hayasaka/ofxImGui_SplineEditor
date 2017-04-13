#pragma once

namespace ImGui
{
//    class Spline
//    {
//    public:
//        Spline();
//        void setup
//    };
    
    
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
    
    IMGUI_API CurveEditor BeginCurveEditor(const char* label);
    IMGUI_API bool CurveSegment(ImVec2* point, CurveEditor& editor);
    IMGUI_API void EndCurveEditor(const CurveEditor& editor);

} // namespace ImGui

