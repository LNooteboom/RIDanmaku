#include "redefined.h"
#include <string.h>
#include <audio.h>

struct Danmaku *danmaku;

static void i_end(struct IchigoVm *vm) {
	if (danmaku->state.stage >= danmaku->nStages || danmaku->state.practice || !replayHasNextStage()) {
		replayStop();
		bossEnd(danmaku);
		danBgEnd(danmaku);
		//danPlayerDestroy(d);
		ichigoClear(&danmaku->danScript);
		menuEndScreen(false, !danmaku->state.practice);
	} else {
		/* Load next stage */
		danmaku->state.stage += 1;
		danmakuSwitch(LOAD_BLACK, NULL);
	}
}

static void i_normalizeRot(struct IchigoVm *vm) {
	float r = ichigoGetFloat(vm, 1);
	r = fmod(r, 2 * PI);
	if (r > PI) {
		r -= 2 * PI;
	} else if (r < -PI) {
		r += 2 * PI;
	}
	ichigoSetFloat(vm, 0, r);
}

static void i_diffI(struct IchigoVm *vm) {
	switch (danmaku->state.difficulty) {
	case D_EASY:
		ichigoSetInt(vm, 0, ichigoGetInt(vm, 1));
		break;
	case D_NORMAL:
		ichigoSetInt(vm, 0, ichigoGetInt(vm, 2));
		break;
	case D_HARD:
		ichigoSetInt(vm, 0, ichigoGetInt(vm, 3));
		break;
	default:
		ichigoSetInt(vm, 0, ichigoGetInt(vm, 4));
		break;
	}
}

static void i_diffF(struct IchigoVm *vm) {
	switch (danmaku->state.difficulty) {
	case D_EASY:
		ichigoSetFloat(vm, 0, ichigoGetFloat(vm, 1));
		break;
	case D_NORMAL:
		ichigoSetFloat(vm, 0, ichigoGetFloat(vm, 2));
		break;
	case D_HARD:
		ichigoSetFloat(vm, 0, ichigoGetFloat(vm, 3));
		break;
	default:
		ichigoSetFloat(vm, 0, ichigoGetFloat(vm, 4));
		break;
	}
}
static void i_diffWait(struct IchigoVm *vm) {
	struct IchigoCorout *co = vm->is->curCorout;
	switch (danmaku->state.difficulty) {
	case D_EASY:
		co->waitTime += ichigoGetFloat(vm, 0);
		break;
	case D_NORMAL:
		co->waitTime += ichigoGetFloat(vm, 1);
		break;
	case D_HARD:
		co->waitTime += ichigoGetFloat(vm, 2);
		break;
	default:
		co->waitTime += ichigoGetFloat(vm, 3);
		break;
	}
}

static void i_bgm(struct IchigoVm *vm) {
	int idx = ichigoGetInt(vm, 0);
	char buf[50];
	snprintf(buf, 50, "bgm/fsw_%.02d.ogg", idx);
	audioStartMusic(buf);
}
static void i_bgmStop(struct IchigoVm *vm) {
	(void)vm;
	audioStopMusic();
}
void i_shake(struct IchigoVm *vm) {
	float dur = ichigoGetFloat(vm, 0);
	if (dur > (danmaku->shakeDuration - danmaku->shakeTime)) {
		danmaku->shakeDuration = dur;
		danmaku->shakeTime = 0;
		danmaku->shakeIntensity1 = ichigoGetFloat(vm, 1);
		danmaku->shakeIntensity2 = ichigoGetFloat(vm, 2);
	}
}

void danmakuPlaySfx(int slot) {
	struct DanmakuSfx *sfx = &danmaku->sfx[slot];
	if (sfx->channel != 0xFFFF) {
		audioPlaySFX(sfx->channel, sfx->volume, sfx->panning);
	}
}

void danmakuStopSfx(int slot) {
	struct DanmakuSfx *sfx = &danmaku->sfx[slot];
	if (sfx->channel != 0xFFFF) {
		audioStopSFX(sfx->channel);
	}
}

void i_sfx(struct IchigoVm *vm) {
	danmakuPlaySfx(ichigoGetInt(vm, 0));
}

void i_sfxStop(struct IchigoVm *vm) {
	danmakuStopSfx(ichigoGetInt(vm, 0));
}

static void i_initSfxLoad(struct IchigoVm *vm) {
	int channel = ichigoGetInt(vm, 0);
	uint16_t strLen;
	const char *file = ichigoGetString(&strLen, vm, 1);
	audioLoadSFX(channel, file);
}
static void i_initSfxSet(struct IchigoVm *vm) {
	int slot = ichigoGetInt(vm, 0);
	danmaku->sfx[slot].channel = ichigoGetInt(vm, 1);
	danmaku->sfx[slot].volume = ichigoGetInt(vm, 2);
	danmaku->sfx[slot].panning = ichigoGetInt(vm, 3);
}

static void i_initStages(struct IchigoVm *vm) {
	danmaku->nStages = ichigoGetInt(vm, 0);
	danmaku->nExtraStages = ichigoGetInt(vm, 1);
}

