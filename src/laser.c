#include "redefined.h"

static void drawLightBall(int col, float laserWidth);
static void setRectColl(struct CollisionInfo *c, struct Transform *tf, int type, float length, float width, float fac);

/* STRAIGHT */

void laserStSetCollider(struct CollisionInfo *c, struct StraightLaser *l, struct Transform *tf) {
	setRectColl(c, tf, l->type, l->length, l->width, -0.5f);
}

static void laserStNotifier(void *arg, void *component, int type) {
	struct LaserController *lc = arg;
	struct StraightLaser *l = component;
	if (type == NOTIFY_CREATE) {
		l->next = 0;
		l->prev = lc->stLast;
		if (l->prev) {
			struct StraightLaser *o = getComponent(DAN_LASER_ST, l->prev);
			o->next = l->entity;
		} else {
			lc->stFirst = l->entity;
		}
		lc->stLast = l->entity;
	} else if (type == NOTIFY_DELETE) {
		struct StraightLaser *o;
		if (l->prev) {
			o = getComponent(DAN_LASER_ST, l->prev);
			o->next = l->next;
		} else {
			lc->stFirst = l->next;
		}
		if (l->next) {
			o = getComponent(DAN_LASER_ST, l->next);
			o->prev = l->prev;
		} else {
			lc->stLast = l->prev;
		}
	} else if (type == NOTIFY_PURGE) {
		lc->stFirst = 0;
		lc->stLast = 0;
	}
}

static void laserStUpdate(struct LaserController *lc, struct StraightLaser *l, struct CollisionInfo *c) {
	struct BulletMgr *m = getComponent(DAN_BULLET_MGR, l->entity);
	struct Transform *tf = getComponent(TRANSFORM, l->entity); /* TF is position of laser head */
	switch (l->state) {
	case L_ST_GROW:
		l->range -= m->speed * gameSpeed;
		l->length += m->speed * gameSpeed;
		if (l->length >= l->endLength) {
			l->length = l->endLength;
			l->state = L_ST_NORM;
		}
		break;
	case L_ST_NORM:
		l->range -= m->speed * gameSpeed;
		if (l->range <= 0) {
			l->range = 0;
			l->state = L_ST_SHRINK;
		}
		break;
	case L_ST_SHRINK:
		l->length -= m->speed * gameSpeed;
		if (l->length <= 0) {
			deleteEntity(l->entity);
			return;
		}
		/* Undo speed applied by BM, this is ugly */
		tf->x -= m->speed * tf->rotReal;
		tf->y -= m->speed * tf->rotImag;
		break;
	}

	float boundW = l->length, boundH = l->width;
	CMUL(boundW, boundH, boundW, boundH, tf->rotReal, tf->rotImag);
	boundW = fabsf(boundW);
	boundH = fabsf(boundH);
	if (tf->x < -192 - boundW || tf->x > 192 + boundW || tf->y < -boundH || tf->y > 448 + boundH) {
		struct BulletMgr *m = getComponentOpt(DAN_BULLET_MGR, l->entity);
		if (!m || m->offscreen <= 0) {
			deleteEntity(l->entity);
			return;
		}
	}

	if (l->grazeCounter)
		l->grazeCounter -= 1;

	laserStSetCollider(c, l, tf);
	c->w1 = danmaku->player.grazebox;
	if (collisionCR(c)) {
		c->w1 = danmaku->player.hitbox;
		if (collisionCR(c)) {
			danPlayerHit();
		} else if (!l->grazeCounter) {
			danPlayerGraze();
			l->grazeCounter = 30;
		}
	}
}

static void laserStDraw(struct StraightLaser *l) {
	struct Transform *tf = getComponent(TRANSFORM, l->entity);
	drawTransform(tf);
	drawColor(1, 1, 1, 1);
	if (l->state == L_ST_SHRINK && l->flags & LASER_ST_DELETE_BALL) {
		drawBlend(BLEND_ADD);
		drawLightBall(l->color, l->width);
		drawTransform(tf);
	}
	drawTransformRotation(tf);
	drawTranslate(l->length * -0.5f, 0);
	drawRotate(DEG2RAD(90));
	drawBlend(l->flags & LASER_ST_ALPHA ? BLEND_ALPHA : BLEND_ADD);
	struct BulletType *t = &danmaku->bullet.types[l->type];
	float src[4];
	bulletSrcRect(src, t, l->color, 0);
	drawSrcRect(src[0], src[1], src[2], src[3]);
	drawRect(l->width, l->length);

	if (l->state == L_ST_GROW && !(l->flags & LASER_ST_NO_CREATE_BALL)) {
		drawTranslate(0, l->length * 0.5f);
		float fac = l->length / l->endLength;
		float s = lerp(2, 0.5f, fac);
		drawScale(s, s);
		drawColor(1, 1, 1, lerp(0, 1, fac));
		drawBlend(BLEND_ADD);
		drawLightBall(l->color, l->width);
	}
}

