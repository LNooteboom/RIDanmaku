#include "redefined.h"
#include <physics/collision.h>

#include <string.h>
#include <ctype.h>

#define FIREBALL_ANIM_TIME 16

void bulletDeleteCircle(float x, float y, float r, bool item, bool force) {
	struct CollisionInfo cc;
	cc.x1 = x;
	cc.y1 = y;
	cc.w1 = r;
	for (struct Bullet *b = clBegin(DAN_BULLET); b; b = clNext(DAN_BULLET, b)) {
		struct Transform *tf = getComponent(TRANSFORM, b->entity);
		cc.x2 = tf->x;
		cc.y2 = tf->y;
		cc.w2 = danmaku->bullet.types[b->type].hitbox;
		if (b->animState <= BULLET_AS_START3 && collisionCC(&cc)) {
			if (!force) {
				struct BulletMgr *m = getComponentOpt(DAN_BULLET_MGR, b->entity);
				if (m && m->invuln > 0)
					continue;
			}
			if (item) {
				itemNew(tf->x, tf->y, ITEM_CANCEL);
			}
			removeComponent(DAN_BULLET_MGR, b->entity);
			removeComponent(DAN_BULLET_MARK, b->entity);
			bulletAnim(b, BULLET_AS_DELETE1);
		}
	}
}
void bulletDeleteRect(float x, float y, float rot, float w, float h, bool item, bool force) {
	struct CollisionInfo cc;
	cc.x2 = x;
	cc.y2 = y;
	cc.w2 = w;
	cc.h2 = h;
	cc.rr2 = cosf(rot);
	cc.ri2 = sinf(rot);
	for (struct Bullet *b = clBegin(DAN_BULLET); b; b = clNext(DAN_BULLET, b)) {
		struct Transform *tf = getComponent(TRANSFORM, b->entity);
		cc.x1 = tf->x;
		cc.y1 = tf->y;
		cc.w1 = danmaku->bullet.types[b->type].hitbox;
		if (!b->animState && collisionCR(&cc)) {
			if (!force) {
				struct BulletMgr *m = getComponentOpt(DAN_BULLET_MGR, b->entity);
				if (m && m->invuln > 0)
					continue;
			}
			if (!b->grazeCounter && item) {
				itemNew(tf->x, tf->y, ITEM_CANCEL);
			}
			removeComponent(DAN_BULLET_MGR, b->entity);
			removeComponent(DAN_BULLET_MARK, b->entity);
			bulletAnim(b, BULLET_AS_DELETE1);
		}
	}
}

static const float bulletAnimTime[BULLET_AS_N] = {
	0, 8, 15, 25, 15, 8
};