static void danmakuStart(struct Danmaku *d) {
	d->active = true;

	camX = 0; /* Origin in the middle */
	camY = 224; /* Origin at the top */
	clearColor = 0;

	rttIntW = rttW = 384;
	rttIntH = rttH = 448;
	rttX = 32 + 192 - 320;
	rttY = 240;

	if (d->state.replay) {
		replayStartPlaying();
	} else {
		/* Record a replay */
		replayStartRecording();
	}

	menuSetIngame();

	danBgStart(d);
	itemStart(d);

	/* Ui */
	drawVmAddFile("dan_ui");

	scoreStart();

	d->uiBg = getNewEntity();
	drawVmNew(d->uiBg, "uiBackground");
	d->uiScore = getNewEntity();
	drawVmNew(d->uiScore, "uiScore");
	d->uiHiScore = newEntity();
	drawVmNew(d->uiHiScore, "uiHiScore");
	struct IchigoLocals *l = getComponent(DRAW_VM_LOCALS, d->uiHiScore);
	l->i[0] = d->score.hiscore;

	d->uiPoints = getNewEntity();
	drawVmNew(d->uiPoints, "uiPoints");
	l = getComponent(DRAW_VM_LOCALS, d->uiPoints);
	l->i[0] = d->score.pointItems;

	d->uiGraze = getNewEntity();
	drawVmNew(d->uiGraze, "uiGraze");
	l = getComponent(DRAW_VM_LOCALS, d->uiGraze);
	l->i[0] = d->score.graze;

	d->uiFps = getNewEntity();
	drawVmNew(d->uiFps, "uiFps");

	d->uiDifficulty = getNewEntity();
	drawVmNew(d->uiDifficulty, "uiDifficulty");
	l = getComponent(DRAW_VM_LOCALS, d->uiDifficulty);
	l->i[0] = d->state.difficulty;

	loadModelFile("mesh/distortGrid.mes");
	drawVmAddFile("dan_eff");
	danPlayerCreate(d, &d->state);

	/* Start script */
	ichigoAddFile(&d->danScript, d->script);
	enemyNewMain();

//#ifndef RELEASE
	drawVmAddFile("ascii");
//#endif
}

static void danmakuEnd(struct Danmaku *d) {
	d->active = false;
	bossEnd(d);
	danBgEnd(d);
	danPlayerDestroy(d);
	ichigoClear(&d->danScript);

	replayStop();
	if (strcmp(sceneName, "@danmaku")) {
		if (!replayIsPlaying())
			replaySaveRecording(0, "Last");
		replayClearRecording();
	}

	scoreSave();
}

static void danmakuUpdate(void *arg) {
	struct Danmaku *d = arg;
	if (!d->active)
		return;

	/* Update UI */
	struct IchigoLocals *l = getComponentOpt(DRAW_VM_LOCALS, d->uiHiScore);
	int hs = d->state.score == d->score.hiscore ? d->score.displayScore : d->score.hiscore;
	if (l && l->i[0] != hs) {
		l->i[0] = hs;
		drawVmEvent2(d->uiHiScore, 2);
	}
	l = getComponentOpt(DRAW_VM_LOCALS, d->uiScore);
	if (l && l->i[0] != d->score.displayScore) {
		l->i[0] = d->score.displayScore;
		drawVmEvent2(d->uiScore, 2);
	}
	
	l = getComponentOpt(DRAW_VM_LOCALS, d->uiFps);
	if (l) {
		l->i[0] = fps;
		drawVmEvent2(d->uiFps, 2);
	}
	l = getComponentOpt(DRAW_VM_LOCALS, d->uiPoints);
	if (l && l->i[0] != d->uiPoints) {
		l->i[0] = d->score.pointItems;
		drawVmEvent2(d->uiPoints, 2);
	}
	l = getComponentOpt(DRAW_VM_LOCALS, d->uiGraze);
	if (l && l->i[0] != d->uiGraze) {
		l->i[0] = d->score.graze;
		drawVmEvent2(d->uiGraze, 2);
	}

	if (d->shakeDuration > 0 && d->shakeTime < d->shakeDuration) {
		float intensity = lerp(d->shakeIntensity1, d->shakeIntensity2, d->shakeTime / d->shakeDuration);
		d->shakeTime += gameSpeed;
		camX = 0 + randomFloat(-1, 1) * intensity;
		camY = 224 + randomFloat(-1, 1) * intensity;
	} else {
		camX = 0; /* Origin in the middle */
		camY = 224; /* Origin at the top */
	}

#ifndef RELEASE
	drawVmDelete(d->uiDebug);
	d->uiDebug = newEntity();
	struct Transform *tf = newComponent(TRANSFORM, d->uiDebug);
	tf->x = -400;
	tf->y = 400;
	drawVmNew(d->uiDebug, "ascii1");
	l = getComponentOpt(DRAW_VM_LOCALS, d->uiDebug);
	snprintf(d->debugBuf, 512, " phys: %.2f\nphysEngine: %.2f\nnorm: %.2f\nlate: %.2f\nui/dvm: %.2f\ndraw: %.2f\ntotal: %.2f\nbullet: %d\n",
		updateTiming.phys, updateTiming.physEngine, updateTiming.norm, updateTiming.late, updateTiming.ui, updateTiming.draw, updateTiming.total, clCount(DAN_BULLET));
	l->str[0] = d->debugBuf;
#endif
}

