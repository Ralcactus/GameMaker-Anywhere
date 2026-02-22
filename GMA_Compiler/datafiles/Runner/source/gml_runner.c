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

static void quick_printfakecursor(const char* fakecursor){
	printf("fakecursor at:       %c\n", *fakecursor);
}

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

#pragma endregion

#pragma region //interpreting code
/*Notes!
- The cursor progresses 1 character at a time
- the fake cursor is used to check lines ahead of the current one
- remember to set the normal cursor to the fake cursor after fake cursor is done!
*/


#pragma region //if interpreting

#pragma region //inputs

#pragma region //gamepad_button_check
static bool runner_interpret_input_held(const char* padindex, const char* button){
	if (gamepad_button_check(input_convertstring(button))){
		//printf("Holding button:       %s\n", button);
		return true;
	}
	return false;
}

static bool runner_gamepad_button_check(const char* cursor){
	char padindex[256];
	char button[256];

	int pad_i = 0;
	while (*cursor != ',' && *cursor != '\0'){
		//add each character to the buffer
		padindex[pad_i++] = *cursor;
		cursor++;
	}
	padindex[pad_i] = '\0';

	cursor++;
	while (*cursor == ' ')
		cursor++;

	int button_i = 0;
	while (*cursor != ')' && *cursor != '\0'){
		//add each character to the buffer
		button[button_i++] = *cursor;
		cursor++;
	}
	button[button_i] = '\0';

	return runner_interpret_input_held(padindex, button);
}
#pragma endregion

#pragma region //gamepad_button_check_pressed
static bool runner_interpret_input_pressed(const char* padindex, const char* button){
	if (gamepad_button_check_pressed(input_convertstring(button))){
		//printf("Pressing button:       %s\n", button);
		return true;
	}
	return false;
}

static bool runner_gamepad_button_check_pressed(const char* cursor){
	char padindex[256];
	char button[256];

	int pad_i = 0;
	while (*cursor != ',' && *cursor != '\0'){
		//add each character to the buffer
		padindex[pad_i++] = *cursor;
		cursor++;
	}
	padindex[pad_i] = '\0';

	cursor++;
	while (*cursor == ' ')
		cursor++;

	int button_i = 0;
	while (*cursor != ')' && *cursor != '\0'){
		//add each character to the buffer
		button[button_i++] = *cursor;
		cursor++;
	}
	button[button_i] = '\0';

	return runner_interpret_input_pressed(padindex, button);
}
#pragma endregion

#pragma region //gamepad_button_check_released
static bool runner_interpret_input_released(const char* padindex, const char* button){
	if (gamepad_button_check_released(input_convertstring(button))){
		//printf("Released button:       %s\n", button);
		return true;
	}
	return false;
}

static bool runner_gamepad_button_check_released(const char* cursor){
	char padindex[256];
	char button[256];

	int pad_i = 0;
	while (*cursor != ',' && *cursor != '\0'){
		//add each character to the buffer
		padindex[pad_i++] = *cursor;
		cursor++;
	}
	padindex[pad_i] = '\0';

	cursor++;
	while (*cursor == ' ')
		cursor++;

	int button_i = 0;
	while (*cursor != ')' && *cursor != '\0'){
		//add each character to the buffer
		button[button_i++] = *cursor;
		cursor++;
	}
	button[button_i] = '\0';

	return runner_interpret_input_released(padindex, button);
}
#pragma endregion

#pragma endregion

#pragma region //checking values

