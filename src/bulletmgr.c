#include "redefined.h"
#include <audio.h>
#include <string.h>

#define TAU 6.283185307f

static int transformIndex = 0;

static float angleToPlayer(float x, float y) {
	float px, py;
	danPlayerPos(&px, &py);
	return atan2f(py - y, px - x);
}

static entity_t createTfAndMgr(struct BulletSpawner *s, float x, float y, float spd, float ang) {
	entity_t en = 0;
	x += cosf(ang) * s->dist;
	y += sinf(ang) * s->dist;
	bool allowSpawn = true;
	if (s->protectRange > 0) {
		float px, py;
		danPlayerPos(&px, &py);
		px -= x;
		py -= y;
		allowSpawn = px * px + py * py > s->protectRange * s->protectRange;
	}

	if (allowSpawn) {
		en = newEntity();
		struct Transform *tf = newComponent(TRANSFORM, en);
		tf->x = x;
		tf->y = y;
		tfSetRotation(tf, ang);

		struct BulletMgr *m = newComponent(DAN_BULLET_MGR, en);
		m->speed = spd;

		m->transIndex = transformIndex;
		m->enmFn = s->enmFn;
		m->transSound = s->soundTrans;
		memcpy(m->trans, s->trans, sizeof(m->trans));
	}
	return en;
}
static entity_t createBullet(struct BulletSpawner *s, float x, float y, float spd, float ang) {
	entity_t en = createTfAndMgr(s, x, y, spd, ang);
	if (en) {
		struct Bullet *b = newComponent(DAN_BULLET, en);
		b->type = s->type;
		b->color = s->color;
		b->ovColor = 0xFFFFFFFF;
		b->size = danmaku->bullet.types[s->type].w;
	}
	return en;
}
static void createLaserSt(struct BulletSpawner *s, float x, float y, float spd, float ang, int flags, float initLen, float maxLen, float range, float width) {
	x += cosf(ang) * initLen;
	y += sinf(ang) * initLen;
	entity_t en = createTfAndMgr(s, x, y, spd, ang);
	if (en) {
		struct StraightLaser *l = newComponent(DAN_LASER_ST, en);
		l->type = s->type;
		l->color = s->color;
		l->flags = flags;
		l->length = initLen;
		l->endLength = maxLen;
		if (range) {
			l->range = range;
		} else {
			l->range = 999999.0f;
		}
		l->width = width;
	}
}
static entity_t createLaserInf(entity_t parent, struct BulletSpawner *s, float x, float y, float spd, float ang, int flags, float length, float width,
	int startTime, int expandTime, int duration, int shrinkTime)
{
	entity_t en = createTfAndMgr(s, x, y, spd, ang);
	if (en) {
		struct InfiniteLaser *l = newComponent(DAN_LASER_INF, en);

		if (!s->posAbs) {
			l->parent = parent;
			l->offX = s->offX;
			l->offY = s->offY;
			l->dist = s->dist;
		}

		l->type = s->type;
		l->color = s->color;
		l->flags = flags;
		l->length = length;
		l->width = width;
		l->startTime = startTime;
		l->expandTime = expandTime;
		l->duration = duration;
		l->shrinkTime = shrinkTime;
	}

	return en;
}
static void createLaserCu(struct BulletSpawner *s, float x, float y, float spd, float ang, int flags, float width,
	int startTime, int expandTime, int duration)
{
	entity_t en = createTfAndMgr(s, x, y, spd, ang);
	if (en) {
		struct CurveLaser *l = newComponent(DAN_LASER_CU, en);

		l->color = s->color;
		l->flags = flags;
		l->width = width;
		l->startTime = startTime;
		l->expandTime = expandTime;
		l->duration = duration;
	}
}

static float lerp2(float a, float b, int i, int cnt) {
	//return cnt == 1 ? a : lerp(a, b, (float)i / (cnt - 1));
	return lerp(a, b, (float)i / cnt);
}
static void shFan(struct BulletSpawner *s, float ang, float x, float y) {
	for (int j = 0; j < s->cnt2; j++) {
		float spd = lerp2(s->spd1, s->spd2, j, s->cnt2);
		float a = ang - ((s->cnt1 - 1) * 0.5f * s->ang2);

		for (int i = 0; i < s->cnt1; i++) {
			createBullet(s, x, y, spd, a);
			a += s->ang2;
		}
	}
}
static void shRing(struct BulletSpawner *s, float ang, float x, float y) {
	for (int j = 0; j < s->cnt2; j++) {
		float spd = lerp2(s->spd1, s->spd2, j, s->cnt2);
		float a = ang + j * s->ang2;
		for (int i = 0; i < s->cnt1; i++) {
			createBullet(s, x, y, spd, a);
			a += TAU / s->cnt1;
		}
	}
}
static void shPyr(struct BulletSpawner *s, float ang, float x, float y) {
	int odd = s->cnt2 % 2;
	int cn2 = s->cnt2 / 2 + odd;
	float aoff = odd ? 0 : s->ang2 * 0.5f;
	for (int j = 0; j < cn2; j++) {
		float spd = lerp2(s->spd1, s->spd2, j, cn2);
		float a = ang + aoff;
		for (int i = 0; i < s->cnt1; i++) {
			createBullet(s, x, y, spd, a);
			a += TAU / s->cnt1;
		}
		if (j || !odd) {
			a = ang - aoff;
			for (int i = 0; i < s->cnt1; i++) {
				createBullet(s, x, y, spd, a);
				a += TAU / s->cnt1;
			}
		}
		aoff += s->ang2;
	}
}

