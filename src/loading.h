#ifndef LOADING_H
#define LOADING_H

#include <events.h>

enum LoadingType {
	LOAD_NONE,
	LOAD_INTRO,
	LOAD_BLACK
};
extern enum LoadingType loadingType;

static inline void swScene(enum LoadingType lt, const char *scene) {
	loadingType = lt;
	if (lt)
		switchSceneDelayed(scene, 10);
	else
		switchScene(scene);
}

void loadingInit(void);
void loadingFini(void);

#endif