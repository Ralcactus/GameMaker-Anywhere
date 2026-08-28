//freezes the app after teh message is showed
function show_debug_message_FREEZE(str){
	show_debug_message(str)
	
	//bye!
	while (true){};
}