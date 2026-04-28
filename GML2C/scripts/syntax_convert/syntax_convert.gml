//TODO:
//finish == convert
//add ; convert

function syntax_convert(originalcode){
	for (var i = 1; i <= string_length(originalcode); i++)
	{
		var char_code = string_char_at(originalcode, i);

		originalcode = scr_ifbracket_convert(i, char_code, originalcode);
		//originalcode = scr_setto_convert(i, char_code, originalcode);
	}
	
	return originalcode;
}
