#include <stdio.h>
#include <string.h>
#include "cross_platform/input.h"
#include "cross_platform/other.h"
#include "cross_platform/file_manage.h"

int main(){
    #ifdef __3DS__
        gfxInitDefault();
        consoleInit(GFX_BOTTOM, NULL);
    #endif

    printf("Hello World!\n");

    //main loop
    #ifdef __3DS__
    while (aptMainLoop()){
    #endif    
    #ifdef __WIIU__
    while (true){
    #endif
        if (gamepad_button_check(0, gp_start))
            break;
    }

    ExitApplication();
    return 0;
    }
}
