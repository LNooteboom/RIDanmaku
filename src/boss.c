#include "redefined.h"
#include <string.h>

static void bossStart(struct BossController *bc, entity_t en) {
	bc->boss = en;
}

static void setStars(struct BossController *bc, int newStars) {
	if (newStars > bc->stars) {
		for (int i = bc->stars; i < newStars; i++) {
			bc->starDvm[i] = newEntity();
			drawVmNew(bc->starDvm[i], "uiBossStar");
			struct IchigoLocals *l = getComponent(DRAW_VM_LOCALS, bc->starDvm[i]);
			l->i[0] = i;
		}
		bc->stars = newStars;
	} else if (newStars < bc->stars) {
		for (int i = newStars; i < bc->stars; i++) {
			drawVmDelete(bc->starDvm[i]);
			bc->starDvm[i] = 0;
		}
		bc->stars = newStars;
	}
}


static void bossEnd2(struct BossController *bc) {
	bc->boss = 0;
	if (bc->spellNameDvm) {
		drawVmDelete(bc->spellBgDvm);
		bc->spellBgDvm = 0;
	}
	if (bc->spellCircleDvm) {
		drawVmDelete(bc->spellCircleDvm);
		bc->spellCircleDvm = 0;
	}
	if (bc->spellNameDvm) {
		drawVmDelete(bc->spellNameDvm);
		bc->spellNameDvm = 0;
	}
	bc->spellPortraitDvm = 0;
	if (bc->distortDvm) {
		drawVmDelete(bc->distortDvm);
		bc->distortDvm = 0;
	}
	if (bc->nameDvm) {
		drawVmDelete(bc->nameDvm);
		bc->nameDvm = 0;
	}
	if (bc->timeDvm) {
		drawVmDelete(bc->timeDvm);
		bc->timeDvm = 0;
	}

	for (int i = 0; i < 17; i++) {
		if (bc->lifeDvm[i]) {
			drawVmDelete(bc->lifeDvm[i]);
			bc->lifeDvm[i] = 0;
		}
		if (i < 16)
			bc->lifeMarkers[i] = 0;
	}

	setStars(bc, 0);
}

static void updateLifeBarPiece(entity_t *dvm, float life, float maxLife, float marker, float prevMarker, int color) {
	if (life < prevMarker) {
		if (*dvm) {
			drawVmDelete(*dvm);
			*dvm = 0;
		}
	} else {
		if (!*dvm) {
			*dvm = newEntity();
			drawVmNew(*dvm, "uiBossLife");
		} else {
			drawVmEvent2(*dvm, 2);
		}
		struct IchigoLocals *l = getComponent(DRAW_VM_LOCALS, *dvm);
		l->i[0] = color;
		l->f[0] = prevMarker / maxLife;
		l->f[1] = fminf(life, marker) / maxLife - l->f[0];
	}
}
static void updateLifeBar(struct BossController *bc, struct Enemy *e) {
	float maxLife = e->maxLife;
	float life = fmaxf(0, lerp(bc->curLife, e->life / maxLife, 0.1f));
	bc->curLife = life;
	life *= maxLife;
	float marker = 0;
	for (int i = 0; i < 16; i++) {
		if (bc->lifeMarkers[i] > 0) {
			updateLifeBarPiece(&bc->lifeDvm[i], life, maxLife, bc->lifeMarkers[i], marker, bc->lifeColors[i]);
			marker = bc->lifeMarkers[i];
		}
	}
	updateLifeBarPiece(&bc->lifeDvm[16], life, maxLife, maxLife, marker, 0xFFFFFFFF);
}
static void updateTime(struct BossController *bc, struct Enemy *e) {
	struct IchigoLocals *l = getComponentOpt(DRAW_VM_LOCALS, bc->timeDvm);
	float time = fmaxf(0, e->interrupts[bc->timeInterrupt].time - e->time);
	if (time > 0) {
		if (l) {
			if (time < 99.99f * 60)
				l->i[0] = time * (100.0f / 60);
			else
				l->i[0] = 9999;
			drawVmEvent2(bc->timeDvm, 2);
		}
		int iTime = time;
		if (iTime % 60 == 0 && iTime <= 600) {
			danmakuPlaySfx(iTime <= 300 ? 6 : 5);
		}
	}
}

static void bossUpdate(void *arg) {
	struct BossController *bc = arg;
	struct Transform *tf;
	if (!bc->boss) {
		return;
	}
	if (!(tf = getComponentOpt(TRANSFORM, bc->boss))) {
		bossEnd2(bc);
		return;
	}

	struct Transform *tf2 = getComponentOpt(TRANSFORM, bc->spellCircleDvm);
	if (tf2) {
		tf2->x = tf->x;
		tf2->y = tf->y;
	}
	tf2 = getComponentOpt(TRANSFORM, bc->distortDvm);
	if (tf2) {
		tf2->x = tf->x;
		tf2->y = tf->y;
	}

	struct Enemy *bossEnemy = getComponent(DAN_ENEMY, bc->boss);
	updateLifeBar(bc, bossEnemy);
	updateTime(bc, bossEnemy);
}


