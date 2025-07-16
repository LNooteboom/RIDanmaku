import "dvm";
import "ascii";
import "menu_main";
import "menu_pause";
import "menu_settings";
import "menu_score";

const int EVENT_SELECT = 2;
const int EVENT_DESELECT = 3;
const int EVENT_CHOOSE = 4;
const int EVENT_FADE = 5;
const int EVENT_UNFADE = 6;

inline void colorSelected() {
	color(1, 1.2, 1);
	color2(1, -0.2, 1);
}
inline void colorUnselected() {
	color(1.7, 1.7, 1.7);
	color2(0, 0, 0);
}

inline void scaleMenu() {
	scale(1.0 / 3, 1.0 / 3, 1);
}

void scriptsMenuItem(int event) {
	if (!event) {
		pos(-256, 192 + I1 * 16, 0);
		//scale(0.5, 0.5, 1);
		if (I0)
			colorHex(0xFFFF00);
		FLAGS |= FLAG_COLOR_CHILDREN;
	}
	ascii2(event);
}

