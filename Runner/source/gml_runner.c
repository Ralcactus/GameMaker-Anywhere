#include <citro2d.h>
#include "gml_functions.h"
#include <stdio.h>
#include "cJSON.h"
#include <string.h>
#include "gml_runner.h"
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
static const cJSON* root = NULL;

#pragma region //shortcut stuff
//carry over the root from the main.c (probably better way i could've done this...)
void GML_SetRoot(const cJSON* garrys_in_the_room_tonight)
{
	root = garrys_in_the_room_tonight;
}

//return next object
static int runner_next_object_index(int* cursor, int object_index)
{
    for (int tries = 0; tries < (int)SpriteCount; tries++)
    {
        if (*cursor >= (int)SpriteCount)
            *cursor = 0;

        int object = *cursor;
        (*cursor)++;

        if (runner_sprite_is_object(object) && sprite_object_id[object] == object_index)
            return object;
    }

    return -1;
}
#pragma endregion

//convert a string of an input to a macro 
static int input_convertstring(const char* string)
{
	if (strncmp(string, "gp_face1", 8) == 0)
		return gp_face1; 

	if (strncmp(string, "gp_face2", 8) == 0)
		return gp_face2; 

	if (strncmp(string, "gp_face3", 8) == 0)
		return gp_face3; 

	if (strncmp(string, "gp_face4", 8) == 0)
		return gp_face4; 

	if (strncmp(string, "gp_start", 8) == 0)
		return gp_start; 

	if (strncmp(string, "gp_select", 9) == 0)
		return gp_select; 

	if (strncmp(string, "gp_padl", 7) == 0)
		return gp_padl;

	if (strncmp(string, "gp_padr", 7) == 0)
		return gp_padr; 

	if (strncmp(string, "gp_padu", 7) == 0)
		return gp_padu; 

	if (strncmp(string, "gp_padd", 7) == 0)
		return gp_padd; 

		
	return -4;
}

static void quick_printfakecursor(const char* fakecursor){
	printf("fakecursor at:       %c\n", *fakecursor);
}

#pragma region //applying code
/*Notes!
- The cursor progresses 1 character at a time
- the fake cursor is used to check lines ahead of the current one
- remember to set the normal cursor to the fake cursor after fake cursor is done!
*/

/*TODO
	In runner_interpret_xy add decimal supportfi
*/

//interpret x and y of the objects
static void runner_interpret_xy(int object_index, const char* code)
{
	float object_x = sprites[object_index].spr.params.pos.x;
	float object_y = sprites[object_index].spr.params.pos.y;
    const char* cursor = code;

    while (*cursor != '\0')
    {
        char character = *cursor;
		const char* fakecursor = cursor;
        //printf("current char: %c\n", character);

		//is this a x statment?
		if (character == 'x' || character == 'y'){
			//save if we are changing the x or y
			char postype = character;

			//the next character
			fakecursor++;

			if (*fakecursor == ' ')
				fakecursor++;


			//is this a proper add statment
			if (*fakecursor == '-' || *fakecursor == '+' || *fakecursor == '*' || *fakecursor == '='){
				//store the operation
				const char operationtype = *fakecursor;
				fakecursor++;

				while (*fakecursor == ' ')
					fakecursor++;

				if (*fakecursor == '=')
					fakecursor++;

				while (*fakecursor == ' ')
					fakecursor++;

				int thenumber = 0;
				while (*fakecursor == '0' || *fakecursor == '1' || *fakecursor == '2' || *fakecursor == '3' || *fakecursor == '4' || *fakecursor == '5' || *fakecursor == '6' || *fakecursor == '7' || *fakecursor == '8' || *fakecursor == '9'){
					thenumber = thenumber * 10 + (*fakecursor - '0');
					fakecursor++;
				}


				//add value
				if (operationtype == '+'){
					if (postype == 'x')
						object_x += thenumber;
					else
						object_y += thenumber;
				}

				//minus value
				if (operationtype == '-'){
					if (postype == 'x')
						object_x -= thenumber;
					else
						object_y -= thenumber;
				}

				//multiply value
				if (operationtype == '*'){
					if (postype == 'x')
						object_x *= thenumber;
					else
						object_y *= thenumber;
				}

				//set value
				if (operationtype == '='){
					if (postype == 'x')
						object_x = thenumber;
					else
						object_y = thenumber;
				}
			}
		}
		if (fakecursor == cursor)
			cursor++;
		else
			cursor = fakecursor;

	}
	C2D_SpriteSetPos(&sprites[object_index].spr, object_x, object_y);
}

