#include "menu.h"
#include <gfx/drawvm.h>
#include <events.h>
#include "redefined.h"

/*
 Pause Menu
*/
void pauseMenuChoose(struct MenuController *m) {
	struct DrawVm *d;
	enum MenuState state;
	switch (m->selected) {
	case 0: /* Continue */
		menuDvmEventAll(m, MENU_EVENT_CHOOSE, MENU_EVENT_DELETE);
		pauseMenuEnd(m);
		break;
	case 1: /* Quit */
	case 2: /* Retry */
		/* Ask for confirm */
		menuDvmEventAll(m, MENU_EVENT_FADE, MENU_EVENT_FADE);
		drawVmEvent2(m->background[1], MENU_EVENT_FADE);

		m->state = m->selected == 1 ? MENU_PAUSE_QUIT : MENU_PAUSE_RETRY;
		m->background[2] = newEntity();
		d = drawVmNew(m->background[2], m->state == MENU_PAUSE_QUIT ? "pauseMenuQuitConfirm" : "pauseMenuRetryConfirm");
		d->layer = 47;
		m->buttons[3] = newEntity();
		d = drawVmNew(m->buttons[3], m->state == MENU_PAUSE_QUIT ? "pauseMenuQuitYes" : "pauseMenuRetryYes");
		d->layer = 47;
		m->buttons[4] = newEntity();
		d = drawVmNew(m->buttons[4], m->state == MENU_PAUSE_QUIT ? "pauseMenuQuitNo" : "pauseMenuRetryNo");
		d->layer = 47;
		drawVmEvent(d, MENU_EVENT_SELECT);
		m->selected = 4;
		m->selectMin = 3;
		m->selectMax = 4;
		m->nButtons = 5;
		m->leftRight = true;
		break;
	case 3: /* Yes */
		state = m->state;
		pauseMenuEnd(m);
		if (state == MENU_PAUSE_QUIT) {
			swScene(LOAD_BLACK, "@menu");
		} else {
			swScene(LOAD_BLACK, sceneName);
		}
		break;
	case 4: /* No */
		drawVmDelete(m->background[2]);
		drawVmDelete(m->buttons[3]);
		drawVmDelete(m->buttons[4]);
		m->selectMin = 0;
		m->selectMax = 2;
		m->nButtons = 3;
		menuDvmEventAll(m, MENU_EVENT_UNFADE, MENU_EVENT_UNFADE);
		drawVmEvent2(m->background[1], MENU_EVENT_UNFADE);
		menuSelect(m, m->state == MENU_PAUSE_QUIT ? 1 : 2);
		m->state = MENU_PAUSE;
		m->leftRight = false;
		break;
	}
}
void pauseMenuStart(struct MenuController *m) {
	m->selected = 0;
	m->state = MENU_PAUSE;
	m->nButtons = 3;
	m->leftRight = false;
	m->selectMin = 0;
	m->selectMax = 2;
	struct DrawVm *d, *cont;
	m->background[0] = newEntity();
	d = drawVmNew(m->background[0], "pauseMenuBackground");
	d->layer = 47;
	m->background[1] = newEntity();
	d = drawVmNew(m->background[1], "pauseMenuTitle");
	d->layer = 47;
	m->buttons[0] = newEntity();
	cont = drawVmNew(m->buttons[0], "pauseMenuContinue");
	cont->layer = 47;
	m->buttons[1] = newEntity();
	d = drawVmNew(m->buttons[1], "pauseMenuQuit");
	d->layer = 47;
	m->buttons[2] = newEntity();
	d = drawVmNew(m->buttons[2], "pauseMenuReload");
	d->layer = 47;
	drawVmEvent(cont, MENU_EVENT_SELECT);

	gamePause();
}
void pauseMenuEnd(struct MenuController *m) {
	drawVmDelete(m->background[0]);
	drawVmDelete(m->background[1]);
	drawVmDelete(m->background[2]);
	drawVmDelete(m->buttons[0]);
	drawVmDelete(m->buttons[1]);
	drawVmDelete(m->buttons[2]);
	drawVmDelete(m->buttons[3]);
	drawVmDelete(m->buttons[4]);
	m->state = MENU_INGAME;
	m->leftRight = false;
	gameUnpause();
}

