#pragma once
#include <algorithm>
#include <imgui.h>

#include "../../ModuleBase.h"
#include "../../../Hooks/Hooks.h"

static BKCSlider __array_list_slider_red = BKCSlider("Font Red", 0.91f, 0.0f, 1.0f);
static BKCSlider __array_list_slider_green = BKCSlider("Font Green", 0.64f, 0.0f, 1.0f);
static BKCSlider __array_list_slider_blue = BKCSlider("Font Blue", 0.13f, 0.0f, 1.0f);
static BKCSlider __array_list_rgb_speed = BKCSlider("Rainbow Speed", 0.1f, 0.01f, 1.0f, "test");
static BKCCheckbox __array_list_rainbow = BKCCheckbox("Rainbow Font UwU", false);
static BKCModule __array_list = {
    "Array List", VISUAL, 0x0, true, { &__array_list_slider_red, &__array_list_slider_blue, &__array_list_slider_green, &__array_list_rainbow, &__array_list_rgb_speed}
};

static ImU32 color_bg = ImGui::ColorConvertFloat4ToU32({0.00f, 0.00f, 0.00f, 0.85f});

/**
 * ModuleArrayList is a module that displays all modules in the cheat.
 */
class ModuleArrayList : ModuleBase
{
public:
    ModuleArrayList() : ModuleBase(&__array_list)
    {
    }

    static ImU32 get_rainbow_color(float time, float saturation, float value, float speed)
    {
        float hue = std::fmod(time * speed, 1.0f);
        ImVec4 color_hsv(hue, saturation, value, 1.0f);
        ImVec4 color_rgb;
        ImGui::ColorConvertHSVtoRGB(color_hsv.x, color_hsv.y, color_hsv.z, color_rgb.x, color_rgb.y, color_rgb.z);
        return ImGui::ColorConvertFloat4ToU32({color_rgb.x, color_rgb.y, color_rgb.z, 1.0f});
    }

    void do_module(void* arg) override
    {
        ImGui::PushFont(BKCImGuiHooker::arraylist_font);

        ImU32 color_array = ImGui::ColorConvertFloat4ToU32({__array_list_slider_red.value, __array_list_slider_blue.value, __array_list_slider_green.value, 1.00f});

        if (__array_list_rainbow.enabled)
        {
            color_array = get_rainbow_color((float)ImGui::GetTime(), 1.0f, 1.0f, __array_list_rgb_speed.value); 
        }
        
        // ArrayList
        float x = 5;
        float y = 48 * BKCImGuiHooker::scale_factor;
        float size = ImGui::GetFontSize();
        float modc = 0;

        std::vector<BKCModule*> module_list = {};
        for (const auto mod : BKCImGuiHooker::modules) module_list.push_back(mod);
        std::sort(module_list.begin(), module_list.end(), alphabetical_cmp());

        for (auto& module : module_list)
        {
            if (module->enabled) modc++;
        }
        ImGui::GetBackgroundDrawList()->AddRectFilled({x, y}, {
                                                          x + 200 * BKCImGuiHooker::scale_factor,
                                                          y + modc * (size + 2) + 10
                                                      }, color_bg, 10);
        
        for (auto& module : module_list)
        {
            if (module->enabled)
            {
                ImGui::GetBackgroundDrawList()->AddText(nullptr, size, {x + 5, y + 2}, color_array, module->name.c_str());
                y += size + 2;
            }
        }

        ImGui::PopFont();
    }

    

private:
    struct alphabetical_cmp
    {
        bool operator()(const BKCModule* mod1, const BKCModule* mod2) const
        {
            return mod1->name.compare(mod2->name) <= 0;
        }
    };
};