static void bulletUpdate(void *arg) {
	struct BulletController *bc = arg;

	struct CollisionInfo cc;
	danPlayerPos(&cc.x1, &cc.y1);

	struct Bullet *last[4] = { NULL, NULL, NULL, NULL };
	for (int i = 0; i < 4; i++) {
		bc->drawFirst[i] = 0;
	}

	for (struct Bullet *b = clBegin(DAN_BULLET); b; b = clNext(DAN_BULLET, b)) {
		struct Transform *tf = getComponent(TRANSFORM, b->entity);
		float margin = 48;
		if (tf->x < -192 - margin || tf->x > 192 + margin || tf->y < -margin || tf->y > 448 + margin) {
			struct BulletMgr *m = getComponentOpt(DAN_BULLET_MGR, b->entity);
			if (!m || m->offscreen <= 0)
				deleteEntity(b->entity);
			continue;
		}

		if (b->grazeCounter)
			b->grazeCounter--;

		struct BulletType *t = &bc->types[b->type];
		if (!b->animState) {
			cc.x2 = tf->x;
			cc.y2 = tf->y;
			cc.w2 = t->hitbox;
			cc.w1 = danmaku->player.grazebox;
			if (collisionCC(&cc)) {
				cc.w1 = danmaku->player.hitbox;
				if (collisionCC(&cc)) {
					danPlayerHit();
					struct BulletMgr *m = getComponentOpt(DAN_BULLET_MGR, b->entity);
					if (!m || m->invuln <= 0)
						bmDelete(b->entity, BULLET_AS_DELETE1);
				} else if (!b->grazeCounter) {
					danPlayerGraze();
					b->grazeCounter = 30;
				}
			}
		}

		/* Update animation time and state */
		
		float td = gameSpeed;
		switch (t->animMode) {
		case BULLET_ANIM_ROTATE_CW:
			b->spriteRot += DEG2RAD(10) * td;
			if (b->spriteRot > PI)
				b->spriteRot -= 2 * PI;
			break;
		case BULLET_ANIM_ROTATE_CCW:
			b->spriteRot += DEG2RAD(-10) * td;
			if (b->spriteRot < -PI)
				b->spriteRot += 2 * PI;
			break;
		case BULLET_ANIM_ROTATE_CW_SLOW:
			b->spriteRot += DEG2RAD(5) * td;
			if (b->spriteRot > PI)
				b->spriteRot -= 2 * PI;
			break;
		case BULLET_ANIM_ROTATE_CCW_SLOW:
			b->spriteRot += DEG2RAD(-5) * td;
			if (b->spriteRot < -PI)
				b->spriteRot += 2 * PI;
			break;
		case BULLET_ANIM_BUBBLE:
			b->spriteRot += DEG2RAD(45) * td;
			if (b->spriteRot > PI)
				b->spriteRot -= 2 * PI;
			break;
		default:
			break;
		}

		int layer;
		if (b->animState) {
			if (b->animState == BULLET_AS_DELETE1) {
				//struct Transform *tf = getComponent(TRANSFORM, b->entity);
				//tf->y -= 0.1f * t->w * td;
				float fac = b->animTime / bulletAnimTime[BULLET_AS_DELETE1];
				b->size = modeLerp(t->w * 2, 0, fac, INTERP_EASEIN1);
				float a = lerp(1, 0, fac);
				b->ovColor = (b->ovColor & 0x00FFFFFF) | ((int)(a * 255.0f) << 24);
			}
			b->animTime -= td;
			if (b->animTime <= 0) {
				if (b->animState == BULLET_AS_DELETE1 || b->animState == BULLET_AS_DELETE2) {
					deleteEntity(b->entity);
					continue;
				}
				b->animState = 0;
				b->animTime = 0;
			}
			layer = b->animState >= BULLET_AS_DELETE1 || t->blend? 0 : 3;
		} else {
			if (t->blend) {
				layer = 0;
			} else {
				if (t->w < 16)
					layer = 3;
				else if (t->w < 32)
					layer = 2;
				else
					layer = 1;
			}
			if (t->animMode == BULLET_ANIM_FIREBALL) {
				b->animTime += td;
				if (b->animTime >= FIREBALL_ANIM_TIME) {
					b->animTime -= FIREBALL_ANIM_TIME;
				}
			}
		}

		if (last[layer]) {
			last[layer]->drawNext = b->entity;
		} else {
			bc->drawFirst[layer] = b->entity;
		}
		last[layer] = b;
		b->drawNext = 0;
	}
	componentListOrderedClean(DAN_BULLET);
}

float bulletAnim(struct Bullet *b, enum BulletAnimState anim) {
	if (anim >= BULLET_AS_START1 && anim <= BULLET_AS_DELETE1) {
		b->spriteRot = randomFloat(-PI, PI);
	}
	b->animTime = bulletAnimTime[anim];
	b->animState = anim;
	return b->animTime;
}

