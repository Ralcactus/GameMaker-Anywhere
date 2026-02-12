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

void remove_all_chars(char* str, char c) {
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}

static inline void skip_emptyspace(const char** cursor_butgarryateit)
{
	//i love stupid ass variable names :garry:
	const char* cursor_butgarryateit_thesequal = *cursor_butgarryateit;
	while (*cursor_butgarryateit_thesequal == ' ' || *cursor_butgarryateit_thesequal == '\t' || *cursor_butgarryateit_thesequal == '\r' || *cursor_butgarryateit_thesequal == '\n')
		cursor_butgarryateit_thesequal++;
	
	*cursor_butgarryateit = cursor_butgarryateit_thesequal;
}

static int input_convertstring(const char* s, const char** out_end)
{
	*out_end = s + 8; 
	if (strncmp(s, "gp_face1", 8) == 0){
		return gp_face1; 
	}

	if (strncmp(s, "gp_face2", 8) == 0){
		return gp_face2; 
	}

	if (strncmp(s, "gp_face3", 8) == 0){
		return gp_face3; 
	}

	if (strncmp(s, "gp_face4", 8) == 0){
		return gp_face4; 
	}

	if (strncmp(s, "gp_start", 8) == 0){
		return gp_start; 
	}

	if (strncmp(s, "gp_select", 9) == 0){
		return gp_select; 
	}

	return -4;
}

static const char* skip_block(const char* cursor)
{
	int depth = 1;
	while (*cursor && depth > 0)
	{
		if (*cursor == '{'){
			depth++;
		}
		else if (*cursor == '}'){
			depth--;
		}

		cursor++;
	}

	return cursor;
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

    return -1; //make compiler happy
}
#pragma endregion

#pragma region //if statments

//check if button is held
static bool if_gamepad_button_check(const char** p_cursor){
	const char* cursor = *p_cursor;
	
	//gamepad check function
	if (strncmp(cursor, "gamepad_button_check", 20) == 0)
	{
		cursor += 20;

		while (*cursor && *cursor != '('){
			cursor++;
		}

		if (*cursor == '(')
			cursor++;

		//skip the device text
		strtol(cursor, (char**)&cursor, 10);

		while (*cursor && *cursor != ','){
			cursor++;
		}

		if (*cursor == ',')
			cursor++;

		//skip empty space again again!
		skip_emptyspace(&cursor);

		int button = input_convertstring(cursor, &cursor);

		while (*cursor && *cursor != ')'){
			cursor++;
		}

		if (*cursor == ')')
			cursor++;

		while (*cursor && *cursor != '{'){
			cursor++;
		}

		if (*cursor == '{') 
			cursor++;

		if (!gamepad_button_check(button))
			cursor = skip_block(cursor);

		*p_cursor = cursor;
		return true;
	}
	else{
		return false;
	}
}

//check if button is pressed
static bool if_gamepad_button_check_pressed(const char** p_cursor){
	const char* cursor = *p_cursor;
	
	//gamepad check function
	if (strncmp(cursor, "gamepad_button_check_pressed", 28) == 0)
	{
		cursor += 28;

		while (*cursor && *cursor != '('){
			cursor++;
		}

		if (*cursor == '(')
			cursor++;

		//skip the device text
		strtol(cursor, (char**)&cursor, 10);

		while (*cursor && *cursor != ','){
			cursor++;
		}

		if (*cursor == ',')
			cursor++;

		//skip empty space again again!
		skip_emptyspace(&cursor);

		int button = input_convertstring(cursor, &cursor);

		while (*cursor && *cursor != ')'){
			cursor++;
		}

		if (*cursor == ')')
			cursor++;

		while (*cursor && *cursor != '{'){
			cursor++;
		}

		if (*cursor == '{') 
			cursor++;

		if (!gamepad_button_check_pressed(button))
			cursor = skip_block(cursor);

		*p_cursor = cursor;
		return true;
	}
	else{
		return false;
	}
}

