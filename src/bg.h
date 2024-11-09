#ifndef DAN_BG_H
#define DAN_BG_H

#include <mem.h>

struct Danmaku;

struct DanBgController {
	float camX, camY, camZ;
	float posEx1[3];
	float posEx2[3];
	struct Interp posInterp;
	float dirX, dirY, dirZ;
	float dirEx1[3];
	float dirEx2[3];
	struct Interp dirInterp;
	float upX, upY, upZ;
	struct Interp upInterp;
	float fov;
	struct Interp fovInterp;
	float fogR, fogG, fogB;
	struct Interp fogColorInterp;
	float fogNear, fogFar;
	struct Interp fogDistInterp;

	struct Vector dvms;

	entity_t bgVm;
	struct IchigoState bgScript;
	struct IchigoVar bgVars[64];
};

void danBgStart(struct Danmaku *d);
void danBgEnd(struct Danmaku *d);

void danBgInit(struct Danmaku *d);
void danBgFini(struct Danmaku *d);

#endif