static void getPos(float *x, float *y, struct Transform *tf, struct BulletSpawner *s) {
	*x = s->x + s->offX;
	*y = s->y + s->offY;
	if (!s->posAbs) {
		*x += tf->x;
		*y += tf->y;
	}
}
static void bmShoot(struct Transform *tf, struct BulletSpawner *s) {
	float x, y;
	getPos(&x, &y, tf, s);
	float pa = angleToPlayer(x, y) + s->ang1;
	float a;

	if (s->cnt1 && s->cnt2 && s->soundShoot >= 0) {
		danmakuPlaySfx(s->soundShoot);
	}

	switch (s->aim) {
	case AIM_AT:
		shFan(s, pa, x, y);
		break;
	case AIM_ST:
		shFan(s, s->ang1, x, y);
		break;
	case AIM_AT_R:
		shRing(s, pa, x, y);
		break;
	case AIM_ST_R:
		shRing(s, s->ang1, x, y);
		break;
	case AIM_AWAY_R:
		pa += s->cnt1 == 0 ? 0 : PI / s->cnt1;
		shRing(s, pa, x, y);
		break;
	case AIM_ST_R2:
		a = s->ang1 + (s->cnt1 == 0 ? 0 : PI / s->cnt1);
		shRing(s, a, x, y);
		break;
	case AIM_RAND:
		for (int j = 0; j < s->cnt2; j++) {
			float spd = lerp2(s->spd1, s->spd2, j, s->cnt2);
			for (int i = 0; i < s->cnt1; i++) {
				createBullet(s, x, y, spd, randomFloat(s->ang1 - s->ang2, s->ang1 + s->ang2));
			}
		}
		break;
	case AIM_RAND_R:
		for (int j = 0; j < s->cnt2; j++) {
			a = s->ang1 + j * s->ang2;
			for (int i = 0; i < s->cnt1; i++) {
				createBullet(s, x, y, randomFloat(s->spd1, s->spd2), a);
				a += TAU / s->cnt1;
			}
		}
		break;
	case AIM_MEEK:
		for (int i = 0; i < s->cnt1 * s->cnt2; i++) {
			a = randomFloat(s->ang1 - s->ang2, s->ang1 + s->ang2);
			createBullet(s, x, y, randomFloat(s->spd1, s->spd2), a);
		}
		break;
	case AIM_AT_PYR:
		shPyr(s, pa, x, y);
		break;
	case AIM_ST_PYR:
		shPyr(s, s->ang1, x, y);
		break;
	}
}

/*
 * Updates
 */