//check if button is released
static bool if_gamepad_button_check_released(const char** p_cursor){
	const char* cursor = *p_cursor;

	//gamepad check function
	if (strncmp(cursor, "gamepad_button_check_released", 29) == 0)
	{
		cursor += 29;

		while (*cursor && *cursor != '('){
			cursor++;
		}

		if (*cursor == '(')
			cursor++;

		//skip the device text
		strtol(cursor, (char**)&cursor, 10);

		while (*cursor && *cursor != ','){
			cursor++;
		}

		if (*cursor == ',')
			cursor++;

		//skip empty space again again!
		skip_emptyspace(&cursor);

		int button = input_convertstring(cursor, &cursor);

		while (*cursor && *cursor != ')'){
			cursor++;
		}

		if (*cursor == ')')
			cursor++;

		while (*cursor && *cursor != '{'){
			cursor++;
		}

		if (*cursor == '{') 
			cursor++;

		if (!gamepad_button_check_released(button))
			cursor = skip_block(cursor);

		*p_cursor = cursor;
		return true;
	}
	else{
		return false;
	}
}

static bool runner_apply_if_code(const char** p_cursor){
	const char* cursor = *p_cursor;

	//check if this is a if statment
	if (strncmp(cursor, "if", 2) == 0)
	{
		cursor += 2;

		//skip empty space
		skip_emptyspace(&cursor);

		if (*cursor == '(')
		{
			cursor++;
		}
		//skip empty space again!
		skip_emptyspace(&cursor);	



				
		//the if code checks
		//gamepad_button_pressed
		if (if_gamepad_button_check_pressed(&cursor))
		{
			*p_cursor = cursor;
			return true;
		}

		//gamepad_button_released
		if (if_gamepad_button_check_released(&cursor))
		{
			*p_cursor = cursor;
			return true;
		}

		//gamepad_button_check (held)
		if (if_gamepad_button_check(&cursor))
		{
			*p_cursor = cursor;
			return true;
		}


	}

	return false;
}
#pragma endregion

#pragma region //applying code
//interpret x and y of the objects
static void runner_interpret_xy(int object_index, const char* code)
{
	float current_x = sprites[object_index].spr.params.pos.x;
	float current_y = sprites[object_index].spr.params.pos.y;
	const char* cursor = code;

	while (cursor && *cursor)
	{
		//skip empty space
		skip_emptyspace(&cursor);

		if (*cursor == '{' || *cursor == '}')
		{
			cursor++;
			continue;
		}

		if (runner_apply_if_code(&cursor))
			continue;


		//check if this is a x or y statement
		char axis = *cursor;
		if (axis != 'x' && axis != 'y')
		{
			while (*cursor && *cursor != ';')
				cursor++;
			
			if (*cursor == ';')
				cursor++;

			continue;
		}
		cursor++;

		//skip empty space
		skip_emptyspace(&cursor);


		//check if doing addition, subtraction, set to, ect
		bool to_add = false;
		bool to_subtract = false;
		bool to_multi = false;
		
		if (*cursor == '+')
		{
			cursor++;
			to_add = true;
			cursor++;
		}
		else if (*cursor == '-')
		{
			cursor++;
			to_subtract = true;
			cursor++;
		}		
		else if (*cursor == '*')
		{
			cursor++;
			to_multi = true;
			cursor++;
		}
		else if (*cursor == '=')
		{
			cursor++;
		}

		//parse number
		char* endptr = NULL;
		float value = (float)strtod(cursor, &endptr);

		if (endptr != cursor)
		{
			if (axis == 'x'){
				if (to_add)
					current_x += value;
				else if (to_subtract)
					current_x -= value;
				else if (to_multi)
					current_x *= value;
				else
					current_x = value;
			}
			
			if (axis == 'y'){
				if (to_add)
					current_y += value;
				else if (to_subtract)
					current_y -= value;
				else if (to_multi)
					current_y *= value;
				else
					current_y = value;
			}
			cursor = endptr;
		}


		//continue if at the end of a line
		if (*cursor == ';')
			cursor++;

		while (*cursor == '\n' || *cursor == '\r')
			cursor++;
	}

	

	C2D_SpriteSetPos(&sprites[object_index].spr, current_x, current_y);
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