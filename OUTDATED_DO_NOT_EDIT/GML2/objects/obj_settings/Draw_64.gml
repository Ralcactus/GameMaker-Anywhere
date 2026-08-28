draw_rectangle(room_width/2-width, room_height/2-height, room_width/2+width, room_height/2+height, true);

var _scissor = gpu_get_scissor();
gpu_set_scissor(room_width/2 - width, room_height/2 - height, width*2-1, height*2-1);

draw_sprite_tiled_ext(spr_optionBG, 0, bgx, bgy, 1, 1, c_gray, 1);

draw_set_halign(fa_left);
draw_set_valign(fa_middle);

//checkboxes
for (var i = 0; i < array_length(SettingsNames); ++i) {
	if (i != TurnThisSettingBoxThingGray)
		draw_sprite(spr_checkbox, variable_global_get(SettingsVariables[i]), 100, 300+i*40);
	else
		draw_sprite_ext(spr_checkbox, variable_global_get(SettingsVariables[i]), 100, 300+i*40, 1, 1, 0, c_gray, 1);
	
	draw_text(140, 316+i*40, SettingsNames[i]);
}

//buttons
for (var i = 0; i < array_length(ButtonSprites); ++i) {
	draw_sprite(ButtonSprites[i], 0, ButtonX[i], ButtonY[i]);
}

gpu_set_scissor(_scissor);