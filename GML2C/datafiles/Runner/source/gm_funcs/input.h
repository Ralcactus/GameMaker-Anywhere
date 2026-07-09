//3DS
#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>

    //define buttons
    #define gp_start KEY_START
    #define gp_select KEY_SELECT
    #define gp_face1 KEY_B
    #define gp_face2 KEY_A
    #define gp_face3 KEY_Y
    #define gp_face4 KEY_X
    #define gp_padl KEY_DLEFT
    #define gp_padr KEY_DRIGHT
    #define gp_padu KEY_DUP
    #define gp_padd KEY_DDOWN
    #define gp_axislh 32785
    #define gp_axislv 32786

    //funcs
    bool gamepad_button_check(int pad, u32 Button);
    bool gamepad_button_check_pressed(int pad, u32 Button);
    bool gamepad_button_check_released(int pad, u32 Button);
#endif

//GAMECUBE
#ifdef __gamecube__
    #include <gccore.h>

    #define gp_start PAD_BUTTON_START
    #define gp_select PAD_BUTTON_SELECT
    #define gp_face1 PAD_BUTTON_B
    #define gp_face2 PAD_BUTTON_A
    #define gp_face3 PAD_BUTTON_Y
    #define gp_face4 PAD_BUTTON_X
    #define gp_padl PAD_BUTTON_LEFT
    #define gp_padr PAD_BUTTON_RIGHT
    #define gp_padu PAD_BUTTON_UP
    #define gp_padd PAD_BUTTON_DOWN
    #define gp_axislh 32785
    #define gp_axislv 32786

    bool gamepad_button_check(int pad, u32 Button);
    bool gamepad_button_check_pressed(int pad, u32 Button);
    bool gamepad_button_check_released(int pad, u32 Button);
#endif

//WII
#ifdef __wii__
    #include <gccore.h>
    #include <wiiuse/wpad.h>
    
    #define gp_start WPAD_BUTTON_PLUS
    #define gp_select WPAD_BUTTON_MINUS
    #define gp_face1 WPAD_BUTTON_B
    #define gp_face2 WPAD_BUTTON_A
    #define gp_face3 WPAD_BUTTON_1
    #define gp_face4 WPAD_BUTTON_2
    #define gp_padl WPAD_BUTTON_LEFT
    #define gp_padr WPAD_BUTTON_RIGHT
    #define gp_padu WPAD_BUTTON_UP
    #define gp_padd WPAD_BUTTON_DOWN
    #define gp_axislh 32785
    #define gp_axislv 32786

    bool gamepad_button_check(int pad, u32 Button);
    bool gamepad_button_check_pressed(int pad, u32 Button);
    bool gamepad_button_check_released(int pad, u32 Button);
#endif

#pragma region //Keyboard (may need to be changed if some random platform uses a keyboard lol)
    bool keyboard_check(int key);
    bool keyboard_check_pressed(int key);
    bool keyboard_check_released(int key);
    bool keyboard_check_direct(int key);
    
    #define vk_nokey 0
    #define vk_anykey 1
    #define vk_left 37
    #define vk_right 39
    #define vk_up 38
    #define vk_down 40
    #define vk_enter 13
    #define vk_escape 27
    #define vk_space 32
    #define vk_shift 16
    #define vk_control 17
    #define vk_alt 18
    #define vk_backspace 8
    #define vk_tab 9
    #define vk_home 36
    #define vk_end 35
    #define vk_delete 46
    #define vk_insert 45
    #define vk_pageup 33
    #define vk_pagedown 34
    #define vk_pause 19
    #define vk_printscreen 44
    #define vk_f1 112
    #define vk_f2 113
    #define vk_f3 114
    #define vk_f4 115
    #define vk_f5 116
    #define vk_f6 117
    #define vk_f7 118
    #define vk_f8 119
    #define vk_f9 120
    #define vk_f10 121
    #define vk_f11 122
    #define vk_f12 123
    #define vk_numpad0 96
    #define vk_numpad1 97
    #define vk_numpad2 98
    #define vk_numpad3 99
    #define vk_numpad4 100
    #define vk_numpad5 101
    #define vk_numpad6 102
    #define vk_numpad7 103
    #define vk_numpad8 104
    #define vk_numpad9 105
    #define vk_multiply 106
    #define vk_divide 111
    #define vk_add 107
    #define vk_subtract 109
    #define vk_decimal 110
    #define vk_lshift 160
    #define vk_lcontrol 162
    #define vk_lalt 164
    #define vk_rshift 161
    #define vk_rcontrol 163
    #define vk_ralt 165
#pragma endregion

#pragma region //globals
//vars
extern float mouse_x;
extern float mouse_y;
extern float gamepad_button_deadzone_0; 

//funcs
void gamepad_scanner();
int gamepad_get_device_count();
bool gamepad_is_connected(int pad);
void gamepad_set_vibration(int pad, int left_motor, int right_motor);
bool gamepad_is_supported();
const char* gamepad_get_description(int pad);
float gamepad_get_axis_deadzone(int pad);
void gamepad_set_axis_deadzone(int pad, float threshold);
float gamepad_button_value(int pad, int button);
float gamepad_axis_count(int pad);
float gamepad_axis_value(int pad, int axis);

//touchscreen
float display_mouse_get_y();
float display_mouse_get_x();
#pragma endregion