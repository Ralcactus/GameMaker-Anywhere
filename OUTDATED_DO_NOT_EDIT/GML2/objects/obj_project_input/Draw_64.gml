if (!active)
	image_blend = c_gray;
else
	image_blend = c_white;

draw_sprite_ext(sprite_index, image_index, x, y, image_xscale, image_yscale, image_angle, image_blend, image_alpha);

draw_set_halign(fa_left);
draw_set_valign(fa_top);
draw_set_color(c_white);

draw_text(x+5, y-20, "Project Path");

if (current_time % 1000 < 500 && active)
    draw_text(x+5, y+5, global.selected_yyp + "|");
else
    draw_text(x+5, y+5, global.selected_yyp);

if (!file_selecthover)
	draw_sprite(spr_select_file, 0, x+sprite_width+10, y);
else
	draw_sprite_ext(spr_select_file, 0, x+sprite_width+10, y, 1, 1, 0, c_gray, 1);
