#include "redefined.h"
#include <input.h>

bool keyPressed(enum Keys key) {
	return game->keys.pressed & (1 << key);
}

int keyMatch(enum Keys *keys, int nKeys, struct Event *ev) {
	struct KeymapController *kc = &game->keys;
	for (int i = 0; i < nKeys; i++) {
		struct Keymap *m = &kc->map[keys[i]];
		if ((ev->type == EVENT_KEY && (ev->param == m->key1 || ev->param == m->key2)) ||
			(ev->type == EVENT_GAMEPAD_BTN && ev->param == m->gamepadBtn))
			return i;
	}
	return -1;
}

void keySetSimulation(void) {
	game->keys.simulate = true;
	game->keys.pressed = 0;
}
void keyNoSimulation(void) {
	game->keys.simulate = false;
}
void keySimulatePress(enum Keys key) {
	if (game->keys.simulate)
		game->keys.pressed |= (1 << key);
}
void keySimulateRelease(enum Keys key) {
	if (game->keys.simulate)
		game->keys.pressed &= ~(1 << key);
}

static void keymapEvent(void *arg, struct Event *ev) {
	struct Redefined *game = arg;
	struct KeymapController *kc = &game->keys;
	if (ev->type == EVENT_END_SCENE) {
		kc->pressed = 0;
		return;
	}
	bool gamepad = ev->type == EVENT_GAMEPAD_BTN;
	bool pressed = ev->param2;
	for (int i = kc->simulate? K_MENU : 0; i < K_N; i++) {
		struct Keymap *km = &kc->map[i];
		if ((!gamepad && (ev->param == km->key1 || ev->param == km->key2)) ||
			(gamepad && ev->param == km->gamepadBtn)) {
			if (pressed) {
				kc->pressed |= (1 << i);
			} else {
				kc->pressed &= ~(1 << i);
			}
		}
	}
}
void keymapInit(struct Redefined *game) {
	struct KeymapController *kc = &game->keys;

	kc->map[K_LEFT].key1	= SDLK_LEFT;
	kc->map[K_RIGHT].key1	= SDLK_RIGHT;
	kc->map[K_UP].key1		= SDLK_UP;
	kc->map[K_DOWN].key1	= SDLK_DOWN;
	kc->map[K_FOCUS].key1	= SDLK_LSHIFT;
	kc->map[K_SHOOT].key1	= SDLK_z;
	kc->map[K_BOMB].key1	= SDLK_x;
	kc->map[K_MENU].key1	= SDLK_ESCAPE;
	kc->map[K_MENU_SELECT].key1 = SDLK_z;
	kc->map[K_MENU_CANCEL].key1 = SDLK_x;
	kc->map[K_DIALOG_SKIP].key1 = SDLK_LCTRL;

	kc->map[K_LEFT].gamepadBtn = GP_BUTTON_DPAD_LEFT;
	kc->map[K_RIGHT].gamepadBtn = GP_BUTTON_DPAD_RIGHT;
	kc->map[K_UP].gamepadBtn = GP_BUTTON_DPAD_UP;
	kc->map[K_DOWN].gamepadBtn = GP_BUTTON_DPAD_DOWN;
	kc->map[K_FOCUS].gamepadBtn = GP_BUTTON_SHOULDER_L;
	kc->map[K_SHOOT].gamepadBtn = GP_BUTTON_SHOULDER_R;

	addInputHandler(EVENT_KEY | EVENT_END_SCENE | EVENT_GAMEPAD_BTN, keymapEvent, game);
}
void keymapFini(struct Redefined *game) {
	(void) game;
	removeInputHandler(keymapEvent);
}