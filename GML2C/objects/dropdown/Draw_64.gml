draw_set_halign(fa_left)
var su = draw.su,
	dw = draw.dw,
	dh = draw.dh;
	
if (!surface_exists(su)) {
	su = surface_create(dw, dh);
	draw.su = su;
	draw.re = true;
}

if (draw.re) {

	surface_set_target(su);
	draw_clear_alpha(0, 0);
	
	draw_set_color(draw.bc);
	draw_rectangle(0, 0, dw, dh, false);
	
	var eh = draw.eh,
		oy = draw.oy,
		tc = draw.tc,
		hc = draw.hc,
		pa = draw.pa,
		cu = curt.cu,
		a = curt.li,
		i = 0,
		l = curt.le;
	
	draw_set_font(draw.ft);
	draw_set_color(tc);
	
	repeat (l) {
		
		var dy = eh * i - oy;
		if (i == cu) {
			draw_set_color(hc);
			draw_rectangle(0, dy, dw, dy + eh, false);
			draw_set_color(tc);
		}
		
		draw_text(pa, dy + eh / 2, a[i]);
		i ++;
		
	}
	
	var th = l * eh;
	if (th > dh) {
		var dy = oy / th * dh;
		draw_set_alpha(0.2);
		draw_rectangle(dw - 4, dy, dw, dy + dh / th * dh, false);
		draw_set_alpha(1);
	}
	
	var bo = draw.bo;
	if (bo > -1) {
		draw_set_color(bo);
		draw_rectangle(1, 1, dw - 2, dh - 2, true);
	}
	
	surface_reset_target();
	draw.re = false;
	
}

draw_surface(su, draw.sx, draw.sy);