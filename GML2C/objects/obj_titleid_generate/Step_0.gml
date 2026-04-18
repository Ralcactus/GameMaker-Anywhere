if (instance_position(mouse_x, mouse_y, id)){
	if (mouse_check_button_pressed(mb_left)){
		var result = "";
	
		for (var i = 0; i < 16; i++){
			result += string_char_at("0123456789ABCDEF", irandom(string_length("0123456789ABCDEF") - 1) + 1);
		}
    
		global.title_id = result;
	}
	
	image_blend = c_gray;
}
else
	image_blend = c_white;
