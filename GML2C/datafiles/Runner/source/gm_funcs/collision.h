#include <stdio.h>
#include <string.h>
#include "drawing.h"
#include "misc.h"

bool place_meeting(float _x, float _y, int object);
bool place_empty(float _x, float _y, int object);
bool place_free(float _x, float _y);
bool position_empty(float _x, float _y);
bool position_meeting(float _x, float _y, int object);
void position_destroy(float _x, float _y);
int instance_place(float _x, float _y, int object);
int instance_position(float _x, float _y, int object);