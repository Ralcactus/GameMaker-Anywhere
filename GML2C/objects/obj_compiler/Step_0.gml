//dont take inputs
if (global.exporting)
	exit; 

if (global.current_field >= 0) {
    switch(global.current_field) {
        case 0: //name
            global.game_name = keyboard_string;
            break;
            
        case 1: //id
            var filtered = "";
            for (var i = 1; i <= string_length(keyboard_string); i++) {
                var char = string_char_at(keyboard_string, i);
                if (string_pos(char, "0123456789ABCDEFabcdef") > 0) {
                    filtered += string_upper(char);
                }
            }
            keyboard_string = string_copy(filtered, 1, 16);
            global.title_id = keyboard_string;
            break;
            
        case 2: //publisher
            global.publisher = keyboard_string;
            break;
    }
}

if (CompletionStatusFromExecutedProcess(compile_powershell_window) && logging == true){
	run_commandpowershell(destination+"\\output\\", "explorer .")
	logging = false;
}

/*
if (CompletionStatusFromExecutedProcess(pid)){
	show_message("done");
}