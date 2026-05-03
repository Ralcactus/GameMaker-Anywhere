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

//helper funcs not gm
int get_current_object_touching(int x, int y, float xscale, float yscale, int sprite_index, int selfid);
