#include "redefined.h"

void danBgUpdate(void *arg) {
	struct DanBgController *b = arg;
	if (!danmaku->active) {
		return;
	}

	if (b->posInterp.endTime) {
		interpUpdateBezier(&b->posInterp, 3, &b->camX, gameSpeed, b->posEx1, b->posEx2);
		if (b->posInterp.time >= b->posInterp.endTime) {
			b->posInterp.endTime = 0;
		}
	}
	if (b->dirInterp.endTime) {
		interpUpdateBezier(&b->dirInterp, 3, &b->dirX, gameSpeed, b->dirEx1, b->dirEx2);
		if (b->dirInterp.time >= b->dirInterp.endTime) {
			b->dirInterp.endTime = 0;
		}
	}
	if (b->upInterp.endTime) {
		interpUpdate(&b->upInterp, 3, &b->upX, gameSpeed);
		if (b->upInterp.time >= b->upInterp.endTime) {
			b->upInterp.endTime = 0;
		}
	}
	if (b->fovInterp.endTime) {
		interpUpdate(&b->fovInterp, 1, &b->fov, gameSpeed);
		if (b->fovInterp.time >= b->fovInterp.endTime) {
			b->fovInterp.endTime = 0;
		}
	}
	if (b->fogColorInterp.endTime) {
		interpUpdate(&b->fogColorInterp, 3, &b->fogR, gameSpeed);
		if (b->fogColorInterp.time >= b->fogColorInterp.endTime) {
			b->fogColorInterp.endTime = 0;
		}
	}
	if (b->fogDistInterp.endTime) {
		interpUpdate(&b->fogDistInterp, 2, &b->fogNear, gameSpeed);
		if (b->fogDistInterp.time >= b->fogDistInterp.endTime) {
			b->fogDistInterp.endTime = 0;
		}
	}

	cam3DLook(b->camX, b->camY, b->camZ, b->dirX, b->dirY, b->dirZ, b->upX, b->upY, b->upZ);
	cam3DFov = b->fov;
	fogColor = ((int)(b->fogR * 255) & 0xFF) << 16 |
		((int)(b->fogG * 255) & 0xFF) << 8 |
		((int)(b->fogB * 255) & 0xFF) << 0;
	fogMin = b->fogNear;
	fogMax = b->fogFar;
}

/* Danmaku script fns */
static void i_bgFile(struct IchigoVm *vm) {
	struct DanBgController *bg = &danmaku->bg;
	uint16_t strLen;
	ichigoAddFile(&bg->bgScript, ichigoGetString(&strLen, vm, 0));
}
static void i_bgCall(struct IchigoVm *vm) {
	struct DanBgController *bg = &danmaku->bg;
	struct IchigoVm *bgVm = getComponentOpt(ICHIGO_VM, bg->bgVm);
	if (bgVm) {
		uint16_t strLen;
		ichigoVmExec(bgVm, ichigoGetString(&strLen, vm, 0), "");
	}
}