/* INFINITE */

static void laserInfUpdate(struct InfiniteLaser *l, struct CollisionInfo *c) {
	struct BulletMgr *m = getComponent(DAN_BULLET_MGR, l->entity);
	struct Transform *tf = getComponent(TRANSFORM, l->entity);

	if (l->parent) {
		struct Transform *ptf = getComponentOpt(TRANSFORM, l->parent);
		if (ptf) {
			tf->x = ptf->x + l->offX + l->dist * tf->rotReal;
			tf->y = ptf->y + l->offY + l->dist * tf->rotImag;
		} else {
			l->state = L_INF_SHRINK;
		}
	}

	l->time += gameSpeed;
	switch (l->state) {
	case L_INF_START:
		if (l->time >= l->startTime) {
			l->time = 0;
			l->state = L_INF_GROW;
		}
		break;
	case L_INF_GROW:
		if (l->time > l->expandTime) {
			l->time = 0;
			l->state = L_INF_NORM;
		}
		break;
	case L_INF_NORM:
		if (l->time > l->duration) {
			l->time = 0;
			l->state = L_INF_SHRINK;
		}
		break;
	case L_INF_SHRINK:
		if (l->time > l->shrinkTime) {
			deleteEntity(l->entity);
			return;
		}
		break;
	}

	if (tf->x < -192 || tf->x > 192 || tf->y < 0|| tf->y > 448) {
		struct BulletMgr *m = getComponentOpt(DAN_BULLET_MGR, l->entity);
		if (!m || m->offscreen <= 0) {
			deleteEntity(l->entity);
			return;
		}
	}

	if (l->grazeCounter)
		l->grazeCounter -= 1;

	if (l->state == L_INF_NORM) {
		setRectColl(c, tf, l->type, l->length, l->width, 0.5f);
		c->w1 = danmaku->player.grazebox;
		if (collisionCR(c)) {
			c->w1 = danmaku->player.hitbox;
			if (collisionCR(c)) {
				danPlayerHit();
			} else if (!l->grazeCounter) {
				danPlayerGraze();
				l->grazeCounter = 30;
			}
		}
	}
}

static void laserInfDraw(struct InfiniteLaser *l) {
	struct Transform *tf = getComponent(TRANSFORM, l->entity);
	drawTransform(tf);
	drawColor(1, 1, 1, 1);
	drawBlend(BLEND_ADD);
	drawLightBall(l->color, l->width);

	drawTransform(tf);
	drawTransformRotation(tf);
	drawTranslate(l->length * 0.5f, 0);
	drawRotate(DEG2RAD(90));

	float width = l->width;
	if (l->state == L_INF_START) {
		drawSrcRect(8, 8, 1, 1);
		width = 1;
	} else {
		struct BulletType *t = &danmaku->bullet.types[l->type];
		float src[4];
		bulletSrcRect(src, t, l->color, 0);
		drawSrcRect(src[0], src[1], src[2], src[3]);
	}
	if (l->state == L_INF_GROW) {
		float fac = l->time / l->expandTime;
		if (l->flags & LASER_INF_FADE) {
			drawColor(1, 1, 1, fac);
		} else {
			width = modeLerp(1, l->width, fac, INTERP_EASEOUT1);
		}
	} else if (l->state == L_INF_SHRINK) {
		float fac = l->time / l->shrinkTime;
		if (l->flags & LASER_INF_FADE) {
			drawColor(1, 1, 1, 1.0f - fac);
		} else {
			width = modeLerp(l->width, 0, fac, INTERP_EASEIN1);
		}
	}

	drawRect(width, l->length);
}

/* CURVE */

static void laserCuNotifier(void *arg, void *component, int type) {
	(void)arg;
	struct CurveLaser *l = component;
	if (type == NOTIFY_CREATE) {
		l->head = -1;
		vecCreate(&l->nodes, sizeof(struct CurveLaserNode));
	} else if (type == NOTIFY_DELETE) {
		vecDestroy(&l->nodes);
	} else if (type == NOTIFY_PURGE) {
		for (l = clBegin(DAN_LASER_CU); l; l = clNext(DAN_LASER_CU, l)) {
			vecDestroy(&l->nodes);
		}
	}
}

