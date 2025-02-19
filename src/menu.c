#include "menu.h"
#include <gfx/drawvm.h>
#include <events.h>
#include <string.h>
#include "redefined.h"
#include "keymap.h"
#include <audio.h>

#define EVENT_DELETE 1
#define EVENT_SELECT 2
#define EVENT_UNSELECT 3
#define EVENT_CHOOSE 4
#define EVENT_FADE 5
#define EVENT_UNFADE 6

static void menuSelect(struct MenuController *m, int sel);
static void menuDvmEventAll(struct MenuController *m, int selEvent, int unselEvent) {
	for (int i = 0; i < m->nButtons; i++) {
		int ev = i == m->selected ? selEvent : unselEvent;
		if (ev == EVENT_DELETE)
			drawVmDelete(m->buttons[i]);
		else
			drawVmEvent2(m->buttons[i], ev);
	}
}


/*
Settings menu
*/

static const char *settingsMenuItems[5] = {
	"settingsMenuBGM",
	"settingsMenuSFX",
	"settingsMenuResolution",
	"settingsMenuQuit"
};
static const int settingToVolume[11] = {
	0, 13, 25, 38, 51, 64, 76, 89, 102, 114, 127
};

static const int volumeToSetting(int vol) {
	for (int i = 0; i < 9; i++) {
		if (settingToVolume[i] <= vol && settingToVolume[i + 1] > vol) {
			return i;
		}
	}
	return 10;
}

static void settingsMenuStart(struct MenuController *m) {
	m->selected = 0;
	m->state = MENU_SETTINGS;
	m->nButtons = 4;
	m->leftRight = false;
	m->selectMin = 0;
	m->selectMax = 3;
	for (int i = 0; i < m->nButtons; i++) {
		m->buttons[i] = newEntity();
		drawVmNew(m->buttons[i], settingsMenuItems[i]);
	}
	struct IchigoLocals *l = getComponent(DRAW_VM_LOCALS, m->buttons[0]);
	l->i[0] = volumeToSetting(settings.engine.bgmVol);
	l = getComponent(DRAW_VM_LOCALS, m->buttons[1]);
	l->i[0] = volumeToSetting(settings.engine.sfxVol);
	l = getComponent(DRAW_VM_LOCALS, m->buttons[2]);
	l->i[0] = settings.engine.resW;
	l->i[1] = settings.engine.resH;

	drawVmEvent2(m->buttons[0], EVENT_SELECT);
}
static void settingsMenuValueChange(struct MenuController *m, int change) {
	struct IchigoLocals *l;
	switch (m->selected) {
	case 0:
		l = getComponentOpt(DRAW_VM_LOCALS, m->buttons[0]);
		if (l) {
			l->i[0] = max(0, min(10, l->i[0] + change));
			settings.engine.bgmVol = settingToVolume[l->i[0]];
			drawVmEvent2(m->buttons[0], 4);
		}
		break;
	case 1:
		l = getComponentOpt(DRAW_VM_LOCALS, m->buttons[1]);
		if (l) {
			l->i[0] = max(0, min(10, l->i[0] + change));
			settings.engine.sfxVol = settingToVolume[l->i[0]];
			drawVmEvent2(m->buttons[1], 4);
		}
		break;
	case 2:
		l = getComponentOpt(DRAW_VM_LOCALS, m->buttons[2]);
		if (l) {
			const int *res = getNextResolution(l->i[0], l->i[1], change);
			l->i[0] = settings.engine.resW = res[0];
			l->i[1] = settings.engine.resH = res[1];
			drawVmEvent2(m->buttons[2], 4);
		}
		break;
	}
}
static void mainMenuStartNoBg(struct MenuController *m);
static void settingsMenuChoose(struct MenuController *m) {
	if (m->selected == 3) {
		/* Set resolution if changed */
		if (settings.engine.resW != realWinW || settings.engine.resH != realWinH) {
			drawSetResolution(settings.engine.resW, settings.engine.resH);
			rttIntW = realWinW;
			rttIntH = realWinH;
		}
		/* Write settings to file */
		saveSettings();

		/* Delete settings menu */
		for (int i = 0; i < m->nButtons; i++) {
			drawVmDelete(m->buttons[i]);
		}

		/* Back to main menu */
		mainMenuStartNoBg(m);
	}
}

/* Shot select */

static void shotMenuEnd(struct MenuController *m);
static void difficultyMenuStart(struct MenuController *m);