void bossEnd(struct Danmaku *game) {
	struct BossController *bc = &game->boss;
	bossEnd2(bc);
}

void bossSetTimer(struct Enemy *e, int irq) {
	struct BossController *bc = &danmaku->boss;
	if (e->interrupts[irq].time > 0) {
		bc->timeInterrupt = irq;
		if (!bc->timeDvm) {
			bc->timeDvm = newEntity();
			drawVmNew(bc->timeDvm, "uiBossTime");
		}
	}
}


static void i_spell(struct IchigoVm *vm) {
	struct BossController *bc = &danmaku->boss;
	bc->spellIdx = ichigoGetInt(vm, 0);
	bc->spellTime = ichigoGetInt(vm, 1);
	bc->spellBonus = ichigoGetInt(vm, 2);
	
	uint16_t strLen;
	struct IchigoLocals *l;
	bc->spellBgDvm = newEntity();
	const char *bg = ichigoGetString(&strLen, vm, 3);
	if (strLen) {
		drawVmNew(bc->spellBgDvm, bg);
	}

	/*bc->spellPortraitDvm = newEntity();
	drawVmNew(bc->spellPortraitDvm, "spellPortrait");
	l = getComponentOpt(DRAW_VM_LOCALS, danmaku->boss.spellPortraitDvm);
	if (l) {
		l->str[0] = ichigoGetString(&strLen, vm, 3);
	}*/
	
	bc->spellNameDvm = newEntity();
	drawVmNew(bc->spellNameDvm, "spellName");
	l = getComponentOpt(DRAW_VM_LOCALS, danmaku->boss.spellNameDvm);
	if (l) {
		//l->str[0] = ichigoGetString(&strLen, vm, 4);
		l->str[0] = &bc->spells[bc->spellIdx].name[0];
	}

	struct Transform *bossTf = getComponentOpt(TRANSFORM, bc->boss);
	if (bossTf) {
		bc->spellCircleDvm = newEntity();
		struct Transform *tf = newComponent(TRANSFORM, bc->spellCircleDvm);
		tf->x = bossTf->x;
		tf->y = bossTf->y;
		struct DrawVm *dvm = drawVmNew(bc->spellCircleDvm, "effSpellCircle");
		dvm->flags |= DVM_FLAG_DELETE_ALL;
		struct IchigoLocals *l = getComponentOpt(DRAW_VM_LOCALS, bc->spellCircleDvm);
		if (l) {
			l->f[0] = bc->spellTime;
		}
	}

	drawVmNew(newEntity(), "effSpellDeclare");

	struct Enemy *e = getComponent(DAN_ENEMY, vm->en);
	e->flags |= ENM_RESIST;

	bc->capture = 1;
	danmakuPlaySfx(7);
}

static void i_spellEnd(struct IchigoVm *vm) {
	struct BossController *bc = &danmaku->boss;
	drawVmDelete(bc->spellNameDvm);
	drawVmDelete(bc->spellCircleDvm);
	drawVmDelete(bc->spellBgDvm);
	struct Enemy *e = getComponent(DAN_ENEMY, vm->en);
	e->flags &= ~ENM_RESIST;
}

static void i_setBoss(struct IchigoVm *vm) {
	struct BossController *bc = &danmaku->boss;
	int type = ichigoGetInt(vm, 0);
	if (type == -1) {
		bossEnd2(bc);
	} else  if (type == 0) {
		bossStart(bc, vm->en);

		uint16_t strLen;
		const char *name = ichigoGetString(&strLen, vm, 1);
		bc->nameDvm = newEntity();
		drawVmNew(bc->nameDvm, "uiBossName");
		struct IchigoLocals *l = getComponent(DRAW_VM_LOCALS, bc->nameDvm);
		l->str[0] = name;
	}
}

static void i_distort(struct IchigoVm *vm) {
	struct BossController *bc = &danmaku->boss;
	if (bc->distortDvm) {
		drawVmDelete(bc->distortDvm);
		bc->distortDvm = 0;
	}
	int radius = ichigoGetInt(vm, 0);
	if (radius) {
		bc->distortDvm = newEntity();


		struct Transform *tf2 = newComponent(TRANSFORM, bc->distortDvm);
		struct Transform *tf = getComponentOpt(TRANSFORM, bc->boss);
		if (tf) {
			tf2->x = tf->x;
			tf2->y = tf->y;
		}

		struct DrawVm *dvm = drawVmNew(bc->distortDvm, "effDistort");
		dvm->flags |= DVM_FLAG_DELETE_ALL;
		struct IchigoLocals *l = getComponentOpt(DRAW_VM_LOCALS, bc->distortDvm);
		if (l) {
			l->i[0] = radius;
			l->i[1] = ichigoGetInt(vm, 1);
		}
	}
}

