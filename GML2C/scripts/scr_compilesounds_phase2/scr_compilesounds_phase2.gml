function scr_compilesounds_phase2(){
	var inc_audios = file_text_open_append(destination + "source/include_audio.h");
	
	for (var i = 0; i  < currentsound_count; i++) {
		file_text_write_string(inc_audios, "#include \"" + string(i) + "_ogg.h\"\n");
	}
	
	file_text_close(inc_audios);
	
	var ref_audios = file_text_open_append(destination + "source/include_audio.cpp");
	
	file_text_write_string(ref_audios, "const uint8_t *ogg_refs[] = {\n");
	for (var i = 0; i  < currentsound_count; i++) {
		if (i < currentsound_count -1)
			file_text_write_string(ref_audios, "_" + string(i) + "_ogg,\n");
		else
			file_text_write_string(ref_audios, "_" + string(i) + "_ogg\n");
	}
	file_text_write_string(ref_audios, "};\n\n");
	
	file_text_write_string(ref_audios, "const size_t ogg_refs_size[] = {\n");
	for (var i = 0; i  < currentsound_count; i++) {
		if (i < currentsound_count -1)
			file_text_write_string(ref_audios, "_" + string(i) + "_ogg_size,\n");
		else
			file_text_write_string(ref_audios, "_" + string(i) + "_ogg_size\n");
	}
	file_text_write_string(ref_audios, "};\n\n");
	
	file_text_close(ref_audios);
}