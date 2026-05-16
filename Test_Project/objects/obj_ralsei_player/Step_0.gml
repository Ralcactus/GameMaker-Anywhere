//move up
if (gamepad_button_check(0, gp_padu)){
	vsp = -4;
	sprite_index = spr_ralsei_up;
}

//move down
if (gamepad_button_check(0, gp_padd)){
	vsp = 4;
	sprite_index = spr_ralsei_down;
}

//move right
if (gamepad_button_check(0, gp_padr)){
	hsp = 4;
	sprite_index = spr_ralsei_right;
}

//move left
if (gamepad_button_check(0, gp_padl)){
	hsp = -4;
	sprite_index = spr_ralsei_left;
}

if (gamepad_button_check_pressed(0, gp_face2)){
	gamepad_set_vibration(0, 1, 1);
	room_goto(Room2);
}
show_debug_message(string(global.teh));
x += hsp;
y += vsp;
if (hsp > 0)
	hsp-=0.1;
	
if (hsp < 0)
	hsp+=0.1;
	
if (vsp > 0)
	vsp-=0.1;
	
if (vsp < 0)
	vsp+=0.1;
	
//camera
camera_set_view_pos(view_camera[0], x-camera_get_view_width(view_camera[0])/2,y-camera_get_view_height(view_camera[0])/2);
