#ifdef __3DS__
    #include <3ds.h>
    #include <citro2d.h>
#endif

#include "gml_functions.h"
#include <stdio.h>

#pragma region //Input functions
//check gamepad button is held

#ifdef __DKPRO__
	bool gamepad_button_check_pressed(int pad, u32 Button) {
		if (g_keysDown & Button)
			return true;
		else
			return false;
	}
#endif

//check gamepad button is pressed

#ifdef __DKPRO__
	bool gamepad_button_check(int pad, u32 Button) {
		if (g_keysHeld & Button)
			return true;
		else
			return false;
	}
#endif

//check gamepad button is released

#ifdef __DKPRO__
	bool gamepad_button_check_released(int pad, u32 Button) {
		if (g_keysUp & Button)
			return true;
		else
			return false;
	}
#endif

#ifdef __RAYLIB__
    bool gamepad_button_check_pressed(int device, int button){
        return IsGamepadButtonPressed(device, button);
    }

    bool gamepad_button_check(int device, int button){
        return IsGamepadButtonDown(device, button);
    }

    bool gamepad_button_check_released(int device, int button){
        return IsGamepadButtonReleased(device, button);
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
