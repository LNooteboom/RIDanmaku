#include "redefined.h"
#include <audio.h>

static void enemyDamage(struct Enemy *e, int dmg) {
	if (!(e->flags & ENM_INVINCIBLE) && e->invulnTime <= 0) {
		if (e->flags & ENM_RESIST) {
			e->life -= dmg / 7.0f;
		} else {
			e->life -= dmg;
		}
		scoreAdd(dmg * 10);
		danmakuPlaySfx(0);

		if (e->damageAnmTime <= 0) {
			e->damageAnmTime = 10;
		}
	}
}

void enemyDamageCircle(float x, float y, float r, int dmg) {
	struct CollisionInfo cc;
	cc.rr1 = cc.rr2 = 1.0f;
	cc.ri1 = cc.ri2 = 0;
	cc.x1 = x;
	cc.y1 = y;
	cc.w1 = r;
	for (struct Enemy *e = clBegin(DAN_ENEMY); e; e = clNext(DAN_ENEMY, e)) {
		struct Transform *tf = getComponent(TRANSFORM, e->entity);
		cc.x2 = tf->x;
		cc.y2 = tf->y;
		cc.w2 = e->hurtboxW;
		cc.h2 = e->hurtboxH;
		if (e->hurtboxW && collisionCR(&cc)) {
			enemyDamage(e, dmg);
		}
	}
}
void enemyDamageRect(float x, float y, float rot, float w, float h, int dmg) {
	struct CollisionInfo cc;
	cc.rr1 = 1.0f;
	cc.ri1 = 0;
	cc.x2 = x;
	cc.y2 = y;
	cc.w2 = w;
	cc.h2 = h;
	cc.rr2 = cosf(rot);
	cc.ri2 = sinf(rot);
	for (struct Enemy *e = clBegin(DAN_ENEMY); e; e = clNext(DAN_ENEMY, e)) {
		struct Transform *tf = getComponent(TRANSFORM, e->entity);
		cc.x1 = tf->x;
		cc.y1 = tf->y;
		cc.w1 = e->hurtboxW;
		cc.h1 = e->hurtboxH;
		if (e->hurtboxW && collisionRR(&cc)) {
			enemyDamage(e, dmg);
		}
	}
}

struct Enemy *enemyNew(const char *scriptFn, float x, float y, int hp, int type, struct IchigoLocals *parentLocals) {
	entity_t en = newEntity();
	struct Transform *tf = newComponent(TRANSFORM, en);
	tf->x = x;
	tf->y = y;
	struct Enemy *e = newComponent(DAN_ENEMY, en);
	e->abs.x = x;
	e->abs.y = y;
	e->life = hp;
	e->maxLife = hp;
	e->dropMain = ITEM_NONE;
	struct BulletSpawner *s = newComponent(DAN_BULLET_SPAWNER, en);
	struct IchigoVm *vm = newComponent(ICHIGO_VM, en);
	vm->is = &danmaku->danScript;
	ichigoVmExec(vm, scriptFn, "");

	struct IchigoLocals *l = newComponent(ICHIGO_LOCALS, en);
	if (parentLocals) {
		ichigoCopyLocals(l, parentLocals);
	}
	l->i[0] = type;
	return e;
}
entity_t enemyNewMain(void) {
	struct Enemy *e = enemyNew("main", 0, 0, 999999, 0, NULL);
	e->flags = ENM_NO_HIT | ENM_NO_HURT | ENM_NO_DELETE;
	return e->entity;
}

static float clamp(float f, float min, float max) {
	return fmaxf(min, fminf(max, f));
}
static void enemyUpdateMove(struct Enemy *e, struct EnemyMove *m) {
	float td = e->flags & ENM_NO_TIMESTOP ? 1 : gameSpeed;
	int ip1N = 0, ip2N = 0;
	float *ip1Dest, *ip2Dest;
	bool mirror = e->flags & ENM_MIRROR && m->type != MOVE_VEL_NM;

	if (mirror)
		m->x = -m->x;

	switch (m->type) {
	case MOVE_POS:
		ip1Dest = &m->x;
		ip1N = 3;
		break;
	case MOVE_VEL:
	case MOVE_VEL_NM:
		/* Use separate interps for spd/ang */
		ip1Dest = &m->ang;
		ip1N = 1;
		ip2Dest = &m->spd;
		ip2N = 1;
		m->x += cosf(m->ang) * m->spd * td;
		m->y += sinf(m->ang) * m->spd * td;
		break;
	case MOVE_ELLIPSE:
	{
		ip1Dest = &m->spd;
		ip1N = 3;
		ip2Dest = &m->ellDir;
		ip2N = 2;
		
		float rx = m->rad * m->ellRatio * cosf(m->ang);
		float ry = m->rad * sinf(m->ang);
		float ct = cosf(m->ellDir);
		float st = sinf(m->ellDir);
		m->x = rx * ct - ry * st + m->midX;
		m->y = rx * st + ry * ct + m->midY;

		m->ang += m->spd * td;
		m->rad += m->radInc * td;
		break;
	}
	case MOVE_BEZIER:
		if (m->ip1.endTime) {
			interpUpdateBezier(&m->ip1, 2, &m->x, td, m->ex1, m->ex2);
			if (m->ip1.time >= m->ip1.endTime)
				m->ip1.endTime = 0;
		}
		break;
	case MOVE_RAND:
		ip1Dest = &m->spd;
		ip1N = 1;
		m->x += cosf(m->ang) * m->spd * td;
		m->y += sinf(m->ang) * m->spd * td;
	}

	if (ip1N && m->ip1.endTime) {
		interpUpdate(&m->ip1, ip1N, ip1Dest, td);
		if (m->ip1.time >= m->ip1.endTime)
			m->ip1.endTime = 0;
	}
	if (ip2N && m->ip2.endTime) {
		interpUpdate(&m->ip2, ip2N, ip2Dest, td);
		if (m->ip2.time >= m->ip2.endTime)
			m->ip2.endTime = 0;
	}

	if (mirror)
		m->x = -m->x;
}

