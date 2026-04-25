camera_set_view_pos(view_camera[0], 0, 0);

if (gamepad_button_check_pressed(0, gp_face2)){
	gamepad_set_vibration(0, 0, 0);
	room_goto(MainTest);
}