/* Bg script fns */
#define GET_BG() struct DanBgController *bg = &danmaku->bg
static void ibg_pos(struct IchigoVm *vm) {
	GET_BG();
	bg->camX = ichigoGetFloat(vm, 0);
	bg->camY = ichigoGetFloat(vm, 1);
	bg->camZ = ichigoGetFloat(vm, 2);
	bg->posInterp.endTime = 0;
}
static void ibg_posTime(struct IchigoVm *vm) {
	GET_BG();
	float goal[3] = { ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3), ichigoGetFloat(vm, 4) };
	interpStart(&bg->posInterp, ichigoGetInt(vm, 1), 3, &bg->camX, goal, ichigoGetFloat(vm, 0));
}
static void ibg_dir(struct IchigoVm *vm) {
	GET_BG();
	bg->dirX = ichigoGetFloat(vm, 0);
	bg->dirY = ichigoGetFloat(vm, 1);
	bg->dirZ = ichigoGetFloat(vm, 2);
	bg->dirInterp.endTime = 0;
}
static void ibg_dirTime(struct IchigoVm *vm) {
	GET_BG();
	float goal[3] = { ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3), ichigoGetFloat(vm, 4) };
	interpStart(&bg->dirInterp, ichigoGetInt(vm, 1), 3, &bg->dirX, goal, ichigoGetFloat(vm, 0));
}
static void ibg_up(struct IchigoVm *vm) {
	GET_BG();
	bg->upX = ichigoGetFloat(vm, 0);
	bg->upY = ichigoGetFloat(vm, 1);
	bg->upZ = ichigoGetFloat(vm, 2);
	bg->upInterp.endTime = 0;
}
static void ibg_upTime(struct IchigoVm *vm) {
	GET_BG();
	float goal[3] = { ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3), ichigoGetFloat(vm, 4) };
	interpStart(&bg->upInterp, ichigoGetInt(vm, 1), 3, &bg->upX, goal, ichigoGetFloat(vm, 0));
}
static void ibg_fov(struct IchigoVm *vm) {
	GET_BG();
	bg->fov = ichigoGetFloat(vm, 0);
	bg->fovInterp.endTime = 0;
}
static void ibg_fovTime(struct IchigoVm *vm) {
	GET_BG();
	float goal = ichigoGetFloat(vm, 2);
	interpStart(&bg->fovInterp, ichigoGetInt(vm, 1), 1, &bg->fov, &goal, ichigoGetFloat(vm, 0));
}
static void ibg_posBezier(struct IchigoVm *vm) {
	GET_BG();
	float time = ichigoGetFloat(vm, 0);
	bg->posEx1[0] = ichigoGetFloat(vm, 1);
	bg->posEx1[1] = ichigoGetFloat(vm, 2);
	bg->posEx1[2] = ichigoGetFloat(vm, 3);
	float goal[3] = {
		ichigoGetFloat(vm, 4),
		ichigoGetFloat(vm, 5),
		ichigoGetFloat(vm, 6)
	};
	bg->posEx2[0] = ichigoGetFloat(vm, 7);
	bg->posEx2[1] = ichigoGetFloat(vm, 8);
	bg->posEx2[2] = ichigoGetFloat(vm, 9);
	interpStart(&bg->posInterp, INTERP_BEZIER, 3, &bg->camX, goal, time);
}
static void ibg_dirBezier(struct IchigoVm *vm) {
	GET_BG();
	float time = ichigoGetFloat(vm, 0);
	bg->dirEx1[0] = ichigoGetFloat(vm, 1);
	bg->dirEx1[1] = ichigoGetFloat(vm, 2);
	bg->dirEx1[2] = ichigoGetFloat(vm, 3);
	float goal[3] = {
		ichigoGetFloat(vm, 4),
		ichigoGetFloat(vm, 5),
		ichigoGetFloat(vm, 6)
	};
	bg->dirEx2[0] = ichigoGetFloat(vm, 7);
	bg->dirEx2[1] = ichigoGetFloat(vm, 8);
	bg->dirEx2[2] = ichigoGetFloat(vm, 9);
	interpStart(&bg->dirInterp, INTERP_BEZIER, 3, &bg->dirX, goal, time);
}
static void ibg_dvmNew(struct IchigoVm *vm) {
	GET_BG();
	entity_t en = newEntity();
	uint16_t strLen;
	struct DrawVm *d = drawVmNew(en, ichigoGetString(&strLen, vm, 0));
	if (d) {
		struct Transform *tf = newComponent(TRANSFORM, en);
		tf->x = ichigoGetFloat(vm, 1);
		tf->y = ichigoGetFloat(vm, 2);
		tf->z = ichigoGetFloat(vm, 3);
		d->flags |= DVM_FLAG_DELETE_ALL;
		
		entity_t *pEn = vecInsert(&bg->dvms, -1);
		*pEn = en;
		logDebug("Bg: Created dvm %s\n", d->mainFn);
	}
}
static void ibg_dvmDelete(struct IchigoVm *vm) {
	GET_BG();
	int slot = ichigoGetInt(vm, 0);
	entity_t *pEn = vecAt(&bg->dvms, slot);
	drawVmDelete(*pEn);
	vecDelete(&bg->dvms, slot);
}
static void ibg_dvmEvent(struct IchigoVm *vm) {
	GET_BG();
	int slot = ichigoGetInt(vm, 0);
	entity_t *pEn = vecAt(&bg->dvms, slot);
	drawVmEvent2(*pEn, ichigoGetInt(vm, 1));
}
static void ibg_dvmEventAll(struct IchigoVm *vm) {
	GET_BG();
	int ev = ichigoGetInt(vm, 0);
	for (int i = 0; i < vecCount(&bg->dvms); i++) {
		entity_t *pEn = vecAt(&bg->dvms, i);
		drawVmEvent2(*pEn, ev);
	}
}
static void ibg_dvmFile(struct IchigoVm *vm) {
	uint16_t strLen;
	drawVmAddFile(ichigoGetString(&strLen, vm, 0));
}
static void ibg_modelFile(struct IchigoVm *vm) {
	uint16_t strLen;
	loadModelFile(ichigoGetString(&strLen, vm, 0));
}
static void ibg_fogColor(struct IchigoVm *vm) {
	GET_BG();
	bg->fogR = ichigoGetFloat(vm, 0);
	bg->fogG = ichigoGetFloat(vm, 1);
	bg->fogB = ichigoGetFloat(vm, 2);
	bg->fogColorInterp.endTime = 0;
}
static void ibg_fogDist(struct IchigoVm *vm) {
	GET_BG();
	bg->fogNear = ichigoGetFloat(vm, 0);
	bg->fogFar = ichigoGetFloat(vm, 1);
	bg->fogDistInterp.endTime = 0;
}
static void ibg_fogColorTime(struct IchigoVm *vm) {
	GET_BG();
	float goal[3] = { ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3), ichigoGetFloat(vm, 4) };
	interpStart(&bg->fogColorInterp, ichigoGetInt(vm, 1), 3, &bg->fogR, goal, ichigoGetFloat(vm, 0));
}
static void ibg_fogDistTime(struct IchigoVm *vm) {
	GET_BG();
	float goal[2] = { ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3) };
	interpStart(&bg->fogColorInterp, ichigoGetInt(vm, 1), 2, &bg->fogNear, goal, ichigoGetFloat(vm, 0));
}
static void ibg_clearColor(struct IchigoVm *vm) {
	int r = (int)(ichigoGetFloat(vm, 0) * 255) & 0xFF;
	int g = (int)(ichigoGetFloat(vm, 1) * 255) & 0xFF;
	int b = (int)(ichigoGetFloat(vm, 2) * 255) & 0xFF;
	clearColor = (r << 16) | (g << 8) | (b);
}

