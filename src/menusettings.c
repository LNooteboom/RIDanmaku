#include "menu.h"
#include <gfx/drawvm.h>
#include <events.h>
#include "redefined.h"

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

void settingsMenuStart(struct MenuController *m) {
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

	drawVmEvent2(m->buttons[0], MENU_EVENT_SELECT);
}
void settingsMenuValueChange(struct MenuController *m, int change) {
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

void settingsMenuChoose(struct MenuController *m) {
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