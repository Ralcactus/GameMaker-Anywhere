var device = 4;

if (gamepad_button_check(device, gp_padr)){
	x+= 5;
}

if (gamepad_button_check(device, gp_padu)){
	y -=5;
}

if (gamepad_button_check(device, gp_padl)){
	x -=5;
}

if (gamepad_button_check(device, gp_padd)){
	y +=5;
}

if (gamepad_button_check(device, gp_face2)){
	room_goto(Room2);
}

if (gamepad_button_check(device, gp_start)){
	game_end();
}

if (gamepad_button_check_pressed(device, gp_face1)){
	x = 500;
}

if (gamepad_button_check_released(device, gp_face1)){
	y = 500;
}

camera_set_view_pos(view_camera[0],x,y);