static void rgbToFloat(float *r, float *g, float *b, uint32_t c) {
	*r = ((c >> 16) & 0xFF) / 128.0f;
	*g = ((c >> 8) & 0xFF) / 128.0f;
	*b = (c & 0xFF) / 128.0f;
}
static void ibg_dirLight(struct IchigoVm *vm) {
	dirLight.x = ichigoGetFloat(vm, 0);
	dirLight.y = ichigoGetFloat(vm, 1);
	dirLight.z = ichigoGetFloat(vm, 2);
	rgbToFloat(&dirLight.ambientR, &dirLight.ambientG, &dirLight.ambientB, ichigoGetInt(vm, 3));
	rgbToFloat(&dirLight.diffuseR, &dirLight.diffuseG, &dirLight.diffuseB, ichigoGetInt(vm, 4));
	rgbToFloat(&dirLight.specularR, &dirLight.specularG, &dirLight.specularB, ichigoGetInt(vm, 5));
}

static void ibg_dvmRotate(struct IchigoVm *vm) {
	GET_BG();
	int slot = ichigoGetInt(vm, 0);
	entity_t *pEn = vecAt(&bg->dvms, slot);
	struct Transform *tf = getComponentOpt(TRANSFORM, *pEn);
	if (tf)
		tfSetRotation3D(tf, ichigoGetFloat(vm, 1), ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3));
}

