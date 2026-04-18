if (active){
	global.game_name = keyboard_string;
}
else{
	global.game_name = string_replace(global.game_name, "|", "");
}

if (mouse_check_button_pressed(mb_left)){	
	if (instance_position(mouse_x, mouse_y, id)){
		active = true;
		obj_project_input.active = false;
		obj_titleid_input.active = false;
		keyboard_string = global.game_name;
	}
	else{
		active = false;
	}
}	