static void i_lifeMarker(struct IchigoVm *vm) {
	struct BossController *bc = &danmaku->boss;
	int slot = ichigoGetInt(vm, 0);
	bc->lifeMarkers[slot] = ichigoGetFloat(vm, 1);
	bc->lifeColors[slot] = ichigoGetInt(vm, 2);
}

static void i_timerReset(struct IchigoVm *vm) {
	struct BossController *bc = &danmaku->boss;
	if (bc->timeDvm) {
		drawVmDelete(bc->timeDvm);
		bc->timeDvm = 0;
		bc->timeInterrupt = -1;
	}
}

static void i_setTimeout(struct IchigoVm *vm) {
	struct Enemy *e = getComponent(DAN_ENEMY, vm->en);
	int slot = ichigoGetInt(vm, 0);
	uint16_t strLen;
	const char *fn = ichigoGetString(&strLen, vm, 1);
	if (strLen) {
		e->interrupts[slot].timeHandler = fn;
	}
}

static void i_initSpell(struct IchigoVm *vm) {
	int idx = ichigoGetInt(vm, 0);
	int diffMask = ichigoGetInt(vm, 1);
	int stage = ichigoGetInt(vm, 2);
	uint16_t nameLen;
	const char *name = ichigoGetString(&nameLen, vm, 3);
	if (nameLen > 63)
		nameLen = 63;
	for (int i = 0; i < 5; i++) {
		if (diffMask & (1 << i)) {
			struct SpellInfo *s = &danmaku->boss.spells[idx];
			s->stage = stage;
			s->difficulty = i;
			memcpy(s->name, name, nameLen);
			s->name[nameLen] = 0;
			idx += 1;
		}
	}
}

static void i_stars(struct IchigoVm *vm) {
	setStars(&danmaku->boss, ichigoGetInt(vm, 0));
}

static float i_getVarBOSS_X(struct IchigoVm *vm) {
	struct Transform *tf = getComponentOpt(TRANSFORM, danmaku->boss.boss);
	return tf ? tf->x : 0;
}
static float i_getVarBOSS_Y(struct IchigoVm *vm) {
	struct Transform *tf = getComponentOpt(TRANSFORM, danmaku->boss.boss);
	return tf ? tf->y : 0;
}
static float i_getVarBOSS_Z(struct IchigoVm *vm) {
	struct Transform *tf = getComponentOpt(TRANSFORM, danmaku->boss.boss);
	return tf ? tf->z : 0;
}
static int i_getVarBOSS_ALIVE(struct IchigoVm *vm) {
	return danmaku->boss.boss ? 1 : 0;
}
static int i_getVarCAPTURE(struct IchigoVm *vm) {
	return danmaku->boss.capture;
}
static void i_setVarCAPTURE(struct IchigoVm *vm, int val) {
	danmaku->boss.capture = val;
}

void bossInit(struct Danmaku *game) {
	danmakuSetInstr(150, i_spell);
	danmakuSetInstr(151, i_spellEnd);
	danmakuSetInstr(152, i_setBoss);
	danmakuSetInstr(153, i_distort);
	danmakuSetInstr(154, i_lifeMarker);
	danmakuSetInstr(155, i_timerReset);
	danmakuSetInstr(158, i_setTimeout);
	danmakuSetInstr(159, i_stars);
	danmakuSetInstr(183, i_initSpell);

	danmakuSetVar(105, REG_FLOAT, i_getVarBOSS_X, NULL);
	danmakuSetVar(106, REG_FLOAT, i_getVarBOSS_Y, NULL);
	danmakuSetVar(107, REG_FLOAT, i_getVarBOSS_Z, NULL);
	danmakuSetVar(108, REG_INT, i_getVarBOSS_ALIVE, NULL);
	danmakuSetVar(109, REG_INT, i_getVarCAPTURE, i_setVarCAPTURE);

	addUpdate(UPDATE_NORM, bossUpdate, &game->boss);
	game->boss.spells = globalAlloc(sizeof(struct SpellInfo) * BOSS_MAX_SPELL);
}
void bossFini(struct Danmaku *game) {
	globalDealloc(game->boss.spells);
	removeUpdate(UPDATE_NORM, bossUpdate);
	drawShaderDelete(game->boss.distortShader);
}