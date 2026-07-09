camera_set_view_pos(view_camera[0], 0, 0);

if (gamepad_button_check_pressed(4, gp_face2)){
	audio_play_sound(mu_NoelleHouse, 1, 1);	
}

if (gamepad_button_check_pressed(4, gp_face1)){
	audio_play_sound(mu_GirlNextDoor, 1, 1);	
}

if (gamepad_button_check_pressed(4, gp_start))
	room_goto(General_test);