static void shotMenuChoose(struct MenuController *m) {
	menuDvmEventAll(m, EVENT_DELETE, EVENT_DELETE);
	struct DanmakuStartParams *dsp = &danmaku->startParams;
	dsp->shotType = m->selected;
	dsp->lives = 2;
	//dsp->stage = 2;
	dsp->power = 0;
	dsp->difficulty = D_EXTRA;
	danmakuSwitch(LOAD_BLACK, NULL);
}
static void shotMenuStart(struct MenuController *m) {
	m->selected = danmaku->startParams.shotType;
	m->nButtons = 2;
	m->selectMin = 0;
	m->selectMax = 1;
	m->leftRight = false;
	m->state = MENU_SHOTTYPE;

	for (int i = 0; i < 2; i++) {
		m->buttons[i] = newEntity();
		snprintf(m->buttonFns[i], 16, "shotMenuPl%.02d%c", 0, 'a' + i);
		drawVmNew(m->buttons[i], m->buttonFns[i]);
	}
	drawVmEvent2(m->buttons[m->selected], EVENT_SELECT);
}
static void shotMenuEnd(struct MenuController *m) {
	for (int i = 0; i < 2; i++) {
		drawVmDelete(m->buttons[i]);
	}
	
	difficultyMenuStart(m);
}

/* Difficulty select */

static void difficultyMenuEnd(struct MenuController *m);
static void difficultyMenuChoose(struct MenuController *m) {
	danmaku->startParams.difficulty = m->selected;
	menuDvmEventAll(m, EVENT_DELETE, EVENT_DELETE);
	shotMenuStart(m);
}

static void difficultyMenuStart(struct MenuController *m) {
	m->selected = danmaku->startParams.difficulty;
	m->nButtons = 4;
	m->selectMin = 0;
	m->selectMax = 3;
	m->leftRight = false;
	m->state = MENU_DIFFICULTY;

	for (int i = 0; i < 4; i++) {
		m->buttons[i] = newEntity();
	}
	drawVmNew(m->buttons[0], "difficultyMenuEasy");
	drawVmNew(m->buttons[1], "difficultyMenuNormal");
	drawVmNew(m->buttons[2], "difficultyMenuHard");
	drawVmNew(m->buttons[3], "difficultyMenuLunatic");
	drawVmEvent2(m->buttons[m->selected], EVENT_SELECT);
}

static void difficultyMenuEnd(struct MenuController *m) {
	drawVmDelete(m->buttons[0]);
	drawVmDelete(m->buttons[1]);
	drawVmDelete(m->buttons[2]);
	drawVmDelete(m->buttons[3]);
	mainMenuStartNoBg(m);
}

/*
Scripts menu
*/
struct ScriptMenuFile {
	char name[64];
	char disp[64];
};
static void scriptsMenuUpdate(struct MenuController *m) {
	for (int i = 0; i < 16; i++) {
		if (m->buttons[i])
			drawVmDelete(m->buttons[i]);

		entity_t en = newEntity();
		m->buttons[i] = en;
		struct DrawVm *d = drawVmNew(en, "scriptsMenuItem");
		d->flags |= DVM_FLAG_DELETE_ALL;
		struct IchigoLocals *l = getComponent(DRAW_VM_LOCALS, en);
		unsigned int idx = m->scriptScroll + i;
		if (idx < m->scriptFiles.nElements) {
			struct ScriptMenuFile *f = vecAt(&m->scriptFiles, idx);
			l->str[0] = f->disp;
		} else {
			l->str[0] = "-------------------------------- --/--/-- --:--";
		}
		l->i[0] = m->selected == idx;
		l->i[1] = i;
	}
}
static void scriptsMenuStart(struct MenuController *m) {
	m->state = MENU_SCRIPTS;
	m->scriptScroll = 0;
	m->selected = 0;
	for (int i = 0; i < 16; i++) {
		m->buttons[i] = 0;
	}
	vecCreate(&m->scriptFiles, sizeof(struct ScriptMenuFile));

	struct FoundFile ff;
	bool f = assetDirFirst(&ff, "dan");
	while (f) {
		size_t len = strlen(ff.name);
		if (!ff.flags && len > 4 && !memcmp(&ff.name[len - 4], ".ich", 4) && strcmp(ff.name, "danmaku.ich")) {
			struct ScriptMenuFile *dest = vecInsert(&m->scriptFiles, -1);
			strncpy(dest->name, ff.name, 63);
			dest->name[63] = 0;
			if (len < 64)
				dest->name[len - 4] = 0;

			char nameBuf[33];
			nameBuf[32] = 0;
			bool nameEnd = false;
			for (int i = 0; i < 32; i++) {
				if (nameEnd || !dest->name[i]) {
					nameBuf[i] = ' ';
					nameEnd = true;
				} else {
					nameBuf[i] = dest->name[i];
				}
			}

			snprintf(dest->disp, 63, "%s %.02d/%.02d/%.02d %.02d:%.02d", nameBuf,
				ff.changedDay, ff.changedMonth, ff.changedYear % 100, ff.changedHour, ff.changedMinute);
			dest->disp[63] = 0;
			logDebug("%s\n", dest->disp);
		}
		f = assetDirNext(&ff);
	}
	assetDirClose(&ff);
	scriptsMenuUpdate(m);
}
static void scriptsMenuEnd(struct MenuController *m) {
	for (int i = 0; i < 16; i++) {
		if (m->buttons[i]) {
			drawVmDelete(m->buttons[i]);
			m->buttons[i] = 0;
		}
	}
	vecDestroy(&m->scriptFiles);
}
static void scriptsMenuKey(struct MenuController *m, int match) {
	switch (match) {
	case 0: /* up */
		if (m->selected > 0)
			m->selected -= 1;
		if (m->selected < m->scriptScroll)
			m->scriptScroll = m->selected;
		scriptsMenuUpdate(m);
		break;
	case 1: /* down */
		if (m->selected < m->scriptFiles.nElements - 1)
			m->selected += 1;
		if (m->selected - 16 >= m->scriptScroll)
			m->scriptScroll = m->selected - 15;
		scriptsMenuUpdate(m);
		break;
	case 2: /* choose */
		{
			struct ScriptMenuFile *f = vecAt(&m->scriptFiles, m->selected);
			struct DanmakuStartParams *dsp = &danmaku->startParams;
			memset(dsp, 0, sizeof(*dsp));
			dsp->lives = 2;
			dsp->power = 200;
			dsp->difficulty = D_NORMAL;
			dsp->shotType = 1;
			danmakuSwitch(LOAD_BLACK, f->name);
			scriptsMenuEnd(m);
		}
		break;
	case 3: /* exit */
		scriptsMenuEnd(m);
		mainMenuStartNoBg(m);
		break;
	}
}

