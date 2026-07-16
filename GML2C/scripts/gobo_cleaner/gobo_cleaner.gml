function gobo_cleaner(gmlfile_path){
	var gobo;
	if (os_type == os_linux || os_type == os_macosx) {
	    run_commandpowershell(
	        "/",
	        "chmod +x '" + working_directory + "other/gobo'",
	        false
	    );
	}

	if (os_type == os_windows){
		gobo = run_commandpowershell("C:/", working_directory+"Other/gobo.exe --write-stdout --skip-write '" + gmlfile_path + "'", false); //run gobo
	} else if (os_type == os_linux || os_type == os_macosx) {
	    var gobo_path = working_directory + "other/gobo";
	    var gobo_dir = working_directory + "other";

	    gobo = run_commandpowershell(
	        gobo_dir,
	        "chmod +x './gobo' && './gobo' --write-stdout --skip-write '" + gmlfile_path + "'",
	        false
	    );
}
	var output = ExecutedProcessReadFromStandardOutput(gobo); //read the output
	var startingpoint = string_length("Formatting " + filename_name(gmlfile_path))+4; //find the starting formatting start text and +4 to skip the "l..."
	var fullstring_pt1 = string_copy(output, startingpoint, string_length(output) - startingpoint + 1); //copy the output besides the formating start text

	FreeExecutedProcessStandardOutput(gobo);
	FreeExecutedProcessStandardInput(gobo);

	//delete the timer text
	var lastD = string_last_pos("D", fullstring_pt1); //find the D in "Done in 13 ms"
	var fullstring_pt2 = string_delete(fullstring_pt1, lastD, string_length(fullstring_pt1) - lastD + 1); //Delete everything in front

	return fullstring_pt2;
} 