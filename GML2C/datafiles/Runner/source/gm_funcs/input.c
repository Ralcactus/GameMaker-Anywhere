#include <stdio.h>
#include <string.h>
#include "misc.h"


//handle 3ds inputs
#ifdef __3DS__
    #include <3ds.h>

    static u32 keys_held = 0;
    static u32 keys_down = 0;
    static u32 keys_up = 0;

    void gamepad_scanner(){
        hidScanInput();
        keys_held = hidKeysHeld();
        keys_down = hidKeysDown();
        keys_up = hidKeysUp();
    }

    #pragma region //gamepad_ funcs
    //controller button held
    bool gamepad_button_check(int pad, u32 Button){
        if (keys_held & Button){
            //printf("holding %lu on pad %d\n", Button, pad);
            return true;
        }
        else
            return false;
    }

    bool gamepad_button_check_pressed(int pad, u32 Button){
        if (keys_down & Button){
            printf("pressing %lu on pad %d\n", Button, pad);
            return true;
        }
        else
            return false;
    }

    bool gamepad_button_check_released(int pad, u32 Button){
        if (keys_up & Button){
            printf("releasing %lu on pad %d\n", Button, pad);
            return true;
        }
        else
            return false;
    }

    void gamepad_set_vibration(int pad, int left_motor, int right_motor){
        //3ds has no rumble, i kinda wish it did lol
    }
    
    int gamepad_get_device_count(){
        return 1;
    }

    bool gamepad_is_connected(int pad){
        if (pad == 0)
            return 1;
        else
            return 0;
    }
    #pragma endregion

    //touchscreen
    float display_mouse_get_x(){
        touchPosition touch;
        hidTouchRead(&touch);
        return touch.px*view0_camWidth/400+140;
    }

    float display_mouse_get_y(){
        touchPosition touch;
        hidTouchRead(&touch);
        return touch.py*view0_camHeight/240;
    }
#endif

//handle Wii U inputs
#ifdef __WIIU__
    #include <vpad/input.h>
    
    //controller button held
    bool gamepad_button_check(int pad, VPADButtons Button){
        return false;
    }

    //controller button pressed
    bool gamepad_button_check_pressed(int pad, VPADButtons Button){
        return false;
    }

    //controller button released
    bool gamepad_button_check_released(int pad, VPADButtons Button){
        return false;
    }

#endif