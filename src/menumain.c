#include "menu.h"
#include <gfx/drawvm.h>
#include <events.h>
#include "redefined.h"

/* Shot select */
void shotMenuChoose(struct MenuController *m) {
	menuDvmEventAll(m, MENU_EVENT_DELETE, MENU_EVENT_DELETE);
	struct DanmakuStartParams *dsp = &danmaku->startParams;
	dsp->shotType = m->selected;
	dsp->lives = 2;
	dsp->stage = 1;
	dsp->power = 0;
	dsp->difficulty = D_EXTRA;
	danmakuSwitch(LOAD_BLACK, NULL);
}
void shotMenuStart(struct MenuController *m) {
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
	drawVmEvent2(m->buttons[m->selected], MENU_EVENT_SELECT);
}
void shotMenuEnd(struct MenuController *m) {
	for (int i = 0; i < 2; i++) {
		drawVmDelete(m->buttons[i]);
	}

	difficultyMenuStart(m);
}

/* Difficulty select */

void difficultyMenuChoose(struct MenuController *m) {
	danmaku->startParams.difficulty = m->selected;
	menuDvmEventAll(m, MENU_EVENT_DELETE, MENU_EVENT_DELETE);
	shotMenuStart(m);
}

void difficultyMenuStart(struct MenuController *m) {
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
	drawVmEvent2(m->buttons[m->selected], MENU_EVENT_SELECT);
}

void difficultyMenuEnd(struct MenuController *m) {
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
void scriptsMenuStart(struct MenuController *m) {
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
void scriptsMenuEnd(struct MenuController *m) {
	for (int i = 0; i < 16; i++) {
		if (m->buttons[i]) {
			drawVmDelete(m->buttons[i]);
			m->buttons[i] = 0;
		}
	}
	vecDestroy(&m->scriptFiles);
}
void scriptsMenuKey(struct MenuController *m, int match) {
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

void mainMenuChoose(struct MenuController *m) {
	menuDvmEventAll(m, MENU_EVENT_CHOOSE, MENU_EVENT_DELETE);
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

void mainMenuStartNoBg(struct MenuController *m) {
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
	drawVmEvent(getComponent(DRAW_VM, m->buttons[0]), MENU_EVENT_SELECT);
}

void mainMenuStart(struct MenuController *m) {
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