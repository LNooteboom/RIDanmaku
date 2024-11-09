#ifndef KEYMAP_H
#define KEYMAP_H

#include <stdint.h>
#include <events.h>

enum Keys {
	K_LEFT,
	K_RIGHT,
	K_UP,
	K_DOWN,
	K_FOCUS,
	K_SHOOT,
	K_BOMB,
	K_MENU,
	K_MENU_SELECT,
	K_MENU_CANCEL,
	K_DIALOG_SKIP,

	K_N
};

struct Redefined;

struct Keymap {
	int key1, key2;
	int gamepadBtn;
};
struct KeymapController {
	struct Keymap map[K_N];
	uint32_t pressed;
	bool simulate;
};

bool keyPressed(enum Keys key);
int keyMatch(enum Keys *keys, int nKeys, struct Event *ev);

void keySetSimulation(void);
void keyNoSimulation(void);
void keySimulatePress(enum Keys key);
void keySimulateRelease(enum Keys key);

void keymapInit(struct Redefined *game);
void keymapFini(struct Redefined *game);

#endif