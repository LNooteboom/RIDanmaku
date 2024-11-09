#include "redefined.h"

/* Danmaku instrs */
static void i_dialogStart(struct IchigoVm *vm) {
	dialogStart(ichigoGetString(NULL, vm, 0));
}
static int i_getVarDIALOG_WAIT(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	return dc->danWait;
}
static void i_setVarDIALOG_WAIT(struct IchigoVm *vm, int val) {
	struct DialogController *dc = &game->dialog;
	if (dc->active)
		dc->danWait = val;
}



static void dialogWaitEnd(struct DialogController *dc) {
	dc->waitMax = 0;
	dc->waitTime = 0;
	drawVmEvent2(dc->dvmText, 3); /* Clear text */
	if (dc->dvmIntro) {
		drawVmDelete(dc->dvmIntro);
		dc->dvmIntro = 0;
	}
}

static void dialogUpdate(void *arg) {
	struct DialogController *dc = arg;
	if (!dc->active)
		return;

	if (dc->waitMax) {
		if ((dc->skippable && dc->waitTime >= 4 && keyPressed(K_DIALOG_SKIP)) ||
			(keyPressed(K_MENU_SELECT) && !dc->nextKey)) {

			dialogWaitEnd(dc);
		} else {
			struct IchigoVm *vm = getComponent(ICHIGO_VM, dc->vm);
			vm->coroutines[0].waitTime = 1;
			if (++dc->waitTime >= dc->waitMax) {
				dialogWaitEnd(dc);
			}
		}
	}
	dc->nextKey = keyPressed(K_MENU_SELECT);
}

static void dialogEvent(void *arg, struct Event *ev) {
	struct DialogController *dc = arg;

	if (dc->active)
		dialogEnd();
	ichigoClear(&dc->iState);
}

void dialogStart(const char *fn) {
	struct DialogController *dc = &game->dialog;
	dc->active = true;
	dc->danWait = true;
	gameSpeed = 1;
	dc->skippable = true;
	drawVmAddFile("dialog");

	char buf[8];
	if (!drawVmLanguage) {
		snprintf(buf, 8, "en%.02d", game->danmaku.player.playerType);
	} else if (drawVmLanguage == 1) {
		snprintf(buf, 8, "jp%.02d", game->danmaku.player.playerType);
	} else {
		snprintf(buf, 8, "xx%.02d", game->danmaku.player.playerType);
	}
	ichigoAddFile(&dc->iState, buf);

	dc->vm = newEntity();
	struct IchigoVm *vm = newComponent(ICHIGO_VM, dc->vm);
	ichigoVmNew(&dc->iState, vm, dc->vm);
	ichigoVmExec(vm, fn, "");
}
void dialogEnd(void) {
	struct DialogController *dc = &game->dialog;
	dc->active = false;
	dc->danWait = false;
	drawVmDelete(dc->dvmPlayer);
	dc->dvmPlayer = 0;
	drawVmDelete(dc->dvmBoss);
	dc->dvmBoss = 0;
	drawVmDelete(dc->dvmText);
	dc->dvmText = 0;
	drawVmDelete(dc->dvmIntro);
	dc->dvmIntro = 0;
	deleteEntity(dc->vm);
	dc->vm = 0;
	dc->textY = 0;
	dc->waitMax = 0;
	dc->waitTime = 0;
}


static void idlg_end(struct IchigoVm *vm) {
	dialogEnd();
}

static void idlg_playerShow(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	const char *fn = ichigoGetString(NULL, vm, 0);
	dc->dvmPlayer = newEntity();
	struct DrawVm *d = drawVmNew(dc->dvmPlayer, fn);
	d->flags |= DVM_FLAG_DELETE_ALL;
}

static void idlg_playerHide(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	drawVmDelete(dc->dvmPlayer);
	dc->dvmPlayer = 0;
}

static void idlg_playerFace(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	struct IchigoLocals *l = getComponentOpt(DRAW_VM_LOCALS, dc->dvmPlayer);
	if (l) {
		l->i[0] = ichigoGetInt(vm, 0);
		drawVmEvent2(dc->dvmPlayer, 2);
	}
}

static void idlg_playerEffect(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	drawVmEvent2(dc->dvmPlayer, 5 + ichigoGetInt(vm, 0));
}

static void idlg_bossShow(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	const char *fn = ichigoGetString(NULL, vm, 0);
	dc->dvmBoss = newEntity();
	struct DrawVm *d = drawVmNew(dc->dvmBoss, fn);
	d->flags |= DVM_FLAG_DELETE_ALL;
}

