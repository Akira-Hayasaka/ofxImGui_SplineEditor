#pragma once

namespace ImGui
{
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
        int dispSize;
    };
    
    IMGUI_API CurveEditor BeginCurveEditor(const char* label, const int bin);
    IMGUI_API void SplineCurve(const int bin, vector<ImVec2>& controlPoints, CurveEditor& editor);
    IMGUI_API void EndCurveEditor(const CurveEditor& editor);

} // namespace ImGui

