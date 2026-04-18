if (active){
	global.publisher = keyboard_string;
}
else{
	global.publisher = string_replace(global.publisher, "|", "");
}

if (mouse_check_button_pressed(mb_left)){	
	if (instance_position(mouse_x, mouse_y, id)){
		active = true;
		obj_project_input.active = false;
		obj_titleid_input.active = false;
		keyboard_string = global.publisher;
	}
	else{
		active = false;
	}
}	
