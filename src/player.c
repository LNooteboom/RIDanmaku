#include "redefined.h"
#include <basics.h>
#include <audio.h>
#include <input.h>

#define DAN_MAX_LIVES 8
#define DAN_LIFE_PIECES 5

static void addPower(struct DanPlayerController *p, int power) {
	struct DanmakuState *s = &danmaku->state;
	bool maxPower = s->power == p->maxPower;
	s->power = min(s->power + power, p->maxPower);

	if (!maxPower && s->power % 100 == 0)
		danmakuPlaySfx(19);
}

void danPlayerAddPower(void) {
	struct DanPlayerController *p = &danmaku->player;
	addPower(p, p->powerItemValue);
}

void danPlayerAddPowerBig(void) {
	addPower(&danmaku->player, 100);
}

void danPlayerAddLife(void) {
	struct DanmakuState *s = &danmaku->state;
	if (s->lives < DAN_MAX_LIVES) {
		s->lives += 1;
		if (s->lives == DAN_MAX_LIVES) {
			s->lifePieces = 0;
		}
		danmakuPlaySfx(20);
	}
}

void danPlayerAddLifepiece(void) {
	struct DanmakuState *s = &danmaku->state;
	if (s->lives < DAN_MAX_LIVES) {
		if (s->lifePieces == DAN_LIFE_PIECES - 1) {
			s->lives += 1;
			s->lifePieces = 0;
			danmakuPlaySfx(20);
		} else {
			s->lifePieces += 1;
		}
	}
}

void danPlayerHit(void) {
	struct DanPlayerController *p = &danmaku->player;
	if (p->state == DP_STATE_NORM) {
		p->state = DP_STATE_DEATH_GRACE;
		p->time = 0;
		danmakuPlaySfx(17);
	} else if (p->state == DP_STATE_BOMB && p->onBombHit && !p->bombHit) {
		p->bombHit = true;
		struct IchigoVm *vm = getComponent(ICHIGO_VM, p->entity);
		ichigoVmExec(vm, p->onBombHit, "");
	}
}
void danPlayerGraze(void) {
	entity_t eff = newEntity();
	struct Transform *ptf = getComponent(TRANSFORM, danmaku->player.entity);
	struct Transform *tf = newComponent(TRANSFORM, eff);
	tf->x = ptf->x;
	tf->y = ptf->y;
	tf->z = ptf->z;
	struct DrawVm *d = drawVmNew(eff, "effGraze");
	d->flags |= DVM_FLAG_DELETE_ALL;
	danmakuPlaySfx(21);
	scoreGraze();
}

void danPlayerCreate(struct Danmaku *game, struct DanmakuState *state) {
	struct DanPlayerController *p = &game->player;
	char nameBuf[32];
	snprintf(nameBuf, 32, "pl%.02d%c", state->player, state->shotType + 'a');
	logDebug("Loading player script: %s\n", nameBuf);
	ichigoAddFile(&p->playerScript, nameBuf);

	p->entity = newEntity();
	struct Transform *tf = newComponent(TRANSFORM, p->entity);
	tf->x = 0;
	tf->y = 300;
	struct IchigoVm *vm = newComponent(ICHIGO_VM, p->entity);
	vm->is = &p->playerScript;
	struct IchigoLocals *l = newComponent(ICHIGO_LOCALS, p->entity);

	p->state = DP_STATE_NORM;
	p->speed = 3.75f;
	p->focusSpeed = 2.0f;
	p->hitbox = 1.5f;
	p->grazebox = 22.0f;
	p->maxPower = 400;
	p->powerItemValue = 1;
	p->magnetRange = 32;
	p->focusMagnetRange = 32;
	
	p->onBomb = NULL;
	p->onBombHit = NULL;
	p->bombPowerCost = 100;
	p->moveState = 0;
	p->oldMoveState = 0;

	ichigoVmExec(vm, "main", "");

	p->uiLives = newEntity();
	drawVmNew(p->uiLives, "uiLives");
	l = getComponent(DRAW_VM_LOCALS, p->uiLives);
	l->i[0] = state->lives;
	l->i[1] = DAN_MAX_LIVES;
	l->i[2] = state->lifePieces;

	p->uiPower = newEntity();
	drawVmNew(p->uiPower, "uiPower");
	l = getComponent(DRAW_VM_LOCALS, p->uiPower);
	l->i[0] = state->power + p->maxPower * 1000;
}