static int ibg_getVarDVM_COUNT(struct IchigoVm *vm) {
	return vecCount(&danmaku->bg.dvms);
}

static IchigoInstr *danBgInstrs[64] = {
	[1] = ibg_pos,
	[2] = ibg_posTime,
	[3] = ibg_dir,
	[4] = ibg_dirTime,
	[5] = ibg_up,
	[6] = ibg_upTime,
	[7] = ibg_fov,
	[8] = ibg_fovTime,
	[9] = ibg_posBezier,
	[10] = ibg_dirBezier,
	[11] = ibg_dvmNew,
	[12] = ibg_dvmDelete,
	[13] = ibg_dvmEvent,
	[14] = ibg_dvmFile,
	[15] = ibg_modelFile,
	[16] = ibg_fogColor,
	[17] = ibg_fogDist,
	[18] = ibg_fogColorTime,
	[19] = ibg_fogDistTime,
	[20] = ibg_clearColor,
	[21] = ibg_dvmEventAll,
	[22] = ibg_dirLight,
	[23] = ibg_dvmRotate,
	[24] = i_modeLerp
};

void danBgStart(struct Danmaku *d) {
	d->bg.camX = d->bg.camY = d->bg.camZ = 0;
	d->bg.dirX = d->bg.dirZ = 0;
	d->bg.dirY = 1;
	d->bg.upX = d->bg.upY = 0;
	d->bg.upZ = 1;
	d->bg.fov = DEG2RAD(60);
	d->bg.fogR = d->bg.fogG = d->bg.fogB = 1;
	d->bg.fogNear = 1024;
	d->bg.fogFar = 2048;

	d->bg.posInterp.endTime = 0;
	d->bg.dirInterp.endTime = 0;
	d->bg.upInterp.endTime = 0;
	d->bg.fovInterp.endTime = 0;
	d->bg.fogColorInterp.endTime = 0;
	d->bg.fogDistInterp.endTime = 0;

	d->bg.bgVm = newEntity();
	struct IchigoVm *bgVm = newComponent(ICHIGO_VM, d->bg.bgVm);
	bgVm->is = &d->bg.bgScript;

	vecCreate(&d->bg.dvms, sizeof(entity_t));
}
void danBgEnd(struct Danmaku *d) {
	vecDestroy(&d->bg.dvms);
	ichigoClear(&d->bg.bgScript);
}

void danBgInit(struct Danmaku *d) {
	ichigoInit(&d->bg.bgScript, "danbg");
	ichigoSetInstrTable(&d->bg.bgScript, danBgInstrs, 64);
	ichigoSetVarTable(&d->bg.bgScript, d->bg.bgVars, 64);
	ichigoBindLocals(&d->bg.bgScript);

	d->bg.bgVars[23].regType = REG_INT;
	d->bg.bgVars[23].get.i = ibg_getVarDVM_COUNT;

	danmakuSetInstr(140, i_bgFile);
	danmakuSetInstr(141, i_bgCall);

	memset(&dirLight, 0, sizeof(dirLight));
	dirLight.ambientR = dirLight.ambientG = dirLight.ambientB = 1.0f;

	addUpdate(UPDATE_LATE, danBgUpdate, &d->bg);
}
void danBgFini(struct Danmaku *d) {
	removeUpdate(UPDATE_LATE, danBgUpdate);
}