#ifndef LASER_H
#define LASER_H

#include <ecs.h>

#define LASER_ST_ALPHA 1
#define LASER_ST_NO_CREATE_BALL 2
#define LASER_ST_DELETE_BALL 4

#define LASER_INF_FADE 1 /* Fades in instead of expanding */

enum StraightLaserState {
	L_ST_GROW,
	L_ST_NORM,
	L_ST_SHRINK
};
struct StraightLaser {
	entity_t entity;
	entity_t next, prev;
	enum StraightLaserState state;
	uint16_t type, color;
	int flags;
	float width;

	int grazeCounter;

	float length;
	float endLength;
	float range;
};

enum InfiniteLaserState {
	L_INF_START,
	L_INF_GROW,
	L_INF_NORM,
	L_INF_SHRINK
};
struct InfiniteLaser {
	entity_t entity;

	entity_t parent;
	float offX, offY;
	float dist;

	enum InfiniteLaserState state;
	uint16_t type, color;
	int flags;
	float length;
	float width;

	int grazeCounter;

	float time;
	int startTime;
	int expandTime;
	int duration;
	int shrinkTime;
};

struct CurveLaserNode {
	float x, y;
	float rr, ri;
};
enum CurveLaserState {
	L_CU_START,
	L_CU_GROW,
	L_CU_NORM,
	L_CU_SHRINK
};
struct CurveLaser {
	entity_t entity;
	int color;
	int flags;

	int grazeCounter;

	int startTime;
	int expandTime;
	int duration;
	int shrinkTime;
	float time;

	float width;

	enum CurveLaserState state;

	int head;
	struct Vector nodes;
};


struct LaserController {
	entity_t stFirst, stLast;
	struct Texture *curveLaserTex;
};

struct Danmaku;

void laserStSetCollider(struct CollisionInfo *c, struct StraightLaser *l, struct Transform *tf);

void laserInit(struct Danmaku *dan);
void laserFini(struct Danmaku *dan);

#endif