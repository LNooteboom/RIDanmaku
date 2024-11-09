import "menu";

const int PAUSE_FADE_TIME = 15;
inline void pauseMenuBtn(int event, float srcX, float srcY, float srcW, int y, float startX) {
	const float py = 128 + y * 48;
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PAUSE_2);
		texture(0, "tex/ui/Pause.png");
		srcRect(srcX, srcY, srcW, 128);
		scaleMenu();
		anchor(1, 1);
		pos(startX, py, 0);
		posTime(0, PAUSE_FADE_TIME, EASEOUT2, -128, py, 0);
		color(0.5, 0.5, 0.5);
		I0 = 0;
		alpha(0);
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 1);
		stop();
	case EVENT_CHOOSE:
		wait(10);
	case EVENT_DELETE:
		posTime(0, PAUSE_FADE_TIME, EASEIN2, startX, py, 0);
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 0);
		wait(PAUSE_FADE_TIME);
		setDelete();
	case EVENT_SELECT:
		color(1, 1, 1);
		I0 = 1;
		break;
	case EVENT_DESELECT:
		color(0.5, 0.5, 0.5);
		I0 = 0;
		break;
	case EVENT_FADE:
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 0.5);
		colorTime(2, PAUSE_FADE_TIME, LINEAR, 0.5, 0.5, 0.5);
		break;
	case EVENT_UNFADE:
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 1);
		if (I0)
			colorTime(2, PAUSE_FADE_TIME, LINEAR, 1, 1, 1);
		break;
	}
}
void pauseMenuContinue(int event) {
	pauseMenuBtn(event, 0, 128, 1024, 1, 100);
}
void pauseMenuQuit(int event) {
	pauseMenuBtn(event, 0, 256, 640, 2, -100);
}
void pauseMenuReload(int event) {
	pauseMenuBtn(event, 0, 384, 640, 3, 100);
}
void pauseMenuTitle(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PAUSE_2);
		texture(0, "tex/ui/Pause.png");
		srcRect(0, 0, 384, 128);
		scaleMenu();
		anchor(0, 1);
		pos(-256, 128, 0);
		posTime(0, PAUSE_FADE_TIME, EASEOUT2, 0, 128, 0);
		alpha(0);
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 1);
		stop();
	case EVENT_DELETE:
		posTime(0, PAUSE_FADE_TIME, EASEIN2, -256, 128, 0);
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 0);
		wait(PAUSE_FADE_TIME);
		setDelete();
	case EVENT_FADE:
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 0.5);
		colorTime(2, PAUSE_FADE_TIME, LINEAR, 0.5, 0.5, 0.5);
		break;
	case EVENT_UNFADE:
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 1);
		colorTime(2, PAUSE_FADE_TIME, LINEAR, 1, 1, 1);
		break;
	}
}

void pauseMenuBackground(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PAUSE_1);
		texture(0, "tex/White.png");
		srcRect(0, 0, 854, 480);
		pos(0, 240, 0);
		colorMode(COLOR_UD);
		color(1, 1, 1);
		color2(0.4, 0.4, 0.4);
		alpha(0);
		alpha2(0);
		alphaTime(0, PAUSE_FADE_TIME, LINEAR, 0.4);
		alpha2Time(1, PAUSE_FADE_TIME, LINEAR, 0.4);
		stop();
	case EVENT_DELETE:
		alphaTime(0, PAUSE_FADE_TIME, LINEAR, 0);
		wait(PAUSE_FADE_TIME);
		setDelete();
	}
}

void pauseMenuConfirm(int event, float y) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PAUSE_2);
		texture(0, "tex/ui/Pause.png");
		srcRect(0, 512, 384, 128);
		scaleMenu();
		anchor(1, 1);
		pos(-256, y, 0);
		posTime(0, PAUSE_FADE_TIME, EASEOUT2, -96, y, 0);
		alpha(0);
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 1);
		stop();
	case EVENT_DELETE:
		posTime(0, PAUSE_FADE_TIME, EASEOUT2, 550, y, 0);
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 0);
		wait(PAUSE_FADE_TIME);
		setDelete();
	}
}