static const uint32_t runningFlag[BT_N] = {
	[BT_INVALID] = 0,
	[BT_WAIT] = 1,
	[BT_WAITSIG] = 1,
	[BT_JMP] = 0,

	[BT_FAST] = 2,
	[BT_VEL] = 2,
	[BT_ACCEL] = 2,
	[BT_ACCEL_ANGV] = 2,
	[BT_STEP] = 2,
	[BT_SAVE] = 2,

	[BT_BOUNCE] = 4,
	[BT_WRAP] = 4,

	[BT_OFFSCR] = 0,
	[BT_INVULN] = 0,
	[BT_SPRITE] = 0,
	[BT_OPACITY] = 0,
	[BT_SIZE] = 0,
	[BT_SOUND] = 0,
	[BT_DELETE] = 0,
	[BT_SHOOT1] = 0,
	[BT_SHOOT2] = 0,
	[BT_NEWENM] = 0,

	[BT_CLOUD] = 8,

	[BT_MARK] = 0
};
static void bmTransStart(struct BulletMgrControl *ct, struct BulletMgr *m, struct Transform *tf, struct BulletTrans *t) {
	bool cont = false; /* Set to true if the transform is continuous (effects happen over multiple frames) */
	bool sfx = false;
	switch (t->type) {
	case BT_WAIT: /* a = count (start), d = count (remaining) */
		cont = true;
		t->s = t->a;
		break;
	case BT_WAITSIG: /* a = signals */
		cont = true;
		break;
	case BT_JMP: /* a = dest, b = cnt */
		if (t->b > 0) {
			m->transIndex = t->a;
			t->b--;
		}
		break;
	case BT_FAST: /* no params */
		cont = true;
		m->moveTime = BM_FAST_TIME;
		m->speed += BM_FAST_SPEED;
		break;
	case BT_VEL: /* p = spd, q = ang */
		sfx = true;
		if (!IS_NEGF(t->p)) {
			tfSetRotation(tf, t->p);
		}
		if (!IS_NEGF(t->q)) {
			m->speed = t->q;
		}
		break;
	case BT_ACCEL: /* a = time, p = dir, q = accel */
		cont = true;
		sfx = true;
		m->moveTime = t->a;
		break;
	case BT_ACCEL_ANGV: /* a = time, p = accel, q = wvel */
		cont = true;
		sfx = true;
		m->moveTime = t->a;
		break;
	case BT_STEP: /* a = time, b = cnt, c = type, p = ang, q = spd */
		if (!t->b)
			break;
		cont = true;
		//sfx = true;
		m->moveTime = t->a;
		m->stepCount = t->b;
		m->stepDecel = m->speed / t->a;
		break;
	case BT_SAVE: /* No params */
		m->saveX = tf->x;
		m->saveY = tf->y;
		m->saveAngle = atan2f(tf->rotImag, tf->rotReal);
		break;
	case BT_BOUNCE: /* a = cnt, b = walls, p = spd, q = x, r = y */
		cont = true;
		m->bounceCount = t->a; /* cnt */
		if (IS_NEGF(t->q))
			t->q = 0;
		if (IS_NEGF(t->r))
			t->r = 0;
		break;
	case BT_WRAP:
		cont = true;
		m->bounceCount = t->a; /* cnt */
		break;
	case BT_OFFSCR:
		m->offscreen = t->a;
		break;
	case BT_INVULN:
		m->invuln = t->a;
		break;
	case BT_SPRITE:
	{
		struct Bullet *b = getComponentOpt(DAN_BULLET, m->entity);
		if (b) {
			b->type = t->a;
			b->color = t->b;
		}
		break;
	}
	case BT_OPACITY:
	case BT_SIZE:
		cont = true;
		t->s = 0; /* d = curTime, a = end time */
		if (!t->a)
			t->a = 1;
		break;
	case BT_SOUND:
		danmakuPlaySfx(t->a);
		break;
	case BT_CLOUD:
	{
		cont = true;
		struct Bullet *b = getComponentOpt(DAN_BULLET, m->entity);
		if (b) {
			float time = bulletAnim(b, t->a + 1);
			tf->x -= m->speed * tf->rotReal * 4;
			tf->y -= m->speed * tf->rotImag * 4;
		}
		break;
	}
	case BT_MARK:
		if (t->a) {
			struct BulletMark *mrk = getComponentOpt(DAN_BULLET_MARK, m->entity);
			if (!mrk)
				mrk = newComponent(DAN_BULLET_MARK, m->entity);
			mrk->mark = t->a;
		} else {
			removeComponent(DAN_BULLET_MARK, m->entity);
		}
		break;
	default:
		break;
	}

	if (cont) {
		t->flags |= BT_RUNNING;
		m->transRunning |= runningFlag[t->type];
	}
	if (sfx && m->transSound >= 0) {
		danmakuPlaySfx(m->transSound);
	}
}
/* Returns speed */
static void bmTransStep(struct BulletMgr *m, struct Transform *tf, struct BulletTrans *t) {
	if (m->transSound >= 0)
		danmakuPlaySfx(m->transSound);

	float ang;
	float rr, ri;

	switch (t->c) {
	case STEP_ANGREL:
		rr = cosf(t->p);
		ri = sinf(t->p);
		CMUL(tf->rotReal, tf->rotImag, tf->rotReal, tf->rotImag, rr, ri);
		m->speed = t->q;
		break;
	case STEP_ANGPL:
		tfSetRotation(tf, angleToPlayer(tf->x, tf->y) + t->p);
		m->speed = t->q;
		break;
	case STEP_ANGSAVEPL:
		tfSetRotation(tf, angleToPlayer(m->saveX, m->saveY) + t->p);
		m->speed = t->q;
		break;
	case STEP_ANGSAVE:
		tfSetRotation(tf, m->saveAngle);
		m->speed = t->q;
		break;
	case STEP_ANG:
		tfSetRotation(tf, t->p);
		m->speed = t->q;
		break;
	case STEP_ANGRND:
		ang = randomFloat(-t->p, t->p);
		rr = cosf(ang);
		ri = sinf(ang);
		CMUL(tf->rotReal, tf->rotImag, tf->rotReal, tf->rotImag, rr, ri);
		m->speed = t->q;
		break;
	case STEP_ANGPLRND:
		tfSetRotation(tf, angleToPlayer(tf->x, tf->y) + randomFloat(-t->p, t->p));
		m->speed = t->q;
		break;
	case STEP_SPDRND:
		m->speed = randomFloat(-t->p, t->p);
		break;
	}
}
static void bmTransUpdate(struct BulletMgrControl *ct, struct BulletMgr *m, struct Transform *tf, struct BulletTrans *t) {
	bool ended = false;
	float td = gameSpeed;
	float x, y, spd;
	switch (t->type) {
	case BT_WAIT:
		t->s -= td;
		ended = t->s <= 0;
		break;
	case BT_FAST:
		m->speed -= BM_FAST_SPEED / BM_FAST_TIME;
		m->moveTime -= td;
		ended = m->moveTime <= 0;
		break;
	case BT_ACCEL:
		if (IS_NEGF(t->q)) {
			m->speed += t->p * td;
			if (m->speed < 0) {
				m->speed = -m->speed;
				tf->rotReal = -tf->rotReal;
				tf->rotImag = -tf->rotImag;
				t->p = fabsf(t->p);
			}
		} else {
			x = tf->rotReal * m->speed + cosf(t->q) * t->p;
			y = tf->rotImag * m->speed + sinf(t->q) * t->p;
			spd = sqrtf(x * x + y * y);
			tf->rotReal = x / spd;
			tf->rotImag = y / spd;
			m->speed = spd;
		}
		m->moveTime -= td;
		ended = m->moveTime <= 0;
		break;
	case BT_ACCEL_ANGV:
		m->speed += t->p;
		x = cosf(t->q);
		y = sinf(t->q);
		CMUL(tf->rotReal, tf->rotImag, tf->rotReal, tf->rotImag, x, y);
		m->moveTime -= td;
		ended = m->moveTime <= 0;
		break;
	case BT_WAITSIG: /* a = signals */
		ended = ct->signal & t->a;
		break;
	case BT_STEP:
		m->speed -= m->stepDecel * td;
		m->moveTime -= td;
		if (m->moveTime <= 0) {
			bmTransStep(m, tf, t);
			m->stepCount -= 1;
			ended = m->stepCount <= 0;
			if (!ended) {
				m->moveTime = t->a;
				m->stepDecel = m->speed / t->a;
			}
		}
		break;
	case BT_BOUNCE:
	{
		spd = IS_NEGF(t->p) ? m->speed : t->p;
		float offs;
		if (t->b & 1 && tf->y <= t->r) {
			/* Up */
			offs = t->r - tf->y;
			tf->y = t->r + offs;
			tf->rotImag = fabsf(tf->rotImag);
			m->speed = spd;
			m->bounceCount -= 1;
		} else if (t->b & 2 && tf->y >= (448 - t->r)) {
			/* Down */
			offs = tf->y - (448 - t->r);
			tf->y = (448 - t->r) - offs;
			tf->rotImag = -fabsf(tf->rotImag);
			m->speed = spd;
			m->bounceCount -= 1;
		} else if (t->b & 4 && tf->x <= (-192 + t->q)) {
			/* Left */
			offs = (-192 + t->q) - tf->x;
			tf->x = (-192 + t->q) + offs;
			tf->rotReal = fabsf(tf->rotReal);
			m->speed = spd;
			m->bounceCount -= 1;
		} else if (t->b & 8 && tf->x >= (192 - t->q)) {
			offs = tf->x - (192 - t->q);
			tf->x = (192 - t->q) - offs;
			tf->rotReal = -fabsf(tf->rotReal);
			m->speed = spd;
			m->bounceCount -= 1;
		}
		ended = m->bounceCount <= 0;
		break;
	}
	case BT_WRAP:
	{
		const float o = 32;
		bool wr = false;
		if (t->b & 1 && tf->y <= -o) {
			/* Up */
			tf->y += 448 + o * 2;
			wr = true;
		} else if (t->b & 2 && tf->y >= (448 + o)) {
			/* Down */
			tf->y -= 448 + o * 2;
			wr = true;
		} else if (t->b & 4 && tf->x <= (-192 - o)) {
			/* Left */
			tf->x += 384 + o * 2;
			wr = true;
		} else if (t->b & 8 && tf->x >= (192 + o)) {
			/* Right */
			tf->x -= 384 + o * 2;
			wr = true;
		}
		if (wr) {
			t->d--;
			ended = t->d <= 0;
			//audioPlaySFX(m->transSound, danmakuSfxVol[m->transSound], 0);
			if (!IS_NEGF(t->p))
				m->speed = t->p;
		}
		break;
	}
	case BT_OPACITY:
	{
		/* TODO */
		break;
	}
	case BT_SIZE:
	{
		/* TODO */
		break;
	}
	case BT_CLOUD:
	{
		struct Bullet *b = getComponentOpt(DAN_BULLET, m->entity);
		if (b) {
			ended = !b->animState;
		}
	}
	default:
		break;
	}

	if (ended) {
		t->flags &= ~BT_RUNNING;
		m->transRunning &= ~(runningFlag[t->type]);
	}
}
void bmDelete(entity_t en, int anim) {
	struct Bullet *b = getComponentOpt(DAN_BULLET, en);
	if (anim && b) {
		removeComponent(DAN_BULLET_MGR, en);
		bulletAnim(b, anim == 1 ? BULLET_AS_DELETE1 : BULLET_AS_DELETE2);
	} else {
		deleteEntity(en);
	}
}
static void bmUpdate(struct BulletMgrControl *ct, struct BulletMgr *m) {
	struct Transform *tf = getComponent(TRANSFORM, m->entity);

	for (int i = 0; i < m->transIndex; i++) {
		struct BulletTrans *t = &m->trans[i];
		if (t->flags & BT_RUNNING) {
			bmTransUpdate(ct, m, tf, t);
		}
	}

	/* Start new */
	while (m->transIndex < BULLETMGR_MAX_TRANS) {
		struct BulletTrans *t = &m->trans[m->transIndex];
		uint32_t tr = runningFlag[t->type];
		if (t->type == BT_INVALID ||
			(!(t->flags & BT_ASYNC) && m->transRunning) ||
			(tr & m->transRunning))
			break;
		if (t->type == BT_DELETE) {
			bmDelete(m->entity, t->a);
			return;
		} else if (t->type == BT_SHOOT2) {
			struct BulletTrans *t1 = &m->trans[m->transIndex - 1];
			if (!m->transIndex || t1->type != BT_SHOOT1)
				return;
			
			transformIndex = t1->c;
			ct->tempSpawner.aim = t1->d;
			ct->tempSpawner.spd1 = t1->p;
			ct->tempSpawner.spd2 = t1->q;
			ct->tempSpawner.type = t->a;
			ct->tempSpawner.color = (uint32_t)t->b | 0xFF000000;
			ct->tempSpawner.cnt1 = t->c;
			ct->tempSpawner.cnt2 = t->d;
			ct->tempSpawner.ang1 = t->p;
			ct->tempSpawner.ang2 = t->q;
			bmShoot(tf, &ct->tempSpawner);
			transformIndex = 0;

			if (t1->a) {
				bmDelete(m->entity, t->a);
				return;
			}
		}
		bmTransStart(ct, m, tf, t);
		m->transIndex++;
	}

	float xv = m->speed * tf->rotReal * gameSpeed;
	float yv = m->speed * tf->rotImag * gameSpeed;
	if (m->transRunning & 8) {
		/* Delay cloud */
		tf->x += xv * 0.25f;
		tf->y += yv * 0.25f;
	} else {
		tf->x += xv;
		tf->y += yv;
	}

	if (m->offscreen > 0) {
		m->offscreen -= gameSpeed;
	}
	m->time += gameSpeed;
}

