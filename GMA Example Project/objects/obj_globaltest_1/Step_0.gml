camera_set_view_pos(view_camera[0], 0, 0);

if (gamepad_button_check_pressed(4, gp_face2)){
	global.mario+=1;
	show_debug_message(string(global.mario));
}

if (gamepad_button_check_pressed(4, gp_start))
	room_goto(Sound_test);
