#ifndef BULLET_H
#define BULLET_H

#include <ecs.h>
#include <stdint.h>

struct Danmaku;

enum BulletBorderMode {
	BULLET_BORDER_NONE,
	BULLET_BORDER_LR,
	BULLET_BORDER_UD,
	BULLET_BORDER_ALL
};
enum BulletAnimMode {
	BULLET_ANIM_NONE,
	BULLET_ANIM_DIRECTION,
	BULLET_ANIM_ROTATE_CW,
	BULLET_ANIM_ROTATE_CCW,
	BULLET_ANIM_PULSE,
	BULLET_ANIM_BUBBLE,
	BULLET_ANIM_FIREBALL,
	BULLET_ANIM_ROTATE_CW_SLOW,
	BULLET_ANIM_ROTATE_CCW_SLOW
};

enum BulletAnimState {
	BULLET_AS_NONE,
	BULLET_AS_START1,
	BULLET_AS_START2,
	BULLET_AS_START3,
	BULLET_AS_DELETE1,
	BULLET_AS_DELETE2,

	BULLET_AS_N
};

struct BulletType {
	uint16_t x, y;
	uint16_t w, h;
	uint8_t border;
	uint8_t colors;
	uint8_t blend;
	uint8_t animMode;
	float hitbox;
};

struct Bullet {
	entity_t entity;
	uint16_t type;
	uint16_t animState;
	uint8_t color;
	uint8_t grazeCounter;
	uint32_t ovColor;
	float animTime;
	float size;
	float spriteRot;

	entity_t drawNext;
};

struct BulletController {
	struct BulletType types[256];

	struct Texture *tex;

	entity_t drawFirst[4];
};

void bulletDeleteCircle(float x, float y, float r, bool item, bool force);
void bulletDeleteRect(float x, float y, float rot, float w, float h, bool item, bool force);

void bulletSrcRect(float *out, struct BulletType *t, int color, float animTime);

float bulletAnim(struct Bullet *b, enum BulletAnimState anim);

void bulletInit(struct Danmaku *game);
void bulletFini(struct Danmaku *game);

#endif
