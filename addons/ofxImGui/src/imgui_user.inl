#include "imgui.h"
#include "imgui_internal.h"
#include <math.h>

static const float NODE_SLOT_RADIUS = 4.0f;

namespace ImGui
{
	const float CurveEditor::GRAPH_MARGIN = 14;
	const float CurveEditor::HEIGHT = 100;

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

		editor.graph_size.x = bin;//CalcItemWidth() + (style.FramePadding.x * 2);
		editor.graph_size.y = bin;//CurveEditor::HEIGHT;

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

	void SplineCurve(vector<ImVec2>& curve, vector<ImVec2>& controlPoints, CurveEditor& editor)
	{
		ImGuiWindow* window = GetCurrentWindow();
		const ImRect inner_bb(editor.inner_bb_min, editor.inner_bb_max);

		auto transform = [inner_bb](const ImVec2& p) -> ImVec2
		{
			return ImVec2(inner_bb.Min.x + p.x, inner_bb.Min.y + p.y);
		};

		// AddPolyline(const ImVec2* points, const int points_count, ImU32 col, bool closed, float thickness, bool anti_aliased)
		vector<ImVec2> toBBLine;
		for (auto c : curve)
			toBBLine.push_back(transform(c));
		window->DrawList->AddPolyline(&toBBLine[0], toBBLine.size(), IM_COL32_WHITE, false, 1.0, true);
	}

	bool CurveSegment(ImVec2* points, CurveEditor& editor)
	{
		ImGuiWindow* window = GetCurrentWindow();

		const ImRect inner_bb(editor.inner_bb_min, editor.inner_bb_max);

		ImVec2 p_last = points[0];
		ImVec2 tangent_last = points[1];
		ImVec2 tangent = points[2];
		ImVec2 p = points[3];

		auto transform = [inner_bb](const ImVec2& p) -> ImVec2
		{
			return ImVec2(inner_bb.Min.x * (1 - p.x) + inner_bb.Max.x * p.x,
				inner_bb.Min.y * p.y + inner_bb.Max.y * (1 - p.y));
		};

		auto handlePoint = [&window, &editor, transform, inner_bb](ImVec2& p) -> bool
		{
			static const float SIZE = 3;

			ImVec2 cursor_pos = GetCursorScreenPos();
			ImVec2 pos = transform(p);

			SetCursorScreenPos(pos - ImVec2(SIZE, SIZE));
			PushID(editor.point_idx);
			++editor.point_idx;
			InvisibleButton("", ImVec2(2 * NODE_SLOT_RADIUS, 2 * NODE_SLOT_RADIUS));

			ImU32 col = IsItemHovered() ? GetColorU32(ImGuiCol_PlotLinesHovered) : GetColorU32(ImGuiCol_PlotLines);

			window->DrawList->AddLine(pos + ImVec2(-SIZE, 0), pos + ImVec2(0, SIZE), col);
			window->DrawList->AddLine(pos + ImVec2(SIZE, 0), pos + ImVec2(0, SIZE), col);
			window->DrawList->AddLine(pos + ImVec2(SIZE, 0), pos + ImVec2(0, -SIZE), col);
			window->DrawList->AddLine(pos + ImVec2(-SIZE, 0), pos + ImVec2(0, -SIZE), col);

			bool changed = false;
			if (IsItemActive() && IsMouseDragging(0))
			{
				pos += GetIO().MouseDelta;
				ImVec2 v;
				v.x = (pos.x - inner_bb.Min.x) / (inner_bb.Max.x - inner_bb.Min.x);
				v.y = (inner_bb.Max.y - pos.y) / (inner_bb.Max.y - inner_bb.Min.y);

				v = ImClamp(v, ImVec2(0, 0), ImVec2(1, 1));
				p = v;
				changed = true;
			}
			PopID();

			SetCursorScreenPos(cursor_pos);
			return changed;
		};

		auto handleTangent = [&window, &editor, transform, inner_bb](ImVec2& t, const ImVec2& p) -> bool
		{
			static const float SIZE = 2;
			static const float LENGTH = 18;

			auto normalized = [](const ImVec2& v) -> ImVec2
			{
				float len = 1.0f / sqrtf(v.x *v.x + v.y * v.y);
				return ImVec2(v.x * len, v.y * len);
			};

			ImVec2 cursor_pos = GetCursorScreenPos();
			ImVec2 pos = transform(p);
			ImVec2 tang = pos + normalized(ImVec2(t.x, -t.y)) * LENGTH;

			SetCursorScreenPos(tang - ImVec2(SIZE, SIZE));
			PushID(editor.point_idx);
			++editor.point_idx;
			InvisibleButton("", ImVec2(2 * NODE_SLOT_RADIUS, 2 * NODE_SLOT_RADIUS));

			window->DrawList->AddLine(pos, tang, GetColorU32(ImGuiCol_PlotLines));

			ImU32 col = IsItemHovered() ? GetColorU32(ImGuiCol_PlotLinesHovered) : GetColorU32(ImGuiCol_PlotLines);

			window->DrawList->AddLine(tang + ImVec2(-SIZE, SIZE), tang + ImVec2(SIZE, SIZE), col);
			window->DrawList->AddLine(tang + ImVec2(SIZE, SIZE), tang + ImVec2(SIZE, -SIZE), col);
			window->DrawList->AddLine(tang + ImVec2(SIZE, -SIZE), tang + ImVec2(-SIZE, -SIZE), col);
			window->DrawList->AddLine(tang + ImVec2(-SIZE, -SIZE), tang + ImVec2(-SIZE, SIZE), col);

			bool changed = false;
			if (IsItemActive() && IsMouseDragging(0))
			{
				tang = GetIO().MousePos - pos;
				tang = normalized(tang);
				tang.y *= -1;

				t = tang;
				changed = true;
			}
			PopID();

			SetCursorScreenPos(cursor_pos);
			return changed;
		};

		bool changed = false;

		if (editor.point_idx < 0)
		{
			if (handlePoint(p_last))
			{
				p_last.x = 0;
				points[0] = p_last;
				changed = true;
			}
		}
		else
		{
			window->DrawList->AddBezierCurve(
				transform(p_last),
				transform(p_last + tangent_last),
				transform(p + tangent),
				transform(p),
				GetColorU32(ImGuiCol_PlotLines),
				1.0f,
				20);

			if (handleTangent(tangent_last, p_last))
			{
				points[1] = ImClamp(tangent_last, ImVec2(0, -1), ImVec2(1, 1));
				changed = true;
			}

			if (handleTangent(tangent, p))
			{
				points[2] = ImClamp(tangent, ImVec2(-1, -1), ImVec2(0, 1));
				changed = true;
			}

			if (handlePoint(p))
			{
				points[3] = p;
				changed = true;
			}
		}

		return changed;
	}

} // namespace ImGui