void danPlayerDestroy(struct Danmaku *game) {
	struct DanPlayerController *p = &game->player;
	deleteEntity(p->entity);
	ichigoClear(&p->playerScript);
	for (int i = 0; i < DAN_PLAYER_MAX_OPTION; i++) {
		if (p->options[i]) {
			deleteEntity(p->options[i]);
			p->options[i] = 0;
		}
	}
	if (p->focus1) {
		deleteEntity(p->focus1);
		p->focus1 = 0;
	}
	if (p->focus2) {
		deleteEntity(p->focus2);
		p->focus2 = 0;
	}

	deleteEntity(p->uiLives);
	deleteEntity(p->uiPower);
}

void danPlayerPos(float *x, float *y) {
	struct Transform *tf = getComponentOpt(TRANSFORM, danmaku->player.entity);
	if (tf) {
		*x = tf->x;
		*y = tf->y;
	}
}

static void updatePBullet(struct DanPlayerBullet *b) {
	struct Transform *tf = getComponent(TRANSFORM, b->entity);
	tf->x += b->speed * tf->rotReal * gameSpeed;
	tf->y += b->speed * tf->rotImag * gameSpeed;
	b->time += gameSpeed;
	float br = 32;
	if (tf->x < -192 - br || tf->x > 192 + br ||
		tf->y < -br || tf->y > 448 + br) {
		deleteEntity(b->entity);
	}
}

static void danPlayerPhysUpdate(void *arg) {
	if (!danmaku->active)
		return;

	struct DanPlayerController *p = arg;
	if (p->state != DP_STATE_DEATH) {
		int x = 0, y = 0;
		if (keyPressed(K_LEFT))
			x -= 1;
		if (keyPressed(K_RIGHT))
			x += 1;
		if (keyPressed(K_UP))
			y -= 1;
		if (keyPressed(K_DOWN))
			y += 1;

		float xv = 0, yv = 0;
		if (x || y) {
			normalize(&xv, &yv, x, y);
		} else if (gamepadConnected) {
			xv = getGamepadAxis(GP_AXIS_L_X);
			yv = getGamepadAxis(GP_AXIS_L_Y);
			float mag2 = xv * xv + yv * yv;
			if (mag2 > 1) {
				normalize(&xv, &yv, xv, yv);
			} else if (mag2 < 0.01f) {
				xv = yv = 0; /* Deadzone */
			}

		}
		float speed = keyPressed(K_FOCUS) ? p->focusSpeed : p->speed;
		xv *= speed;
		yv *= speed;

		float minSpeed = 0.8f;
		if (xv < -minSpeed)
			p->moveState = 1;
		else if (xv > minSpeed)
			p->moveState = 2;
		else
			p->moveState = 0;

		struct Transform *tf = getComponent(TRANSFORM, p->entity);
		tf->x = fmaxf(-192 + 6, fminf(192 - 6, tf->x + xv * gameSpeed));
		tf->y = fmaxf(16, fminf(448 - 24, tf->y + yv * gameSpeed));
	}

	/* Update player bullets */
	for (struct DanPlayerBullet *b = clBegin(DAN_PLAYER_BULLET); b; b = clNext(DAN_PLAYER_BULLET, b)) {
		updatePBullet(b);
	}

	danmaku->item.magnetRange = p->state != DP_STATE_DEATH && keyPressed(K_FOCUS) ? p->focusMagnetRange : p->magnetRange;
}

