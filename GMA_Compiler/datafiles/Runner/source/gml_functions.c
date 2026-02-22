#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>
#endif

#include "gml_functions.h"
#include <stdio.h>

#pragma region //Input functions
//check gamepad button is held

#ifdef __DKPRO__
	bool gamepad_button_check_pressed(u32 ButtonPressed) {
		if (g_keysDown & ButtonPressed)
			return true;
		else
			return false;
	}
#endif

//check gamepad button is pressed

#ifdef __DKPRO__
	bool gamepad_button_check(u32 ButtonPressed) {
		if (g_keysHeld & ButtonPressed)
			return true;
		else
			return false;
	}
#endif

//check gamepad button is released

#ifdef __DKPRO__
	bool gamepad_button_check_released(u32 ButtonPressed) {
		if (g_keysUp & ButtonPressed)
			return true;
		else
			return false;
	}
#endif
#pragma endregion

#pragma region //Drawing functions

#ifdef __3DS__
	void draw_sprite(int spritenumber, C2D_SpriteSheet sheet, float x, float y) {
		if (SpriteCount >= MAX_SPRITES)
			return;

		Sprite* sprite = &sprites[SpriteCount];
		C2D_SpriteFromSheet(&sprite->spr, sheet, spritenumber);
		C2D_SpriteSetPos(&sprite->spr, x, y);

		SpriteCount++;
	}
#endif
#pragma endregion