/*
 Main menu
*/

static const char *mainMenuButtons[9] = {
	"mainMenuGameStart",
	"mainMenuExtraStart",
	"mainMenuPracticeStart",
	"mainMenuSpellPractice",
	"mainMenuReplay",
	"mainMenuPlayerData",
	"mainMenuMusicRoom",
	"mainMenuSettings",
	"mainMenuQuit"
};

static void mainMenuChoose(struct MenuController *m) {
	menuDvmEventAll(m, EVENT_CHOOSE, EVENT_DELETE);
	m->state = MENU_NONE;
	switch (m->selected) {
	case 0: /* Game start */
		//scriptsMenuStart(m);
		difficultyMenuStart(m);
		break;
	case 7: /* Settings */
		settingsMenuStart(m);
		break;
	case 8: /* Quit */
		quit = true;
		break;
	}
	
}
static void mainMenuStartNoBg(struct MenuController *m) {
	m->selected = 0;
	m->state = MENU_MAIN;
	m->nButtons = 9;
	m->leftRight = false;
	m->selectMin = 0;
	m->selectMax = 8;
	for (int i = 0; i < m->nButtons; i++) {
		m->buttons[i] = newEntity();
		drawVmNew(m->buttons[i], mainMenuButtons[i]);
	}
	drawVmEvent(getComponent(DRAW_VM, m->buttons[0]), EVENT_SELECT);
}
static void mainMenuStart(struct MenuController *m) {
	mainMenuStartNoBg(m);

	camReset();
	cam3DLook(0, -22, 1.5f, 0, cosf(DEG2RAD(0)), sinf(DEG2RAD(0)), 0, 0, 1);
	cam3DFov = DEG2RAD(45);
	rttX = 0;
	rttY = 240;
	rttW = winW;
	rttIntW = realWinW;
	rttH = winH;
	rttIntH = realWinH;
	drawVmNew(getNewEntity(), "mainMenuBackground");

	memset(&dirLight, 0, sizeof(dirLight));
	dirLight.x = 0;
	dirLight.y = 0;
	dirLight.z = 1;
	dirLight.ambientB = dirLight.ambientR = dirLight.ambientG = 1.0f;
	dirLight.diffuseB = dirLight.diffuseG = dirLight.diffuseR = 0.5f;
	dirLight.ambientR = 1.0f;
	dirLight.ambientG = 0.7f;
	dirLight.ambientB = 0.5f;
	dirLight.diffuseR = 0.5f;
	dirLight.diffuseG = 0.3f;
	dirLight.diffuseB = 0.3f;
	dirLight.specularB = dirLight.specularG = dirLight.specularR = 0.0f;
	memset(&pointLights[0], 0, sizeof(struct Light) * DRAW_MAX_POINTLIGHTS);

#ifdef RELEASE
	if (loadingType == LOAD_INTRO)
		loadingDelay(600);
#endif
	loadingType = LOAD_BLACK;
}