static void danmakuEvent(void *arg, struct Event *ev) {
	struct Danmaku *d = arg;
	if (ev->type == EVENT_START_SCENE) {
		if (!strcmp(sceneName, "@danmaku")) {
			danmakuStart(d);
		}
	} else if (ev->type == EVENT_END_SCENE) {
		if (d->active)
			danmakuEnd(d);
	}
#ifndef RELEASE
	if (ev->type == EVENT_KEY && d->active) {
		bool diffch = false;
		if (ev->param == SDLK_PAGEUP && ev->param2 && d->state.difficulty < D_LUNATIC) {
			diffch = true;
			d->state.difficulty += 1;
		} else if (ev->param == SDLK_PAGEDOWN && ev->param2 && d->state.difficulty > D_EASY) {
			diffch = true;
			d->state.difficulty -= 1;
		}
		if (diffch) {
			struct IchigoLocals *l = getComponentOpt(DRAW_VM_LOCALS, d->uiDifficulty);
			if (l) {
				l->i[0] = d->state.difficulty;
			}
			drawVmEvent2(d->uiDifficulty, 2);
		}
	}
#endif
}

void danmakuSwitch(enum LoadingType lt, const char *script) {
	struct Danmaku *d = danmaku;
	struct DanmakuState *dsp = &d->state;
	if (script) {
		strncpy(d->script, script, 64);
		dsp->stage = d->nStages;
	} else {
		snprintf(d->script, 64, "st%.02d", dsp->stage);
	}
	swScene(lt, "@danmaku");
}

void danmakuResetState(void) {
	struct DanmakuState *s = &danmaku->state;
	s->stage = 0;
	s->replay = false;
	s->spellId = 0;
	s->practice = false;
	s->power = 0;
	s->lives = 2;
	s->lifePieces = 0;
	s->continuesUsed = 0;
	s->score = 0;
	s->gauge = 0;
	s->piv = 0;
}

void danmakuInit(struct Redefined *ri) {
	struct Danmaku *d = &ri->danmaku;
	danmaku = d;
	ichigoInit(&d->danScript, "dan");
	ichigoSetInstrTable(&d->danScript, globalAlloc(DAN_MAX_INSTR * sizeof(IchigoInstr *)), DAN_MAX_INSTR);
	ichigoSetVarTable(&d->danScript, globalAlloc(DAN_MAX_VAR * sizeof(struct IchigoVar)), DAN_MAX_VAR);
	ichigoBindLocals(&d->danScript);

	danmakuSetInstr(1, i_end);
	danmakuSetInstr(2, i_modeLerp);
	danmakuSetInstr(3, i_normalizeRot);
	danmakuSetInstr(4, i_diffI);
	danmakuSetInstr(5, i_diffF);
	danmakuSetInstr(6, i_diffWait);
	danmakuSetInstr(7, i_sfx);
	danmakuSetInstr(8, i_bgm);
	danmakuSetInstr(9, i_bgmStop);
	danmakuSetInstr(27, i_shake);
	danmakuSetInstr(142, i_sfxStop);

	// init stuff
	danmakuSetInstr(180, i_initSfxLoad);
	danmakuSetInstr(181, i_initSfxSet);
	danmakuSetInstr(184, i_initStages);

	addUpdate(UPDATE_NORM, danmakuUpdate, d);
	addInputHandler(EVENT_START_SCENE | EVENT_END_SCENE | EVENT_KEY, danmakuEvent, d);

	for (int i = 0; i < 128; i++) {
		d->sfx[i].channel = 0xFFFF;
	}

	replayInit(d);
	scoreInit(d);
	danPlayerInit(d);
	enemyInit(d);
	bulletMgrInit(d);
	bulletInit(d);
	danBgInit(d);
	bossInit(d);
	laserInit(d);
	itemInit(d);

	/* Call init script */
	struct IchigoVm initVm;
	ichigoVmNew(&d->danScript, &initVm, 0);
	ichigoAddFile(&d->danScript, "danmaku");
	ichigoVmExec(&initVm, "initialize", "");
	ichigoVmUpdate(&initVm, 1);
	ichigoVmDelete(&initVm);
	ichigoClear(&d->danScript);
}

void danmakuFini(struct Redefined *ri) {
	struct Danmaku *d = &ri->danmaku;
	itemFini(d);
	laserFini(d);
	bossFini(d);
	danBgFini(d);
	bulletFini(d);
	bulletMgrFini(d);
	enemyFini(d);
	danPlayerFini(d);
	ichigoFini(&d->danScript);
	scoreFini(d);
	replayFini(d);
	danmaku = NULL;
}
