import "menu";

void settingsMenu0Suffix(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/ui/Buttons.png");
		srcRect(512, 128, 64, 64);
		anchor(1, 1);
		colorMode(COLOR_UD);
		colorUnselected();
		stop();
	case EVENT_DELETE:
		setDelete();
	case EVENT_SELECT:
		colorSelected();
		break;
	case EVENT_DESELECT:
		colorUnselected();
		break;
	}
}
void settingsMenuPSuffix(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/ui/Buttons.png");
		srcRect(960, 192, 64, 64);
		anchor(1, 1);
		colorMode(COLOR_UD);
		colorUnselected();
		stop();
	case EVENT_DELETE:
		setDelete();
	case EVENT_SELECT:
		colorSelected();
		break;
	case EVENT_DESELECT:
		colorUnselected();
		break;
	}
}
void settingsMenuValue(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/ui/Buttons.png");
		FLAGS |= FLAG_NO_CHILD_EVENT;
		anchor(1, 1);

		if (I0 == 10) {
			srcRect(576, 128, 64, 64);
			childNew("settingsMenu0Suffix", 24, 0, 0);
			childNew("settingsMenu0Suffix", 48, 0, 0);
			childNew("settingsMenuPSuffix", 72, 0, 0);
		} else {
			if (I0 >= 7)
				srcRect(832 + (I0 - 7) * 64, 0, 64, 64);
			else
				srcRect(512 + I0 * 64, 128, 64, 64);
			childNew("settingsMenu0Suffix", 24, 0, 0);
			childNew("settingsMenuPSuffix", 48, 0, 0);
		}

		colorMode(COLOR_UD);
		colorUnselected();
		stop();
	case EVENT_DELETE:
		childEvent(EVENT_DELETE);
		setDelete();
	case EVENT_SELECT:
		colorSelected();
		childEvent(EVENT_SELECT);
		break;
	case EVENT_DESELECT:
		colorUnselected();
		childEvent(EVENT_DESELECT);
		break;
	}

}
void settingsMenuResolutionNumber(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/ui/Buttons.png");
		if (I2 >= 7)
			srcRect(832 + (I2 - 7) * 64, 0, 64, 64);
		else
			srcRect(512 + I2 * 64, 128, 64, 64);

		colorMode(COLOR_UD);
		colorUnselected();
		anchor(1, 1);
		stop();
	case EVENT_DELETE:
		setDelete();
	case EVENT_SELECT:
		colorSelected();
		break;
	case EVENT_DESELECT:
		colorUnselected();
		break;
	}
}
void settingsMenuResolutionX(int event) {
	switch (event) {
	case EVENT_CREATE:
		texture(0, "tex/ui/Buttons.png");
		srcRect(960, 128, 64, 64);

		colorMode(COLOR_UD);
		colorUnselected();
		anchor(1, 1);
		stop();
	case EVENT_DELETE:
		setDelete();
	case EVENT_SELECT:
		colorSelected();
		break;
	case EVENT_DESELECT:
		colorUnselected();
		break;
	}
}
void settingsMenuResolutionValue(int event) {
	switch (event) {
	case EVENT_CREATE:
		FLAGS |= FLAG_NO_CHILD_EVENT;

		{
			string fn = "settingsMenuResolutionNumber";
			float x;
			if (I0 >= 1000) {
				I2 = (I0 / 1000) % 10;
				childNew(fn, 0, 0, 0);
				I2 = (I0 / 100) % 10;
				childNew(fn, 24, 0, 0);
				I2 = (I0 / 10) % 10;
				childNew(fn, 48, 0, 0);
				I2 = I0 % 10;
				childNew(fn, 72, 0, 0);
				x = 96;
			} else {
				I2 = (I0 / 100) % 10;
				childNew(fn, 0, 0, 0);
				I2 = (I0 / 10) % 10;
				childNew(fn, 24, 0, 0);
				I2 = I0 % 10;
				childNew(fn, 48, 0, 0);
				x = 72;
			}

			childNew("settingsMenuResolutionX", x, 0, 0);

			if (I1 >= 1000) {
				I2 = (I1 / 1000) % 10;
				childNew(fn, x + 24, 0, 0);
				I2 = (I1 / 100) % 10;
				childNew(fn, x + 48, 0, 0);
				I2 = (I1 / 10) % 10;
				childNew(fn, x + 72, 0, 0);
				I2 = I1 % 10;
				childNew(fn, x + 96, 0, 0);
			} else {
				I2 = (I1 / 100) % 10;
				childNew(fn, x + 24, 0, 0);
				I2 = (I1 / 10) % 10;
				childNew(fn, x + 48, 0, 0);
				I2 = I1 % 10;
				childNew(fn, x + 72, 0, 0);
			}
		}
		stop();
	case EVENT_DELETE:
		childEvent(EVENT_DELETE);
		setDelete();
	case EVENT_SELECT:
		childEvent(EVENT_SELECT);
		break;
	case EVENT_DESELECT:
		childEvent(EVENT_DESELECT);
		break;
	}
}