static void laserCuAddNode(struct CurveLaser *l, struct Transform *tf) {
	assert(l->head == l->nodes.nElements - 1);
	l->head++;
	struct CurveLaserNode *n = vecInsert(&l->nodes, -1);
	n->x = tf->x;
	n->y = tf->y;
	n->rr = tf->rotReal;
	n->ri = tf->rotImag;
}
static void laserCuUpdateNode(struct CurveLaser *l, struct Transform *tf) {
	l->head = (l->head == l->nodes.nElements - 1) ? 0 : l->head + 1;
	struct CurveLaserNode *n = vecAt(&l->nodes, l->head);
	n->x = tf->x;
	n->y = tf->y;
	n->rr = tf->rotReal;
	n->ri = tf->rotImag;
}
static void laserCuDeleteNode(struct CurveLaser *l) {
	if (l->head == l->nodes.nElements - 1) {
		vecDelete(&l->nodes, 0);
		l->head--;
	} else {
		vecDelete(&l->nodes, l->head + 1);
	}
}

static void laserCuUpdate(struct CurveLaser *l, struct CollisionInfo *c) {
	struct Transform *tf = getComponent(TRANSFORM, l->entity);
	//l->time += gameSpeed;
	l->time += 1; /* Works unreliably with slowed time */
	switch (l->state) {
	case L_CU_START:
		if (l->time >= l->startTime) {
			l->time = 0;
			l->state = L_INF_GROW;
		}
		break;
	case L_CU_GROW:
		laserCuAddNode(l, tf);
		if (l->time > l->expandTime) {
			l->time = 0;
			l->state = L_INF_NORM;
		}
		break;
	case L_CU_NORM:
		laserCuUpdateNode(l, tf);
		if (l->time > l->duration) {
			l->time = 0;
			l->state = L_INF_SHRINK;
		}
		break;
	case L_CU_SHRINK:
		//laserCuUpdateNode(l, tf);
		laserCuDeleteNode(l);
		if (l->nodes.nElements == 0) {
			deleteEntity(l->entity);
			return;
		}
		break;
	}

	if (l->grazeCounter)
		l->grazeCounter -= 1;

	/* Check offscreen and collisions */
	int len = l->nodes.nElements;
	c->w2 = l->width * (2.4f / 16);
	struct BulletMgr *m = getComponentOpt(DAN_BULLET_MGR, l->entity);
	if (len) {
		bool offscreen = m && !m->offscreen;
		bool hit = false;
		float margin = l->width;
		float w = 192 + margin;
		int idx = l->head;
		for (int i = 0; i < len; i++) {
			struct CurveLaserNode *n = vecAt(&l->nodes, idx);
			idx = idx ? idx - 1 : len - 1;

			if (offscreen && n->x > -w && n->x < w && n->y > -margin && n->y < 448 + margin) {
				offscreen = false;
			}
			
			if (i >= 3 && i < len - 3) {
				c->x2 = n->x;
				c->y2 = n->y;
				c->w1 = danmaku->player.grazebox;
				if (collisionCC(c)) {
					c->w1 = danmaku->player.hitbox;
					if (!hit && collisionCC(c)) {
						hit = true;
						danPlayerHit();
					} else if (!l->grazeCounter) {
						danPlayerGraze();
						l->grazeCounter = 30;
					}
				}
			}
		}
		if (offscreen) {
			deleteEntity(l->entity);
			return;
		}
	}
}

