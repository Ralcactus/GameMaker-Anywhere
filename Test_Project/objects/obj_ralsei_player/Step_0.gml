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


if (gamepad_button_check_pressed(4, gp_face2))
	room_goto(Ralsei_Love);
