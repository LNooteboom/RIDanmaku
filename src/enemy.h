#ifndef ENEMY_H
#define ENEMY_H

#include <ecs.h>
#include <basics.h>
#include "items.h"

#define ENM_N_INTERRUPTS	4
#define ENM_N_DVMS			8

struct Danmaku;

#define ENM_NO_HURT		1
#define ENM_NO_HIT		2
#define ENM_INVINCIBLE	4
#define ENM_OFFSCR_LR	8
#define ENM_OFFSCR_UD	16
#define ENM_MIRROR		32
#define ENM_HIDDEN		64
#define ENM_RESIST		128
#define ENM_NO_TIMESTOP 256
#define ENM_NO_DELETE	512
#define ENM_KILLED		1024

enum EnemyMoveType {
	MOVE_POS,
	MOVE_VEL,
	MOVE_VEL_NM,
	MOVE_ELLIPSE,
	MOVE_BEZIER,
	MOVE_RAND
};
struct EnemyMove {
	enum EnemyMoveType type;
	float x, y, z;
	float ang, spd;
	float rad, radInc;
	float midX, midY;
	float ellDir, ellRatio;
	float ex1[2];
	float ex2[2];
	struct Interp ip1;
	struct Interp ip2;

};

struct EnemyInterrupt {
	float life;
	float time;
	const char *lifeHandler;
	const char *timeHandler;
};

struct Enemy {
	entity_t entity;

	/* General */
	int flags;
	int score;
	float life;
	int maxLife;
	float time;

	/* Health and interrupts */
	float invulnTime;
	float hurtboxW, hurtboxH;
	float hitboxW, hitboxH;
	struct EnemyInterrupt interrupts[ENM_N_INTERRUPTS];
	const char *deathFn;
	int damageAnmTime;

	/* Movement */
	bool hasLimit;
	float minX, minY;
	float maxX, maxY;
	struct EnemyMove abs;
	struct EnemyMove rel;

	/* DVM */
	int moveState;
	int dvmMove;
	entity_t dvm[ENM_N_DVMS];
	const char *dvmDeath;

	/* Items */
	float dropW, dropH;
	enum ItemType dropMain;
	int dropExtra[N_ITEMS];
};

struct EnemyController {
	entity_t mainEnemy;
	entity_t bossEnemy[8];

	int timeout;

	int gi[4];
	float gf[8];
};

entity_t enemyNewMain(void);
void enemyDelete(entity_t entity);

void enemyDamageCircle(float x, float y, float r, int dmg);
void enemyDamageRect(float x, float y, float rot, float w, float h, int dmg);

void enemyInit(struct Danmaku *d);
void enemyFini(struct Danmaku *d);

#endif