/* End */

static void menuEnd(void);
static void menuEndScore(struct ScoreLeaderBoardEntry *lb, bool cleared);

void menuEndScreen(bool allowContinue, bool cleared) {
	game->menu.allowContinue = allowContinue;
	struct ScoreLeaderBoardEntry *lb = scoreGetLeaderBoard();
	game->menu.scorePlace = scoreGetPlace(lb);
	if (!danmaku->startParams.practice && game->danmaku.continuesUsed == 0 && game->menu.scorePlace != -1) {
		menuEndScore(lb, cleared);
	} else {
		game->menu.background[0] = 0;
		menuEnd();
	}
}

static void menuEnd(void) {
	struct MenuController *m = &game->menu;
	m->state = MENU_END;
	m->nButtons = 3;
	m->leftRight = false;
	m->selectMax = 2;
	if (m->allowContinue && danmaku->continuesUsed < MAX_CONTINUES) {
		m->selected = 0;
		m->selectMin = 0;
	} else {
		m->selected = 1;
		m->selectMin = 1;
	}

	struct DrawVm *d;
	if (!m->background[0]) {
		m->background[0] = newEntity();
		d = drawVmNew(m->background[0], "pauseMenuBackground");
		d->layer = 47;
	}
	m->background[1] = newEntity();
	d = drawVmNew(m->background[1], "endMenuTitle");
	d->layer = 47;

	m->buttons[0] = newEntity();
	d = drawVmNew(m->buttons[0], "endMenuContinue");
	d->layer = 47;
	struct IchigoLocals *l = getComponent(DRAW_VM_LOCALS, m->buttons[0]);
	l->i[0] = MAX_CONTINUES - danmaku->continuesUsed;

	m->buttons[1] = newEntity();
	d = drawVmNew(m->buttons[1], "pauseMenuQuit");
	d->layer = 47;
	m->buttons[2] = newEntity();
	d = drawVmNew(m->buttons[2], "pauseMenuReload");
	d->layer = 47;

	drawVmEvent2(m->buttons[m->selected], MENU_EVENT_SELECT);
	gamePause();
}

void endMenuEnd(struct MenuController *m) {
	drawVmDelete(m->background[0]);
	drawVmDelete(m->background[1]);
	drawVmDelete(m->buttons[0]);
	drawVmDelete(m->buttons[1]);
	drawVmDelete(m->buttons[2]);
	drawVmDelete(m->buttons[3]);
	drawVmDelete(m->buttons[4]);

	m->state = MENU_INGAME;
	gameUnpause();
}

