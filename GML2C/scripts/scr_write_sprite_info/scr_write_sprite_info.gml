//this needs to be cleaned up so bad, its so messy
function scr_write_sprite_info(){
	var spriteinfoC = file_text_open_append(destination + "source/helpers/get_spriteinfo.cpp");
	var realspritewidth_array = string_replace(string_replace(global.SpriteWidths, "[", "{"), "]", "}");
	var realspriteheight_array = string_replace(string_replace(global.SpriteHeights, "[", "{"), "]", "}");
	
	var realspriteoriginX_array = string_replace(string_replace(global.SpriteOriginX, "[", "{"), "]", "}");
	var realspriteoriginY_array = string_replace(string_replace(global.SpriteOriginY, "[", "{"), "]", "}");
	
	file_text_write_string(spriteinfoC, "int SpriteWidths[] = " + string(realspritewidth_array) + ";\n");
	file_text_write_string(spriteinfoC, "int SpriteHeights[] = " + string(realspriteheight_array) + ";\n");
	file_text_write_string(spriteinfoC, "int SpriteOriginX[] = " + string(realspriteoriginX_array) + ";\n");
	file_text_write_string(spriteinfoC, "int SpriteOriginY[] = " + string(realspriteoriginY_array) + ";\n");
	
	//sprite bounding box
	var real_spriteboundbox_TOP_array = string_replace(string_replace(global.SpriteBoxTOP, "[", "{"), "]", "}");
	var real_spriteboundbox_BOTTOM_array = string_replace(string_replace(global.SpriteBoxBOTTOM, "[", "{"), "]", "}");
	var real_spriteboundbox_LEFT_array = string_replace(string_replace(global.SpriteBoxLEFT, "[", "{"), "]", "}");
	var real_spriteboundbox_RIGHT_array = string_replace(string_replace(global.SpriteBoxRIGHT, "[", "{"), "]", "}");
	
	file_text_write_string(spriteinfoC, "int SpriteCollideTOP[] = " + string(real_spriteboundbox_TOP_array) + ";\n");
	file_text_write_string(spriteinfoC, "int SpriteCollideBOTTOM[] = " + string(real_spriteboundbox_BOTTOM_array) + ";\n");
	file_text_write_string(spriteinfoC, "int SpriteCollideLEFT[] = " + string(real_spriteboundbox_LEFT_array) + ";\n");
	file_text_write_string(spriteinfoC, "int SpriteCollideRIGHT[] = " + string(real_spriteboundbox_RIGHT_array) + ";\n");


	var realspriteframecount_array = string_replace(string_replace(global.SpriteFrameCount, "[", "{"), "]", "}");
	file_text_write_string(spriteinfoC, "int SpriteFrameCount[] = " + string(realspriteframecount_array) + ";\n");
	
	
	//sprite playback timer (has section to convert floats to have f in them)
	file_text_write_string(spriteinfoC, "float SpriteAnimTimer[] = {");
	var towrite = "";
	for (var i = 0; i < array_length(global.SpriteAnimTimer); ++i) {
		var insert = string(global.SpriteAnimTimer[i]);
		
		if (global.SpriteAnimTimer[i] != floor(global.SpriteAnimTimer[i]))
			insert+="f";

		towrite += string(insert) + ",";
	}
	towrite = string_delete(towrite, string_length(towrite), 1);
	file_text_write_string(spriteinfoC, towrite + "};\n");
	
	
	
	var realSpriteAnimSpeedType_array = string_replace(string_replace(global.SpriteAnimSpeedType, "[", "{"), "]", "}");
	file_text_write_string(spriteinfoC, "int SpriteAnimSpeedType[] = " + string(realSpriteAnimSpeedType_array) + ";\n");
	
	file_text_close(spriteinfoC);
}