static void idlg_bossHide(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	drawVmDelete(dc->dvmBoss);
	dc->dvmBoss = 0;
}

static void idlg_bossFace(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	struct IchigoLocals *l = getComponentOpt(DRAW_VM_LOCALS, dc->dvmBoss);
	if (l) {
		l->i[0] = ichigoGetInt(vm, 0);
		drawVmEvent2(dc->dvmBoss, 2);
	}
}

static void idlg_bossEffect(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	drawVmEvent2(dc->dvmBoss, 5 + ichigoGetInt(vm, 0));
}

static void idlg_textboxShow(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	dc->dvmText = newEntity();
	struct DrawVm *d = drawVmNew(dc->dvmText, "dialogText");
	d->flags |= DVM_FLAG_DELETE_ALL;
}

static void idlg_textboxHide(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	drawVmDelete(dc->dvmText);
	dc->dvmText = 0;
}

static void idlg_speakerPlayer(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	drawVmEvent2(dc->dvmPlayer, 3);
	drawVmEvent2(dc->dvmBoss, 4);
}

static void idlg_speakerBoss(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	drawVmEvent2(dc->dvmPlayer, 4);
	drawVmEvent2(dc->dvmBoss, 3);
}

static void idlg_speakerNone(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	drawVmEvent2(dc->dvmPlayer, 4);
	drawVmEvent2(dc->dvmBoss, 4);
}

static void idlg_intro(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	const char *fn = ichigoGetString(NULL, vm, 0);
	dc->dvmPlayer = newEntity();
	struct DrawVm *d = drawVmNew(dc->dvmIntro, fn);
	d->flags |= DVM_FLAG_DELETE_ALL;
}

static void idlg_skippable(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	dc->skippable = ichigoGetInt(vm, 0);
}

static void idlg_textY(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	dc->textY = ichigoGetInt(vm, 0);
}

static void idlg_text(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	struct IchigoLocals *l = getComponentOpt(DRAW_VM_LOCALS, dc->dvmText);
	if (l) {
		l->i[0] = dc->textY;
		l->str[0] = ichigoGetString(NULL, vm, 0);
		drawVmEvent2(dc->dvmText, 2);
	}
	vm->coroutines[0].waitTime = 1;
}

static void idlg_next(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	dc->waitMax = ichigoGetInt(vm, 0);
	dc->waitTime = 0;
	dc->textY = 0;
	vm->coroutines[0].waitTime = 1;
}

static void idlg_musicEnd(struct IchigoVm *vm) {
	// TODO
}

static void idlg_musicBoss(struct IchigoVm *vm) {
	// TODO
}

static void idlg_danContinue(struct IchigoVm *vm) {
	struct DialogController *dc = &game->dialog;
	dc->danWait = false;
}

static IchigoInstr *dialogInstrs[64] = {
	idlg_end,
	idlg_playerShow,
	idlg_playerHide,
	idlg_playerFace,
	idlg_playerEffect,
	idlg_bossShow,
	idlg_bossHide,
	idlg_bossFace,
	idlg_bossEffect,
	idlg_textboxShow,
	idlg_textboxHide,
	idlg_speakerPlayer,
	idlg_speakerBoss,
	idlg_speakerNone,
	idlg_intro,
	idlg_skippable,
	idlg_textY,
	idlg_text,
	idlg_next,
	idlg_musicEnd,
	idlg_musicBoss,
	i_sfx,
	idlg_danContinue
};

static void setVar(struct DialogController *dc, int idx, enum IchigoRegType type, void *get, void *set) {
	dc->iVars[idx].regType = type;
	dc->iVars[idx].get.i = get;
	dc->iVars[idx].set.i = set;
}
void dialogInit(struct Redefined *game) {
	struct DialogController *dc = &game->dialog;
	ichigoInit(&dc->iState, "dlg");
	ichigoSetInstrTable(&dc->iState, dialogInstrs, 64);
	ichigoSetVarTable(&dc->iState, dc->iVars, 32);

	//setVar(dc, 23, REG_INT, i_getVarLANG, NULL);

	danmakuSetInstr(160, &i_dialogStart);
	danmakuSetVar(110, REG_INT, &i_getVarDIALOG_WAIT, i_setVarDIALOG_WAIT);

	addUpdate(UPDATE_NORM, dialogUpdate, dc);
	addInputHandler(EVENT_END_SCENE, dialogEvent, dc);
}
void dialogFini(struct Redefined *game) {
	removeUpdate(UPDATE_NORM, dialogUpdate);
	removeInputHandler(dialogEvent);
}