static void enemyPhysUpdate(void *arg) {
	struct EnemyControl *ec = arg;
	for (struct Enemy *e = clBegin(DAN_ENEMY); e; e = clNext(DAN_ENEMY, e)) {
		enemyUpdateMove(e, &e->abs);
		if (e->hasLimit) {
			e->abs.x = clamp(e->abs.x, e->minX - e->rel.x, e->maxX - e->rel.x);
			e->abs.y = clamp(e->abs.y, e->minY - e->rel.y, e->maxY - e->rel.y);
		}
		enemyUpdateMove(e, &e->rel);
		if (e->hasLimit) {
			e->rel.x = clamp(e->rel.x, e->minX - e->abs.x, e->maxX - e->abs.x);
			e->rel.y = clamp(e->rel.y, e->minY - e->abs.y, e->maxY - e->abs.y);
		}

		struct Transform *tf = getComponent(TRANSFORM, e->entity);
		float oldX = tf->x;
		tf->x = e->abs.x + e->rel.x;
		tf->y = e->abs.y + e->rel.y;
		tf->z = e->abs.z + e->rel.z;

		if (!(e->flags & ENM_OFFSCR_LR) && (tf->x < -192 - 32 || tf->x > 192 + 32) ||
			!(e->flags & ENM_OFFSCR_UD) && (tf->y < -32 || tf->y > 448 + 32)) {
			deleteEntity(e->entity);
			continue;
		}

		int moveState;
		if (fabsf(tf->x - oldX) < 1.0f)
			moveState = 0;
		else if (tf->x < oldX)
			moveState = 1;
		else
			moveState = 2;

		int ev = 0;
		if (moveState != e->moveState) {
			if (moveState == 0)
				ev = e->moveState == 1 ? 3 : 5;
			else if (moveState == 1)
				ev = 2;
			else
				ev = 4;
			e->moveState = moveState;
		}

		for (int i = 0; i < ENM_N_DVMS; i++) {
			if (e->dvm[i]) {
				struct Transform *dtf = getComponentOpt(TRANSFORM, e->dvm[i]);
				if (dtf) {
					dtf->x = tf->x;
					dtf->y = tf->y;
					dtf->z = tf->z;
					dtf->rotReal = tf->rotReal;
					dtf->rotImag = tf->rotImag;
					if (ev && (e->dvmMove & (1 << i)))
						drawVmEvent2(e->dvm[i], ev);
				}
			}
		}
		
	}
}

static void enemyCollision(struct Enemy *e, struct DanPlayerBullet *pb) {
	enemyDamage(e, pb->damage);
	danPlayerBulletHit(pb);
}

static void dropItems(struct Enemy *e, struct Transform *tf) {
	float dropClear = 0;
	if (e->dropMain >= 0) {
		itemNew(tf->x, tf->y, e->dropMain);
		if (e->dropMain == ITEM_LIFEPIECE || e->dropMain == ITEM_1UP)
			dropClear = 24;
		else
			dropClear = 16;
	}

	float w2 = fmaxf(0, e->dropW / 2.0f - dropClear);
	float h2 = fmaxf(0, e->dropH / 2.0f - dropClear);
	for (int i = 0; i < N_ITEMS; i++) {
		for (int j = 0; j < e->dropExtra[i]; j++) {
			float a = randomFloat(-PI, PI);
			float x = cosf(a) * (randomFloat(0, w2) + dropClear);
			float y = sinf(a) * (randomFloat(0, h2) + dropClear);
			itemNew(tf->x + x, tf->y + y, i);
		}
	}

	
}

void enemyKill(struct Enemy *e, struct Transform *tf) {
	if (e->deathFn) {
		struct IchigoVm *vm = getComponent(ICHIGO_VM, e->entity);
		ichigoVmKillAll(vm);
		ichigoVmExec(vm, e->deathFn, "");
		ichigoVmUpdate(vm, 1.0f);
	}
	dropItems(e, tf);
	danmakuPlaySfx(1);
	if (e->dvmDeath) {
		entity_t dvm = newEntity();
		struct Transform *dtf = newComponent(TRANSFORM, dvm);
		dtf->x = tf->x;
		dtf->y = tf->y;
		struct DrawVm *d = drawVmNew(dvm, e->dvmDeath);
		d->flags |= DVM_FLAG_DELETE_ALL;
	}
	deleteEntity(e->entity);
}

