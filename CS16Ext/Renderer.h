#pragma once
#include <Windows.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

void RenderLine(const ImVec2& from, const ImVec2& to, ImVec4 color, float thickness)
{
	ImDrawList* Draw = ImGui::GetWindowDrawList();
	Draw->AddLine(from, to, ImGui::GetColorU32(color), thickness);
}

void RenderCircle(const ImVec2& position, float radius, ImVec4 color, float thickness, uint32_t segments)
{
	ImDrawList* Draw = ImGui::GetWindowDrawList();
	Draw->AddCircle(position, radius, ImGui::GetColorU32(color), segments, thickness);
}

void RenderCircleFilled(const ImVec2& position, float radius, ImVec4 color, uint32_t segments)
{
	ImDrawList* Draw = ImGui::GetWindowDrawList();
	Draw->AddCircleFilled(position, radius, ImGui::GetColorU32(color), segments);
}

void RenderRect(const ImVec2& from, const ImVec2& to, ImVec4 color, float rounding, uint32_t roundingCornersFlags, float thickness)
{
	ImDrawList* Draw = ImGui::GetWindowDrawList();
	Draw->AddRect(from, to, ImGui::GetColorU32(color), rounding, roundingCornersFlags, thickness);
}

void RenderRectFilled(const ImVec2& from, const ImVec2& to, ImVec4 color, float rounding, uint32_t roundingCornersFlags)
{
	ImDrawList* Draw = ImGui::GetWindowDrawList();
	Draw->AddRectFilled(from, to, ImGui::GetColorU32(color), rounding, roundingCornersFlags);
}
float RenderText(const std::string text, const ImVec2& position, float size, ImVec4 color, bool center, ImFont* DefaultFont)
{
	ImDrawList* Draw = ImGui::GetWindowDrawList();

	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = DefaultFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			Draw->AddText(DefaultFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, color.w)), line.c_str());
			Draw->AddText(DefaultFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, color.w)), line.c_str());
			Draw->AddText(DefaultFont, size, { (position.x - textSize.x / 2.0f) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, color.w)), line.c_str());
			Draw->AddText(DefaultFont, size, { (position.x - textSize.x / 2.0f) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, color.w)), line.c_str());

			Draw->AddText(DefaultFont, size, { position.x - textSize.x / 2.0f, position.y + textSize.y * i }, ImGui::GetColorU32(color), line.c_str());
		}
		else
		{
			Draw->AddText(DefaultFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, color.w)), line.c_str());
			Draw->AddText(DefaultFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, color.w)), line.c_str());
			Draw->AddText(DefaultFont, size, { (position.x) + 1.0f, (position.y + textSize.y * i) - 1.0f }, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, color.w)), line.c_str());
			Draw->AddText(DefaultFont, size, { (position.x) - 1.0f, (position.y + textSize.y * i) + 1.0f }, ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, color.w)), line.c_str());

			Draw->AddText(DefaultFont, size, { position.x, position.y + textSize.y * i }, ImGui::GetColorU32(color), line.c_str());
		}

		y = position.y + textSize.y * (i + 1);
		i++;
	}

	return y;
}