static void bulletMgrFunc(struct BulletMgrControl *ct, const char *fn, entity_t fnEntity, int mark, int cond) {
	struct IchigoLocals *l = getComponentOpt(ICHIGO_LOCALS, fnEntity);
	if (!l)
		return; /* Enemy no longer exists */

	int list = mark ? DAN_BULLET_MARK : DAN_BULLET_MGR;
	float px, py;
	danPlayerPos(&px, &py);
	for (entity_t *e = clBegin(list); e; e = clNext(list, e)) {
		if (mark > 0 && ((struct BulletMark *)e)->mark != mark)
			continue;
		if (!mark && getComponentOpt(DAN_BULLET_MARK, *e))
			continue;
		struct Transform *tf = getComponent(TRANSFORM, *e);

		bool allow = false;
		float dx, dy;
		switch (cond & (FUNC_COND_INVERT - 1)) {
		case FUNC_COND_ALL:
			allow = true;
			break;
		case FUNC_COND_PLNEAR:
			dx = tf->x - px;
			dy = tf->y - py;
			allow = (dx * dx + dy * dy) < (l->f[0] * l->f[0]);
			break;
		case FUNC_COND_CIRCLE:
			dx = tf->x - l->f[1];
			dy = tf->y - l->f[2];
			allow = (dx * dx + dy * dy) < (l->f[0] * l->f[0]);
			break;
		case FUNC_COND_RECT:
			allow = tf->x >= l->f[0] && tf->x < l->f[1] && tf->y >= l->f[2] && tf->y < l->f[3];
			break;
		case FUNC_COND_MRKNEAR:
			if (l->i[0]) {
				for (struct BulletMark *mrk = clBegin(DAN_BULLET_MARK); !allow && mrk; mrk = clNext(DAN_BULLET_MARK, mrk)) {
					if (mrk->entity != *e && mrk->mark == l->i[0]) {
						struct Transform *tf2 = getComponent(TRANSFORM, mrk->entity);
						dx = tf->x - tf2->x;
						dy = tf->y - tf2->y;
						allow = (dx * dx + dy * dy) < (l->f[0] * l->f[0]);
					}
				}
			} else {
				for (struct BulletMgr *m = clBegin(DAN_BULLET_MGR); !allow && m; m = clNext(DAN_BULLET_MGR, m)) {
					if (m->entity != *e && !getComponentOpt(DAN_BULLET_MARK, m->entity)) {
						struct Transform *tf2 = getComponent(TRANSFORM, m->entity);
						dx = tf->x - tf2->x;
						dy = tf->y - tf2->y;
						allow = (dx * dx + dy * dy) < (l->f[0] * l->f[0]);
					}
				}
			}
			break;
		case FUNC_COND_ENMNEAR:
		{
			struct Transform *enmTf = getComponentOpt(TRANSFORM, fnEntity);
			if (enmTf) {
				dx = tf->x - enmTf->x;
				dy = tf->y - enmTf->y;
				allow = (dx * dx + dy * dy) < (l->f[0] * l->f[0]);
			}
			break;
		}
		case FUNC_COND_MRKLASERCOLL:
			for (struct BulletMark *mrk = clBegin(DAN_BULLET_MARK); !allow && mrk; mrk = clNext(DAN_BULLET_MARK, mrk)) {
				struct StraightLaser *lsr = getComponentOpt(DAN_LASER_ST, mrk->entity);
				if (lsr && mrk->entity != *e && mrk->mark == l->i[0]) {
					struct Transform *tf2 = getComponent(TRANSFORM, mrk->entity);
					struct CollisionInfo ci = {
						tf->x, tf->y, 1, 0, l->f[0], l->f[0],
						0, 0, 0, 0, 0, 0 };
					laserStSetCollider(&ci, lsr, tf2);
					allow = collisionCR(&ci);
				}
			}
			break;
		}
		if (cond & FUNC_COND_INVERT)
			allow = !allow;

		if (allow) {
			ichigoVmNew(&danmaku->danScript, &ct->funcVm, fnEntity);
			ichigoVmExec(&ct->funcVm, fn, "eff", tf->entity, tf->x, tf->y);
			ichigoVmUpdate(&ct->funcVm, 1);
			ichigoVmKillAll(&ct->funcVm);
		}
	}
}

