//bg + grow/shrink

if (!DestroySettings){
	width += 15;
	height += 15;
}
else{
	width -= 15;
	height -= 15;	
	
	if (width <= 300 && height <= 0){
		instance_destroy();
	}
}

bgx += 0.2;
//bgy += 0.2;

if (width > room_width/2 - 25)
    width = room_width/2 - 25;
	
if (height > room_height/2 - 25)
    height = room_height/2 - 25;
	
//checkboxes
TurnThisSettingBoxThingGray = -4;
for (var i = 0; i < array_length(SettingsNames); ++i){
	if (mouse_x > 100 && mouse_x < 132 && mouse_y > 300+i*40 && mouse_y < 332+i*40){
		TurnThisSettingBoxThingGray = i;
		
		if (mouse_check_button_pressed(mb_left)){
			if (variable_global_get(SettingsVariables[i]) == false)
				variable_global_set(SettingsVariables[i], true);
			else
				variable_global_set(SettingsVariables[i], false);
		}
	}
}

//buttons
//TurnThisSettingBoxThingGray = -4;
for (var i = 0; i < array_length(ButtonSprites); ++i){
	if (mouse_x > ButtonX[i] && mouse_x < ButtonX[i]+sprite_get_width(ButtonSprites[i]) && mouse_y > ButtonY[i] && mouse_y < ButtonY[i]+sprite_get_height(ButtonSprites[i])){
		//TurnThisSettingBoxThingGray = i;
		
		if (mouse_check_button_pressed(mb_left)){
			ButtonFunctions[i]();
		}
	}
}