#pragma region //check pos value
static bool runner_interpret_value_ismine(const char* value_tocheck, const char* operation, float object_x, float object_y, const char* type)
{
	//operation is ==/=, check is value is == or =
    float value = (float)atof(value_tocheck);
    if (strcmp(operation, "==") == 0 || strcmp(operation, "=") == 0)
    {
        if ((strcmp(type, "x") == 0 && object_x == value) || (strcmp(type, "y") == 0 && object_y == value))
            return true;
    }

	//operation is >, check is value is >
    if (strcmp(operation, ">") == 0)
    {
        if ((strcmp(type, "x") == 0 && object_x > value) || (strcmp(type, "y") == 0 && object_y > value))
            return true;
    }

	//operation is >=, check is value is >=
    if (strcmp(operation, ">=") == 0)
    {
        if ((strcmp(type, "x") == 0 && object_x >= value) || (strcmp(type, "y") == 0 && object_y >= value))
            return true;
    }

	//operation is <, check is value is <
    if (strcmp(operation, "<") == 0)
    {
        if ((strcmp(type, "x") == 0 && object_x < value) || (strcmp(type, "y") == 0 && object_y < value))
            return true;
    }

	//operation is <=, check is value is <=
    if (strcmp(operation, "<=") == 0)
    {
        if ((strcmp(type, "x") == 0 && object_x <= value) || (strcmp(type, "y") == 0 && object_y <= value))
            return true;
    }

    return false;
}


static bool runner_check_current_pos(const char* cursor, float object_x, float object_y, const char* type){
	char operationtype[256];
	char value_tocheck[256];

	while (*cursor == ' ')
		cursor++;

	int operation_i = 0;
	while (*cursor != ' ' && *cursor != '\0'){
		//add each character to the buffer
		operationtype[operation_i++] = *cursor;
		cursor++;
	}
	operationtype[operation_i] = '\0';

	cursor++;
	while (*cursor == ' ')
		cursor++;

	int value_i = 0;
	while (*cursor != ' ' && *cursor != ')' && *cursor != '\0'){
		//add each character to the buffer
		value_tocheck[value_i++] = *cursor;
		cursor++;
	}
	value_tocheck[value_i] = '\0';
	

	return runner_interpret_value_ismine(value_tocheck, operationtype, object_x, object_y, type);
}
#pragma endregion

#pragma endregion

static bool runner_if_middleman(const char* function, const char* args, float object_x, float object_y)
{
    if (strcmp(function, "gamepad_button_check") == 0)
        return runner_gamepad_button_check(args);

    if (strcmp(function, "gamepad_button_check_pressed") == 0)
        return runner_gamepad_button_check_pressed(args);

    if (strcmp(function, "gamepad_button_check_released") == 0)
        return runner_gamepad_button_check_released(args);

    if (strcmp(function, "x") == 0 || strcmp(function, "y") == 0)
        return runner_check_current_pos(args, object_x, object_y, function);


    return false;
}

//check for and handle if statments
static bool runner_interpret_if(const char* code, float object_x, float object_y)
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
			while (*fakecursor != '(' && *fakecursor != ' ' && *fakecursor != '\0'){
				//add each character to the buffer
				function[i++] = *fakecursor;
				fakecursor++;
			}
			function[i] = '\0';

			//skip past the "(" or the " "
			while (*fakecursor == '(' || *fakecursor == ' ')
				fakecursor++;

			return runner_if_middleman(function, fakecursor, object_x, object_y);
		}

		if (fakecursor == cursor)
			cursor++;
		else
			cursor = fakecursor;

	}

	return false;
}

static const char* skip_block(const char* cursor)
{
    if (*cursor != '{')
        return cursor;

    int depth = 1;
    cursor++; // skip first {

    while (*cursor && depth > 0)
    {
        if (*cursor == '{')
            depth++;
        else if (*cursor == '}')
            depth--;

        cursor++;
    }

    return cursor;
}

#pragma endregion