static void enemyUpdate(void* arg) {
	struct EnemyController *ec = arg;
	for (struct Enemy* e = clBegin(DAN_ENEMY); e; e = clNext(DAN_ENEMY, e)) {
		struct Transform *tf = getComponent(TRANSFORM, e->entity);
		struct CollisionInfo ci;
		ci.rr1 = ci.rr2 = 1.0f;
		ci.ri1 = ci.ri2 = 0.0f;
		ci.x2 = tf->x;
		ci.y2 = tf->y;
		if (!(e->flags & ENM_NO_HURT)) {
			/* Check collision with player bullets */
			ci.w2 = e->hurtboxW;
			ci.h2 = e->hurtboxH;
			for (struct DanPlayerBullet *pb = clBegin(DAN_PLAYER_BULLET); pb; pb = clNext(DAN_PLAYER_BULLET, pb)) {
				if (!pb->damage)
					continue;
				struct Transform *ptf = getComponent(TRANSFORM, pb->entity);
				ci.x1 = ptf->x;
				ci.y1 = ptf->y;
				ci.w1 = pb->hitboxW;
				ci.h1 = pb->hitboxH;
				if (collisionRR(&ci)) {
					enemyCollision(e, pb);
				}
			}
		}
		if (!(e->flags & ENM_NO_HIT)) {
			ci.w2 = e->hitboxW;
			ci.h2 = e->hitboxH;
			danPlayerPos(&ci.x1, &ci.y1);
			ci.w1 = danmaku->player.hitbox;
			if (collisionCR(&ci)) {
				danPlayerHit(); /* Collision with player */
			}
		}

		if (e->flags & ENM_KILLED) {
			deleteEntity(e->entity);
			continue;
		}
		if (danmaku->boss.boss != e->entity && e->life <= 0) {
			enemyKill(e, tf);
			continue;
		}

		/* Do interrupt */
		struct IchigoVm *vm = getComponent(ICHIGO_VM, e->entity);
		int irq = -1;
		struct EnemyInterrupt *ei = NULL;
		bool lifeIrq = true;
		for (int i = 0; i < ENM_N_INTERRUPTS; i++) {
			ei = &e->interrupts[i];
			if (ei->time > 0 && e->time > ei->time && ei->timeHandler) {
				irq = i;
				lifeIrq = false;
				break;
			}
			if (ei->life >= 0 && e->life <= ei->life && ei->lifeHandler) {
				irq = i;
				break;
			}
		}
		if (irq != -1) {
			if (danmaku->boss.boss == e->entity) {
				ec->timeout = !lifeIrq;
				if (!lifeIrq)
					danmaku->boss.capture = 0; /* timeout */
			}
			e->time = 0;
			ichigoVmKillAll(vm);
			ichigoVmExec(vm, lifeIrq? e->interrupts[irq].lifeHandler : e->interrupts[irq].timeHandler, "");

			for (int i = 0; i < ENM_N_INTERRUPTS; i++) {
				e->interrupts[i].time = 0;
				e->interrupts[i].life = -1;
			}
		}

		e->time += gameSpeed;
		if (e->invulnTime > 0)
			e->invulnTime -= gameSpeed;

		if (e->damageAnmTime > 0) {
			struct DrawVm *d = getComponentOpt(DRAW_VM, e->dvm[0]);
			if (d) {
				d->colMode = e->damageAnmTime == 2 || e->damageAnmTime == 3? COLOR2 : COLOR1;
			}
			e->damageAnmTime -= 1;
		}
	}
}

static void enemyNotifier(void *arg, void *component, int type) {
	struct Enemy *e = component;
	if (type == NOTIFY_CREATE) {
		e->dropMain = ITEM_NONE;
		e->dropW = 64;
		e->dropH = 64;
	} else if (type == NOTIFY_DELETE) {
		for (int i = 0; i < ENM_N_DVMS; i++) {
			if (e->dvm[i]) {
				struct IchigoVm *vm = getComponentOpt(DRAW_VM_VM, e->dvm[i]);
				if (vm) {
					ichigoVmKillAll(vm);
				}
				drawVmDelete(e->dvm[i]);
			}
		}
	}
}

/* 
Functions
*/
#define GET_ENM() struct Enemy *e = getComponent(DAN_ENEMY, vm->en)
#define GET_TF() struct Transform *tf = getComponent(TRANSFORM, vm->en)

static void i_enmNew(struct IchigoVm *vm) {
	uint16_t strLen;
	GET_TF();
	struct IchigoLocals *lp = getComponent(ICHIGO_LOCALS, vm->en);
	struct Enemy *e = enemyNew(ichigoGetString(&strLen, vm, 1), tf->x + ichigoGetFloat(vm, 2), tf->y + ichigoGetFloat(vm, 3), ichigoGetInt(vm, 4), ichigoGetInt(vm, 5), lp);
	ichigoSetEntity(vm, 0, e->entity);
}
static void i_enmNewA(struct IchigoVm *vm) {
	uint16_t strLen;
	struct IchigoLocals *lp = getComponent(ICHIGO_LOCALS, vm->en);
	struct Enemy *e = enemyNew(ichigoGetString(&strLen, vm, 1), ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3), ichigoGetInt(vm, 4), ichigoGetInt(vm, 5), lp);
	ichigoSetEntity(vm, 0, e->entity);
}
static void i_enmNewM(struct IchigoVm *vm) {
	uint16_t strLen;
	GET_TF();
	struct IchigoLocals *lp = getComponent(ICHIGO_LOCALS, vm->en);
	struct Enemy *e = enemyNew(ichigoGetString(&strLen, vm, 1), tf->x + ichigoGetFloat(vm, 2), tf->y + ichigoGetFloat(vm, 3), ichigoGetInt(vm, 4), ichigoGetInt(vm, 5), lp);
	e->flags |= ENM_MIRROR;
	ichigoSetEntity(vm, 0, e->entity);
}
static void i_enmNewAM(struct IchigoVm *vm) {
	uint16_t strLen;
	struct IchigoLocals *lp = getComponent(ICHIGO_LOCALS, vm->en);
	struct Enemy *e = enemyNew(ichigoGetString(&strLen, vm, 1), ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3), ichigoGetInt(vm, 4), ichigoGetInt(vm, 5), lp);
	e->flags |= ENM_MIRROR;
	ichigoSetEntity(vm, 0, e->entity);
}
static void i_enmDelete(struct IchigoVm *vm) {
	entity_t en = ichigoGetEntity(vm, 0);
	deleteEntity(en);
}
static void i_setInterrupt(struct IchigoVm *vm) {
	GET_ENM();
	int slot = ichigoGetInt(vm, 0);
	e->interrupts[slot].life = ichigoGetInt(vm, 1);
	float time = ichigoGetFloat(vm, 2);
	if (time > 0)
		time += e->time;
	e->interrupts[slot].time = time;

	uint16_t strLen;
	const char *handler = ichigoGetString(&strLen, vm, 3);
	if (!strLen)
		handler = NULL;
	e->interrupts[slot].lifeHandler = handler;
	e->interrupts[slot].timeHandler = handler;

	if (danmaku->boss.boss == vm->en) {
		bossSetTimer(e, slot);
	}
}
static void i_hurtbox(struct IchigoVm *vm) {
	GET_ENM();
	e->hurtboxW = ichigoGetFloat(vm, 0);
	e->hurtboxH = ichigoGetFloat(vm, 1);
}
static void i_hitbox(struct IchigoVm *vm) {
	GET_ENM();
	e->hitboxW = ichigoGetFloat(vm, 0);
	e->hitboxH = ichigoGetFloat(vm, 1);
}
static void i_lifeSet(struct IchigoVm *vm) {
	GET_ENM();
	e->life = ichigoGetInt(vm, 0);
	e->maxLife = e->life;
}

