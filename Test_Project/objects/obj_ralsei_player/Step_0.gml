//move up
if (gamepad_axis_value(4, gp_axislv) <= -0.1){
	y -= 4;
	sprite_index = spr_ralsei_up;
}

//move down
if (gamepad_axis_value(4, gp_axislv)){
	y += 4;
	sprite_index = spr_ralsei_down;
}

//move right
if (gamepad_axis_value(4, gp_axislh)){
	x += 4;
	sprite_index = spr_ralsei_right;
}

//move left
if (gamepad_axis_value(4, gp_axislh) <= -0.1){
	x -= 4;
	sprite_index = spr_ralsei_left;
}

if (gamepad_button_check_pressed(4, gp_face2)){
	room_goto(Room2);
}

//camera
camera_set_view_pos(view_camera[0], x-camera_get_view_width(view_camera[0])/2,y-camera_get_view_height(view_camera[0])/2);
