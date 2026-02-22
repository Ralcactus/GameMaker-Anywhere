draw_set_font(fnt_example);
if (!pause) p = "pause"; else p = "resume";
if (!ProcIdExists(procId) || exe == "") e = "<undefined>"; else e = exe;
if (!ProcIdExists(procId) || cwd == "") d = "<undefined>"; else d = cwd;
if (!ProcIdExists(procId) || cmd == "") c = "<undefined>"; else c = cmd;
str = "Press 'p' key to " + p + " enumeration, or press 'e' key to inspect \n" +
"the environment variables belonging to the process identifier." +
" \n\npid=" + string(procId) + " \nppid=" + string(ppid) +
" \nexe=" + e + " \ncwd=" + d + " \ncmd=" + c;
draw_set_color(c_ltgrey);
draw_set_halign(fa_center);
draw_set_valign(fa_middle);
draw_text(x, y, str);