static void i_dropMain(struct IchigoVm *vm) {
	GET_ENM();
	e->dropMain = ichigoGetInt(vm, 0);
}
static void i_dropExtra(struct IchigoVm *vm) {
	GET_ENM();
	e->dropExtra[ichigoGetInt(vm, 0)] += ichigoGetInt(vm, 1);
}
static void i_dropArea(struct IchigoVm *vm) {
	GET_ENM();
	e->dropW = ichigoGetFloat(vm, 0);
	e->dropH = ichigoGetFloat(vm, 1);
}
static void i_dropItems(struct IchigoVm *vm) {
	GET_ENM();
	struct Transform *tf = getComponent(TRANSFORM, e->entity);
	dropItems(e, tf);
	for (int i = 0; i < N_ITEMS; i++) {
		e->dropExtra[i] = 0;
	}
	e->dropMain = -1;
}
static void i_dropClear(struct IchigoVm *vm) {
	GET_ENM();
	for (int i = 0; i < N_ITEMS; i++) {
		e->dropExtra[i] = 0; /* Only clear extra drop */
	}
}
static void i_enmKillAll(struct IchigoVm *vm) {
	for (struct Enemy *e = clBegin(DAN_ENEMY); e; e = clNext(DAN_ENEMY, e)) {
		if (!(e->flags & ENM_NO_DELETE) && e->entity != vm->en) {
			e->flags |= ENM_KILLED;
		}
	}
}
static void i_die(struct IchigoVm *vm) {
	GET_ENM();
	e->flags |= ENM_KILLED;
}

static void i_invuln(struct IchigoVm *vm) {
	GET_ENM();
	e->invulnTime = ichigoGetFloat(vm, 0);
}
static void i_onDeath(struct IchigoVm *vm) {
	GET_ENM();
	uint16_t strLen;
	const char *str = ichigoGetString(&strLen, vm, 0);
	e->deathFn = strLen ? str : NULL;
}


/* Movement */
static void interpResetAbs(struct Enemy *e) {
	e->abs.ip1.endTime = 0;
	e->abs.ip2.endTime = 0;
}
static void interpResetRel(struct Enemy *e) {
	e->rel.ip1.endTime = 0;
	e->rel.ip2.endTime = 0;
}

static void i_movePos(struct IchigoVm *vm) {
	GET_ENM();
	interpResetAbs(e);
	e->abs.type = MOVE_POS;
	e->abs.x = ichigoGetFloat(vm, 0);
	if (e->flags & ENM_MIRROR)
		e->abs.x = -e->abs.x;
	e->abs.y = ichigoGetFloat(vm, 1);
	e->abs.z = ichigoGetFloat(vm, 2);
}
static void i_movePosTime(struct IchigoVm *vm) {
	GET_ENM();
	interpResetAbs(e);
	e->abs.type = MOVE_POS;
	float v[2] = { ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3) };
	float pos[2] = { e->abs.x, e->abs.y };
	if (e->flags & ENM_MIRROR)
		pos[0] = -pos[0];
	interpStart(&e->abs.ip1, ichigoGetInt(vm, 1), 2, pos, v, ichigoGetFloat(vm, 0));
}
static void i_movePosRel(struct IchigoVm *vm) {
	GET_ENM();
	interpResetRel(e);
	e->rel.type = MOVE_POS;
	e->rel.x = ichigoGetFloat(vm, 0);
	if (e->flags & ENM_MIRROR)
		e->rel.x = -e->rel.x;
	e->rel.y = ichigoGetFloat(vm, 1);
	e->rel.z = ichigoGetFloat(vm, 2);
}
static void i_movePosRelTime(struct IchigoVm *vm) {
	GET_ENM();
	interpResetRel(e);
	e->rel.type = MOVE_POS;
	float v[2] = { ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3) };
	float pos[2] = { e->rel.x, e->rel.y };
	if (e->flags & ENM_MIRROR)
		pos[0] = -pos[0];
	interpStart(&e->rel.ip1, ichigoGetInt(vm, 1), 2, pos, v, ichigoGetFloat(vm, 0));
}

static void i_moveVel(struct IchigoVm *vm) {
	GET_ENM();
	interpResetAbs(e);
	e->abs.type = MOVE_VEL;
	e->abs.ang = ichigoGetFloat(vm, 0);
	e->abs.spd = ichigoGetFloat(vm, 1);
}
static void i_moveVelTime(struct IchigoVm *vm) {
	GET_ENM();
	interpResetAbs(e);
	if (e->abs.type != MOVE_VEL) {
		e->abs.ang = e->abs.spd = 0;
	}
	e->abs.type = MOVE_VEL;
	float ang = ichigoGetFloat(vm, 2);
	float spd = ichigoGetFloat(vm, 3);
	float time = ichigoGetFloat(vm, 0);
	int mode = ichigoGetInt(vm, 1);
	if (!IS_NEGF(ang))
		interpStart(&e->abs.ip1, mode, 1, &e->abs.ang, &ang, time);
	if (!IS_NEGF(spd))
		interpStart(&e->abs.ip2, mode, 1, &e->abs.spd, &spd, time);
}
static void i_moveVelRel(struct IchigoVm *vm) {
	GET_ENM();
	interpResetRel(e);
	e->rel.type = MOVE_VEL;
	e->rel.ang = ichigoGetFloat(vm, 0);
	e->rel.spd = ichigoGetFloat(vm, 1);
}
static void i_moveVelRelTime(struct IchigoVm *vm) {
	GET_ENM();
	interpResetRel(e);
	if (e->rel.type != MOVE_VEL) {
		e->rel.ang = e->rel.spd = 0;
	}
	e->rel.type = MOVE_VEL;
	float ang = ichigoGetFloat(vm, 2);
	float spd = ichigoGetFloat(vm, 3);
	float time = ichigoGetFloat(vm, 0);
	int mode = ichigoGetInt(vm, 1);
	if (!IS_NEGF(ang))
		interpStart(&e->rel.ip1, mode, 1, &e->rel.ang, &ang, time);
	if (!IS_NEGF(spd))
		interpStart(&e->rel.ip2, mode, 1, &e->rel.spd, &spd, time);
}