#pragma region //if interpreting

//check if we are holding the button a if wants
static void runner_interpret_input_held(const char* padindex, const char* button){
	if (gamepad_button_check(input_convertstring(button))){
		printf("Holding button:       %s\n", button);
	}
}

//check for and handle if statments
static void runner_interpret_if(int object_index, const char* code)
{
    const char* cursor = code;

    while (*cursor != '\0')
    {
        char character = *cursor;
		const char* fakecursor = cursor;


		//is this a if statment?
		if (character == 'i'){
			fakecursor++;
			
			//is this a if statment pt2
			if (*fakecursor == 'f')
				fakecursor++;
			else{
				cursor+=2;
				continue;
			}

			
			if (*fakecursor == ' ')
				fakecursor++;
			if (*fakecursor == '(')
				fakecursor++;


			//store the function characters
			char function[256];
			int i = 0;
			while (*fakecursor != '(' && *fakecursor != '\0'){
				//add each character to the buffer
				function[i++] = *fakecursor;
				fakecursor++;
			}
			function[i] = '\0';

			//continue past the (
			fakecursor++;
			
			//checking gamepad inputs (later add gamepad_button_check_pressed and gamepad_button_check_released!!)
			if (strcmp(function, "gamepad_button_check") == 0){
				//printf("INSIDE:       %s\n", function);
				char padindex[256];
				char button[256];

				int pad_i = 0;
				while (*fakecursor != ',' && *fakecursor != '\0'){
					//add each character to the buffer
					padindex[pad_i++] = *fakecursor;
					fakecursor++;
				}
				padindex[pad_i] = '\0';

				//skip comma
				fakecursor++;

				while (*fakecursor == ' ')
					fakecursor++;

				int button_i = 0;
				while (*fakecursor != ')' && *fakecursor != '\0'){
					//add each character to the buffer
					button[button_i++] = *fakecursor;
					fakecursor++;
				}
				button[button_i] = '\0';

				runner_interpret_input_held(padindex, button);
			}
		}

		if (fakecursor == cursor)
			cursor++;
		else
			cursor = fakecursor;

	}
}
#pragma endregion


#pragma endregion

#pragma region //passing on the gml code to the interpreters

void GML_interpret(const char* code, int object_def_index){
	runner_interpret_xy(object_def_index, code);
	runner_interpret_if(object_def_index, code);
}

//runs the create code (on object creation)
void RunGML_create(const char* code, int object_def_index)
{
	//Select next object instance
	static bool did_create[MAX_SPRITES] = {0};
	static int create_cursor = 0;

	for (int n = 0; n < (int)SpriteCount; n++)
	{
		int instance_index = runner_next_object_index(&create_cursor, object_def_index);

		if (instance_index < 0)
			return;

		//check and set if the creates ran
		if (did_create[instance_index])
			continue;
		did_create[instance_index] = true;

		//interpret the gml code
		GML_interpret(code, instance_index);
	}
}

//runs the step code (once per frame)
void RunGML_step(const char* code, int object_def_index)
{
	//Select next object instance
	static int step_cursor = 0;
	static bool ran_this_frame[MAX_SPRITES] = {0};

	for (int n = 0; n < (int)SpriteCount; n++)
	{
		int instance_index = runner_next_object_index(&step_cursor, object_def_index);
		if (instance_index < 0)
			return;

		if (ran_this_frame[instance_index])
			continue;

		ran_this_frame[instance_index] = true;

		//interpret the gml code
		GML_interpret(code, instance_index);
	}

	for (int i = 0; i < (int)SpriteCount; i++){
		ran_this_frame[i] = false;
	}
}



//submits code for events
void RunGML(){
	const cJSON* objs = cJSON_GetObjectItemCaseSensitive(root, "Objects");
	const cJSON* all  = cJSON_GetObjectItemCaseSensitive(objs, "all_objects");
	const cJSON* object = NULL;
	int object_index = 0;

	cJSON_ArrayForEach(object, all)
	{
		const cJSON* code_create = cJSON_GetObjectItemCaseSensitive(object, "CreateCode");
		const cJSON* code_step   = cJSON_GetObjectItemCaseSensitive(object, "StepCode");

		if (code_create && cJSON_IsString(code_create) && code_create->valuestring && code_create->valuestring[0] != '\0')
			RunGML_create(code_create->valuestring, object_index);

		if (code_step && cJSON_IsString(code_step) && code_step->valuestring && code_step->valuestring[0] != '\0')
			RunGML_step(code_step->valuestring, object_index);

		object_index++;
	}
}

#pragma endregion