#include "menu.h"
#include <gfx/drawvm.h>
#include <events.h>
#include "redefined.h"

#define REPLAY_MAX_IDX 10

void replayLoadMenuStart(struct MenuController *m) {
	m->state = MENU_REPLAY_LOAD;
	m->nButtons = REPLAY_MAX_IDX;
	m->selectMin = 0;
	m->selectMax = m->nButtons - 1;
	m->selected = 0;

	m->replayText = globalAlloc(REPLAY_MAX_IDX * 128);
	for (int i = 0; i < REPLAY_MAX_IDX; i++) {
		char *txt = &m->replayText[i * 128];
		replayGetInfo(txt, i);

		entity_t en = newEntity();
		m->buttons[i] = en;
		struct DrawVm *d = drawVmNew(en, "replayMenuItem");
		d->flags |= DVM_FLAG_DELETE_ALL;
		struct IchigoLocals *l = getComponent(DRAW_VM_LOCALS, en);
		l->str[0] = txt;
		l->i[0] = m->selected == i;
		l->i[1] = i;
	}

	drawVmEvent2(m->buttons[m->selected], MENU_EVENT_SELECT);
}

void replayLoadMenuChoose(struct MenuController *m) {
	danmakuResetState();
	int err = replayLoadRecording(m->selected);
	if (!err) {
		danmaku->state.replay = true;
		danmakuSwitch(LOAD_BLACK, NULL);
	}
}

void replayLoadMenuEnd(struct MenuController *m) {
	globalDealloc(m->replayText);
}