static void i_moveVelNM(struct IchigoVm *vm) {
	GET_ENM();
	interpResetAbs(e);
	e->abs.type = MOVE_VEL_NM;
	e->abs.ang = ichigoGetFloat(vm, 0);
	e->abs.spd = ichigoGetFloat(vm, 1);
}
static void i_moveVelNMTime(struct IchigoVm *vm) {
	GET_ENM();
	interpResetAbs(e);
	if (e->abs.type != MOVE_VEL_NM) {
		e->abs.ang = e->abs.spd = 0;
	}
	e->abs.type = MOVE_VEL_NM;
	float ang = ichigoGetFloat(vm, 2);
	float spd = ichigoGetFloat(vm, 3);
	float time = ichigoGetFloat(vm, 0);
	int mode = ichigoGetInt(vm, 1);
	if (!IS_NEGF(ang))
		interpStart(&e->abs.ip1, mode, 1, &e->abs.ang, &ang, time);
	if (!IS_NEGF(spd))
		interpStart(&e->abs.ip2, mode, 1, &e->abs.spd, &spd, time);
}
static void i_moveVelNMRel(struct IchigoVm *vm) {
	GET_ENM();
	interpResetRel(e);
	e->rel.type = MOVE_VEL_NM;
	e->rel.ang = ichigoGetFloat(vm, 0);
	e->rel.spd = ichigoGetFloat(vm, 1);
}
static void i_moveVelNMRelTime(struct IchigoVm *vm) {
	GET_ENM();
	interpResetRel(e);
	if (e->rel.type != MOVE_VEL_NM) {
		e->rel.ang = e->rel.spd = 0;
	}
	e->rel.type = MOVE_VEL_NM;
	float ang = ichigoGetFloat(vm, 2);
	float spd = ichigoGetFloat(vm, 3);
	float time = ichigoGetFloat(vm, 0);
	int mode = ichigoGetInt(vm, 1);
	if (!IS_NEGF(ang))
		interpStart(&e->rel.ip1, mode, 1, &e->rel.ang, &ang, time);
	if (!IS_NEGF(spd))
		interpStart(&e->rel.ip2, mode, 1, &e->rel.spd, &spd, time);
}

static void i_moveBezier(struct IchigoVm *vm) {
	GET_ENM();
	interpResetAbs(e);
	e->abs.type = MOVE_BEZIER;
	float t = ichigoGetFloat(vm, 0);
	e->abs.ex1[0] = ichigoGetFloat(vm, 1);
	e->abs.ex1[1] = ichigoGetFloat(vm, 2);
	float v[2] = { ichigoGetFloat(vm, 3), ichigoGetFloat(vm, 4) };
	e->abs.ex2[0] = ichigoGetFloat(vm, 5);
	e->abs.ex2[1] = ichigoGetFloat(vm, 6);

	float pos[2] = { e->abs.x, e->abs.y };
	if (e->flags & ENM_MIRROR)
		pos[0] = -pos[0];
	interpStart(&e->abs.ip1, INTERP_BEZIER, 2, pos, v, t);
}
static void i_moveBezierRel(struct IchigoVm *vm) {
	GET_ENM();
	interpResetRel(e);
	e->rel.type = MOVE_BEZIER;
	float t = ichigoGetFloat(vm, 0);
	e->rel.ex1[0] = ichigoGetFloat(vm, 1);
	e->rel.ex1[1] = ichigoGetFloat(vm, 2);
	float v[2] = { ichigoGetFloat(vm, 3), ichigoGetFloat(vm, 4) };
	e->rel.ex2[0] = ichigoGetFloat(vm, 5);
	e->rel.ex2[1] = ichigoGetFloat(vm, 6);

	float pos[2] = { e->rel.x, e->rel.y };
	if (e->flags & ENM_MIRROR)
		pos[0] = -pos[0];
	interpStart(&e->rel.ip1, INTERP_BEZIER, 2, pos, v, t);
}

