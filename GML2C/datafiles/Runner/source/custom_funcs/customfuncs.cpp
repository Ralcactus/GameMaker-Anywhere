//citro2d has this declared already lol
#undef function

#include "customfuncs.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../sprite_toid.h"
#include "../gm_funcs/drawing.h"
#include "../gm_funcs/input.h"
#include "../gm_funcs/input.h"
#include "../room_toid.h"
#include "../gm_funcs/misc.h"
#include "../gm_funcs/collision.h"
#include "../gm_funcs/audio.h"
#include "../custom_funcs/customfuncs.h"
#include "../variable_handler.h"
#include <variant>
#include "../Helpers/other.h"

#define x CurrentObjectRunning->GetVar(varId_x)
#define y CurrentObjectRunning->GetVar(varId_y)
#define sprite_index CurrentObjectRunning->GetVar(varId_sprite_index)
#define image_xscale CurrentObjectRunning->GetVar(varId_image_xscale)
#define image_yscale CurrentObjectRunning->GetVar(varId_image_yscale)
#define id CurrentObjectRunning->GetVar(varId_id)
#define visible CurrentObjectRunning->GetVar(varId_visible)
#define solid CurrentObjectRunning->GetVar(varId_solid)
#define persistent CurrentObjectRunning->GetVar(varId_persistent)
#define depth CurrentObjectRunning->GetVar(varId_depth)
#define layer CurrentObjectRunning->GetVar(varId_layer)
#define on_ui_layer CurrentObjectRunning->GetVar(varId_on_ui_layer)
#define collision_space CurrentObjectRunning->GetVar(varId_collision_space)
#define direction CurrentObjectRunning->GetVar(varId_direction)
#define friction CurrentObjectRunning->GetVar(varId_friction)
#define gravity CurrentObjectRunning->GetVar(varId_gravity)
#define gravity_direction CurrentObjectRunning->GetVar(varId_gravity_direction)
#define hspeed CurrentObjectRunning->GetVar(varId_hspeed)
#define vspeed CurrentObjectRunning->GetVar(varId_vspeed)
#define speed CurrentObjectRunning->GetVar(varId_speed)
#define xstart CurrentObjectRunning->GetVar(varId_xstart)
#define ystart CurrentObjectRunning->GetVar(varId_ystart)
#define xprevious CurrentObjectRunning->GetVar(varId_xprevious)
#define yprevious CurrentObjectRunning->GetVar(varId_yprevious)
#define object_index CurrentObjectRunning->GetVar(varId_object_index)
#define sprite_width CurrentObjectRunning->GetVar(varId_sprite_width)
#define sprite_height CurrentObjectRunning->GetVar(varId_sprite_height)
#define sprite_xoffset CurrentObjectRunning->GetVar(varId_sprite_xoffset)
#define sprite_yoffset CurrentObjectRunning->GetVar(varId_sprite_yoffset)
#define image_alpha CurrentObjectRunning->GetVar(varId_image_alpha)
#define image_angle CurrentObjectRunning->GetVar(varId_image_angle)
#define image_blend CurrentObjectRunning->GetVar(varId_image_blend)
#define image_index CurrentObjectRunning->GetVar(varId_image_index)
#define image_number CurrentObjectRunning->GetVar(varId_image_number)
#define image_speed CurrentObjectRunning->GetVar(varId_image_speed)

#define function void


