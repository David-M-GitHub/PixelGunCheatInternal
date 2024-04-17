#pragma once

#include <imgui.h>
#include <iostream>
#include <vector>
#include <d3d11.h>
#include <list>

// Boykisser Central Module Vars

/*
 * Setting base class
 */
class BKCSetting
{
public:
    int type = 0;
    std::string name;
    std::string tooltip;
};

/*
 * Checkbox setting
 * @param setting_name: name of the setting
 * @param checked: default value of the setting (checked or not)
 * @param extra_info: extra information about the setting
 */
class BKCCheckbox : public BKCSetting
{
public:
    BKCCheckbox(const std::string& setting_name, const bool checked, const std::string& extra_info = "")
    {
        name = setting_name;
        enabled = checked;
        tooltip = extra_info;
        type = 1;
    }

    bool enabled;
};

/*
 * Slider setting (float)
 * @param setting_name: name of the setting
 * @param val: default value of the setting
 * @param min: minimum value of the setting
 * @param max: maximum value of the setting
 * @param extra_info: extra information about the setting
 */
class BKCSlider : public BKCSetting
{
public:
    BKCSlider(const std::string& setting_name, const float val, const float min, const float max,
              const std::string& extra_info = "")
    {
        name = setting_name;
        value = val;
        minimum = min;
        maximum = max;
        tooltip = extra_info;
        type = 2;
    }

    float value;
    float minimum;
    float maximum;
};

/*
 * Slider setting (int)
 * @param setting_name: name of the setting
 * @param val: default value of the setting
 * @param min: minimum value of the setting
 * @param max: maximum value of the setting
 * @param extra_info: extra information about the setting
 */
class BKCSliderInt : public BKCSetting
{
public:
    BKCSliderInt(const std::string& setting_name, const int val, const int min, const int max,
                 const std::string& extra_info = "")
    {
        name = setting_name;
        value = val;
        minimum = min;
        maximum = max;
        tooltip = extra_info;
        type = 3;
    }

    int value;
    int minimum;
    int maximum;
};

/*
 * Dropdown setting
 * @param setting_name: name of the setting
 * @param def_val: default value of the setting
 * @param vals: vector of possible values
 * @param extra_info: extra information about the setting
 */
class BKCDropdown : public BKCSetting
{
public:
    BKCDropdown(const std::string& setting_name, const std::string& def_val, const std::vector<std::string>& vals,
                const std::string& extra_info = "")
    {
        name = setting_name;
        values = vals;
        current_value = def_val;
        current_index = indexof(def_val);
        tooltip = extra_info;
        type = 4;
    }

    int indexof(std::string val)
    {
        auto indexer = std::ranges::find(values, val);
        return indexer != values.end() ? (int)(indexer - values.begin()) : -1;
    }

    int current_index;
    std::string current_value;
    std::vector<std::string> values;
};

enum BKCCategory
{
    NONE = 0,
    GENERAL = 1,
    COMBAT = 2,
    VISUAL = 3,
    MOVEMENT = 4,
    PLAYER = 5,
    EXPLOIT = 6
};

/*
 * Module class
 * @param name: name of the module
 * @param category: category of the module
 * @param key: key to toggle the module
 * @param enabled: default value of the module (enabled or not)
 * @param settings: vector of settings for the module
 */
class BKCModule
{
public:
    std::string name;
    BKCCategory category = NONE;
    WPARAM key = 0x0;
    bool enabled = false;
    std::vector<BKCSetting*> settings = {};

    void toggle()
    {
        enabled = !enabled;
    }
};

/*
 * ImGui hooker class
 * This class is responsible for hooking up the ImGui.
 */
class BKCImGuiHooker
{
public:
    static std::list<BKCModule*> modules;
    static ImFont* gui_font;
    static ImFont* watermark_font;
    static ImFont* arraylist_font;
    static std::string c_Title;
    static std::string c_RealBuild;
    static float scale_factor;
    static bool modules_loaded;
    static bool config_loaded;
    static bool c_GuiEnabled;
    static void setup_imgui_hwnd(HWND handle, ID3D11Device* device, ID3D11DeviceContext* device_context);
    static void start(ID3D11RenderTargetView* g_mainRenderTargetView, ID3D11DeviceContext* g_pd3dDeviceContext);
};
