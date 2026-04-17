function syntax_convert(originalcode){
	//variables also might be handled here? 

	for (var i = 1; i <= string_length(originalcode); i++)
	{
		var char_code = string_char_at(originalcode, i);

		//weird if statements with no brackets around them
		originalcode = scr_ifbracket_convert(i, char_code, originalcode);
		originalcode = scr_setto_convert(i, char_code, originalcode);
	}
	
	return originalcode;
}
