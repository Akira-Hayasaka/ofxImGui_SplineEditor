#include "imgui.h"
#include "imgui_internal.h"
#include <math.h>

namespace ImGui
{
	const float CurveEditor::GRAPH_MARGIN = 14;

	CurveEditor BeginCurveEditor(const char* label, const int bin)
	{
		CurveEditor editor;
		editor.valid = false;

		ImGuiWindow* window = GetCurrentWindow();
		if (window->SkipItems) return editor;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		editor.beg_pos = GetCursorScreenPos();
		ImVec2 cursor_pos = editor.beg_pos + ImVec2(CurveEditor::GRAPH_MARGIN, CurveEditor::GRAPH_MARGIN);
		SetCursorScreenPos(cursor_pos);

		const ImVec2 label_size = CalcTextSize(label, nullptr, true);

		editor.graph_size.x = bin;
		editor.graph_size.y = bin;

		const ImRect frame_bb(cursor_pos, cursor_pos + editor.graph_size);
		const ImRect inner_bb(frame_bb.Min + style.FramePadding, frame_bb.Max - style.FramePadding);
		editor.inner_bb_min = inner_bb.Min;
		editor.inner_bb_max = inner_bb.Max;

		const ImRect total_bb(frame_bb.Min,
			frame_bb.Max +
			ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0));

		ItemSize(total_bb, style.FramePadding.y);
		if (!ItemAdd(total_bb, nullptr)) return editor;

		editor.valid = true;
		PushID(label);

