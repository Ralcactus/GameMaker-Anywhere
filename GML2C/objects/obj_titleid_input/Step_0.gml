if (active){
    var filtered = "";
    for (var i = 1; i <= string_length(keyboard_string); i++){
        var char = string_char_at(keyboard_string, i);
        if (string_pos(char, "0123456789ABCDEFabcdef") > 0){
            filtered += string_upper(char);
        }
    }

	global.title_id = string_copy(filtered, 1, 16);
}
else{
	global.title_id = string_replace(global.title_id, "|", "");
}

if (mouse_check_button_pressed(mb_left)){
	if (instance_position(mouse_x, mouse_y, id)){
		active = true;
		obj_gamename_input.active = false;
		obj_project_input.active = false;
		keyboard_string = global.title_id;
	}
	else
		active = false;
}	
