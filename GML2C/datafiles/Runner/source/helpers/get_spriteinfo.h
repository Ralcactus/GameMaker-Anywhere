#include <stdio.h>
#include <string.h>
#include "gm_funcs/drawing.h"
#include "room_handler.h"
#include "misc.h"
#include "helpers/asset_toid.h"
//all the info is written in the compiler

//sprite info
extern int SpriteWidths[];
extern int SpriteHeights[];
extern int SpriteOriginX[];
extern int SpriteOriginY[];

//bounding box info
extern int SpriteCollideTOP[];
extern int SpriteCollideBOTTOM[];
extern int SpriteCollideLEFT[];
extern int SpriteCollideRIGHT[];

//sprite animation info
extern int SpriteFrameCount[];
extern float SpriteAnimTimer[];
extern int SpriteAnimSpeedType[];