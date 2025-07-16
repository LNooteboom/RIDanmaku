#include "redefined.h"
#include <audio.h>

#define ITEM_SPAWN_TIME 40
#define ITEM_GET_TIME 8

struct ItemSprite {
	int x, y, w, h;
};
static const struct ItemSprite itemSprites[N_ITEMS] = {
	{ 1,  1, 14, 14}, /* ITEM_POWER */
	{16,  0, 16, 16}, /* ITEM_POWER_BIG */
	{33,  1, 14, 14}, /* ITEM_POINT */
	{48,  0, 16, 16}, /* ITEM_GREEN */
	{ 0, 32, 16, 16}, /* ITEM_1UP */
	{16, 32, 16, 16}, /* ITEM_LIFEPIECE */
	{33, 33, 14, 14}  /* ITEM_GREEN_SMALL */
};

void itemNew(float x, float y, enum ItemType type) {
	entity_t en = newEntity();
	struct Transform *tf = newComponent(TRANSFORM, en);
	tf->x = x;
	tf->y = y;
	struct Item *it = newComponent(DAN_ITEM, en);
	if ((type == ITEM_POWER || type == ITEM_POWER_BIG) && danmaku->state.power == danmaku->player.maxPower) {
		it->type = ITEM_CANCEL;
	} else {
		it->type = type;
	}
	it->ySpeed = -4;
}

static void itemCollect(struct ItemController *ic, struct Item *it, struct Transform *tf) {
	switch (it->type) {
	case ITEM_POWER:
		danPlayerAddPower();
		break;
	case ITEM_POWER_BIG:
		danPlayerAddPowerBig();
		break;
	case ITEM_1UP:
		danPlayerAddLife();
		break;
	case ITEM_LIFEPIECE:
		danPlayerAddLifepiece();
		break;
	}
	danmakuPlaySfx(18);
	
	scoreGetItem(it, tf->x, tf->y);
	it->state = IS_GET;
	it->time = 0;
}

static void itemUpdate(void *arg) {
	struct ItemController *ic = arg;
	float px, py;
	danPlayerPos(&px, &py);
	for (struct Item *it = clBegin(DAN_ITEM); it; it = clNext(DAN_ITEM, it)) {
		struct Transform *tf = getComponent(TRANSFORM, it->entity);
		if (tf->x < -200 || tf->x > 200 || tf->y > 456) {
			deleteEntity(it->entity);
			continue;
		}

		if ((it->type == ITEM_POWER || it->type == ITEM_POWER_BIG) && danmaku->state.power == danmaku->player.maxPower) {
			it->type = ITEM_CANCEL;
		}

		float dx = px - tf->x;
		float dy = py - tf->y;
		float dst = sqrtf(dx * dx + dy * dy);
		if (py < ic->autoCollectY && it->state != IS_GET) {
			it->state = IS_MAGNET;
		}
		if ((ic->autoCollect || ic->bombCollect > 0) && it->state == IS_NORM) {
			it->state = IS_MAGNET;
		}

		switch (it->state) {
		case IS_SPAWN:
		{
			// float angle = lerp(-PI * 2, 0, it->time / ITEM_SPAWN_TIME);
			// tfSetRotation(tf, angle);
			if ((int)it->time % 2 == 0) {
				float rr = tf->rotReal, ri = tf->rotImag;
				tf->rotReal = ri;
				tf->rotImag = -rr;
			}
			it->time += gameSpeed;
			if (it->time >= ITEM_SPAWN_TIME) {
				it->state = (ic->autoCollect || it->type == ITEM_CANCEL)? IS_MAGNET : IS_NORM;
			}
			break;
		}
		case IS_NORM:
			tf->rotReal = 1;
			tf->rotImag = 0;
			if (dst < ic->magnetRange) {
				it->state = IS_MAGNET_SLOW;
			}
			break;
		case IS_MAGNET:
			tf->rotReal = 1;
			tf->rotImag = 0;
			tf->x += dx / dst * 8;
			tf->y += dy / dst * 8;
			if (dst < 8.0f) {
				itemCollect(ic, it, tf);
			}
			break;
		case IS_MAGNET_SLOW:
		{
			float spd = lerp(3, 6, 1.0f - fmaxf(0, fminf(1, dst / ic->magnetRange)));
			tf->x += dx / dst * spd;
			tf->y += dy / dst * spd;
			if (dst < 8.0f) {
				itemCollect(ic, it, tf);
			}
			break;
		}
		case IS_GET:
			tf->rotReal = 1;
			tf->rotImag = 0;
			it->time += gameSpeed;
			if (it->time > ITEM_GET_TIME) {
				deleteEntity(it->entity);
				continue;
			}
			break;
		}
		if (it->state == IS_SPAWN || it->state == IS_NORM) {
			tf->y += it->ySpeed * gameSpeed;
			it->ySpeed = fminf(ic->fallSpeed, it->ySpeed + 0.1f * gameSpeed);
		}
	}
	componentListOrderedClean(DAN_ITEM);

	if (ic->bombCollect > 0) {
		ic->bombCollect -= gameSpeed;
	}
}

