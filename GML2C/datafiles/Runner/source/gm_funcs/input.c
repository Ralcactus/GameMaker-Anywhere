#include <stdio.h>
#include <string.h>
#include "misc.h"
#include "input.h"

float mouse_x = 0;
float mouse_y = 0;
float gamepad_button_deadzone_0 = 0.5;

//handle 3ds inputs
#ifdef __3DS__
    #include <3ds.h>

    static u32 keys_held = 0;
    static u32 keys_down = 0;
    static u32 keys_up = 0;
    static circlePosition circle_pos = {0};

    void gamepad_scanner(){
        hidScanInput();
        keys_held = hidKeysHeld();
        keys_down = hidKeysDown();
        keys_up = hidKeysUp();
        hidCircleRead(&circle_pos);

        //touchscreen
        touchPosition touch;
        hidTouchRead(&touch);
        mouse_x = touch.px*view0_camWidth/400+140+view0_camXPos;
        mouse_y = touch.py*view0_camHeight/240+view0_camYPos;
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

    bool gamepad_is_supported(){
        return true;
    }

    char* gamepad_get_description(int pad){
        if (pad == 0)
            return "NINTENDO 3DS Controller";
        else
            return "";
    }

    float gamepad_button_value(int pad, int button){
        if (gamepad_button_check(pad, button))
            return 1;
        else
            return 0;
    }

    float gamepad_axis_count(int pad){
        return 1;
    }

    float gamepad_axis_value(int pad, int axis){
        float value = 0;

        if (axis == gp_axislh)
            value = circle_pos.dx / 154.0f;
        else if (axis == gp_axislv)
            value = circle_pos.dy / 154.0f * -1.0f;

        if (value > -gamepad_button_deadzone_0 && value < gamepad_button_deadzone_0)
            return 0;

        return value;
    }

    #pragma endregion
#endif

//handle Gamecube inputs
#ifdef __gamecube__
    static u32 keys_held = 0;
    static u32 keys_down = 0;
    static u32 keys_up = 0;

    void gamepad_scanner(){
        PAD_ScanPads();
        keys_held = PAD_ButtonsHeld(0);
        keys_down = PAD_ButtonsDown(0);
        keys_up = PAD_ButtonsUp(0);
        
        //mouse
        mouse_x = 0;
        mouse_y = 0;
    }

    #pragma region //gamepad_ funcs
    //controller button held
    bool gamepad_button_check(int pad, u32 Button){
        if (keys_held & Button){
            return true;
        }
        else
            return false;
    }

    bool gamepad_button_check_pressed(int pad, u32 Button){
        if (keys_down & Button){
            return true;
        }
        else
            return false;
    }

    bool gamepad_button_check_released(int pad, u32 Button){
        if (keys_up & Button){
            return true;
        }
        else
            return false;
    }

    void gamepad_set_vibration(int pad, int left_motor, int right_motor){
        if (left_motor > 0 || right_motor > 0)
            PAD_ControlMotor(pad, PAD_MOTOR_RUMBLE);
        else
            PAD_ControlMotor(pad, PAD_MOTOR_STOP);
    }
    
    int gamepad_get_device_count(){
        return 1;
    }

    bool gamepad_is_connected(int pad){
        return true;
    }

    bool gamepad_is_supported(){
        return true;
    }

    char* gamepad_get_description(int pad){
        return "Nintendo Gamecube Controller"; //TODO - make this actually return a desc based on the thingy connected
    }

    //analog buttons
    float gamepad_button_value(int pad, int button){
        if (gamepad_button_check(pad, button))
            return 1;
        else
            return 0;
    }

    float gamepad_axis_count(int pad){
        return 2;
    }

    float gamepad_axis_value(int pad, int axis){
        float value = 0;

        if (axis == gp_axislh)
            value = (float)PAD_StickX(PAD_CHAN0)/128;
        else if (axis == gp_axislv)
            value = (float)PAD_StickY(PAD_CHAN0)/128*-1;

        if (value > -gamepad_button_deadzone_0 && value < gamepad_button_deadzone_0)
            return 0;

        return value;
    }


    #pragma endregion

#endif

//handle Wii inputs
#ifdef __wii__
    static u32 keys_held = 0;
    static u32 keys_down = 0;
    static u32 keys_up = 0;

    void gamepad_scanner(){
        WPAD_ScanPads();
        keys_held = WPAD_ButtonsHeld(0);
        keys_down = WPAD_ButtonsDown(0);
        keys_up = WPAD_ButtonsUp(0);
        
        //mouse
        mouse_x = 0;
        mouse_y = 0;
    }

    #pragma region //gamepad_ funcs
    //controller button held
    bool gamepad_button_check(int pad, u32 Button){
        if (keys_held & Button){
            return true;
        }
        else
            return false;
    }

    bool gamepad_button_check_pressed(int pad, u32 Button){
        if (keys_down & Button){
            return true;
        }
        else
            return false;
    }

    bool gamepad_button_check_released(int pad, u32 Button){
        if (keys_up & Button){
            return true;
        }
        else
            return false;
    }

    void gamepad_set_vibration(int pad, int left_motor, int right_motor){
        if (left_motor > 0 || right_motor > 0)
            PAD_ControlMotor(pad, PAD_MOTOR_RUMBLE);
        else
            PAD_ControlMotor(pad, PAD_MOTOR_STOP);
    }
    
    int gamepad_get_device_count(){
        return 1;
    }

    bool gamepad_is_connected(int pad){
        return true;
    }

    bool gamepad_is_supported(){
        return true;
    }

    char* gamepad_get_description(int pad){
        return "Nintendo Wiimote"; //TODO - make this actually return a desc based on the thingy connected
    }

    //analog buttons
    float gamepad_button_value(int pad, int button){
        if (gamepad_button_check(pad, button))
            return 1;
        else
            return 0;
    }

    float gamepad_axis_count(int pad){
        return 0;
    }

    float gamepad_axis_value(int pad, int axis){
        float value = 0;

        /*
        if (axis == gp_axislh)
            value = (float)PAD_StickX(PAD_CHAN0)/128;
        else if (axis == gp_axislv)
            value = (float)PAD_StickY(PAD_CHAN0)/128*-1;

        if (value > -gamepad_button_deadzone_0 && value < gamepad_button_deadzone_0)
            return 0;
        */

        return value;
    }


    #pragma endregion

#endif

#pragma region //globals

//funcs
float gamepad_get_axis_deadzone(int pad){
    return gamepad_button_deadzone_0;
}

void gamepad_set_axis_deadzone(int pad, float deadzone){
    gamepad_button_deadzone_0 = deadzone;
}

//mouse stuff
float display_mouse_get_x(){
    return mouse_x-view0_camXPos;
}

float display_mouse_get_y(){
    return mouse_y-view0_camYPos;;
}

#pragma endregion