		RenderFrame(
			frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, inner_bb.Min.y), label);

		SetCursorScreenPos(cursor_pos);

		editor.point_idx = -1;

		return editor;
	}

	void EndCurveEditor(const CurveEditor& editor)
	{
		SetCursorScreenPos(editor.inner_bb_min);
		PopID();

		InvisibleButton("bg", editor.inner_bb_max - editor.inner_bb_min);
		SetCursorScreenPos(editor.beg_pos + ImVec2(0, editor.graph_size.y + 2 * CurveEditor::GRAPH_MARGIN + 4));
	}

	void SplineCurve(const int bin, vector<ImVec2>& controlPoints, CurveEditor& editor)
	{
		ImGuiWindow* window = GetCurrentWindow();
		const ImRect inner_bb(editor.inner_bb_min, editor.inner_bb_max);

		auto transform = [inner_bb](const ImVec2& p) -> ImVec2
		{
			return ImVec2(inner_bb.Min.x + p.x, inner_bb.Min.y + p.y);
		};

        auto interpolate = [bin](const vector<ImVec2> curve) -> vector<float>
        {
            // build the tridiagonal system
            // (assume 0 boundary conditions: y2[0]=y2[-1]=0)
            const vector<ImVec2>& c = curve;
            int m = c.size();
            vector< vector<float> > matrix(m, vector<float>(3, 0));
            vector<float> result(m, 0);
            matrix[0][1] = 1;
            for(int i = 1; i < m - 1; i++) {
                matrix[i][0] = (c[i].x-c[i-1].x) / 6;
                matrix[i][1] = (c[i+1].x-c[i-1].x) / 3;
                matrix[i][2] = (c[i+1].x-c[i].x) / 6;
                result[i] = (c[i+1].y-c[i].y)/(c[i+1].x-c[i].x) - (c[i].y-c[i-1].y)/(c[i].x-c[i-1].x);
            }
            matrix[m-1][1]=1;
            
            // solving pass1 (up->down)
            for(int i = 1; i < m; i++) {
                float k = matrix[i][0] / matrix[i-1][1];
                matrix[i][1] -= k * matrix[i-1][2];
                matrix[i][0] = 0;
                result[i] -= k * result[i-1];
            }
            
            // solving pass2 (down->up)
            for(int i = m - 2; i >= 0; i--) {
                float k = matrix[i][2] / matrix[i+1][1];
                matrix[i][1] -= k * matrix[i+1][0];
                matrix[i][2] = 0;
                result[i] -= k * result[i+1];
            }
            
            // return second derivative value for each point P
            vector<float> sd(m);
            for (int i = 0; i < m; i++) {
                sd[i] = result[i] / matrix[i][1];
            }

            vector<float> lut(bin);
            const ImVec2& first = curve.front();
            const ImVec2& last = curve.back();
            for(int x = 0; x < first.x; x++) {
                lut[x] = first.y;
            }
            for(int i = 0; i < m - 1; i++) {
                const ImVec2& cur = curve[i];
                const ImVec2& next = curve[i+1];
                for (int x = cur.x; x < next.x; x++) {
                    float t = (x-cur.x) / (next.x-cur.x);
                    float a = 1-t;
                    float b = t;
                    float h = next.x-cur.x;
                    lut[x] = (a*cur.y + b*next.y + (h*h/6)*((a*a*a-a)*sd[i]+(b*b*b-b)*sd[i+1]));
                }
            }
            for(int x = last.x; x < bin; x++) {
                lut[x] = last.y;
            }
            for(int x = 0; x < bin; x++) {
                lut[x] = ofClamp(lut[x], 0, bin - 1);
            }
            
            return lut;
        };
        
        auto distance = [](const ImVec2 p1, const ImVec2 p2) -> float
        {
            float vx = p1.x - p2.x;
            float vy = p1.y - p2.y;
            return (float)sqrt(vx*vx + vy*vy);
        };
        
        auto xsort = [](const ImVec2& p1, const ImVec2& p2) -> bool
        {
            return p1.x < p2.x;
        };
        
        static const float SIZE = 3;
        int rad = 5;
        int idx = 0;
        bool bActiveOrHovered = false;
        for (auto& c : controlPoints)
        {
            ImVec2 cen(transform(c));
            ImVec2 cursor_pos = GetCursorScreenPos();
            SetCursorScreenPos(cen - ImVec2(SIZE, SIZE));
            
            PushID(editor.point_idx);
            ++editor.point_idx;
            InvisibleButton("", ImVec2(2 * rad, 2 * rad));
            
            if (IsItemActive() && IsMouseDragging(0) && idx != 0 && idx != controlPoints.size()-1)
            {
                if (inner_bb.Contains(GetIO().MousePos))
                {
                    ImVec2 left = transform(controlPoints.at(idx-1));
                    ImVec2 right = transform(controlPoints.at(idx+1));
                    ImVec2 dest = transform(GetIO().MousePos - inner_bb.Min);
                    if (left.x < dest.x && dest.x < right.x)
                        c = GetIO().MousePos - inner_bb.Min;
                }
                cen = transform(c);
                window->DrawList->AddCircleFilled(cen, rad, IM_COL32(0, 255, 255, 255), 16);
                
                if (!bActiveOrHovered) bActiveOrHovered = true;
            }
            else if (IsItemHovered())
            {
                if (ImGui::IsKeyPressed(259) && idx != 0 && idx != controlPoints.size()-1)
                    controlPoints.erase(controlPoints.begin() + idx);
                else
                    window->DrawList->AddCircleFilled(cen, rad, IM_COL32(255, 0, 0, 255), 16);
                
                if (!bActiveOrHovered) bActiveOrHovered = true;
            }
            else
            {
                window->DrawList->AddCircle(cen, rad-2, IM_COL32_WHITE, 16, 2);
            }
            
            PopID();
            SetCursorScreenPos(cursor_pos);
            idx++;
        }
        
        if (!bActiveOrHovered &&
            ImGui::IsMouseClicked(0) && inner_bb.Contains(GetIO().MousePos))
        {
            controlPoints.push_back(GetIO().MousePos - inner_bb.Min);
            sort(controlPoints.begin(), controlPoints.end(), xsort);
        }
		
        vector<float> lut = interpolate(controlPoints);
        vector<ImVec2> toBBLine;
		for (auto e : lut)
			toBBLine.push_back(transform(ImVec2(toBBLine.size(), e)));
		window->DrawList->AddPolyline(&toBBLine[0], toBBLine.size(), IM_COL32_WHITE, false, 1.0, true);
	}

} // namespace ImGui
