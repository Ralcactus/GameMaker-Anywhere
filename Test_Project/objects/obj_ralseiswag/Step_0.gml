if (gamepad_button_check(4, gp_padr)){
	x+= 4;
}

if (gamepad_button_check(4, gp_padu)){
	y -=4;
}

if (gamepad_button_check(4, gp_padl)){
	x -=4;
}

if (gamepad_button_check(4, gp_padd)){
	y +=4;
}

if (gamepad_button_check(4, gp_face2)){
	room_goto(Room2);
}

if (gamepad_button_check(4, gp_start)){
	//game_end();
}

if (gamepad_button_check_pressed(4, gp_face1)){
	x = 500;
}

if (gamepad_button_check_released(4, gp_face1)){
	y = 500;
}

//camera_set_view_pos(view_camera[0],x,y);