static void bulletMgrUpdate(void *arg) {
	struct BulletMgrControl *ct = arg;
	for (struct BulletMgr *m = clBegin(DAN_BULLET_MGR); m; m = clNext(DAN_BULLET_MGR, m)) {
		bmUpdate(ct, m);
	}

	ct->signal = 0;

	if (ct->func)
		bulletMgrFunc(ct, ct->func, ct->funcEntity, ct->funcMark, ct->funcCond);
}

/*
 * Ichigo
 */
static struct BulletSpawner *getSpawner(struct IchigoVm *vm) {
	return &((struct BulletSpawnerList *)getComponent(DAN_BULLET_SPAWNER, vm->en))->list[ichigoGetInt(vm, 0)];
}
static void i_bmNew(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	memset(s, 0, sizeof(*s));
	s->cnt1 = s->cnt2 = 1;
	s->spd1 = s->spd2 = 1.0f;
	s->type = 0;
	s->color = 1;
	s->soundShoot = 2;
	s->soundTrans = 3;
}
static void i_bmCopy(struct IchigoVm *vm) {
	struct BulletSpawnerList *sl = getComponent(DAN_BULLET_SPAWNER, vm->en);
	int dst = ichigoGetInt(vm, 0);
	int src = ichigoGetInt(vm, 1);
	if (dst == src)
		return;
	memcpy(&sl->list[dst], &sl->list[src], sizeof(struct BulletSpawner));
}
static void i_bmShoot(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	struct Transform *tf = getComponent(TRANSFORM, vm->en);
	bmShoot(tf, s);
}
static void i_bmShootSingle(struct IchigoVm *vm) {
	struct BulletSpawner *s = &((struct BulletSpawnerList *)getComponent(DAN_BULLET_SPAWNER, vm->en))->list[ichigoGetInt(vm, 1)];
	struct Transform *tf = getComponent(TRANSFORM, vm->en);
	float x, y;
	getPos(&x, &y, tf, s);
	entity_t en = createBullet(s, x, y, s->spd1, s->ang1);
	ichigoSetEntity(vm, 0, en);
}