static void updateTarget(struct DanPlayerController *p, struct Transform *tf) {
	struct Enemy *target = getComponentOpt(DAN_ENEMY, p->targetEnemy);
	struct Transform *targetTf = NULL;
	if (!target) {
		/* Look for new target */
		float closestDist2 = 1000000;
		for (struct Enemy *e = clBegin(DAN_ENEMY); e; e = clNext(DAN_ENEMY, e)) {
			if (!e->hurtboxW || e->flags & ENM_NO_HURT)
				continue;
			struct Transform *etf = getComponent(TRANSFORM, e->entity);
			if (fabsf(etf->x) > 192 + 32 || etf->y < -32 || etf->y > 448 + 32)
				continue;
			float dx = tf->x - etf->x;
			float dy = tf->y - etf->y;
			float dst2 = dx * dx + dy * dy;
			if (dst2 < closestDist2) {
				target = e;
				targetTf = etf;
				closestDist2 = dst2;
			}
		}
		p->targetEnemy = target ? target->entity : 0;
	} else {
		targetTf = getComponent(TRANSFORM, target->entity);
	}

	if (targetTf) {
		p->targetX = targetTf->x;
		p->targetY = targetTf->y;
	}
}

static void updateFocus(struct DanPlayerController *p, struct Transform *tf) {
	if (p->state != DP_STATE_DEATH && keyPressed(K_FOCUS)) {
		if (!p->focusActive) {
			p->focusActive = true;
			if (p->focus1)
				deleteEntity(p->focus1);
			if (p->focus2)
				deleteEntity(p->focus2);

			p->focus1 = newEntity();
			struct Transform *ftf = newComponent(TRANSFORM, p->focus1);
			ftf->x = tf->x;
			ftf->y = tf->y;
			struct DrawVm *d = drawVmNew(p->focus1, "effFocus");
			d->flags |= DVM_FLAG_DELETE_ALL;

			p->focus2 = newEntity();
			ftf = newComponent(TRANSFORM, p->focus2);
			ftf->x = tf->x;
			ftf->y = tf->y;
			d = drawVmNew(p->focus2, "effFocus2");
			d->flags |= DVM_FLAG_DELETE_ALL;
		}
	} else {
		if (p->focusActive) {
			p->focusActive = false;
			drawVmDelete(p->focus1);
			drawVmDelete(p->focus2);
		}
	}
	struct Transform *ftf = getComponentOpt(TRANSFORM, p->focus1);
	if (ftf) {
		ftf->x = tf->x;
		ftf->y = tf->y;
	}
	ftf = getComponentOpt(TRANSFORM, p->focus2);
	if (ftf) {
		ftf->x = tf->x;
		ftf->y = tf->y;
	}
}

static void updateUi(struct DanPlayerController *p) {
	struct IchigoLocals *l = getComponentOpt(DRAW_VM_LOCALS, p->uiLives);
	struct DanmakuState *s = &danmaku->state;
	if (l) {
		if (l->i[0] != s->lives || l->i[2] != s->lifePieces) {
			l->i[0] = s->lives;
			l->i[2] = s->lifePieces;
			drawVmEvent2(p->uiLives, 2);
		}
	}

	l = getComponentOpt(DRAW_VM_LOCALS, p->uiPower);
	int pValue = s->power + p->maxPower * 1000;
	if (l && l->i[0] != pValue) {
		l->i[0] = pValue;
		drawVmEvent2(p->uiPower, 2);
	}
}