static void itDraw(struct ItemController *ic, struct Item *it) {
	const struct ItemSprite *sp = &itemSprites[it->type];
	struct Transform *tf = getComponent(TRANSFORM, it->entity);
	float w, h;
	drawMatIdentity();
	if (tf->y < sp->h / -2) {
		/* Draw arrow */
		int srcY = sp->h % 16 ? 16 : sp->y + sp->h;
		int srcH = sp->h % 16 ? 16 : sp->h;
		drawTranslate(truncf(tf->x), srcH / 2.0f);
		drawSrcRect(sp->x, srcY, srcH, srcH);
		w = srcH;
		h = srcH;
		drawColor(1, 1, 1, lerp(1, 0.2f, fmaxf(0, fminf(1, tf->y / -96))));
	} else {
		drawTranslate(truncf(tf->x), truncf(tf->y));
		drawTransformRotation(tf);
		drawSrcRect(sp->x, sp->y, sp->w, sp->h);

		w = sp->w;
		h = sp->h;

		float sz = 1;
		float a = 1;
		if (it->state == IS_SPAWN) {
			sz = fminf(1, it->time / 8);
		} else if (it->state == IS_GET) {
			sz = 1.0f - (it->time / ITEM_GET_TIME);
			a = sz;
		}
		if (it->type == ITEM_CANCEL)
			a *= 0.5;
		drawScale(sz, sz);
		drawColor(1, 1, 1, a);
	}

	drawRect(w, h);
}
static void itemDraw(void *arg) {
	struct ItemController *ic = arg;
	drawTexture(0, ic->tex);
	for (struct Item *it = clBegin(DAN_ITEM); it; it = clNext(DAN_ITEM, it)) {
		itDraw(ic, it);
	}
	drawBlend(BLEND_ADD);
	for (struct Item *it = clBegin(DAN_ITEM); it; it = clNext(DAN_ITEM, it)) {
		if (it->type == ITEM_CANCEL) {
			itDraw(ic, it);
		}
	}
}

void itemStart(struct Danmaku *game) {
	game->item.fallSpeed = 2;
	game->item.autoCollectY = 128;
	game->item.magnetRange = 32;
	game->item.autoCollect = 0;
	game->item.bombCollect = 0;
}

void itemInit(struct Danmaku *game) {
	componentListInit(DAN_ITEM, struct Item);
	componentListKeepOrdering(DAN_ITEM, true);

	game->item.tex = loadTexture("tex/dan/Items.png");
	

	addUpdate(UPDATE_NORM, itemUpdate, &game->item);
	addDrawUpdate(DRAW_ITEM, itemDraw, &game->item);
}

void itemFini(struct Danmaku *game) {
	deleteTexture(game->item.tex);

	removeDrawUpdate(DRAW_ITEM);
	removeUpdate(UPDATE_NORM, itemUpdate);
	
	componentListFini(DAN_ITEM);
}
