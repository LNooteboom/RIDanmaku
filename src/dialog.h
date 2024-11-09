#ifndef DIALOG_H
#define DIALOG_H

#include <ecs.h>

struct Redefined;

struct DialogController {
	bool active;
	bool danWait;

	entity_t vm;
	entity_t dvmPlayer;
	entity_t dvmBoss;
	entity_t dvmText;
	entity_t dvmIntro;

	int waitTime, waitMax;
	bool skippable;
	int textY;
	bool nextKey;

	struct IchigoState iState;
	struct IchigoVar iVars[32];
};

void dialogStart(const char *fn);
void dialogEnd(void);

void dialogInit(struct Redefined *game);
void dialogFini(struct Redefined *game);

#endif