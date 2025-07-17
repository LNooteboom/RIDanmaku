#include "menu.h"
#include <gfx/drawvm.h>
#include <events.h>
#include <string.h>
#include "redefined.h"
#include "keymap.h"
#include <audio.h>

void menuDvmEventAll(struct MenuController *m, int selEvent, int unselEvent) {
	for (int i = 0; i < m->nButtons; i++) {
		int ev = i == m->selected ? selEvent : unselEvent;
		if (ev == MENU_EVENT_DELETE)
			drawVmDelete(m->buttons[i]);
		else
			drawVmEvent2(m->buttons[i], ev);
	}
}

static void menuChoose(struct MenuController *m) {
	enum MenuState state = m->state;
	switch (state) {
	case MENU_MAIN:
		mainMenuChoose(m);
		break;
	case MENU_SETTINGS:
		settingsMenuChoose(m);
		break;
	case MENU_DIFFICULTY:
		difficultyMenuChoose(m);
		break;
	case MENU_SHOTTYPE:
		shotMenuChoose(m);
		break;
	case MENU_NONE:
	case MENU_INGAME:
		break;
	case MENU_PAUSE:
	case MENU_PAUSE_QUIT:
	case MENU_PAUSE_RETRY:
	case MENU_PAUSE_REPLAY_SAVE_ASK:
		pauseMenuChoose(m);
		break;
	case MENU_END:
	case MENU_END_QUIT:
	case MENU_END_RETRY:
		endMenuChoose(m);
		break;
	case MENU_REPLAY_LOAD:
		replayLoadMenuChoose(m);
		break;
	case MENU_PAUSE_REPLAY_SAVE:
		replaySaveMenuChoose(m);
		break;
	}
}
static void menuCancel(struct MenuController *m) {
	switch (m->state) {
	case MENU_MAIN:
		menuSelect(m, 2);
		break;
	case MENU_DIFFICULTY:
		difficultyMenuEnd(m);
		break;
	case MENU_SHOTTYPE:
		shotMenuEnd(m);
		break;
	case MENU_PAUSE:
		pauseMenuEnd(m);
		break;
	case MENU_PAUSE_QUIT:
	case MENU_PAUSE_RETRY:
	case MENU_PAUSE_REPLAY_SAVE_ASK:
		m->selected = 4;
		pauseMenuChoose(m);
		break;
	case MENU_END_QUIT:
	case MENU_END_RETRY:
		m->selected = 4;
		endMenuChoose(m);
		break;
	case MENU_REPLAY_LOAD:
		replayLoadMenuEnd(m);
		break;
	case MENU_PAUSE_REPLAY_SAVE:
		replaySaveMenuEnd(m);
		break;
	default:
		break;
	}
}

void menuSelect(struct MenuController *m, int sel) {
	drawVmEvent(getComponent(DRAW_VM, m->buttons[m->selected]), MENU_EVENT_UNSELECT);
	drawVmEvent(getComponent(DRAW_VM, m->buttons[sel]), MENU_EVENT_SELECT);
	m->selected = sel;
}

static void menuKey(struct MenuController *m, struct Event *ev) {
	if (!m->state || !ev->param2)
		return;

	if (ev->param == SDLK_e && m->state == MENU_MAIN) {
		menuDvmEventAll(m, MENU_EVENT_DELETE, MENU_EVENT_DELETE);
		scriptsMenuStart(m);
		return;
	}

	enum Keys map[] = {
		K_UP,
		K_DOWN,
		K_MENU_SELECT,
		K_MENU_CANCEL,
		K_MENU,
		K_LEFT,
		K_RIGHT,
	};
	int match = keyMatch(map, 7, ev);
	if (m->state == MENU_SCRIPTS) {
		scriptsMenuKey(m, match);
		return;
	}
	if (m->state == MENU_END_SCORE) {
		menuEndScoreKey(m, match);
		return;
	}
	switch (match) {
	case 0:
		if (m->state > MENU_INGAME) {
			menuSelect(m, m->selected <= m->selectMin ? m->selectMax : m->selected - 1);
			audioPlaySFX(60, 64, 0);
		}
		break;
	case 1:
		if (m->state > MENU_INGAME) {
			menuSelect(m, m->selected == m->selectMax ? m->selectMin : m->selected + 1);
			audioPlaySFX(60, 64, 0);
		}
		break;
	case 2:
		if (m->state > MENU_INGAME) {
			menuChoose(m);
			audioPlaySFX(61, 64, 0);
		}
		break;
	case 3:
		if (m->state > MENU_INGAME) {
			menuCancel(m);
			audioPlaySFX(62, 64, 0);
		}
		break;
	case 4:
		if (m->state == MENU_INGAME) {
			pauseMenuStart(m);
			audioPlaySFX(63, 64, 0);
		} else if (m->state) {
			menuCancel(m);
			audioPlaySFX(62, 64, 0);
		}
		break;
	case 5:
		if (m->state == MENU_SETTINGS) {
			settingsMenuValueChange(m, -1);
			audioPlaySFX(60, 64, 0);
		} else if (m->state > MENU_INGAME && m->leftRight) {
			menuSelect(m, m->selected <= m->selectMin ? m->selectMax : m->selected - 1);
			audioPlaySFX(60, 64, 0);
		}
		break;
	case 6:
		if (m->state == MENU_SETTINGS) {
			settingsMenuValueChange(m, 1);
			audioPlaySFX(60, 64, 0);
		} else if (m->state > MENU_INGAME && m->leftRight) {
			menuSelect(m, m->selected == m->selectMax ? m->selectMin : m->selected + 1);
			audioPlaySFX(60, 64, 0);
		}
		break;
	}
}

static void menuEvent(void *arg, struct Event *ev) {
	struct MenuController *m = arg;
	switch (ev->type) {
	case EVENT_KEY:
		menuKey(m, ev);
		break;
	case EVENT_START_SCENE:
		for (int i = 0; i < 16; i++) {
			m->background[i] = 0;
			m->buttons[i] = 0;
		}
		drawVmAddFile("menu");
		if (!strcmp(sceneName, "@menu")) {
			mainMenuStart(m);
		}
		break;
	case EVENT_END_SCENE:
		eventBlockUpdates = false;
		drawVmUpdateSkip = 0;
		m->state = MENU_NONE;
		break;
	}
}

void menuSetIngame(void) {
	game->menu.state = MENU_INGAME;
}

void menuInit(struct Redefined *game) {
	struct MenuController *m = &game->menu;
	addInputHandler(EVENT_KEY | EVENT_START_SCENE | EVENT_END_SCENE, menuEvent, m);
	audioLoadSFX(60, "sfx/ui_select.wav");
	audioLoadSFX(61, "sfx/ui_ok.wav");
	audioLoadSFX(62, "sfx/ui_cancel.wav");
	audioLoadSFX(63, "sfx/ui_menu.wav");

	danmakuResetState();
	danmaku->state.difficulty = D_NORMAL;
}
void menuFini(struct Redefined *game) {
	(void) game;
}