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

#pragma region //applying code
/*Notes!
- The cursor progresses 1 character at a time
- the fake cursor is used to check lines ahead of the current one
- remember to set the normal cursor to the fake cursor after fake cursor is done!
*/

/*TODO
	In runner_interpret_xy add decimal support
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
		const char* fakecusor = cursor;
        //printf("current char: %c\n", character);

		//is this a x statment?
		if (character == 'x' || character == 'y'){
			//save if we are changing the x or y
			char postype = character;

			//the next character
			fakecusor++;

			if (*fakecusor == ' ')
				fakecusor++;


			//is this a proper add statment
			if (*fakecusor == '-' || *fakecusor == '+' || *fakecusor == '*' || *fakecusor == '='){
				//store the operation
				const char operationtype = *fakecusor;
				fakecusor++;

				while (*fakecusor == ' ')
					fakecusor++;

				if (*fakecusor == '=')
					fakecusor++;

				while (*fakecusor == ' ')
					fakecusor++;

				int thenumber = 0;
				while (*fakecusor == '0' || *fakecusor == '1' || *fakecusor == '2' || *fakecusor == '3' || *fakecusor == '4' || *fakecusor == '5' || *fakecusor == '6' || *fakecusor == '7' || *fakecusor == '8' || *fakecusor == '9'){
					thenumber = thenumber * 10 + (*fakecusor - '0');
					fakecusor++;
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

		C2D_SpriteSetPos(&sprites[object_index].spr, object_x, object_y);
		if (fakecusor == cursor)
			cursor++;
		else
			cursor = fakecusor;

	}
}

#pragma endregion

#pragma region //passing on the gml code to the in interpreters

void GML_interpret(const char* code, int object_def_index){
	runner_interpret_xy(object_def_index, code);
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