static void i_bmAim(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	s->aim = ichigoGetInt(vm, 1);
}
static void i_bmSprite(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	s->type = ichigoGetInt(vm, 1);
	s->color = ichigoGetInt(vm, 2);
}
static void i_bmOpacity(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	float o = ichigoGetFloat(vm, 1);
	uint32_t color = s->color & 0x00FFFFFF;
	s->color = color | ((int)(o * 255.0f) << 24);
}
static void i_bmOffset(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	s->x = ichigoGetFloat(vm, 1);
	s->y = ichigoGetFloat(vm, 2);
	s->posAbs = false;
}
static void i_bmOffsetAngle(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	float ang = ichigoGetFloat(vm, 1);
	float dist = ichigoGetFloat(vm, 2);
	s->offX = cosf(ang) * dist;
	s->offY = sinf(ang) * dist;
}
static void i_bmDist(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	s->dist = ichigoGetFloat(vm, 1);
}
static void i_bmSpeed(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	s->spd1 = ichigoGetFloat(vm, 1);
	s->spd2 = ichigoGetFloat(vm, 2);
}
static void i_bmAngle(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	s->ang1 = ichigoGetFloat(vm, 1);
	s->ang2 = ichigoGetFloat(vm, 2);
}
static void i_bmCount(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	s->cnt1 = ichigoGetInt(vm, 1);
	s->cnt2 = ichigoGetInt(vm, 2);
}
static void i_bmSound(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	s->soundShoot = ichigoGetInt(vm, 1);
	s->soundTrans = ichigoGetInt(vm, 2);
}
static void i_bmTrans(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	struct BulletTrans *t = &s->trans[s->transIndex];
	t->flags = ichigoGetInt(vm, 1);
	t->type = ichigoGetInt(vm, 2);
	t->a = ichigoGetInt(vm, 3);
	t->b = ichigoGetInt(vm, 4);
	t->c = NEG;
	t->p = ichigoGetFloat(vm, 5);
	t->q = ichigoGetFloat(vm, 6);
	t->r = NEGF;
	t->s = NEGF;
	s->transIndex++;
}
static void i_bmTransSet(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	int idx = ichigoGetInt(vm, 1);
	struct BulletTrans *t = &s->trans[idx];
	t->flags = ichigoGetInt(vm, 2);
	t->type = ichigoGetInt(vm, 3);
	t->a = ichigoGetInt(vm, 4);
	t->b = ichigoGetInt(vm, 5);
	t->c = NEG;
	t->p = ichigoGetFloat(vm, 6);
	t->q = ichigoGetFloat(vm, 7);
	t->r = NEGF;
	t->s = NEGF;
	s->transIndex = idx + 1;
}
static void i_bmTransEx(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	struct BulletTrans *t = &s->trans[s->transIndex];
	t->flags = ichigoGetInt(vm, 1);
	t->type = ichigoGetInt(vm, 2);
	t->a = ichigoGetInt(vm, 3);
	t->b = ichigoGetInt(vm, 4);
	t->c = ichigoGetInt(vm, 5);
	int d = ichigoGetInt(vm, 6);
	t->p = ichigoGetFloat(vm, 7);
	t->q = ichigoGetFloat(vm, 8);
	t->r = ichigoGetFloat(vm, 9);
	if (IS_NEG(d)) {
		t->s = ichigoGetFloat(vm, 10);
	} else {
		t->d = d;
	}
	s->transIndex++;
}
static void i_bmTransSetEx(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	int idx = ichigoGetInt(vm, 1);
	struct BulletTrans *t = &s->trans[idx];
	t->flags = ichigoGetInt(vm, 2);
	t->type = ichigoGetInt(vm, 3);
	t->a = ichigoGetInt(vm, 4);
	t->b = ichigoGetInt(vm, 5);
	t->c = ichigoGetInt(vm, 6);
	int d = ichigoGetInt(vm, 7);
	t->p = ichigoGetFloat(vm, 8);
	t->q = ichigoGetFloat(vm, 9);
	t->r = ichigoGetFloat(vm, 10);
	if (IS_NEG(d)) {
		t->s = ichigoGetFloat(vm, 11);
	} else {
		t->d = d;
	}
	s->transIndex = idx + 1;
}
static void i_bmOffsetAbs(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	s->x = ichigoGetFloat(vm, 1);
	s->y = ichigoGetFloat(vm, 2);
	s->posAbs = true;
}
static void i_bmSignal(struct IchigoVm *vm) {
	danmaku->bmc.signal |= ichigoGetInt(vm, 0);
}

