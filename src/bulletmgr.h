#ifndef BULLETMGR_H
#define BULLETMGR_H

#include <stdint.h>
#include <ecs.h>

#define BULLETMGR_MAX_SPAWN 16
#define BULLETMGR_MAX_TRANS 16
#define BULLETMGR_MAX_LASER_ID 32

#define FUNC_COND_ALL 0
#define FUNC_COND_PLNEAR 1 // within f0 to player
#define FUNC_COND_CIRCLE 2 // within f0 to f1,f2
#define FUNC_COND_RECT 3 // f0<x<f1, f2<y<f3
#define FUNC_COND_MRKNEAR 4 // within f0 to bullet marked i0
#define FUNC_COND_ENMNEAR 5 // within f0 to calling enemy
#define FUNC_COND_MRKLASERCOLL 6
#define FUNC_COND_INVERT 0x80

struct Danmaku;

enum BulletAim {
	AIM_AT, /* Fan aimed at player */
	AIM_ST, /* Fan aimed with static angle */
	AIM_AT_R, /* Ring aimed at player */
	AIM_ST_R, /* Ring aimed at static angle */
	AIM_AWAY_R, /* Ring aimed away from player */
	AIM_ST_R2, /* Ring aimed away from static angle */
	AIM_RAND, /* Fan with random angles */
	AIM_RAND_R,/* Ring with random speeds */
	AIM_MEEK, /* Complete random */
	AIM_AT_PYR, /* Pyramid aimed at player */
	AIM_ST_PYR, /* Pyramid aimed static */
};
enum BulletTransType {
	BT_INVALID,

	/* Control */
	BT_WAIT,
	BT_WAITSIG,
	BT_JMP,

	/* Move */
	BT_FAST,
	BT_VEL,
	BT_ACCEL,
	BT_ACCEL_ANGV,
	BT_STEP,
	BT_SAVE,

	BT_BOUNCE,
	BT_WRAP,

	BT_OFFSCR,
	BT_INVULN,

	/* Misc */
	BT_SPRITE,
	BT_OPACITY,
	BT_SIZE,
	BT_SOUND,
	BT_DELETE,
	
	/* Shooting */
	BT_SHOOT1,
	BT_SHOOT2,

	BT_NEWENM,

	BT_CLOUD,
	BT_MARK,

	BT_N
};
enum BulletTransStep {
	STEP_ANGREL,
	STEP_ANGPL,
	STEP_ANGSAVEPL,
	STEP_ANGSAVE,
	STEP_ANG,
	STEP_ANGRND,
	STEP_ANGPLRND,
	STEP_SPDRND
};

#define BT_ASYNC	1
#define BT_RUNNING	2

#define BT_RUN_WAIT		1
#define BT_RUN_MOVE		2
#define BT_RUN_BOUNCE	4

#define BM_FAST_SPEED 5.0f
#define BM_FAST_TIME 16.0f

struct BulletTrans {
	uint16_t type;
	uint16_t flags;
	int a, b, c;
	float p, q, r;
	union {
		int d;
		float s;
	};
};

struct BulletMgr {
	entity_t entity;

	uint32_t flags;
	float speed; /* Angle is in tf */
	float time, moveTime;
	int stepCount;
	float stepDecel;
	int bounceCount;

	float offscreen;
	float invuln;

	const char *enmFn;

	float saveX, saveY, saveAngle;
	int transSound;
	uint32_t transRunning;
	int transIndex;
	struct BulletTrans trans[BULLETMGR_MAX_TRANS];
};

struct BulletSpawner {
	enum BulletAim aim;
	bool posAbs;
	float x, y;
	float offX, offY;
	float dist;
	float spd1, spd2;
	float ang1, ang2;
	int cnt1, cnt2;
	int soundShoot, soundTrans;
	int type, color;

	float protectRange;

	const char *enmFn;

	int transIndex;
	struct BulletTrans trans[BULLETMGR_MAX_TRANS];
};
struct BulletSpawnerList {
	entity_t entity;

	struct BulletSpawner list[BULLETMGR_MAX_SPAWN];

	int laserId;
	entity_t lasers[BULLETMGR_MAX_LASER_ID];
};

struct BulletMark {
	entity_t entity;
	int mark;
};

struct BulletMgrControl {
	struct BulletSpawner tempSpawner;

	int signal;

	const char *func;
	entity_t funcEntity;
	struct IchigoVm funcVm;
	int funcCond, funcMark;
};


void bmDelete(entity_t en, int anim);

void bulletMgrInit(struct Danmaku *game);
void bulletMgrFini(struct Danmaku *game);

#endif