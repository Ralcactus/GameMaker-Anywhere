#include <citro2d.h>
#include "gml_functions.h"
#include <stdio.h>
#include "cJSON.h"
#include <string.h>
#include "gml_runner.h"
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "main.h"
#include "cross_platform.h"
static const cJSON* root = NULL;

#define MAX_CUSTOM_VARS 64

static var custom_vars[MAX_SPRITES][MAX_CUSTOM_VARS];
static int custom_var_count[MAX_SPRITES] = {0};

static int runner_get_custom_var_index(int object_index, const char* name);
static bool runner_resolve_numeric_token(int object_index, const char* token, float* out_value);

static void quick_printfakecursor(const char* fakecursor){
	printf("fakecursor at:       %c\n", *fakecursor);
}

#pragma region //shortcut stuff
//carry over the root from the main.c (probably better way i could've done this...)
void GML_SetRoot(const cJSON* garrys_in_the_room_tonight){
	root = garrys_in_the_room_tonight;
}

void GML_ResetCustomVariables(void)
{
	memset(custom_vars, 0, sizeof(custom_vars));
	memset(custom_var_count, 0, sizeof(custom_var_count));
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

static bool runner_check_numeric_value(const char* cursor, int object_index, float current_value)
{
	char operationtype[256];
	char value_tocheck[256];

	while (*cursor == ' ')
		cursor++;

	int operation_i = 0;
	while (*cursor != ' ' && *cursor != '\0'){
		operationtype[operation_i++] = *cursor;
		cursor++;
	}
	operationtype[operation_i] = '\0';

	cursor++;
	while (*cursor == ' ')
		cursor++;

	int value_i = 0;
	while (*cursor != ' ' && *cursor != ')' && *cursor != '\0'){
		value_tocheck[value_i++] = *cursor;
		cursor++;
	}
	value_tocheck[value_i] = '\0';

	float compare_value = 0.0f;
	if (!runner_resolve_numeric_token(object_index, value_tocheck, &compare_value))
		return false;

	if (strcmp(operationtype, "==") == 0 || strcmp(operationtype, "=") == 0)
		return current_value == compare_value;

	if (strcmp(operationtype, ">") == 0)
		return current_value > compare_value;

	if (strcmp(operationtype, ">=") == 0)
		return current_value >= compare_value;

	if (strcmp(operationtype, "<") == 0)
		return current_value < compare_value;

	if (strcmp(operationtype, "<=") == 0)
		return current_value <= compare_value;

	return false;
}
#pragma endregion

#pragma endregion

static bool runner_if_middleman(const char* function, const char* args, int object_index, Sprite object)
{
    if (strcmp(function, "gamepad_button_check") == 0)
        return runner_gamepad_button_check(args);

    if (strcmp(function, "gamepad_button_check_pressed") == 0)
        return runner_gamepad_button_check_pressed(args);

    if (strcmp(function, "gamepad_button_check_released") == 0)
        return runner_gamepad_button_check_released(args);

    if (strcmp(function, "x") == 0 || strcmp(function, "y") == 0)
        return runner_check_current_pos(args, object.x, object.y, function);

    int custom_index = runner_get_custom_var_index(object_index, function);
    if (custom_index >= 0)
        return runner_check_numeric_value(args, object_index, custom_vars[object_index][custom_index].f);

    return false;
}

//check for and handle if statements
static bool runner_interpret_if(const char* code, int object_index, Sprite object)
{
    const char* cursor = code;

    while (*cursor != '\0')
    {
        char character = *cursor;
		const char* fakecursor = cursor;


		//is this a if statement?
		if (character == 'i'){
			fakecursor++;
			
			//is this a if statement pt2!!
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

			return runner_if_middleman(function, fakecursor, object_index, object);
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

static void runner_set_vars_to_object(int object_index, Sprite* object)
{ 
    sprites[object_index].x = object->x;
    sprites[object_index].y = object->y;
    #ifdef __3DS__
        sprites[object_index].spr.params.pos.x = sprites[object_index].x;
        sprites[object_index].spr.params.pos.y = sprites[object_index].y;
    #elif __RAYLIB__
        sprites[object_index].texture.x = sprites[object_index].x;
        sprites[object_index].texture.y = sprites[object_index].y;
    #endif
}

static int runner_get_custom_var_index(int object_index, const char* name)
{
    if (object_index < 0 || object_index >= MAX_SPRITES || !name || name[0] == '\0')
        return -1;

    for (int i = 0; i < custom_var_count[object_index]; i++)
    {
        if (strcmp(custom_vars[object_index][i].name, name) == 0)
            return i;
    }

    return -1;
}

static bool runner_builtin_var_value(int object_index, const char* name, float* out_value)
{
    if (!name || !out_value)
        return false;

    if (strcmp(name, "x") == 0)
    {
        *out_value = sprites[object_index].x;
        return true;
    }

    if (strcmp(name, "y") == 0)
    {
        *out_value = sprites[object_index].y;
        return true;
    }

    return false;
}

static bool runner_resolve_numeric_token(int object_index, const char* token, float* out_value)
{
    if (!token || !out_value || token[0] == '\0')
        return false;

    char* end = NULL;
    float numeric = strtof(token, &end);
    if (end && *end == '\0')
    {
        *out_value = numeric;
        return true;
    }

    if (runner_builtin_var_value(object_index, token, out_value))
        return true;

    int custom_index = runner_get_custom_var_index(object_index, token);
    if (custom_index >= 0)
    {
        *out_value = custom_vars[object_index][custom_index].f;
        return true;
    }

    return false;
}

static void runner_var_middleman(int object_index, var value)
{
    if (strcmp(value.name, "x") == 0){
        sprites[object_index].x = value.f;

        #ifdef __3DS__
            sprites[object_index].spr.params.pos.x = sprites[object_index].x;
        #elif __RAYLIB__
            sprites[object_index].texture.x = sprites[object_index].x;
        #endif
        return;
    }

    if (strcmp(value.name, "y") == 0){
        sprites[object_index].y = value.f;

        #ifdef __3DS__
            sprites[object_index].spr.params.pos.y = sprites[object_index].y;
        #elif __RAYLIB__
            sprites[object_index].texture.y = sprites[object_index].y;
        #endif
        return;
    }

    int existing = runner_get_custom_var_index(object_index, value.name);
    if (existing >= 0)
    {
        custom_vars[object_index][existing] = value;
        return;
    }

    if (custom_var_count[object_index] < MAX_CUSTOM_VARS)
    {
        custom_vars[object_index][custom_var_count[object_index]++] = value;
    }
}

//interpret variable of the objects
static void runner_interpret_var(int object_index, const char* code, Sprite object)
{
	runner_set_vars_to_object(object_index, &object);
	bool isNumber = false;

    const char* cursor = code;

    while (*cursor != '\0')
    {
		//is this an if??
		if (cursor[0] == 'i' && cursor[1] == 'f')
		{
			bool if_result = runner_interpret_if(cursor, object_index, object);

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

		//read name of variable
		char identifier[256];
		int id_i = 0;

		while (isalnum(*fakecursor) || *fakecursor == '_')
		{
			identifier[id_i++] = *fakecursor;
			fakecursor++;
		}
		identifier[id_i] = '\0';

		//if nothing read, move on
		if (id_i == 0)
		{
			cursor++;
			continue;
		}

		//skip spaces
		while (*fakecursor == ' ')
			fakecursor++;

		//if next char is '(' -> this is a function call
		if (*fakecursor == '(')
		{
			int depth = 1;
			fakecursor++; // skip '('

			while (*fakecursor && depth > 0)
			{
				if (*fakecursor == '(') depth++;
				else if (*fakecursor == ')') depth--;
				fakecursor++;
			}

			cursor = fakecursor;
			continue;
		}

		// set final variable name
		var value = {0};
		strcpy(value.name, identifier);

		if (!runner_resolve_numeric_token(object_index, identifier, &value.f))
			value.f = 0.0f;

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

			float rhs_number = 0.0f;
			char thechar[256] = {'\0'};
			char rhs_token[256] = {'\0'};

			int rhs_i = 0;
			if (*fakecursor == '-' || *fakecursor == '+' || isdigit((unsigned char)*fakecursor))
			{
				while ((*fakecursor >= '0' && *fakecursor <= '9') || *fakecursor == '.' || *fakecursor == '-' || *fakecursor == '+')
				{
					rhs_token[rhs_i++] = *fakecursor;
					fakecursor++;
				}
			}
			else if ((*fakecursor >= 'a' && *fakecursor <= 'z') || (*fakecursor >= 'A' && *fakecursor <= 'Z') || *fakecursor == '_')
			{
				while ((*fakecursor >= 'a' && *fakecursor <= 'z') || (*fakecursor >= 'A' && *fakecursor <= 'Z') || *fakecursor == '_' || (*fakecursor >= '0' && *fakecursor <= '9'))
				{
					rhs_token[rhs_i++] = *fakecursor;
					fakecursor++;
				}
			}
			rhs_token[rhs_i] = '\0';

			if (rhs_token[0] != '\0' && runner_resolve_numeric_token(object_index, rhs_token, &rhs_number))
			{
				isNumber = true;
			}
			else if (rhs_token[0] != '\0')
			{
				isNumber = false;
				strncpy(thechar, rhs_token, sizeof(thechar) - 1);
			}
			else
			{
				//uh oh! this is not a number or variable we support! kill code!
				return;
			}

			//Numbered value
			if(isNumber){
				//add value
				if (operationtype == '+'){
					value.f += rhs_number;
				}

				//minus value
				if (operationtype == '-'){
					value.f -= rhs_number;
				}

				//multiply value
				if (operationtype == '*'){
					value.f *= rhs_number;
				}

				//divide value
				if (operationtype == '/'){
					value.f /= rhs_number;
				}

				//set value
				if (operationtype == '='){
					value.f = rhs_number;
				}
			} else {
				//Boolean value
				if(operationtype == '='){
					//set value
					if (strncmp(thechar, "true", 4) == 0){
						value.b = true;
					}
					else if (strncmp(thechar, "false", 5) == 0){
						value.b = false;
					}

					//Char value
					if (*fakecursor == '\'' || *fakecursor == '"'){
						char quote_type = *fakecursor;
						fakecursor++;
						if (*fakecursor != quote_type){
							//uh oh! this is not a value we support! kill code!
							return;
						}
						thechar[0] = *fakecursor;
						thechar[1] = '\0';
						fakecursor++;
						if (*fakecursor != quote_type){
							//uh oh! this is not a value we support! kill code!
							return;
						}
							fakecursor++;
							strcpy(value.s, thechar);
					}
				} else {
					//no usual type of variable found lol
					return;
				}
			}
			//these made the game lag :3
			//printf("Setting variable:       %s\n", value.name);
			//printf("Result of variable:       %f\n", value.f);
			runner_var_middleman(object_index, value);
		}
		if (fakecursor == cursor)
			cursor++;
		else
			cursor = fakecursor;

	}
}

//interpret x and y of the objects
static void runner_interpret_xy(int object_index, const char* code, Sprite object)
{
	object.x = sprites[object_index].spr.params.pos.x;
	object.y = sprites[object_index].spr.params.pos.y;
    const char* cursor = code;

    while (*cursor != '\0')
    {
		//is this an if??
		if (cursor[0] == 'i' && cursor[1] == 'f')
		{
			bool if_result = runner_interpret_if(cursor, object_index, object);

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
						object.x += thenumber;
					else
						object.y += thenumber;
				}

				//minus value
				if (operationtype == '-'){
					if (postype == 'x')
						object.x -= thenumber;
					else
						object.y -= thenumber;
				}

				//multiply value
				if (operationtype == '*'){
					if (postype == 'x')
						object.x *= thenumber;
					else
						object.y *= thenumber;
				}

				//set value
				if (operationtype == '='){
					if (postype == 'x')
						object.x = thenumber;
					else
						object.y = thenumber;
				}
			}
		}
		if (fakecursor == cursor)
			cursor++;
		else
			cursor = fakecursor;

	}

	#ifdef __3DS__
		C2D_SpriteSetPos(&sprites[object_index].spr, object.x, object.y);
	#elif __RAYLIB__
		sprites[object_index].texture.x = object.x;
		sprites[object_index].texture.y = object.y;
	#endif
}
#pragma endregion

#pragma region //room stuff
static void runner_interpret_room_goto(int object_index, const char* code, Sprite object)
{
	object.x = sprites[object_index].spr.params.pos.x;
	object.y = sprites[object_index].spr.params.pos.y;
    const char* cursor = code;

    while (*cursor != '\0')
    {
		//is this an if??
		if (cursor[0] == 'i' && cursor[1] == 'f')
		{
			bool if_result = runner_interpret_if(cursor, object_index, object);

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
			if (*fakecursor != '(' || strcmp(function, "room_goto") != 0){
				cursor++;
				continue;
			}

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
static void runner_interpret_game_end(int object_index, const char* code, Sprite object)
{
	object.x = sprites[object_index].spr.params.pos.x;
	object.y = sprites[object_index].spr.params.pos.y;
    const char* cursor = code;

    while (*cursor != '\0')
    {
		//is this an if??
		if (cursor[0] == 'i' && cursor[1] == 'f')
		{
			bool if_result = runner_interpret_if(cursor, object_index, object);

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
			if (*fakecursor != '(' || strcmp(function, "game_end") != 0){
				cursor++;
				continue;
			}
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

static void runner_interpret_camera_set_view_pos(int object_index, const char* code, Sprite object)
{
	object.x = sprites[object_index].spr.params.pos.x;
	object.y = sprites[object_index].spr.params.pos.y;
    const char* cursor = code;

    while (*cursor != '\0')
    {
		#pragma region //if handling
		//is this an if??
		if (cursor[0] == 'i' && cursor[1] == 'f')
		{
			bool if_result = runner_interpret_if(cursor, object_index, object);

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
		#pragma endregion

        char character = *cursor;
		const char* fakecursor = cursor;


		//is this a camera_set_view_pos statment part 1
		if (character == 'c'){
			char function[256];
			int i = 0;

			//is this a room_camera_set_view_posgoto statment part 2
			while (*fakecursor != '(' && *fakecursor != ' ' && *fakecursor != '\0'){
				//add each character to the buffer
				function[i++] = *fakecursor;
				fakecursor++;
			}
			function[i] = '\0';

			//break if this doesn't have a bracket at the end or isn't camera_set_view_pos
			if (*fakecursor != '(' || strcmp(function, "camera_set_view_pos") != 0){
				cursor++;
				continue;
			}

            //skip the (
            fakecursor++;

			char view[256];
			int view_i = 0;
			while (*fakecursor != ',' && *fakecursor != '\0'){
				//add each character to the buffer
				view[view_i++] = *fakecursor;
				fakecursor++;
			}
			view[view_i] = '\0';

			//skip the ,
            fakecursor++;

			//proper units
			const float scale_x = (float)cam_w / screen_w;
			const float scale_y = (float)cam_h / screen_h;
			float camx_setto = 0;
			float camy_setto = 0;
			
			int xpos_i = 0;
			while (*fakecursor != ',' && *fakecursor != ')' && *fakecursor != '\0'){
				if (*fakecursor == 'x'){
					camx_setto = object.x / scale_x;
				}
				else if (*fakecursor == 'y'){
					camy_setto = object.y / scale_y;
				}
				else if (*fakecursor == '+'){
					fakecursor++;
					while (*fakecursor == ' ')
						fakecursor++;
					
					char values[256];
					int values_i = 0;
					while (*fakecursor != ',' && *fakecursor != ' ' && *fakecursor != '\0'){
						//add each character to the buffer
						values[values_i++] = *fakecursor;
						fakecursor++;
					}
					values[values_i] = '\0';
					
					camx_setto+=strtof(values, NULL);
					
				}
				fakecursor++;
			}

			//skip the ,
			if (*fakecursor == ',')
            	fakecursor++;

			while (*fakecursor == ' ')
				fakecursor++;

			int ypos_i = 0;
			while (*fakecursor != ',' && *fakecursor != ')' && *fakecursor != '\0'){
				if (*fakecursor == 'x'){
					camy_setto = object.x / scale_x;
				}
				else if (*fakecursor == 'y'){
					camy_setto = object.y / scale_y;
				}
				else if (*fakecursor == '+'){
					fakecursor++;
					while (*fakecursor == ' ')
						fakecursor++;
					
					char values[256];
					int values_i = 0;
					while (*fakecursor != ',' && *fakecursor != ' ' && *fakecursor != '\0'){
						//add each character to the buffer
						values[values_i++] = *fakecursor;
						fakecursor++;
					}
					values[values_i] = '\0';
					
					camy_setto+=strtof(values, NULL);
					
				}
				fakecursor++;
			}

			while (*fakecursor == ' ')
				fakecursor++;
				
				
			cam_x = camx_setto;
			cam_y = camy_setto;




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
	//runner_interpret_xy(object_def_index, code, sprites[object_def_index]);
	runner_interpret_var(object_def_index, code, sprites[object_def_index]);
	runner_interpret_room_goto(object_def_index, code, sprites[object_def_index]);
	runner_interpret_game_end(object_def_index, code, sprites[object_def_index]);
	runner_interpret_camera_set_view_pos(object_def_index, code, sprites[object_def_index]);
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