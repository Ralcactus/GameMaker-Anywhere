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

    //funcs
    bool gamepad_button_check(int pad, u32 Button);
    bool gamepad_button_check_pressed(int pad, u32 Button);
    bool gamepad_button_check_released(int pad, u32 Button);
    void gamepad_scanner();
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