/*
 Pause Menu
*/
static void pauseMenuEnd(struct MenuController *m);

static void pauseMenuChoose(struct MenuController *m) {
	struct DrawVm *d;
	enum MenuState state;
	switch (m->selected) {
	case 0: /* Continue */
		menuDvmEventAll(m, EVENT_CHOOSE, EVENT_DELETE);
		pauseMenuEnd(m);
		break;
	case 1: /* Quit */
	case 2: /* Retry */
		/* Ask for confirm */
		menuDvmEventAll(m, EVENT_FADE, EVENT_FADE);
		drawVmEvent2(m->background[1], EVENT_FADE);

		m->state = m->selected == 1? MENU_PAUSE_QUIT : MENU_PAUSE_RETRY;
		m->background[2] = newEntity();
		d = drawVmNew(m->background[2], m->state == MENU_PAUSE_QUIT? "pauseMenuQuitConfirm" : "pauseMenuRetryConfirm");
		d->layer = 47;
		m->buttons[3] = newEntity();
		d = drawVmNew(m->buttons[3], m->state == MENU_PAUSE_QUIT ? "pauseMenuQuitYes" : "pauseMenuRetryYes");
		d->layer = 47;
		m->buttons[4] = newEntity();
		d = drawVmNew(m->buttons[4], m->state == MENU_PAUSE_QUIT ? "pauseMenuQuitNo" : "pauseMenuRetryNo");
		d->layer = 47;
		drawVmEvent(d, EVENT_SELECT);
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
		menuDvmEventAll(m, EVENT_UNFADE, EVENT_UNFADE);
		drawVmEvent2(m->background[1], EVENT_UNFADE);
		menuSelect(m, m->state == MENU_PAUSE_QUIT ? 1 : 2);
		m->state = MENU_PAUSE;
		m->leftRight = false;
		break;
	}
}
static void pauseMenuStart(struct MenuController *m) {
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
	drawVmEvent(cont, EVENT_SELECT);

	gamePause();
}
static void pauseMenuEnd(struct MenuController *m) {
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

	drawVmEvent2(m->buttons[m->selected], EVENT_SELECT);
	gamePause();
}

static void endMenuEnd(struct MenuController *m) {
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

static void endMenuChoose(struct MenuController *m) {
	struct DrawVm *d;
	enum MenuState state;
	switch (m->selected) {
	case 0: /* Continue */
		menuDvmEventAll(m, EVENT_CHOOSE, EVENT_DELETE);
		endMenuEnd(m);
		
		danmaku->continuesUsed += 1;
		danmaku->player.lives = 2;
		danmaku->player.lifePieces = 0;

		break;
	case 1: /* Quit */
	case 2: /* Retry */
		/* Ask for confirm */
		menuDvmEventAll(m, EVENT_FADE, EVENT_FADE);
		drawVmEvent2(m->background[1], EVENT_FADE);

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
		drawVmEvent(d, EVENT_SELECT);
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
		m->selectMin = danmaku->continuesUsed < MAX_CONTINUES? 0 : 1;
		m->selectMax = 2;
		m->nButtons = 3;
		menuDvmEventAll(m, EVENT_UNFADE, EVENT_UNFADE);
		drawVmEvent2(m->background[1], EVENT_UNFADE);
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
			c = (edit && j == m->keyboardCursor)? '_' : ' ';
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

static void menuEndScoreKey(struct MenuController *m, int key) {
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


/*
 Generic
*/

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
		pauseMenuChoose(m);
		break;
	case MENU_END:
	case MENU_END_QUIT:
	case MENU_END_RETRY:
		endMenuChoose(m);
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
		m->selected = 4;
		pauseMenuChoose(m);
		break;
	case MENU_END_QUIT:
	case MENU_END_RETRY:
		m->selected = 4;
		endMenuChoose(m);
		break;
	default:
		break;
	}
}

static void menuSelect(struct MenuController *m, int sel) {
	drawVmEvent(getComponent(DRAW_VM, m->buttons[m->selected]), EVENT_UNSELECT);
	drawVmEvent(getComponent(DRAW_VM, m->buttons[sel]), EVENT_SELECT);
	m->selected = sel;
}

static void menuKey(struct MenuController *m, struct Event *ev) {
	if (!m->state || !ev->param2)
		return;

	if (ev->param == SDLK_e && m->state == MENU_MAIN) {
		menuDvmEventAll(m, EVENT_DELETE, EVENT_DELETE);
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

	memset(&danmaku->startParams, 0, sizeof(struct DanmakuStartParams));
	danmaku->startParams.difficulty = D_NORMAL;
}
void menuFini(struct Redefined *game) {
	(void) game;
}