const float SETTINGS_X = 96;
const float SETTINGS_Y = 320;
const float SIXTH = 1.0 / 6;
void settingsMenuBGM(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_MENU_1);
		FLAGS |= FLAG_NO_CHILD_EVENT;
		texture(0, "tex/ui/Buttons.png");
		srcRect(576, 256, 128, 64);
		scaleMenu();
		pos(SETTINGS_X, 576, 0);
		anchor(1, 1);

		childNew("settingsMenuValue", 224, 0, 0);

		colorMode(COLOR_UD);
		colorUnselected();

		wait(20);
		posTime(0, 20, EASEOUT1, SETTINGS_X, SETTINGS_Y, 0);
		wait(20);
		stop();
	case EVENT_DELETE:
		noAsync();
		posTime(0, 20, EASEOUT1, SETTINGS_X, 576, 0);
		wait(20);
		childEvent(EVENT_DELETE);
		setDelete();
	case EVENT_SELECT:
		colorSelected();
		childEvent(EVENT_SELECT);
		break;
	case EVENT_DESELECT:
		colorUnselected();
		childEvent(EVENT_DESELECT);
		break;
	case 4:
		/* Value changed */
		childEvent(EVENT_DELETE);
		childNew("settingsMenuValue", 224, 0, 0);
		childEvent(EVENT_SELECT);
		break;
	}
}
void settingsMenuSFX(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_MENU_1);
		FLAGS |= FLAG_NO_CHILD_EVENT;
		texture(0, "tex/ui/Buttons.png");
		srcRect(704, 256, 128, 64);
		scaleMenu();
		pos(SETTINGS_X, 576, 0);
		anchor(1, 1);

		childNew("settingsMenuValue", 224, 0, 0);

		colorMode(COLOR_UD);
		colorUnselected();

		wait(20);
		posTime(0, 20, EASEOUT1, SETTINGS_X, SETTINGS_Y + 32, 0);
		wait(20);
		stop();
	case EVENT_DELETE:
		noAsync();
		posTime(0, 20, EASEOUT1, SETTINGS_X, 576, 0);
		wait(20);
		childEvent(EVENT_DELETE);
		setDelete();
	case EVENT_SELECT:
		colorSelected();
		childEvent(EVENT_SELECT);
		break;
	case EVENT_DESELECT:
		colorUnselected();
		childEvent(EVENT_DESELECT);
		break;
	case 4:
		/* Value changed */
		childEvent(EVENT_DELETE);
		childNew("settingsMenuValue", 224, 0, 0);
		childEvent(EVENT_SELECT);
		break;
	}
}
void settingsMenuResolution(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_MENU_1);
		FLAGS |= FLAG_NO_CHILD_EVENT;
		texture(0, "tex/ui/Buttons.png");
		srcRect(0, 192, 256, 64);
		scaleMenu();
		pos(SETTINGS_X, 576, 0);
		anchor(1, 1);

		childNew("settingsMenuResolutionValue", 224, 0, 0);

		colorMode(COLOR_UD);
		colorUnselected();

		wait(20);
		posTime(0, 20, EASEOUT1, SETTINGS_X, SETTINGS_Y + 64, 0);
		wait(20);
		stop();
	case EVENT_DELETE:
		noAsync();
		posTime(0, 20, EASEOUT1, SETTINGS_X, 576, 0);
		wait(20);
		childEvent(EVENT_DELETE);
		setDelete();
	case EVENT_SELECT:
		colorSelected();
		childEvent(EVENT_SELECT);
		break;
	case EVENT_DESELECT:
		colorUnselected();
		childEvent(EVENT_DESELECT);
		break;
	case 4:
		/* Value changed */
		childEvent(EVENT_DELETE);
		childNew("settingsMenuResolutionValue", 224, 0, 0);
		childEvent(EVENT_SELECT);
		break;
	}
}
void settingsMenuQuit(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_MENU_1);
		FLAGS |= FLAG_NO_CHILD_EVENT;
		texture(0, "tex/ui/Buttons.png");
		srcRect(384, 128, 128, 64);
		scaleMenu();
		pos(SETTINGS_X, 576, 0);
		anchor(1, 1);

		//childNew("settingsMenuValue", 192, 0, 0);

		colorMode(COLOR_UD);
		colorUnselected();

		wait(20);
		posTime(0, 20, EASEOUT1, SETTINGS_X, SETTINGS_Y + 96, 0);
		wait(20);
		stop();
	case EVENT_DELETE:
		noAsync();
		posTime(0, 20, EASEOUT1, SETTINGS_X, 576, 0);
		wait(20);
		childEvent(EVENT_DELETE);
		setDelete();
	case EVENT_SELECT:
		colorSelected();
		childEvent(EVENT_SELECT);
		break;
	case EVENT_DESELECT:
		colorUnselected();
		childEvent(EVENT_DESELECT);
		break;
	}
}