static void i_moveCircle(struct IchigoVm *vm) {
	GET_ENM();
	interpResetAbs(e);
	e->abs.type = MOVE_ELLIPSE;
	e->abs.midX = e->abs.x;
	e->abs.midY = e->abs.y;
	e->abs.ang = ichigoGetFloat(vm, 0);
	e->abs.spd = ichigoGetFloat(vm, 1);
	e->abs.rad = ichigoGetFloat(vm, 2);
	e->abs.radInc = ichigoGetFloat(vm, 3);
	e->abs.ellRatio = 1;
}
static void i_moveCircleTime(struct IchigoVm *vm) {
	GET_ENM();
	interpResetAbs(e);
	e->abs.type = MOVE_ELLIPSE;
	e->abs.ellRatio = 1;
	float v1[3] = {
		ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3), ichigoGetFloat(vm, 4)
	};
	float v2[2] = {
		0, 1
	};
	float t = ichigoGetFloat(vm, 0);
	int mode = ichigoGetInt(vm, 1);
	interpStart(&e->abs.ip1, mode, 3, &e->abs.spd, v1, t);
	interpStart(&e->abs.ip2, mode, 2, &e->abs.ellDir, v2, t);
}
static void i_moveCircleRel(struct IchigoVm *vm) {
	GET_ENM();
	interpResetRel(e);
	e->rel.type = MOVE_ELLIPSE;
	e->rel.midX = e->rel.x;
	e->rel.midY = e->rel.y;
	e->rel.ang = ichigoGetFloat(vm, 0);
	e->rel.spd = ichigoGetFloat(vm, 1);
	e->rel.rad = ichigoGetFloat(vm, 2);
	e->rel.radInc = ichigoGetFloat(vm, 3);
	e->rel.ellRatio = 1;
}
static void i_moveCircleRelTime(struct IchigoVm *vm) {
	GET_ENM();
	interpResetRel(e);
	e->rel.type = MOVE_ELLIPSE;
	e->rel.ellRatio = 1;
	float v1[3] = {
		ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3), ichigoGetFloat(vm, 4)
	};
	float v2[2] = {
		0, 1
	};
	float t = ichigoGetFloat(vm, 0);
	int mode = ichigoGetInt(vm, 1);
	interpStart(&e->rel.ip1, mode, 3, &e->rel.spd, v1, t);
	interpStart(&e->rel.ip2, mode, 2, &e->rel.ellDir, v2, t);
}

static void i_moveEllipse(struct IchigoVm *vm) {
	GET_ENM();
	interpResetAbs(e);
	e->abs.type = MOVE_ELLIPSE;
	e->abs.midX = e->abs.x;
	e->abs.midY = e->abs.y;
	e->abs.ang = ichigoGetFloat(vm, 0);
	e->abs.spd = ichigoGetFloat(vm, 1);
	e->abs.rad = ichigoGetFloat(vm, 2);
	e->abs.radInc = ichigoGetFloat(vm, 3);
	e->abs.ellDir = ichigoGetFloat(vm, 4);
	e->abs.ellRatio = ichigoGetFloat(vm, 5);
}
static void i_moveEllipseTime(struct IchigoVm *vm) {
	GET_ENM();
	interpResetAbs(e);
	e->abs.type = MOVE_ELLIPSE;
	float v1[3] = {
		ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3), ichigoGetFloat(vm, 4)
	};
	float v2[2] = {
		ichigoGetFloat(vm, 5), ichigoGetFloat(vm, 6)
	};
	float t = ichigoGetFloat(vm, 0);
	int mode = ichigoGetInt(vm, 1);
	interpStart(&e->abs.ip1, mode, 3, &e->abs.spd, v1, t);
	interpStart(&e->abs.ip2, mode, 2, &e->abs.ellDir, v2, t);
}
static void i_moveEllipseRel(struct IchigoVm *vm) {
	GET_ENM();
	interpResetRel(e);
	e->rel.type = MOVE_ELLIPSE;
	e->rel.midX = e->rel.x;
	e->rel.midY = e->rel.y;
	e->rel.ang = ichigoGetFloat(vm, 0);
	e->rel.spd = ichigoGetFloat(vm, 1);
	e->rel.rad = ichigoGetFloat(vm, 2);
	e->rel.radInc = ichigoGetFloat(vm, 3);
	e->rel.ellDir = ichigoGetFloat(vm, 4);
	e->rel.ellRatio = ichigoGetFloat(vm, 5);
}
static void i_moveEllipseRelTime(struct IchigoVm *vm) {
	GET_ENM();
	interpResetRel(e);
	e->rel.type = MOVE_ELLIPSE;
	float v1[3] = {
		ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3), ichigoGetFloat(vm, 4)
	};
	float v2[2] = {
		ichigoGetFloat(vm, 5), ichigoGetFloat(vm, 6)
	};
	float t = ichigoGetFloat(vm, 0);
	int mode = ichigoGetInt(vm, 1);
	interpStart(&e->rel.ip1, mode, 3, &e->rel.spd, v1, t);
	interpStart(&e->rel.ip2, mode, 2, &e->rel.ellDir, v2, t);
}

static void i_moveRand(struct IchigoVm *vm) {
	GET_ENM();
	interpResetAbs(e);
	e->abs.type = MOVE_RAND;
	e->abs.ang = randomFloat(-PI, PI);
	e->abs.spd = ichigoGetFloat(vm, 2);
	float z = 0;
	interpStart(&e->abs.ip1, ichigoGetInt(vm, 1), 1, &e->abs.spd, &z, ichigoGetFloat(vm, 0));
}
static void i_moveRandRel(struct IchigoVm *vm) {
	GET_ENM();
	interpResetRel(e);
	e->rel.type = MOVE_RAND;
	e->rel.ang = randomFloat(-PI, PI);
	e->rel.spd = ichigoGetFloat(vm, 2);
	float z = 0;
	interpStart(&e->rel.ip1, ichigoGetInt(vm, 1), 1, &e->rel.spd, &z, ichigoGetFloat(vm, 0));
}

static void i_moveLimit(struct IchigoVm *vm) {
	GET_ENM();
	e->hasLimit = true;
	float x = ichigoGetFloat(vm, 0);
	float y = ichigoGetFloat(vm, 1);
	float w2 = fabsf(ichigoGetFloat(vm, 2)) / 2;
	float h2 = fabsf(ichigoGetFloat(vm, 3)) / 2;
	e->minX = x - w2;
	e->maxX = x + w2;
	e->minY = y - h2;
	e->maxY = y + h2;
}
static void i_moveLimitReset(struct IchigoVm *vm) {
	GET_ENM();
	e->hasLimit = false;
}