static void danPlayerUpdate(void *arg) {
	if (!danmaku->active)
		return;
	struct DanPlayerController *p = arg;
	struct DanmakuState *s = &danmaku->state;

	if (p->moveState != p->oldMoveState) {
		int ev = 0;
		switch (p->moveState) {
		case 0: /* Now straight */
			ev = p->oldMoveState == 1 ? 3 : 5;
			break;
		case 1:
			ev = 2;
			break;
		case 2:
			ev = 4;
			break;
		}
		drawVmEvent2(p->entity, ev);
		p->oldMoveState = p->moveState;
	}

	struct Transform *tf = getComponent(TRANSFORM, p->entity);
	updateTarget(p, tf);
	
	if (p->state == DP_STATE_DEATH_GRACE && p->time >= 8.0f) {
		p->state = DP_STATE_DEATH;
		p->time = 0;
		if (s->lives) {
			s->lives -= 1;
		} else {
			menuEndScreen(true, false);
		}
		danmaku->boss.capture = 0;
		drawVmDelete(p->entity);
	}
	if (p->state == DP_STATE_DEATH && p->time >= 20.0f) {
		p->state = DP_STATE_DEATH_RECOVER;
		p->time = 0;
		tf->x = 0;
		tf->y = 352;
		drawVmNew(danmaku->player.entity, p->mainDvm);
	}
	if (p->state == DP_STATE_DEATH || p->state == DP_STATE_DEATH_RECOVER) {
		bulletDeleteCircle(0, 224, 512, false, false);
	}
	if (p->state == DP_STATE_DEATH_RECOVER) {
		/* Death recovery blinking */
		struct DrawVm *d = getComponentOpt(DRAW_VM, danmaku->player.entity);
		if (p->time >= 120.0f) {
			if (d)
				d->colMode = COLOR1;
			p->state = DP_STATE_NORM;
			p->time = 0;
		} else if (d) {
			d->colMode = ((int)p->time) % 10 < 2 ? COLOR2 : COLOR1;
		}
	}

	if (!game->dialog.active &&
		(p->state == DP_STATE_NORM || p->state == DP_STATE_DEATH_GRACE) &&
		keyPressed(K_BOMB) &&
		s->power >= p->bombPowerCost &&
		p->onBomb) {

		p->state = DP_STATE_BOMB;
		p->time = 0;
		p->bombHit = false;
		s->power -= p->bombPowerCost;
		danmaku->boss.capture = 0;
		struct IchigoVm *vm = getComponent(ICHIGO_VM, p->entity);
		p->bombCorout = ichigoVmExec(vm, p->onBomb, "");
	}

	updateFocus(p, tf);
	updateUi(p);
	p->time += gameSpeed;
}

void danPlayerBulletHit(struct DanPlayerBullet *pb) {
	if (!(pb->flags & PB_PENETRATE)) {
		struct DrawVm *d = getComponentOpt(DRAW_VM, pb->entity);
		if (d) {
			drawVmEvent(d, DVM_EVENT_DELETE);
			//pb->damage = 0;
			removeComponent(DAN_PLAYER_BULLET, pb->entity);
		} else {
			deleteEntity(pb->entity);
		}
	}
}


static void ipl_log(struct IchigoVm *vm) {
	uint16_t strLen;
	logDebug("[DANPL] %s\n", ichigoGetString(&strLen, vm, 0));
}
static void ipl_hitbox(struct IchigoVm *vm) {
	danmaku->player.hitbox = ichigoGetFloat(vm, 0);
}
static void ipl_grazebox(struct IchigoVm *vm) {
	danmaku->player.grazebox = ichigoGetFloat(vm, 0);
}
static void ipl_onBomb(struct IchigoVm *vm) {
	uint16_t strLen;
	danmaku->player.onBomb = ichigoGetString(&strLen, vm, 0);
	if (!strLen)
		danmaku->player.onBomb = NULL;
}
static void ipl_onBombHit(struct IchigoVm *vm) {
	uint16_t strLen;
	danmaku->player.onBombHit = ichigoGetString(&strLen, vm, 0);
	if (!strLen)
		danmaku->player.onBombHit = NULL;
}
static void ipl_speed(struct IchigoVm *vm) {
	danmaku->player.speed = ichigoGetFloat(vm, 0);
	danmaku->player.focusSpeed = ichigoGetFloat(vm, 1);
}

