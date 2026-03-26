draw_self();
draw_set_halign(fa_left);
draw_set_valign(fa_bottom);
draw_set_colour(c_white);
gpu_set_scissor(x, y+10, sprite_width, sprite_height);
draw_text(30, texty, ExecutedProcessReadFromStandardOutput(obj_compiler.compile_powershell_window));
