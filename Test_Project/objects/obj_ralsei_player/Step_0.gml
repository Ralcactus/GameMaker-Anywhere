//move up
if (gamepad_button_check(0, gp_padu)){
	y -= 4;
	sprite_index = spr_ralsei_up;
}

//move down
if (gamepad_button_check(0, gp_padd)){
	y += 4;
	sprite_index = spr_ralsei_down;
}

//move right
if (gamepad_button_check(0, gp_padr)){
	x += 4;
	sprite_index = spr_ralsei_right;
}

//move left
if (gamepad_button_check(0, gp_padl)){
	x -= 4;
	sprite_index = spr_ralsei_left;
}

if (gamepad_button_check_pressed(0, gp_face2)){
	gamepad_set_vibration(0, 1, 1);
	room_goto(Room2);
}

//camera
camera_set_view_pos(view_camera[0], x-camera_get_view_width(view_camera[0])/2,y-camera_get_view_height(view_camera[0])/2);
