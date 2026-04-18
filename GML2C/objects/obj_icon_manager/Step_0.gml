if (instance_position(mouse_x, mouse_y, id)){
	image_blend = c_gray;

	if (mouse_check_button_pressed(mb_left)){
		var savedicon_path = global.iconpath;
		var savedicon_sprite = global.iconspr;
	
		global.iconpath = get_open_filename("3ds Icon|*.png", "");
		global.iconspr = sprite_add(global.iconpath, 0, false, false, -8, 48);
	
		if (sprite_get_width(global.iconspr) != 48 || sprite_get_height(global.iconspr) != 48){
			show_message("ERROR!\nThe icon must be 48x48!");
			global.iconspr = savedicon_sprite;
			global.iconpath = savedicon_path;
			exit;
		}
	
		sprite_index = global.iconspr;
	}
}
else
	image_blend = c_white;