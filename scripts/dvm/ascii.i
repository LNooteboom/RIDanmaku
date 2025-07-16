import "dvm";

void ascii1Char(int event) {
	switch (event) {
	case EVENT_CREATE:
	{
		texture(0, "tex/ui/Ascii1x.png");
		int x, y;
		if (I0 >= 32 && I0 < 128) {
			int idx = I0 - 32;
			x = idx % 32 * 8;
			y = idx / 32 * 8;
		}
		srcRect(x, y, 8, 8);
		I0 = I1;
		stop();
	}
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		getParentVars(PV_I1);
		if (I0 != I1) {
			noAsync();
			setDelete();
		}
	}
}

void ascii1(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(47);
		mode(MODE_NONE);
		I1 = 0;
	build_children:
		F0 = 0;
		F1 = 0;
		while (I0 = strChar(I1, STR0)) {
			if (I0 != 10) {
				childNew("ascii1Char", F0, F1, 0);
				F0 += 7;
			} else {
				F1 += 10;
				F0 = 0;
			}
		}
		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		noAsync();
		goto build_children;
	}
}

void ascii2Char(int event) {
	switch (event) {
	case EVENT_CREATE:
	{
		texture(0, "tex/ui/Ascii2x.png");
		scale(0.5, 0.5, 1);
		int x, y;
		if (I0 >= 32 && I0 < 128) {
			int idx = I0 - 32;
			x = idx % 16 * 32;
			y = idx / 16 * 32 + 48;
		}
		srcRect(x, y, 32, 32);
		I0 = I1;
		stop();
	}
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		getParentVars(PV_I1);
		if (I0 != I1) {
			noAsync();
			setDelete();
		}
	}
}

void ascii2(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(47);
		mode(MODE_NONE);
		flagSet(FLAG_COLOR_CHILDREN);
		I1 = 0;
	build_children:
		F0 = 0;
		F1 = 0;
		while (I0 = strChar(I1, STR0)) {
			if (I0 != 10) {
				childNew("ascii2Char", F0, F1, 0);
				F0 += 12;
			} else {
				F1 += 10;
				F0 = 0;
			}
		}
		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		noAsync();
		goto build_children;
	case 3:
		colorHex(0xFFFF00);
		break;
	}
}

void leaderboardText(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(47);
		mode(MODE_NONE);
		flagSet(FLAG_COLOR_CHILDREN);
		scale(0.9, 0.9, 0.9);
		I1 = 0;
	build_children:
		F0 = 0;
		F1 = 0;
		while (I0 = strChar(I1, STR0)) {
			if (I0 != 10) {
				childNew("ascii2Char", F0, F1, 0);
				F0 += 10;
			} else {
				F1 += 10;
				F0 = 0;
			}
		}
		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		noAsync();
		goto build_children;
	case 3:
		colorHex(0xFFFF00);
		break;
	}
}

void keyboardChar(int event) {
	switch (event) {
	case EVENT_CREATE:
	{
		texture(0, "tex/ui/Ascii2x.png");
		scale(0.5, 0.5, 1);

		if (I0 > 32 && I0 < 128) {
			int idx = I0 - 32;
			int x = idx % 16 * 32;
			int y = idx / 16 * 32 + 48;
			srcRect(x, y, 32, 32);
		} else if (I0 == 32) {
			srcRect(32, 304, 32, 32);
		} else {
			srcRect(0, 304, 32, 32);
		}
		
		break;
	}
	case EVENT_DELETE:
		noAsync();
		setDelete();
	case 2:
		colorHex(0xFFFF00);
		loop(5) {
			pos(RF2 * 3, RF2 * 3, 0);
			wait(1);
		}
		pos(0, 0, 0);
		break;
	case 3:
		colorHex(0xFFFFFF);
		break;
	}
}