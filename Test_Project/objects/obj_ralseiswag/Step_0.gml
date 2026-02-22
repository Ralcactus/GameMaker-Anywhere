if (gamepad_button_check(4, gp_padr)){
	x+= 5;
}

if (gamepad_button_check(4, gp_padu)){
	y -=5;
}

if (gamepad_button_check(4, gp_padl)){
	x -=5;
}

if (gamepad_button_check(4, gp_padd)){
	y +=5;
}

if (gamepad_button_check(4, gp_face2)){
	room_goto(Room2);
}

if (gamepad_button_check(4, gp_start)){
	game_end();
}

if (gamepad_button_check_pressed(4, gp_face1)){
	x = 500;
}

if (gamepad_button_check_released(4, gp_face1)){
	y = 500;
}

if (x == 0){
	x = 500;
}

if (y == 0){
	y = 500;
}

