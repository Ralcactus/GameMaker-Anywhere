function scr_compileroom_phase2(){
	var safe_name = sanitize_filename(yyfile.name);
	var roomhfile = file_text_open_write(destination + "source\\rooms\\" + safe_name + ".h");
	file_text_write_string(roomhfile, 
		"void scr_runroom_" + yyfile.name +"();"
	)
	file_text_close(roomhfile)
	
	//var roomcfile = file_text_open_append(destination + "source\\rooms\\" + safe_name + ".c");
	var roomcfile = file_text_open_write(destination + "source\\rooms\\" + safe_name + ".c");
	file_text_write_string(roomcfile, 
		"#include <stdio.h>\n" +
		"#include <string.h>\n\n" +
		"void scr_runroom_" + yyfile.name +"(){\n" +
		"	printf(\"RUNNING ROOM " + yyfile.name + "\\n\");\n" +
		"}\n"
	);
	file_text_close(roomcfile)
	
}