static void ipl_dvmFile(struct IchigoVm *vm) {
	uint16_t strLen;
	drawVmAddFile(ichigoGetString(&strLen, vm, 0));
}
static void ipl_dvmPlay(struct IchigoVm *vm) {
	entity_t en = newEntity();
	struct Transform *tf = newComponent(TRANSFORM, en);
	struct Transform *ptf = getComponent(TRANSFORM, danmaku->player.entity);
	tf->x = ptf->x + ichigoGetFloat(vm, 1);
	tf->y = ptf->y + ichigoGetFloat(vm, 2);
	tf->z = ptf->z + ichigoGetFloat(vm, 3);
	uint16_t strLen;
	struct DrawVm *d = drawVmNew(en, ichigoGetString(&strLen, vm, 0));
	d->flags |= DVM_FLAG_DELETE_ALL;
}
static void ipl_dvmPlayAbs(struct IchigoVm *vm) {
	entity_t en = newEntity();
	struct Transform *tf = newComponent(TRANSFORM, en);
	tf->x = ichigoGetFloat(vm, 1);
	tf->y = ichigoGetFloat(vm, 2);
	tf->z = ichigoGetFloat(vm, 3);
	uint16_t strLen;
	struct DrawVm *d = drawVmNew(en, ichigoGetString(&strLen, vm, 0));
	d->flags |= DVM_FLAG_DELETE_ALL;
}
static void ipl_dvmMain(struct IchigoVm *vm) {
	struct DanPlayerController *p = &danmaku->player;
	uint16_t strLen;
	p->mainDvm = ichigoGetString(&strLen, vm, 0);
	drawVmNew(danmaku->player.entity, p->mainDvm);
}
static void ipl_dvmEvent(struct IchigoVm *vm) {
	drawVmEvent2(danmaku->player.entity, ichigoGetInt(vm, 0));
}

static void ipl_optionNew(struct IchigoVm *vm) {
	int idx = ichigoGetInt(vm, 0);
	entity_t en = newEntity();
	danmaku->player.options[idx] = en;
	struct Transform *tf = newComponent(TRANSFORM, en);
	tf->x = ichigoGetFloat(vm, 1);
	tf->y = ichigoGetFloat(vm, 2);
	uint16_t strLen;
	const char *dvmFn = ichigoGetString(&strLen, vm, 3);
	struct DrawVm *d = drawVmNew(en, dvmFn);
	d->flags |= DVM_FLAG_DELETE_ALL;

}
static void ipl_optionDelete(struct IchigoVm *vm) {
	entity_t en = danmaku->player.options[ichigoGetInt(vm, 0)];
	drawVmDelete(en);
}
static void ipl_optionEvent(struct IchigoVm *vm) {
	entity_t en = danmaku->player.options[ichigoGetInt(vm, 0)];
	drawVmEvent2(en, ichigoGetInt(vm, 1));
}
static void ipl_optionGetPos(struct IchigoVm *vm) {
	entity_t en = danmaku->player.options[ichigoGetInt(vm, 0)];
	struct Transform *tf = getComponentOpt(TRANSFORM, en);
	if (tf) {
		ichigoSetFloat(vm, 1, tf->x);
		ichigoSetFloat(vm, 2, tf->y);
	} else {
		ichigoSetFloat(vm, 1, 0);
		ichigoSetFloat(vm, 2, 0);
	}
}
static void ipl_optionSetPos(struct IchigoVm *vm) {
	entity_t en = danmaku->player.options[ichigoGetInt(vm, 0)];
	struct Transform *tf = getComponentOpt(TRANSFORM, en);
	if (tf) {
		tf->x = ichigoGetFloat(vm, 1);
		tf->y = ichigoGetFloat(vm, 2);
	}
}

static void ipl_normalizeRot(struct IchigoVm *vm) {
	float r = normalizeRot(ichigoGetFloat(vm, 1));
	ichigoSetFloat(vm, 0, r);
}