#pragma region //asigning values
//interpret x and y of the objects
static void runner_interpret_xy(int object_index, const char* code)
{
	float object_x = sprites[object_index].spr.params.pos.x;
	float object_y = sprites[object_index].spr.params.pos.y;
    const char* cursor = code;

    while (*cursor != '\0')
    {
		//is this an if??
		if (cursor[0] == 'i' && cursor[1] == 'f')
		{
			bool if_result = runner_interpret_if(cursor, object_x, object_y);

			if (if_result)
			{
				while (*cursor && *cursor != '{')
					cursor++;

				if (*cursor == '{')
					cursor++; // enter block
			}
			else
			{
				while (*cursor && *cursor != '{')
					cursor++;

				cursor = skip_block(cursor);
			}
		}



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

				if (*fakecursor == '='){
					fakecursor++;
				
					//uh oh! this is a check statment! kill code!
					if (*fakecursor == '=')
						return;
				}

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

	#ifdef __3DS__
		C2D_SpriteSetPos(&sprites[object_index].spr, object_x, object_y);
	#elif __RAYLIB__
		sprites[object_index].texture.x = object_x;
		sprites[object_index].texture.y = object_y;
	#endif
}
#pragma endregion

#pragma region //room stuff
static void runner_interpret_room_goto(int object_index, const char* code)
{
	float object_x = sprites[object_index].spr.params.pos.x;
	float object_y = sprites[object_index].spr.params.pos.y;
    const char* cursor = code;

    while (*cursor != '\0')
    {
		//is this an if??
		if (cursor[0] == 'i' && cursor[1] == 'f')
		{
			bool if_result = runner_interpret_if(cursor, object_x, object_y);

			if (if_result)
			{
				while (*cursor && *cursor != '{')
					cursor++;

				if (*cursor == '{')
					cursor++; // enter block
			}
			else
			{
				while (*cursor && *cursor != '{')
					cursor++;

				cursor = skip_block(cursor);
			}
		}



        char character = *cursor;
		const char* fakecursor = cursor;
        //printf("current char: %c\n", character);

		//is this a room_goto statment part 1
		if (character == 'r'){
			char function[256];
			int i = 0;

			//is this a room_goto statment part 2
			while (*fakecursor != '(' && *fakecursor != ' ' && *fakecursor != '\0'){
				//add each character to the buffer
				function[i++] = *fakecursor;
				fakecursor++;
			}
			function[i] = '\0';

			//break if this doesn't have a bracket at the end or isn't room_goto
			if (*fakecursor != '(' || strcmp(function, "room_goto") != 0)
				break;

            //skip the (
            fakecursor++;

			char roomname[256];
			int roomname_i = 0;
			while (*fakecursor != ')' && *fakecursor != '\0'){
				//add each character to the buffer
				roomname[roomname_i++] = *fakecursor;
				fakecursor++;
			}
			roomname[roomname_i] = '\0';

			CurrentRoom = roomname;
			InitCurrentRoom(data_json);
			printf("Going to room: %s\n", CurrentRoom);

            cursor = fakecursor;
        }

        if (fakecursor == cursor)
            cursor++;
        else
            cursor = fakecursor;
    }
}
#pragma endregion

#pragma region //random stuff
static void runner_interpret_game_end(int object_index, const char* code)
{
	float object_x = sprites[object_index].spr.params.pos.x;
	float object_y = sprites[object_index].spr.params.pos.y;
    const char* cursor = code;

    while (*cursor != '\0')
    {
		//is this an if??
		if (cursor[0] == 'i' && cursor[1] == 'f')
		{
			bool if_result = runner_interpret_if(cursor, object_x, object_y);

			if (if_result)
			{
				while (*cursor && *cursor != '{')
					cursor++;

				if (*cursor == '{')
					cursor++; // enter block
			}
			else
			{
				while (*cursor && *cursor != '{')
					cursor++;

				cursor = skip_block(cursor);
			}
		}



        char character = *cursor;
		const char* fakecursor = cursor;
        //printf("current char: %c\n", character);

		//is this a game_end statment part 1
		if (character == 'g'){
			char function[256];
			int i = 0;

			//is this a game_end statment part 2
			while (*fakecursor != '(' && *fakecursor != ' ' && *fakecursor != '\0'){
				//add each character to the buffer
				function[i++] = *fakecursor;
				fakecursor++;
			}
			function[i] = '\0';

			//break if this doesn't have a bracket at the end or isn't game_end
			if (*fakecursor != '(' || strcmp(function, "game_end") != 0)
				break;

            //skip the (
            fakecursor++;

			//KILL THE GAME!!! EVILl!!!!!!!
			EndGame = true;
			printf("Quiting game! goodbye!\n");

            cursor = fakecursor;
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
	runner_interpret_room_goto(object_def_index, code);
	runner_interpret_game_end(object_def_index, code);
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