/* DVM */
static struct DrawVm *dvmSet(struct Enemy *e, int slot, const char *fn) {
	if (e->dvm[slot]) {
		deleteEntity(e->dvm[slot]);
	}
	e->dvm[slot] = newEntity();
	struct Transform *myTf = getComponent(TRANSFORM, e->entity);
	struct Transform *tf = newComponent(TRANSFORM, e->dvm[slot]);
	tf->x = myTf->x;
	tf->y = myTf->y;
	tf->z = myTf->z;
	tf->rotReal = myTf->rotReal;
	tf->rotImag = myTf->rotImag;
	struct DrawVm *d = drawVmNew(e->dvm[slot], fn);
	d->flags |= DVM_FLAG_DELETE_ALL;
	return d;
}
static void i_dvmSet(struct IchigoVm *vm) {
	GET_ENM();
	uint16_t strLen;
	int slot = ichigoGetInt(vm, 0);
	e->dvmMove &= ~(1 << slot);
	dvmSet(e, slot, ichigoGetString(&strLen, vm, 1));
}
static void i_dvmSetMove(struct IchigoVm *vm) {
	GET_ENM();
	uint16_t strLen;
	int slot = ichigoGetInt(vm, 0);
	e->dvmMove |= 1 << slot;
	dvmSet(e, slot, ichigoGetString(&strLen, vm, 1));
}
static void i_dvmDelete(struct IchigoVm *vm) {
	GET_ENM();
	int slot = ichigoGetInt(vm, 0);
	drawVmDelete(e->dvm[slot]);
	e->dvm[slot] = 0;
}
static void i_dvmEvent(struct IchigoVm *vm) {
	GET_ENM();
	int slot = ichigoGetInt(vm, 0);
	drawVmEvent2(e->dvm[slot], ichigoGetInt(vm, 1));
}
static void i_dvmPlay(struct IchigoVm *vm) {
	GET_TF();
	entity_t en = newEntity();
	struct Transform *dtf = newComponent(TRANSFORM, en);
	dtf->x = tf->x + ichigoGetFloat(vm, 1);
	dtf->y = tf->y + ichigoGetFloat(vm, 2);
	dtf->z = tf->z + ichigoGetFloat(vm, 3);
	dtf->rotReal = tf->rotReal;
	dtf->rotImag = tf->rotImag;
	uint16_t strLen;
	struct DrawVm *d = drawVmNew(en, ichigoGetString(&strLen, vm, 0));
	d->flags |= DVM_FLAG_DELETE_ALL;
}
static void i_dvmPlayAbs(struct IchigoVm *vm) {
	entity_t en = newEntity();
	struct Transform *dtf = newComponent(TRANSFORM, en);
	dtf->x = ichigoGetFloat(vm, 1);
	dtf->y = ichigoGetFloat(vm, 2);
	dtf->z = ichigoGetFloat(vm, 3);
	uint16_t strLen;
	struct DrawVm *d = drawVmNew(en, ichigoGetString(&strLen, vm, 0));
	d->flags |= DVM_FLAG_DELETE_ALL;
}
static void i_dvmDeath(struct IchigoVm *vm) {
	GET_ENM();
	uint16_t strLen;
	const char *str = ichigoGetString(&strLen, vm, 0);
	e->dvmDeath = strLen ? str : NULL;
}
static void i_dvmFile(struct IchigoVm *vm) {
	uint16_t strLen;
	drawVmAddFile(ichigoGetString(&strLen, vm, 0));
}
static void i_dvmSetVarI(struct IchigoVm *vm) {
	GET_ENM();
	entity_t dvm = e->dvm[ichigoGetInt(vm, 0)];
	struct IchigoLocals *l = getComponentOpt(DRAW_VM_LOCALS, dvm);
	l->i[ichigoGetInt(vm, 1)] = ichigoGetInt(vm, 2);
}
static void i_dvmSetVarF(struct IchigoVm *vm) {
	GET_ENM();
	entity_t dvm = e->dvm[ichigoGetInt(vm, 0)];
	struct IchigoLocals *l = getComponentOpt(DRAW_VM_LOCALS, dvm);
	l->f[ichigoGetInt(vm, 1)] = ichigoGetFloat(vm, 2);
}


static void i_enmNewAF(struct IchigoVm *vm) {
	if (!danmaku->boss.boss)
		i_enmNewA(vm);
}
static void i_enmNewAFM(struct IchigoVm *vm) {
	if (!danmaku->boss.boss)
		i_enmNewAM(vm);
}



/*
 * Variables
 */

#define ENM_ICH_VAR_GET(T, NAME, DST) static T i_getVar##NAME(struct IchigoVm *vm) {GET_ENM(); return e->DST;}
static float i_getVarAIM(struct IchigoVm *vm) {
	GET_TF();
	float x, y;
	danPlayerPos(&x, &y);
	return atan2f(y - tf->y, x - tf->x);
}
ENM_ICH_VAR_GET(int, LIFE, life)
ENM_ICH_VAR_GET(int, FLAGS, flags)
static void i_setVarFLAGS(struct IchigoVm *vm, int flags) {
	GET_ENM();
	e->flags = flags;
}
ENM_ICH_VAR_GET(int, TIME, time)
ENM_ICH_VAR_GET(float, ABS_X, abs.x)
ENM_ICH_VAR_GET(float, ABS_Y, abs.y)
ENM_ICH_VAR_GET(float, ABS_SPEED, abs.spd)
ENM_ICH_VAR_GET(float, ABS_ANGLE, abs.ang)
ENM_ICH_VAR_GET(float, REL_X, rel.x)
ENM_ICH_VAR_GET(float, REL_Y, rel.y)
ENM_ICH_VAR_GET(float, REL_SPEED, rel.spd)
ENM_ICH_VAR_GET(float, REL_ANGLE, rel.ang)
static float i_getVarPLAYER_X(struct IchigoVm *vm) {
	(void)vm;
	float x, y;
	danPlayerPos(&x, &y);
	return x;
}
static float i_getVarPLAYER_Y(struct IchigoVm *vm) {
	(void)vm;
	float x, y;
	danPlayerPos(&x, &y);
	return y;
}
static int i_getVarDIFFICULTY(struct IchigoVm *vm) {
	(void)vm;
	return danmaku->difficulty;
}
static float i_getVarGAME_SPEED(struct IchigoVm *vm) {
	(void)vm;
	return gameSpeed;
}
static void i_setVarGAME_SPEED(struct IchigoVm *vm, float v) {
	(void)vm;
	gameSpeed = v;
}
static int i_getVarTIMEOUT(struct IchigoVm *vm) {
	(void)vm;
	return danmaku->ec.timeout;
}