/*
* Laser Stuff
*/
static void i_bmLaserStShoot(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	struct Transform *etf = getComponent(TRANSFORM, vm->en);
	float x, y;
	getPos(&x, &y, etf, s);
	float ang = s->ang1;
	if (s->aim == AIM_AT)
		ang += angleToPlayer(x, y);

	createLaserSt(s, x, y, s->spd1, ang, ichigoGetInt(vm, 1),
		ichigoGetFloat(vm, 2), ichigoGetFloat(vm, 3), ichigoGetFloat(vm, 4), ichigoGetFloat(vm, 5));
}
static void i_bmLaserCuShoot(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	struct Transform *etf = getComponent(TRANSFORM, vm->en);
	float x, y;
	getPos(&x, &y, etf, s);
	float ang = s->ang1;
	if (s->aim == AIM_AT)
		ang += angleToPlayer(x, y);

	createLaserCu(s, x, y, s->spd1, ang, ichigoGetInt(vm, 1), ichigoGetFloat(vm, 2),
		ichigoGetInt(vm, 3), ichigoGetInt(vm, 4), ichigoGetInt(vm, 5));
}
static void i_bmLaserInfShoot(struct IchigoVm *vm) {
	struct BulletSpawnerList *sl = getComponent(DAN_BULLET_SPAWNER, vm->en);
	struct BulletSpawner *s = &sl->list[ichigoGetInt(vm, 1)];
	struct Transform *etf = getComponent(TRANSFORM, vm->en);
	float x, y;
	getPos(&x, &y, etf, s);
	float ang = s->ang1;
	if (s->aim == AIM_AT)
		ang += angleToPlayer(x, y);

	entity_t en = createLaserInf(vm->en, s, x, y, s->spd1, ang, ichigoGetInt(vm, 2), ichigoGetFloat(vm, 3), ichigoGetFloat(vm, 4),
		ichigoGetInt(vm, 5), ichigoGetInt(vm, 6), ichigoGetInt(vm, 7), ichigoGetInt(vm, 8));

	ichigoSetEntity(vm, 0, en);

	if (sl->laserId >= 0) {
		sl->lasers[sl->laserId] = en;
		sl->laserId = sl->laserId >= BULLETMGR_MAX_LASER_ID - 1? 0 : sl->laserId + 1;
	}
}
static void i_bmLaserInfRotate(struct IchigoVm *vm) {
	struct Transform *tf = getComponentOpt(TRANSFORM, ichigoGetEntity(vm, 0));
	if (tf) {
		float a = ichigoGetFloat(vm, 1);
		float rr = cosf(a), ri = sinf(a);
		CMUL(tf->rotReal, tf->rotImag, tf->rotReal, tf->rotImag, rr, ri);
	}
}
static void i_bmLaserInfAngle(struct IchigoVm *vm) {
	struct Transform *tf = getComponentOpt(TRANSFORM, ichigoGetEntity(vm, 0));
	if (tf) {
		float a = ichigoGetFloat(vm, 1);
		a += angleToPlayer(tf->x, tf->y);
		tfSetRotation(tf, a);
	}
}
static void i_bmLaserInfOffset(struct IchigoVm *vm) {
	entity_t en = ichigoGetEntity(vm, 0);
	struct InfiniteLaser *l = getComponentOpt(DAN_LASER_INF, en);
	if (l) {
		if (l->parent) {
			l->offX = ichigoGetFloat(vm, 1);
			l->offY = ichigoGetFloat(vm, 2);
		} else {
			struct Transform *tf = getComponent(TRANSFORM, en);
			tf->x = ichigoGetFloat(vm, 1);
			tf->y = ichigoGetFloat(vm, 2);
		}
	}
}
static void i_bmLaserInfTest(struct IchigoVm *vm) {
	entity_t en = ichigoGetEntity(vm, 1);
	struct InfiniteLaser *l = getComponentOpt(DAN_LASER_INF, en);
	ichigoSetInt(vm, 0, l ? 1 : 0);
}
static void i_bmLaserInfBreak(struct IchigoVm *vm) {
	entity_t en = ichigoGetEntity(vm, 0);
	struct InfiniteLaser *l = getComponent(DAN_LASER_INF, en);
	if (l) {
		l->state = L_INF_SHRINK;
		l->time = 0;
	}
}
static void i_bmLaserInfGet(struct IchigoVm *vm) {
	struct BulletSpawnerList *sl = getComponent(DAN_BULLET_SPAWNER, vm->en);
	ichigoSetEntity(vm, 0, sl->lasers[ichigoGetInt(vm, 1)]);
}

static void i_bmClear(struct IchigoVm *vm) {
	struct Transform *tf = getComponent(TRANSFORM, vm->en);
	bulletDeleteCircle(tf->x, tf->y, ichigoGetFloat(vm, 0), ichigoGetInt(vm, 1), true);
}

static void i_bmProtect(struct IchigoVm *vm) {
	struct BulletSpawner *s = getSpawner(vm);
	s->protectRange = ichigoGetFloat(vm, 1);
}


static void i_funcCall(struct IchigoVm *vm) {
	struct BulletMgrControl *ct = &danmaku->bmc;
	const char *fn = ichigoGetString(NULL, vm, 0);
	bulletMgrFunc(ct, fn, vm->en, ichigoGetInt(vm, 1), ichigoGetInt(vm, 2));
}

static void i_funcSet(struct IchigoVm *vm) {
	struct BulletMgrControl *ct = &danmaku->bmc;
	uint16_t sLen;
	ct->func = ichigoGetString(&sLen, vm, 0);
	if (!sLen)
		ct->func = NULL;
	ct->funcEntity = vm->en;
	ct->funcMark = ichigoGetInt(vm, 1);
	ct->funcCond = ichigoGetInt(vm, 2);
}

static void i_bulletGetTime(struct IchigoVm *vm) {
	entity_t en = ichigoGetEntity(vm, 1);
	struct BulletMgr *b = getComponentOpt(DAN_BULLET_MGR, en);
	ichigoSetFloat(vm, 0, b ? b->time : 0);
}

static void i_bulletGetPos(struct IchigoVm *vm) {
	entity_t en = ichigoGetEntity(vm, 0);
	struct Transform *tf = getComponent(TRANSFORM, en);
	ichigoSetFloat(vm, 1, tf->x);
	ichigoSetFloat(vm, 2, tf->y);
}
static void i_bulletSetPos(struct IchigoVm *vm) {
	entity_t en = ichigoGetEntity(vm, 0);
	struct Transform *tf = getComponent(TRANSFORM, en);
	tf->x = ichigoGetFloat(vm, 1);
	tf->y = ichigoGetFloat(vm, 2);
}

static void i_bulletGetAngSpd(struct IchigoVm *vm) {
	entity_t en = ichigoGetEntity(vm, 0);
	struct Transform *tf = getComponent(TRANSFORM, en);
	struct BulletMgr *m = getComponentOpt(DAN_BULLET_MGR, en);
	ichigoSetFloat(vm, 1, tf->rotReal);
	ichigoSetFloat(vm, 2, tf->rotImag);
	ichigoSetFloat(vm, 3, m->speed);
}

