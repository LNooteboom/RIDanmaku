import "menu";
import "ascii";

void mainMenuBtn(int event, float dstX, float dstY, int wa) {
	switch (event) {
	case EVENT_CREATE:
		I0 = 0;
		layer(LAYER_MENU_1);

		pos(430, dstY, 0);
		scaleMenu();
		anchor(1, 1);
		alpha(0);
		colorMode(COLOR_UD);
		colorUnselected();

		wait(wa);
		posTime(0, 40, EASEOUT2, dstX, dstY, 0);
		alphaTime(1, 40, LINEAR, 1);

		wait(40);
		I0 = 1;
		stop();
	case EVENT_DELETE:
		noAsync();
		posTime(0, 30, EASEIN1, 700, dstY, 0);
		wait(30);
		setDelete();
	case EVENT_SELECT:
		colorSelected();
		if (I0) {
			int dx = dstX - 2;
			int dy = dstY - 2;
			loop (4) {
				pos(dx + RF2 * 3, dy + RF2 * 3, 0);
				wait(1);
			}
			pos(dx, dy, 0);
		}
		break;
	case EVENT_DESELECT:
		colorUnselected();
		if (I0)
			pos(dstX, dstY, 0);
		break;
	case EVENT_CHOOSE:
		noAsync();
		loop (10) {
			pos(dstX - 2 + RF2 * 3, dstY - 2 + RF2 * 3, 0);
			wait(1);
		}
		//wait(10);
		posTime(0, 30, EASEIN1, 700, dstY, 0);
		wait(60);
		setDelete();
	}
}

void mainMenuGameStart(int event) {
	if (!event) {
		texture(0, "tex/ui/Buttons.png");
		srcRect(0, 0, 256, 64);
	}
	mainMenuBtn(event, 100, 280, 0);
	//mainMenuBtn(event, 70, 400, 24);
}
void mainMenuExtraStart(int event) {
	if (!event) {
		texture(0, "tex/ui/Buttons.png");
		srcRect(256, 0, 256, 64);
	}
	mainMenuBtn(event, 95, 300, 4);
}
void mainMenuPracticeStart(int event) {
	if (!event) {
		texture(0, "tex/ui/Buttons.png");
		srcRect(512, 0, 320, 64);
	}
	mainMenuBtn(event, 90, 320, 8);
}
void mainMenuSpellPractice(int event) {
	if (!event) {
		texture(0, "tex/ui/Buttons.png");
		srcRect(0, 64, 320, 64);
	}
	mainMenuBtn(event, 85, 340, 12);
}
void mainMenuReplay(int event) {
	if (!event) {
		texture(0, "tex/ui/Buttons.png");
		srcRect(320, 64, 192, 64);
	}
	mainMenuBtn(event, 80, 360, 16);
}
void mainMenuPlayerData(int event) {
	if (!event) {
		texture(0, "tex/ui/Buttons.png");
		srcRect(512, 64, 256, 64);
	}
	mainMenuBtn(event, 75, 380, 20);
}
void mainMenuMusicRoom(int event) {
	if (!event) {
		texture(0, "tex/ui/Buttons.png");
		srcRect(768, 64, 256, 64);
	}
	mainMenuBtn(event, 70, 400, 24);
}
void mainMenuSettings(int event) {
	if (!event) {
		texture(0, "tex/ui/Buttons.png");
		srcRect(0, 128, 192, 64);
	}
	mainMenuBtn(event, 65, 420, 28);
}
void mainMenuQuit(int event) {
	if (!event) {
		texture(0, "tex/ui/Buttons.png");
		srcRect(384, 128, 128, 64);
	}
	mainMenuBtn(event, 60, 440, 32);
}

void logo(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_MENU_1);
		texture(0, "tex/ui/Title.png");
		srcRect(0, 0, 2048, 512);
		scaleMenu();
		pos(0, -128, 0);
		posTime(0, 40, EASEOUT1, 0, 60, 0);
		stop();
	case EVENT_DELETE:
		noAsync();
		setDelete();
	}
}

void mainMenuBackground(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_UI_BG);
		texture(0, "tex/ui/TitleBHA8.png");
		srcRect(0, 0, 2560, 1440);
		scaleMenu();
		anchor(0, 1);
		stop();
	case EVENT_DELETE:
		setDelete();
	}
}


inline void difficultyMenuItem(int event, string txt, float y) {
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
void difficultyMenuEasy(int event) {
	difficultyMenuItem(event, "Easy", 128);
}
void difficultyMenuNormal(int event) {
	difficultyMenuItem(event, "Normal", 192);
}
void difficultyMenuHard(int event) {
	difficultyMenuItem(event, "Hard", 256);
}
void difficultyMenuLunatic(int event) {
	difficultyMenuItem(event, "Lunatic", 320);
}

void shotMenuPl00a(int event) {
	difficultyMenuItem(event, "Rin Satsuki (Flower)", 256);
}
void shotMenuPl00b(int event) {
	difficultyMenuItem(event, "Rin Satsuki (Wind)", 288);
}
