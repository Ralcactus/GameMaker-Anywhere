if (gamepad_button_check(4, gp_padu)){
	y -= 4;
	sprite_index = spr_ralsei_up;
}

if (gamepad_button_check(4, gp_padd)){
	y += 4;
	sprite_index = spr_ralsei_down;
}

if (gamepad_button_check(4, gp_padr)){
	x += 4;
	sprite_index = spr_ralsei_right;
}

if (gamepad_button_check(4, gp_padl)){
	x -= 4;
	sprite_index = spr_ralsei_left;
}

if (gamepad_button_check(4, gp_face1)){
	show_message("hi this is a show_message prompt");
}

if (gamepad_button_check_pressed(4, gp_start)){
	show_message("goodbye!");
	game_end();
}

if (gamepad_button_check_pressed(4, gp_face2))
	room_goto(Ralsei_Love);

camera_set_view_pos(view_camera[0], x-camera_get_view_width(view_camera[0])/2,y-camera_get_view_height(view_camera[0])/2);
