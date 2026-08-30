#include <stdio.h>
#include <string.h>
#include "gm_funcs/drawing.h"
#include "room_handler.h"
#include "misc.h"
#include "helpers/asset_toid.h"
//all the info is written in the compiler
//DO NOT TOUCH ANY OF THE LINE POSITIONS FOR ANYTHING BELOW, THE COMPILER WRITES EVERYTHING TO SPECIFIC LINES!

//sprite info
int SpriteWidths[] = {
int SpriteHeights[] = {
int SpriteOriginX[] = {};
int SpriteOriginY[] = {};

//bounding box info
int SpriteCollideTOP[] = {};
int SpriteCollideBOTTOM[] = {};
int SpriteCollideLEFT[] = {};
int SpriteCollideRIGHT[] = {};

//sprite animation info
int SpriteFrameCount[] = {};
float SpriteAnimTimer[] = {};
int SpriteAnimSpeedType[] = {};