static void ipl_bulletNew(struct IchigoVm *vm) {
	entity_t en = newEntity();
	struct Transform *tf = newComponent(TRANSFORM, en);
	int option = ichigoGetInt(vm, 1);
	tf->x = ichigoGetFloat(vm, 2);
	tf->y = ichigoGetFloat(vm, 3);
	if (option == -1) {
		struct Transform *ptf = getComponent(TRANSFORM, danmaku->player.entity);
		tf->x += ptf->x;
		tf->y += ptf->y;
	} else {
		struct Transform *otf = getComponentOpt(TRANSFORM, danmaku->player.options[option]);
		if (!otf) {
			deleteEntity(en);
			ichigoSetEntity(vm, 0, 0);
			return;
		}
		tf->x += otf->x;
		tf->y += otf->y;
	}

	float rot = ichigoGetFloat(vm, 4);
	tf->rotReal = cosf(rot);
	tf->rotImag = sinf(rot);

	struct DanPlayerBullet *b = newComponent(DAN_PLAYER_BULLET, en);
	b->speed = ichigoGetFloat(vm, 5);
	b->hitboxW = ichigoGetFloat(vm, 6);
	b->hitboxH = ichigoGetFloat(vm, 7);
	b->damage = ichigoGetInt(vm, 8);
	b->flags = ichigoGetInt(vm, 9);

	uint16_t strLen;
	struct DrawVm *d = drawVmNew(en, ichigoGetString(&strLen, vm, 10));
	d->flags |= DVM_FLAG_DELETE_ALL;

	ichigoSetEntity(vm, 0, en);
}
static void ipl_bombCircle(struct IchigoVm *vm) {
	float x = ichigoGetFloat(vm, 0), y = ichigoGetFloat(vm, 1);
	float r = ichigoGetFloat(vm, 2);
	int dmg = ichigoGetInt(vm, 3);
	bulletDeleteCircle(x, y, r, ichigoGetInt(vm, 4), false);
	if (dmg)
		enemyDamageCircle(x, y, r, dmg);
}
static void ipl_bombRect(struct IchigoVm *vm) {
	float x = ichigoGetFloat(vm, 0), y = ichigoGetFloat(vm, 1);
	float rot = ichigoGetFloat(vm, 2);
	float w = ichigoGetFloat(vm, 3), h = ichigoGetFloat(vm, 4);
	int dmg = ichigoGetInt(vm, 5);
	bulletDeleteRect(x, y, rot, w, h, ichigoGetInt(vm, 6), false);
	if (dmg)
		enemyDamageRect(x, y, rot, w, h, dmg);
}

static void ipl_itemFallSpeed(struct IchigoVm *vm) {
	danmaku->item.fallSpeed = ichigoGetFloat(vm, 0);
}
static void ipl_itemMagnetRange(struct IchigoVm *vm) {
	danmaku->player.magnetRange = ichigoGetFloat(vm, 0);
	danmaku->player.focusMagnetRange = ichigoGetFloat(vm, 1);
}
static void ipl_itemAutoCollectY(struct IchigoVm *vm) {
	danmaku->item.autoCollectY = ichigoGetFloat(vm, 0);
}
static void ipl_maxPower(struct IchigoVm *vm) {
	struct DanPlayerController *p = &danmaku->player;
	p->maxPower = ichigoGetInt(vm, 0);
	p->powerItemValue = ichigoGetInt(vm, 1);
}
static void ipl_bombAutoCollect(struct IchigoVm *vm) {
	danmaku->item.bombCollect = ichigoGetFloat(vm, 0);
}
static void ipl_bombEnd(struct IchigoVm *vm) {
	struct DanPlayerController *p = &danmaku->player;
	p->state = DP_STATE_NORM;
}

static void ipl_itemNew(struct IchigoVm *vm) {
	itemNew(ichigoGetFloat(vm, 0), ichigoGetFloat(vm, 1), ichigoGetInt(vm, 2));
}


static float ipl_getVarGAME_SPEED(struct IchigoVm *vm) {
	return gameSpeed;
}
static void ipl_setVarGAME_SPEED(struct IchigoVm *vm, float v) {
	gameSpeed = v;
}
static int ipl_getVarPOWER(struct IchigoVm *vm) {
	return danmaku->state.power;
}
static float ipl_getVarTARGET_X(struct IchigoVm *vm) {
	return danmaku->player.targetX;
}
static float ipl_getVarTARGET_Y(struct IchigoVm *vm) {
	return danmaku->player.targetY;
}
static int ipl_getVarHAVE_TARGET(struct IchigoVm *vm) {
	return danmaku->player.targetEnemy? 1 : 0;
}
static int ipl_getVarSHOOT(struct IchigoVm *vm) {
	struct DanPlayerController *p = &danmaku->player;
	return !game->dialog.active && p->state != DP_STATE_DEATH && p->state != DP_STATE_DEATH_GRACE && keyPressed(K_SHOOT);
}
static int ipl_getVarFOCUS(struct IchigoVm *vm) {
	struct DanPlayerController *p = &danmaku->player;
	return p->focusActive;
}
static int ipl_getVarDEATH(struct IchigoVm *vm) {
	struct DanPlayerController *p = &danmaku->player;
	return p->state == DP_STATE_DEATH;
}