void enemyInit(struct Danmaku *d) {
	componentListInit(DAN_ENEMY, struct Enemy);
	setNotifier(DAN_ENEMY, enemyNotifier, &d->ec);

	danmakuSetInstr(10, i_enmNew);
	danmakuSetInstr(11, i_enmNewA);
	danmakuSetInstr(12, i_enmNewM);
	danmakuSetInstr(13, i_enmNewAM);
	danmakuSetInstr(14, i_enmDelete);
	danmakuSetInstr(15, i_setInterrupt);
	danmakuSetInstr(16, i_hurtbox);
	danmakuSetInstr(17, i_hitbox);
	danmakuSetInstr(18, i_lifeSet);
	danmakuSetInstr(19, i_dropMain);
	danmakuSetInstr(20, i_dropExtra);
	danmakuSetInstr(21, i_dropArea);
	danmakuSetInstr(22, i_dropItems);
	danmakuSetInstr(23, i_invuln);
	danmakuSetInstr(24, i_dropClear);
	danmakuSetInstr(25, i_enmKillAll);
	danmakuSetInstr(26, i_die);
	danmakuSetInstr(28, i_onDeath);

	danmakuSetInstr(30, i_movePos);
	danmakuSetInstr(31, i_movePosTime);
	danmakuSetInstr(32, i_movePosRel);
	danmakuSetInstr(33, i_movePosRelTime);
	danmakuSetInstr(34, i_moveVel);
	danmakuSetInstr(35, i_moveVelTime);
	danmakuSetInstr(36, i_moveVelRel);
	danmakuSetInstr(37, i_moveVelRelTime);
	danmakuSetInstr(38, i_moveVelNM);
	danmakuSetInstr(39, i_moveVelNMTime);
	danmakuSetInstr(40, i_moveVelNMRel);
	danmakuSetInstr(41, i_moveVelNMRelTime);
	danmakuSetInstr(42, i_moveBezier);
	danmakuSetInstr(43, i_moveBezierRel);
	danmakuSetInstr(44, i_moveCircle);
	danmakuSetInstr(45, i_moveCircleTime);
	danmakuSetInstr(46, i_moveCircleRel);
	danmakuSetInstr(47, i_moveCircleRelTime);
	danmakuSetInstr(48, i_moveEllipse);
	danmakuSetInstr(49, i_moveEllipseTime);
	danmakuSetInstr(50, i_moveEllipseRel);
	danmakuSetInstr(51, i_moveEllipseRelTime);
	danmakuSetInstr(52, i_moveRand);
	danmakuSetInstr(53, i_moveRandRel);
	danmakuSetInstr(54, i_moveLimit);
	danmakuSetInstr(55, i_moveLimitReset);

	danmakuSetInstr(80, i_dvmSet);
	danmakuSetInstr(81, i_dvmSetMove);
	danmakuSetInstr(82, i_dvmDelete);
	danmakuSetInstr(83, i_dvmEvent);
	danmakuSetInstr(84, i_dvmPlay);
	danmakuSetInstr(85, i_dvmPlayAbs);
	danmakuSetInstr(86, i_dvmDeath);
	danmakuSetInstr(87, i_dvmFile);
	danmakuSetInstr(88, i_dvmSetVarI);
	danmakuSetInstr(89, i_dvmSetVarF);

	danmakuSetInstr(156, i_enmNewAF);
	danmakuSetInstr(157, i_enmNewAFM);

	danmakuSetVar(23, REG_FLOAT, i_getVarAIM, NULL);
	danmakuSetVar(24, REG_INT, i_getVarLIFE, NULL);
	danmakuSetVar(25, REG_INT, i_getVarFLAGS, i_setVarFLAGS);
	danmakuSetVar(26, REG_INT, i_getVarTIME, NULL);
	danmakuSetVar(27, REG_FLOAT, i_getVarABS_X, NULL);
	danmakuSetVar(28, REG_FLOAT, i_getVarABS_Y, NULL);
	danmakuSetVar(29, REG_FLOAT, i_getVarABS_SPEED, NULL);
	danmakuSetVar(30, REG_FLOAT, i_getVarABS_ANGLE, NULL);
	danmakuSetVar(31, REG_FLOAT, i_getVarREL_X, NULL);
	danmakuSetVar(32, REG_FLOAT, i_getVarREL_Y, NULL);
	danmakuSetVar(33, REG_FLOAT, i_getVarREL_SPEED, NULL);
	danmakuSetVar(34, REG_FLOAT, i_getVarREL_ANGLE, NULL);
	danmakuSetVar(100, REG_FLOAT, i_getVarPLAYER_X, NULL);
	danmakuSetVar(101, REG_FLOAT, i_getVarPLAYER_Y, NULL);
	danmakuSetVar(102, REG_INT, i_getVarDIFFICULTY, NULL);
	danmakuSetVar(103, REG_FLOAT, i_getVarGAME_SPEED, i_setVarGAME_SPEED);
	danmakuSetVar(104, REG_INT, i_getVarTIMEOUT, NULL);

	addUpdate(UPDATE_PHYS, enemyPhysUpdate, &d->ec);
	addUpdate(UPDATE_NORM, enemyUpdate, &d->ec);
}

void enemyFini(struct Danmaku *d) {
	(void)d;
	removeUpdate(UPDATE_PHYS, enemyPhysUpdate);
	removeUpdate(UPDATE_NORM, enemyUpdate);

	componentListFini(DAN_ENEMY);
}