static int color8To16[8] = {
	0, 2, 4, 6, 8, 10, 13, 15
};
static int color4To16[4] = {
	2, 6, 10, 13
};
static int color3To16[3] = {
	14, 0, 13 /* Coin */
};
static void bulletDrawAnim(struct BulletController *bc, struct Bullet *b, struct Transform *tf) {
	struct BulletType *t = &bc->types[b->type];
	bool sc = true;
	if (t->animMode == BULLET_ANIM_NONE) {
		sc = false;
		drawTranslate(-(tf->x - truncf(tf->x)), -(tf->y - truncf(tf->y)));
	}
	drawRotate(b->spriteRot);

	enum BlendMode blend = t->blend ? BLEND_ADD : BLEND_ALPHA;
	float total = bulletAnimTime[b->animState];
	int col;
	if (t->colors == 8)
		col = color8To16[b->color];
	else if (t->colors == 4)
		col = color4To16[b->color];
	else if (t->colors == 3)
		col = color3To16[b->color];
	else
		col = b->color;
	float fac = 1.0f - (b->animTime / bulletAnimTime[b->animState]);

	float srcX, srcY, srcW, srcH;
	float w, h;
	float a;
	if (b->animState >= BULLET_AS_START1 && b->animState <= BULLET_AS_START3) {
		srcW = srcH = 32;
		srcX = col % 8 * 32 + 256;
		srcY = col / 8 * 32;
		
		float scale = modeLerp(b->animState + 1, t->w / 32.0f, fac, INTERP_EASEOUT1);
		a = fac;
		w = 32 * scale;
		h = 32 * scale;
	} else {
		/* Delete1 */
		srcX = col * 16;
		srcY = 176;
		srcW = srcH = 16;
		blend = BLEND_ADD;
		a = 1.0f;
		float sz = modeLerp(t->w, 0, fac, INTERP_EASEIN1);
		w = sz;
		h = sz;
	}
	drawBlend(blend);
	drawSrcRect(srcX, srcY, srcW, srcH);
	drawColor(1, 1, 1, a);
	drawRect(w, h);
}

static void fireballSrcRect(float *out, struct BulletType *t, int color, float animTime) {
	int x, y;
	if (color & 1) {
		x = t->x + t->w * 4;
	} else {
		x = t->x;
	}
	if (color & 2) {
		y = t->y + t->h;
	} else {
		y = t->y;
	}
	x += (int)(animTime / (FIREBALL_ANIM_TIME / 4.0f)) * t->w;
	out[0] = x;
	out[1] = y;
	out[2] = t->w;
	out[3] = t->h;
}
void bulletSrcRect(float *out, struct BulletType *t, int color, float animTime) {
	if (color >= t->colors)
		color = 0;
	if (t->animMode == BULLET_ANIM_FIREBALL) {
		fireballSrcRect(out, t, color, animTime);
		return;
	}

	float blr = t->border & 1 ? 1 : 0;
	float bud = t->border & 2 ? 1 : 0;
	out[0] = t->x + t->w * color + blr;
	out[1] = t->y + bud;
	out[2] = t->w - blr * 2;
	out[3] = t->h - bud * 2;
}

