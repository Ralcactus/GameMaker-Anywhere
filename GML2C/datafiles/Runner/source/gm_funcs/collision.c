#include <stdio.h>
#include <string.h>
#include "drawing.h"
#include "misc.h"

bool place_meeting(float x, float _y, int object){
    return false;
}

bool place_empty(float x, float _y, int object){
    return true;
}

bool place_free(float x, float _y){
    return true;
}

bool position_empty(float x, float _y){
    return true;
}

bool position_meeting(float x, float _y, int object){
    return false;
}

void position_destroy(float x, float _y){
    //so empty...
}

int instance_place(float x, float _y, int object){
    return 0;
}

int instance_position(float x, float _y, int object){
    return 0;
}