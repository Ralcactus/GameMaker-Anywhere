#include <stdio.h>
#include <string.h>
#include "cross_platform/drawing.h"
#include "room_handler.h"

void room_goto(char* room){
    CurrentRoom = room;
}


//ALWAYS PUT THIS AT THE BOTOTM GM CHECKS FOR THE LAST BRACKET TO KNOW WHERE TO PUT THE ROOM CHECKER!!!!
void scr_handleroom(char* CurrentRoom){
    if (strcmp(CurrentRoom, "") == 0){
        //oh no :(
    }
}

