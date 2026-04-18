var mx = mouse_x,
	my = mouse_y,
	sx = draw.sx,
	sy = draw.sy,
	dw = draw.dw,
	dh = draw.dh,
	vi = point_in_rectangle(mx, my, sx, sy, sx + dw, sy + dh);

if (mouse_check_button_pressed(mb_left)) {
	curt.fc = vi;
	alarm[vi] = 1;
	return;
}

#region with mouse

	if (vi) {
		
		// Update current.
		if (abs(draw.my - my) > 3) {
			var eh = draw.eh;
			curt.cu = floor((my - sy + draw.oy) / eh);
			draw.my = my;
			draw.re = true;
			return;
		}
		
		// Update offset.
		var mc = mouse_wheel_up() - mouse_wheel_down();
		if (mc != 0) {
			var eh = draw.eh,
				oy = draw.oy - eh * mc,
				mh = curt.le * eh - dh;
			if (mh < 0) oy = 0;
			else oy = clamp(oy, 0, mh);
			draw.oy = oy;
			draw.re = true;
			return;
		}
		
	}

#endregion

if (!curt.fc) return;

#region return index

	if (keyboard_check_pressed(vk_enter)) {
		alarm[1] = 1;
		return;
	}
	
	if (keyboard_check_pressed(vk_escape)) {
		alarm[0] = 1;
		return;
	}

#endregion

#region update current

	var vc = keyboard_check_pressed(vk_down) - keyboard_check_pressed(vk_up);
	if (vc != 0) {
		curt.br = 40;
		dropdown_cinput(vc);
		return;
	}

	var pv = keyboard_check(vk_down) - keyboard_check(vk_up);
	if (pv != 0) {
		curt.br --;
		if (curt.br < 0) {
			dropdown_cinput(pv);
			curt.br = 10;
		}
		return;
	}

#endregion