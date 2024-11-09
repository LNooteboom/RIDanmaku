import "menu";
import "ascii";

inline void asciiMenuItem(int event, string txt, float y) {
	switch (event) {
	case EVENT_CREATE:
		STR0 = txt;
		pos(0, y, 0);
		FLAGS |= FLAG_COLOR_CHILDREN | FLAG_NO_CHILD_EVENT;
		//colorHex(0xFFFF00);
		ascii2(event);

		break;
	case EVENT_DELETE:
		ascii2(event);
		childEvent(EVENT_DELETE);
		break;
	case EVENT_SELECT:
		colorHex(0xFFFF00);
		break;
	case EVENT_DESELECT:
		colorHex(0xFFFFFF);
		break;
	case EVENT_CHOOSE:
		ascii2(1);
		childEvent(1);
		break;
	}
}

