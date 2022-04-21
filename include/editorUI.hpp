#ifndef EDITOR_UI
#define EDITOR_UI

struct EDITOR_UI_SETTINGS
{
    int shadows{1};
    int bloom{1};
    int AO{1};
    int AOSamples{32};
    float AORadius{1.0f};
    int scene_tone_mapping{ 1 };
    int ui_tone_mapping{ 0 };
    int volumetrics{1};
    int motion_blur{0};
    int show_physics{0};
};

#endif