static void laserCuDraw(struct CurveLaser *l) {
	int len = l->nodes.nElements;
	if (len < 2)
		return;

	drawMatIdentity();
	drawBlend(BLEND_ADD);
	float u1 = l->color * 16.0f / 256;
	float u2 = u1 + 16.0f / 256;
	float v1 = 6.0f / 256;
	float v2 = 250.0f / 256;
	float w2 = l->width * 0.5f;

	size_t vIdxSize = (len - 1) * 6 * sizeof(int);
	unsigned int *vIdx = stackAlloc(vIdxSize);

	int idx = l->head;
	for (int i = 0; i < len; i++) {
		struct CurveLaserNode *n = vecAt(&l->nodes, idx);
		idx = idx ? idx - 1 : len - 1;
		float v = modeLerp(v1, v2, (float)i / (len - 1), INTERP_LINEAR);
		drawVertex(n->x + n->ri * w2, n->y - n->rr * w2, 0, u1, v, 1, 1, 1, 1);
		drawVertex(n->x - n->ri * w2, n->y + n->rr * w2, 0, u2, v, 1, 1, 1, 1);

		if (i) {
			int base = (i - 1) * 2;
			int iBase = (i - 1) * 6;
			vIdx[iBase + 0] = base + 2;
			vIdx[iBase + 1] = base + 0;
			vIdx[iBase + 2] = base + 3;
			vIdx[iBase + 3] = base + 3;
			vIdx[iBase + 4] = base + 1;
			vIdx[iBase + 5] = base + 0;
		}
	}
	drawIndices(len * 2, (len - 1) * 6, vIdx);
	stackDealloc(vIdxSize);
}

/* GENERIC */

static void setRectColl(struct CollisionInfo *c, struct Transform *tf, int type, float length, float width, float fac) {
	struct BulletType *t = &danmaku->bullet.types[type];
	float hb2 = t->hitbox * 2;
	c->x2 = tf->x + length * fac * tf->rotReal;
	c->y2 = tf->y + length * fac * tf->rotImag;
	c->rr2 = tf->rotReal;
	c->ri2 = tf->rotImag;
	c->w2 = lerp(length / t->h * hb2, length, 0.5f);
	c->h2 = width / t->w * hb2;
}

static void drawLightBall(int col, float laserWidth) {
	drawRotate(randomFloat(-PI, PI));
	drawSrcRect(col % 8 * 32 + 256, col / 8 * 32, 32, 32);
	float sz = laserWidth * 2;
	drawRect(sz + randomFloat(-2, 2), sz + randomFloat(-2, 2));
}

static void laserUpdate(void *arg) {
	struct LaserController *lc = arg;
	struct CollisionInfo c;
	danPlayerPos(&c.x1, &c.y1);
	for (struct StraightLaser *l = clBegin(DAN_LASER_ST); l; l = clNext(DAN_LASER_ST, l)) {
		laserStUpdate(lc, l, &c);
	}

	for (struct InfiniteLaser *l = clBegin(DAN_LASER_INF); l; l = clNext(DAN_LASER_INF, l)) {
		laserInfUpdate(l, &c);
	}

	for (struct CurveLaser *l = clBegin(DAN_LASER_CU); l; l = clNext(DAN_LASER_CU, l)) {
		laserCuUpdate(l, &c);
	}
}

static void laserDraw(void *arg) {
	struct LaserController *lc = arg;
	/* Straight */
	drawTexture(0, danmaku->bullet.tex);
	entity_t en = lc->stFirst;
	while (en) {
		struct StraightLaser *l = getComponent(DAN_LASER_ST, en);
		laserStDraw(l);
		en = l->next;
	}

	/* Infinite */
	for (struct InfiniteLaser *l = clBegin(DAN_LASER_INF); l; l = clNext(DAN_LASER_INF, l)) {
		laserInfDraw(l);
	}

	/* Curve */
	drawTexture(0, lc->curveLaserTex);
	for (struct CurveLaser *l = clBegin(DAN_LASER_CU); l; l = clNext(DAN_LASER_CU, l)) {
		laserCuDraw(l);
	}
}

void laserInit(struct Danmaku *dan) {
	componentListInit(DAN_LASER_ST, struct StraightLaser);
	componentListInit(DAN_LASER_INF, struct InfiniteLaser);
	componentListInit(DAN_LASER_CU, struct CurveLaser);

	dan->laser.curveLaserTex = loadTexture("tex/dan/LaserCu.png");
	
	setNotifier(DAN_LASER_ST, laserStNotifier, &dan->laser);
	setNotifier(DAN_LASER_CU, laserCuNotifier, &dan->laser);

	addUpdate(UPDATE_LATE, laserUpdate, &dan->laser);
	addDrawUpdate(DRAW_LASER, laserDraw, &dan->laser);
}
void laserFini(struct Danmaku *dan) {
	removeDrawUpdate(DRAW_LASER);
	removeUpdate(UPDATE_LATE, laserUpdate);

	deleteTexture(dan->laser.curveLaserTex);
	dan->laser.curveLaserTex = NULL;

	componentListFini(DAN_LASER_CU);
	componentListFini(DAN_LASER_INF);
	componentListFini(DAN_LASER_ST);
}