#include <stdio.h>
#include <string.h>

//handle 3ds inputs
#ifdef __3DS__
    #include <3ds.h>

    //controller button held
    bool gamepad_button_check(int pad, u32 Button){
        hidScanInput();
        
        if (hidKeysHeld() & Button){
            printf("holding %lu on pad %d\n", Button, pad);
            return true;
        }
        else
            return false;
    }

    //controller button pressed
    bool gamepad_button_check_pressed(int pad, u32 Button){
        hidScanInput();

        if (hidKeysDown() & Button){
            printf("pressing %lu on pad %d\n", Button, pad);
            return true;
        }
        else
            return false;
    }

    //controller button released
    bool gamepad_button_check_released(int pad, u32 Button){
        hidScanInput();

        if (hidKeysUp() & Button){
            printf("releasing %lu on pad %d\n", Button, pad);
            return true;
        }
        else
            return false;
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