void endMenuChoose(struct MenuController *m) {
	struct DrawVm *d;
	enum MenuState state;
	switch (m->selected) {
	case 0: /* Continue */
		menuDvmEventAll(m, MENU_EVENT_CHOOSE, MENU_EVENT_DELETE);
		endMenuEnd(m);

		danmaku->continuesUsed += 1;
		danmaku->player.lives = 2;
		danmaku->player.lifePieces = 0;

		break;
	case 1: /* Quit */
	case 2: /* Retry */
		/* Ask for confirm */
		menuDvmEventAll(m, MENU_EVENT_FADE, MENU_EVENT_FADE);
		drawVmEvent2(m->background[1], MENU_EVENT_FADE);

		m->state = m->selected == 1 ? MENU_END_QUIT : MENU_END_RETRY;
		m->background[2] = newEntity();
		d = drawVmNew(m->background[2], m->state == MENU_END_QUIT ? "pauseMenuQuitConfirm" : "pauseMenuRetryConfirm");
		d->layer = 47;
		m->buttons[3] = newEntity();
		d = drawVmNew(m->buttons[3], m->state == MENU_END_QUIT ? "pauseMenuQuitYes" : "pauseMenuRetryYes");
		d->layer = 47;
		m->buttons[4] = newEntity();
		d = drawVmNew(m->buttons[4], m->state == MENU_END_QUIT ? "pauseMenuQuitNo" : "pauseMenuRetryNo");
		d->layer = 47;
		drawVmEvent(d, MENU_EVENT_SELECT);
		m->selected = 4;
		m->selectMin = 3;
		m->selectMax = 4;
		m->nButtons = 5;
		m->leftRight = true;
		break;
	case 3: /* Yes */
		state = m->state;
		pauseMenuEnd(m);
		if (state == MENU_END_QUIT) {
			swScene(LOAD_BLACK, "@menu");
		} else {
			swScene(LOAD_BLACK, sceneName);
		}
		break;
	case 4: /* No */
		drawVmDelete(m->background[2]);
		drawVmDelete(m->buttons[3]);
		drawVmDelete(m->buttons[4]);
		m->selectMin = danmaku->continuesUsed < MAX_CONTINUES ? 0 : 1;
		m->selectMax = 2;
		m->nButtons = 3;
		menuDvmEventAll(m, MENU_EVENT_UNFADE, MENU_EVENT_UNFADE);
		drawVmEvent2(m->background[1], MENU_EVENT_UNFADE);
		menuSelect(m, m->state == MENU_PAUSE_QUIT ? 1 : 2);
		m->state = MENU_PAUSE;
		m->leftRight = false;
		break;
	}
}