void pauseMenuYes(int event, float y) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PAUSE_2);
		texture(0, "tex/ui/Pause.png");
		srcRect(384, 512, 192, 128);
		scaleMenu();
		anchor(1, 1);
		pos(550, y, 0);
		posTime(0, PAUSE_FADE_TIME, EASEOUT2, 32, y, 0);
		color(0.5, 0.5, 0.5);
		alpha(0);
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 1);
		stop();
	case EVENT_DELETE:
	case EVENT_CHOOSE:
		posTime(0, PAUSE_FADE_TIME, EASEOUT2, 550, y, 0);
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 0);
		wait(PAUSE_FADE_TIME);
		setDelete();
	case EVENT_SELECT:
		color(1, 1, 1);
		break;
	case EVENT_DESELECT:
		color(0.5, 0.5, 0.5);
		break;
	}
}
void pauseMenuNo(int event, float y) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PAUSE_2);
		texture(0, "tex/ui/Pause.png");
		srcRect(512 + 64, 512, 256, 128);
		scaleMenu();
		anchor(1, 1);
		pos(550, y, 0);
		posTime(0, PAUSE_FADE_TIME, EASEOUT2, 96, y, 0);
		color(0.5, 0.5, 0.5);
		alpha(0);
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 1);
		stop();
	case EVENT_DELETE:
	case EVENT_CHOOSE:
		posTime(0, PAUSE_FADE_TIME, EASEOUT2, 550, y, 0);
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 0);
		wait(PAUSE_FADE_TIME);
		setDelete();
	case EVENT_SELECT:
		color(1, 1, 1);
		break;
	case EVENT_DESELECT:
		color(0.5, 0.5, 0.5);
		break;
	}
}

void pauseMenuQuitConfirm(int event) { pauseMenuConfirm(event, 240); }
void pauseMenuQuitYes(int event) { pauseMenuYes(event, 240); }
void pauseMenuQuitNo(int event) { pauseMenuNo(event, 240); }

void pauseMenuRetryConfirm(int event) { pauseMenuConfirm(event, 288); }
void pauseMenuRetryYes(int event) { pauseMenuYes(event, 288); }
void pauseMenuRetryNo(int event) { pauseMenuNo(event, 288); }

void endMenuTitle(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PAUSE_2);
		texture(0, "tex/ui/Pause.png");
		srcRect(384, 0, 448, 128);
		scaleMenu();
		anchor(0, 1);
		pos(-256, 128, 0);
		posTime(0, PAUSE_FADE_TIME, EASEOUT2, 0, 128, 0);
		alpha(0);
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 1);
		stop();
	case EVENT_DELETE:
		posTime(0, PAUSE_FADE_TIME, EASEIN2, -256, 128, 0);
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 0);
		wait(PAUSE_FADE_TIME);
		setDelete();
	case EVENT_FADE:
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 0.5);
		colorTime(2, PAUSE_FADE_TIME, LINEAR, 0.5, 0.5, 0.5);
		break;
	case EVENT_UNFADE:
		alphaTime(1, PAUSE_FADE_TIME, LINEAR, 1);
		colorTime(2, PAUSE_FADE_TIME, LINEAR, 1, 1, 1);
		break;
	}
}

void endMenuContinueAto(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PAUSE_2);
		texture(0, "tex/ui/Pause.png");
		srcRect(448, 640, 192, 128);
		anchor(1, 1);

		stop();
	case EVENT_DELETE:
		wait(PAUSE_FADE_TIME);
		setDelete();
	}
}

void endMenuContinueNumber(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PAUSE_2);
		texture(0, "tex/ui/Pause.png");
		switch (I0) {
		default:
		case 1: srcRect(772, 640, 128, 128); break;
		case 2: srcRect(900, 640, 128, 128); break;
		case 3: srcRect(772, 768, 128, 128); break;
		case 4: srcRect(900, 768, 128, 128); break;
		case 5: srcRect(900, 640 - 128, 128, 128); break;
		}
		
		anchor(1, 1);

		stop();
	case EVENT_DELETE:
		wait(PAUSE_FADE_TIME);
		setDelete();
	}
}

void endMenuContinueKai(int event) {
	switch (event) {
	case EVENT_CREATE:
		layer(LAYER_PAUSE_2);
		texture(0, "tex/ui/Pause.png");
		srcRect(640, 640, 128, 128);
		anchor(1, 1);

		stop();
	case EVENT_DELETE:
		wait(PAUSE_FADE_TIME);
		setDelete();
	}
}

void endMenuContinue(int event) {
	if (!event) {
		flagSet(FLAG_ALPHA_CHILDREN | FLAG_COLOR_CHILDREN);
		if (I0) {
			childNew("endMenuContinueAto", 448, 0, 0);
			childNew("endMenuContinueNumber", 448 + 160, 0, 0);
			childNew("endMenuContinueKai", 448 + 256, 0, 0);
		}
	}
	pauseMenuBtn(event, 0, 640, 448, 1, 100);
}