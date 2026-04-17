#include <stdio.h>
#include <string.h>
#include "gm_funcs/drawing.h"
#include "room_handler.h"
#include "misc.h"
#include "room_toid.h"

//ALWAYS PUT THIS AT THE BOTTOM GM CHECKS FOR THE LAST BRACKET TO KNOW WHERE TO PUT THE ROOM CHECKER!!!!
void scr_handleroom(int room_id){
    if (room == -1){
        show_message("NO ROOMS FOUND!");
        return;
    }
}