static int ipl_getVarGAUGE(struct IchigoVm *vm) {
	return danmaku->state.gauge;
}
static void ipl_setVarGAUGE(struct IchigoVm *vm, int val) {
	danmaku->state.gauge = val;
}

static int ipl_getVarBOMB(struct IchigoVm *vm) {
	struct DanPlayerController *p = &danmaku->player;
	return p->state == DP_STATE_BOMB ? p->bombCorout : -1;
}

static void setVar(struct DanPlayerController *c, int idx, enum IchigoRegType type, void *get, void *set) {
	c->playerVars[idx].regType = type;
	c->playerVars[idx].get.i = get;
	c->playerVars[idx].set.i = set;
}
static IchigoInstr *plInstrs[64] = {
	[0] = ipl_log,
	[1] = ipl_hitbox,
	[2] = ipl_grazebox,
	[3] = ipl_speed,
	[4] = ipl_normalizeRot,
	[5] = ipl_onBomb,
	[6] = ipl_onBombHit,
	[7] = i_sfx,
	[8] = i_modeLerp,
	[9] = ipl_dvmFile,
	[10] = ipl_dvmMain,
	[11] = ipl_dvmEvent,
	[12] = ipl_dvmPlay,
	[13] = ipl_dvmPlayAbs,
	[14] = ipl_optionNew,
	[15] = ipl_optionDelete,
	[16] = ipl_optionEvent,
	[17] = ipl_optionGetPos,
	[18] = ipl_optionSetPos,
	[19] = ipl_bulletNew,
	[20] = ipl_bombCircle,
	[21] = ipl_bombRect,
	[22] = ipl_itemFallSpeed,
	[23] = ipl_itemMagnetRange,
	[24] = ipl_itemAutoCollectY,
	[25] = ipl_maxPower,
	[26] = ipl_bombAutoCollect,
	[27] = ipl_bombEnd,
	[28] = i_sfxStop,
	[29] = i_shake,
	[30] = ipl_itemNew

};
void danPlayerInit(struct Danmaku *game) {
	struct DanPlayerController *p = &game->player;
	ichigoInit(&p->playerScript, "danpl");
	ichigoSetInstrTable(&p->playerScript, plInstrs, 64);
	ichigoSetVarTable(&p->playerScript, p->playerVars, 64);
	ichigoBindLocals(&p->playerScript);
	setVar(p, 23, REG_FLOAT, ipl_getVarGAME_SPEED, ipl_setVarGAME_SPEED);
	setVar(p, 24, REG_INT, ipl_getVarPOWER, NULL);
	setVar(p, 25, REG_FLOAT, ipl_getVarTARGET_X, NULL);
	setVar(p, 26, REG_FLOAT, ipl_getVarTARGET_Y, NULL);
	setVar(p, 27, REG_INT, ipl_getVarHAVE_TARGET, NULL);
	setVar(p, 28, REG_INT, ipl_getVarSHOOT, NULL);
	setVar(p, 29, REG_INT, ipl_getVarFOCUS, NULL);
	setVar(p, 30, REG_INT, ipl_getVarDEATH, NULL);
	setVar(p, 31, REG_INT, ipl_getVarGAUGE, ipl_setVarGAUGE);
	setVar(p, 32, REG_INT, ipl_getVarBOMB, NULL);

	componentListInit(DAN_PLAYER_BULLET, struct DanPlayerBullet);

	addUpdate(UPDATE_PHYS, danPlayerPhysUpdate, p);
	addUpdate(UPDATE_NORM, danPlayerUpdate, p);
}
void danPlayerFini(struct Danmaku *game) {
	componentListFini(DAN_PLAYER_BULLET);
	removeUpdate(UPDATE_PHYS, danPlayerPhysUpdate);
	removeUpdate(UPDATE_NORM, danPlayerUpdate);
}