static void i_bulletSetAngSpd(struct IchigoVm *vm) {
	entity_t en = ichigoGetEntity(vm, 0);
	struct Transform *tf = getComponent(TRANSFORM, en);
	struct BulletMgr *m = getComponentOpt(DAN_BULLET_MGR, en);
	tf->rotReal = ichigoGetFloat(vm, 1);
	tf->rotImag = ichigoGetFloat(vm, 2);
	m->speed = ichigoGetFloat(vm, 3);
}

static void i_bulletSetTrans(struct IchigoVm *vm) {
	struct BulletMgr *m = getComponentOpt(DAN_BULLET_MGR, ichigoGetEntity(vm, 0));
	if (!m)
		return;
	if (ichigoGetInt(vm, 2)) { // clearRunning
		for (int i = 0; i < m->transIndex; i++) {
			m->trans[i].flags &= ~BT_RUNNING;
		}
		m->transRunning = 0;
	}
	m->transIndex = ichigoGetInt(vm, 1);
}

static void i_bulletGetMark(struct IchigoVm *vm) {
	struct BulletMark *mrk = getComponentOpt(DAN_BULLET_MARK, ichigoGetEntity(vm, 1));
	ichigoSetInt(vm, 0, mrk? mrk->mark : 0);
}

static void i_bulletSetMark(struct IchigoVm *vm) {
	entity_t en = ichigoGetEntity(vm, 0);
	int markVal = ichigoGetInt(vm, 1);
	if (markVal) {
		struct BulletMark *mrk = getComponentOpt(DAN_BULLET_MARK, en);
		if (!mrk)
			mrk = newComponent(DAN_BULLET_MARK, en);
		mrk->mark = ichigoGetInt(vm, 1);
	} else {
		removeComponent(DAN_BULLET_MARK, en);
	}
}

static int i_getVarLASER_ID(struct IchigoVm *vm) {
	struct BulletSpawnerList *sl = getComponent(DAN_BULLET_SPAWNER, vm->en);
	return sl->laserId;
}
static void i_setVarLASER_ID(struct IchigoVm *vm, int val) {
	struct BulletSpawnerList *sl = getComponent(DAN_BULLET_SPAWNER, vm->en);
	sl->laserId = val;
}

/*
 * Init/fini
 */

void bulletMgrInit(struct Danmaku *game) {
	struct BulletMgrControl *ct = &game->bmc;
	componentListInit(DAN_BULLET_SPAWNER, struct BulletSpawnerList);
	componentListInit(DAN_BULLET_MGR, struct BulletMgr);
	componentListInit(DAN_BULLET_MARK, struct BulletMark);

	ct->tempSpawner.soundShoot = 2;
	ct->tempSpawner.soundTrans = 3;

	danmakuSetInstr(100, i_bmNew);
	danmakuSetInstr(101, i_bmCopy);
	danmakuSetInstr(102, i_bmShoot);
	danmakuSetInstr(103, i_bmAim);
	danmakuSetInstr(104, i_bmSprite);
	danmakuSetInstr(105, i_bmOpacity);
	//danmakuSetInstr(106, i_bmDelay);
	danmakuSetInstr(107, i_bmOffset);
	danmakuSetInstr(108, i_bmOffsetAngle);
	danmakuSetInstr(109, i_bmDist);
	danmakuSetInstr(110, i_bmSpeed);
	danmakuSetInstr(111, i_bmAngle);
	danmakuSetInstr(112, i_bmCount);
	danmakuSetInstr(113, i_bmSound);
	danmakuSetInstr(114, i_bmTrans);
	danmakuSetInstr(115, i_bmTransSet);
	danmakuSetInstr(116, i_bmTransEx);
	danmakuSetInstr(117, i_bmTransSetEx);
	danmakuSetInstr(118, i_bmOffsetAbs);
	danmakuSetInstr(119, i_bmSignal);
	danmakuSetInstr(120, i_bmLaserStShoot);
	danmakuSetInstr(121, i_bmLaserInfShoot);
	danmakuSetInstr(122, i_bmLaserInfRotate);
	danmakuSetInstr(123, i_bmLaserInfAngle);
	danmakuSetInstr(124, i_bmLaserInfOffset);
	danmakuSetInstr(125, i_bmLaserInfTest);
	danmakuSetInstr(126, i_bmLaserInfBreak);
	danmakuSetInstr(127, i_bmLaserInfGet);
	danmakuSetInstr(128, i_bmLaserCuShoot);
	danmakuSetInstr(129, i_bmClear);
	danmakuSetInstr(130, i_bmProtect);

	danmakuSetInstr(161, i_funcCall);
	danmakuSetInstr(162, i_funcSet);
	danmakuSetInstr(163, i_bulletGetTime);
	danmakuSetInstr(164, i_bulletSetPos);
	danmakuSetInstr(165, i_bulletGetAngSpd);
	danmakuSetInstr(166, i_bulletSetAngSpd);
	danmakuSetInstr(167, i_bulletSetTrans);
	danmakuSetInstr(168, i_bulletGetMark);
	danmakuSetInstr(169, i_bulletSetMark);
	danmakuSetInstr(170, i_bmShootSingle);
	danmakuSetInstr(171, i_bulletGetPos);

	danmakuSetVar(35, REG_INT, i_getVarLASER_ID, i_setVarLASER_ID);

	addUpdate(UPDATE_NORM, bulletMgrUpdate, &game->bmc);
}
void bulletMgrFini(struct Danmaku *game) {
	struct BulletMgrControl *ct = &game->bmc;
	componentListFini(DAN_BULLET_SPAWNER);
	componentListFini(DAN_BULLET_MGR);
	componentListFini(DAN_BULLET_MARK);

	removeUpdate(UPDATE_NORM, bulletMgrUpdate);
}