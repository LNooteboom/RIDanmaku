#ifndef DPLAYER_H
#define DPLAYER_H

#include "ecs.h"
#include <ich.h>
#include <basics.h>
#include <stdbool.h>

struct Danmaku;

#define DAN_PLAYER_MAX_OPTION 16

#define PB_PENETRATE 1
#define PB_HOMING 2
#define PB_BOMB 4

enum DanPlayerState {
	DP_STATE_NORM,
	DP_STATE_DEATH_GRACE, /* deathbomb window */
	DP_STATE_DEATH,
	DP_STATE_DEATH_RECOVER,
	DP_STATE_BOMB
};

struct DanPlayerBullet {
	entity_t entity;
	int flags;
	int damage;
	float time;
	float hitboxW, hitboxH;

	float speed;
};

struct DanPlayerController {
	entity_t entity;

	int maxPower;
	int powerItemValue;
	float magnetRange, focusMagnetRange;

	enum DanPlayerState state;
	float time;

	/* Radius */
	float hitbox;
	float grazebox;

	const char *mainDvm;
	const char *onBomb;
	const char *onBombHit;
	bool bombHit;
	int bombCorout;
	int bombPowerCost;
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
	float speed, focusSpeed;
	int moveState, oldMoveState;

	entity_t options[DAN_PLAYER_MAX_OPTION];
	entity_t focus1, focus2;
	bool focusActive;

	struct IchigoState playerScript;
	struct IchigoVar playerVars[64];

	entity_t targetEnemy;
	float targetX, targetY;

	entity_t uiLives;
	entity_t uiPower;
};

void danPlayerAddPower(void);
void danPlayerAddPowerBig(void);
void danPlayerAddLife(void);
void danPlayerAddLifepiece(void);

void danPlayerHit(void);
void danPlayerGraze(void);

void danPlayerBulletHit(struct DanPlayerBullet *pb);

void danPlayerCreate(struct Danmaku *game, struct DanmakuState *state);
void danPlayerDestroy(struct Danmaku *game);

void danPlayerPos(float *x, float *y);

void danPlayerInit(struct Danmaku *game);
void danPlayerFini(struct Danmaku *game);

#endif
