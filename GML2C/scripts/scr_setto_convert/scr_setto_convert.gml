function scr_setto_convert(i, char_code, originalcode){
	
	//Variable??
	if (isAlpha(char_code)){
		
		var code_cursor = i;
		
		//skip var name
		while (code_cursor <= string_length(originalcode)){
			var cursorchar = string_char_at(originalcode, code_cursor);
			if (!isAlpha(cursorchar) && !isDigit(cursorchar) && cursorchar != "_")
				break;
						
			code_cursor++;
		}
		
		//skip spaces
		while (code_cursor <= string_length(originalcode)){
			var cursorchar = string_char_at(originalcode, code_cursor);
			if (cursorchar != " " && cursorchar != "\t")
				break;
						
			code_cursor++;
		}
		
		//check =
		if (string_char_at(originalcode, code_cursor) == "="){
			
			code_cursor++;
			
			//skip spaces
			while (code_cursor <= string_length(originalcode)){
				var cursorchar = string_char_at(originalcode, code_cursor);
				if (cursorchar != " " && cursorchar != "\t")
					break;
						
				code_cursor++;
			}
			
			//Char to Int conversion
			if (string_char_at(originalcode, code_cursor) == "\""){
				code_cursor++;
				
				var ascii = 0;
				
				while(string_char_at(originalcode, code_cursor) != "\""){
					ascii = ascii * 256 + ord(string_char_at(originalcode, code_cursor));
					code_cursor++;
				}
				
			}
			
		}
		
	}
	
	
	return originalcode;
}