static void bulletDraw(struct BulletController *bc, struct Bullet *b) {
	struct BulletType *t = &bc->types[b->type];
	struct Transform *tf = getComponent(TRANSFORM, b->entity);
	/* Position */
	drawMatIdentity();
	drawTransform(tf);

	if (b->animState) {
		bulletDrawAnim(bc, b, tf);
		return;
	}

	bool sc = true;
	switch (t->animMode) {
	case BULLET_ANIM_NONE:
		/* round position */
		drawTranslate(-(tf->x - truncf(tf->x)), -(tf->y - truncf(tf->y)));
		sc = false;
		break;
	case BULLET_ANIM_PULSE:
		break; /* TODO */
	case BULLET_ANIM_FIREBALL:
	case BULLET_ANIM_DIRECTION:
		drawTransformRotation(tf);
		drawRotate(DEG2RAD(90));
		break;
	case BULLET_ANIM_ROTATE_CW:
	case BULLET_ANIM_ROTATE_CCW:
	case BULLET_ANIM_BUBBLE:
	case BULLET_ANIM_ROTATE_CW_SLOW:
	case BULLET_ANIM_ROTATE_CCW_SLOW:
		drawRotate(b->spriteRot);
		break;
	}
	drawBlend(t->blend ? BLEND_ADD : BLEND_ALPHA);

	float src[4];
	bulletSrcRect(src, t, b->color, b->animTime);
	drawSrcRect(src[0], src[1], src[2], src[3]);
	drawScale(b->size / t->w, b->size / t->h);
	drawColor(((b->ovColor >> 16) & 0xFF) / 255.0f,
		((b->ovColor >> 8) & 0xFF) / 255.0f,
		((b->ovColor) & 0xFF) / 255.0f,
		((b->ovColor >> 24) & 0xFF) / 255.0f);

	if (sc)
		drawRect(src[2] + 0.5f, src[3] + 0.5f);
	else
		drawRect(src[2], src[3]);
}
static void bulletDrawLayer(struct BulletController *bc, int layer) {
	drawTexture(0, bc->tex);
	entity_t next = 0;
	for (entity_t en = bc->drawFirst[layer]; en; en = next) {
		struct Bullet *b = getComponent(DAN_BULLET, en);

		bulletDraw(bc, b);

		next = b->drawNext;
	}
}
static void bulletDrawLayer1(void *arg) { bulletDrawLayer(arg, 0); }
static void bulletDrawLayer2(void *arg) { bulletDrawLayer(arg, 1); }
static void bulletDrawLayer3(void *arg) { bulletDrawLayer(arg, 2); }
static void bulletDrawLayer4(void *arg) { bulletDrawLayer(arg, 3); }

static void bulletNotifier(void *arg, void *component, int type) {
	struct Bullet *b = component;
	struct BulletController *bc = &danmaku->bullet;
	if (type == NOTIFY_CREATE) {
		b->spriteRot = randomFloat(-PI, PI);
		b->grazeCounter = 30;
	} else if (type == NOTIFY_PURGE) {
		for (int i = 0; i < 4; i++) {
			bc->drawFirst[i] = 0;
		}
	}
}

static void i_initBullet(struct IchigoVm *vm) {
	int idx = ichigoGetInt(vm, 0);
	struct BulletType *t = &danmaku->bullet.types[idx];
	t->x = ichigoGetInt(vm, 1);
	t->y = ichigoGetInt(vm, 2);
	t->w = ichigoGetInt(vm, 3);
	t->h = ichigoGetInt(vm, 4);
	t->border = ichigoGetInt(vm, 5);
	t->colors = ichigoGetInt(vm, 6);
	t->blend = ichigoGetInt(vm, 7);
	t->animMode = ichigoGetInt(vm, 8);
	t->hitbox = ichigoGetFloat(vm, 9);
}

void bulletInit(struct Danmaku *game) {
	componentListInit(DAN_BULLET, struct Bullet);
	setNotifier(DAN_BULLET, bulletNotifier, &game->bullet);
	componentListKeepOrdering(DAN_BULLET, true);

	addUpdate(UPDATE_LATE, bulletUpdate, &game->bullet);
	addDrawUpdate(DRAW_BULLET_1, bulletDrawLayer1, &game->bullet);
	addDrawUpdate(DRAW_BULLET_2, bulletDrawLayer2, &game->bullet);
	addDrawUpdate(DRAW_BULLET_3, bulletDrawLayer3, &game->bullet);
	addDrawUpdate(DRAW_BULLET_4, bulletDrawLayer4, &game->bullet);

	danmakuSetInstr(182, i_initBullet);

	danmaku->bullet.tex = loadTexture("tex/dan/Bullets.png");
}
void bulletFini(struct Danmaku *game) {
	removeUpdate(UPDATE_LATE, bulletUpdate);
	removeDrawUpdate(DRAW_BULLET_1);
	removeDrawUpdate(DRAW_BULLET_2);
	removeDrawUpdate(DRAW_BULLET_3);
	removeDrawUpdate(DRAW_BULLET_4);

	componentListFini(DAN_BULLET);
}