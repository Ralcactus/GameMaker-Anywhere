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

    extern float mouse_x;
    extern float mouse_y;
    extern float gamepad_button_deadzone_0; 

    //funcs
    bool gamepad_button_check(int pad, u32 Button);
    bool gamepad_button_check_pressed(int pad, u32 Button);
    bool gamepad_button_check_released(int pad, u32 Button);
    void gamepad_scanner();
    int gamepad_get_device_count();
    bool gamepad_is_connected(int pad);
    void gamepad_set_vibration(int pad, int left_motor, int right_motor);
    bool gamepad_is_supported();
    char* gamepad_get_description(int pad);
    float gamepad_get_axis_deadzone(int pad);
    void gamepad_set_axis_deadzone(int pad, float threshold);
    float gamepad_button_value(int pad, int button);
    float gamepad_axis_count(int pad);
    float gamepad_axis_value(int pad, int axis);

    //touchscreen
    float display_mouse_get_y();
    float display_mouse_get_x();
#endif

//WII U
#ifdef __WIIU__
    #include <vpad/input.h>

    //define buttons
    #define gp_start VPAD_BUTTON_PLUS
    #define gp_select VPAD_BUTTON_MINUS
    #define gp_face1 VPAD_BUTTON_B
    #define gp_face2 VPAD_BUTTON_A
    #define gp_face3 VPAD_BUTTON_Y
    #define gp_face4 VPAD_BUTTON_X
    #define gp_padl VPAD_BUTTON_LEFT
    #define gp_padr VPAD_BUTTON_RIGHT
    #define gp_padu VPAD_BUTTON_UP
    #define gp_padd VPAD_BUTTON_DOWN

    //funcs
    bool gamepad_button_check(int pad, VPADButtons Button);
    bool gamepad_button_check_pressed(int pad, VPADButtons Button);
    bool gamepad_button_check_released(int pad, VPADButtons Button);
#endif