static const char keyboardOrder[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-=.,!?@:;[]()_/{}|~^#$%&* \x7f";

static void menuEndScoreLeaderBoard(struct MenuController *m, struct ScoreLeaderBoardEntry *lb, int i, bool edit) {
	entity_t en = newEntity();
	m->scoreText[i] = en;
	struct Transform *tf = newComponent(TRANSFORM, en);
	tf->x = -320 + 48;
	tf->y = 96 + i * 20;

	struct DrawVm *d = drawVmNew(en, "leaderboardText");
	d->layer = 47;
	d->flags |= DVM_FLAG_DELETE_ALL;
	struct IchigoLocals *l = getComponent(DRAW_VM_LOCALS, en);
	char *buf = &m->scoreTextContent[i][0];
	l->str[0] = buf;

	sprintf(buf, "%.2d ", i + 1);
	for (int j = 0; j < SCOREF_NAME_LEN; j++) {
		char c = lb[i].name[j];
		if (!c)
			c = (edit && j == m->keyboardCursor) ? '_' : ' ';
		buf[3 + j] = c;
	}
	if (lb[i].stage == 255) {
		snprintf(buf + SCOREF_NAME_LEN + 3, 64 - 20, " %.09llu --/--/-- Stage -", lb[i].score);
	} else if (lb[i].stage == 7) {
		snprintf(buf + SCOREF_NAME_LEN + 3, 64 - 20, " %.09llu %.02d/%.02d/%.02d Extra", lb[i].score, lb[i].year, lb[i].month, lb[i].day);
	} else if (lb[i].stage == 254) {
		snprintf(buf + SCOREF_NAME_LEN + 3, 64 - 20, " %.09llu %.02d/%.02d/%.02d Clear!!", lb[i].score, lb[i].year, lb[i].month, lb[i].day);
	} else if (lb[i].stage == 253) {
		snprintf(buf + SCOREF_NAME_LEN + 3, 64 - 20, " %.09llu %.02d/%.02d/%.02d EX Clear!!", lb[i].score, lb[i].year, lb[i].month, lb[i].day);
	} else {
		snprintf(buf + SCOREF_NAME_LEN + 3, 64 - 20, " %.09llu %.02d/%.02d/%.02d Stage %.1d", lb[i].score, lb[i].year, lb[i].month, lb[i].day, lb[i].stage);
	}

	if (i == m->scorePlace)
		drawVmEvent(d, 3);
}

static void menuEndScore(struct ScoreLeaderBoardEntry *lb, bool cleared) {
	struct MenuController *m = &game->menu;
	m->state = MENU_END_SCORE;
	m->keyboardCursor = strlen(danmaku->score.scoreFile->currentName);
	if (m->keyboardCursor) {
		m->keyboardX = 12;
		m->keyboardY = 6;
	} else {
		m->keyboardX = 0;
		m->keyboardY = 0;
	}

	int x = 0, y = 0;
	for (int i = 0; i < 91; i++) {
		char displayChar = keyboardOrder[i];
		entity_t en = newEntity();
		struct Transform *tf = newComponent(TRANSFORM, en);
		tf->x = (-128 + 32 - 6 * 20) + x * 20;
		tf->y = 320 + y * 20;
		struct DrawVm *d = drawVmNew(en, "keyboardChar");
		d->layer = 47;
		d->flags |= DVM_FLAG_DELETE_ALL;
		struct IchigoLocals *l = getComponent(DRAW_VM_LOCALS, en);
		l->i[0] = displayChar;

		m->keyboardButtons[i] = en;

		if (++x == 13) {
			x = 0;
			y++;
		}
	}
	drawVmEvent2(m->keyboardButtons[m->keyboardX + m->keyboardY * 13], 2);

	int stg = danmaku->stage;
	if (cleared) {
		stg = danmaku->stage == 7 ? 253 : 254;
	}
	struct ScoreLeaderBoardEntry n = { "", danmaku->score.score, 0, 0, 0, stg };
	memcpy(&n.name, danmaku->score.scoreFile->currentName, SCOREF_NAME_LEN);
	scoreInsert(lb, m->scorePlace, &n);
	for (int i = 0; i < 10; i++) {
		menuEndScoreLeaderBoard(m, lb, i, m->scorePlace == i);
	}

	struct DrawVm *d;
	m->background[0] = newEntity();
	d = drawVmNew(m->background[0], "pauseMenuBackground");
	d->layer = 47;

	gamePause();
}

static void menuEndScoreEnd(struct MenuController *m) {
	for (int i = 0; i < 10; i++) {
		drawVmDelete(m->scoreText[i]);
	}
	for (int i = 0; i < 91; i++) {
		drawVmDelete(m->keyboardButtons[i]);
	}

	menuEnd();
}

void menuEndScoreKey(struct MenuController *m, int key) {
	if (key == 2 || key == 3) {
		char c = keyboardOrder[m->keyboardX + m->keyboardY * 13];
		if (c == 0 && key == 2) {
			menuEndScoreEnd(m);
			return;
		}
		audioPlaySFX(60, 64, 0);
		char *name = danmaku->score.scoreFile->currentName;
		if (c == 127 || key == 3) {
			if (m->keyboardCursor) {
				m->keyboardCursor -= 1;
				name[m->keyboardCursor] = 0;
			}
		} else {
			if (m->keyboardCursor < SCOREF_NAME_LEN) {
				name[m->keyboardCursor] = c;
				m->keyboardCursor += 1;
			}
		}

		drawVmDelete(m->scoreText[m->scorePlace]);
		struct ScoreLeaderBoardEntry *lb = scoreGetLeaderBoard();
		memcpy(lb[m->scorePlace].name, name, SCOREF_NAME_LEN);
		menuEndScoreLeaderBoard(m, lb, m->scorePlace, true);
	} else {
		drawVmEvent2(m->keyboardButtons[m->keyboardX + m->keyboardY * 13], 3);
		switch (key) {
		case 0:
			m->keyboardY = m->keyboardY == 0 ? 6 : m->keyboardY - 1;
			break;
		case 1:
			m->keyboardY = m->keyboardY == 6 ? 0 : m->keyboardY + 1;
			break;
		case 5:
			m->keyboardX = m->keyboardX == 0 ? 12 : m->keyboardX - 1;
			break;
		case 6:
			m->keyboardX = m->keyboardX == 12 ? 0 : m->keyboardX + 1;
			break;
		}
		drawVmEvent2(m->keyboardButtons[m->keyboardX + m->keyboardY * 13], 2);
		audioPlaySFX(60, 64, 0);
	}
}