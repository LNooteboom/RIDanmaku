#ifndef ITEMS_H
#define ITEMS_H

#include <ecs.h>

struct Danmaku;

enum ItemType {
	ITEM_POWER,
	ITEM_POWER_BIG,
	ITEM_POINT,
	ITEM_STAR,
	ITEM_1UP,
	ITEM_LIFEPIECE,
	ITEM_CANCEL,
	N_ITEMS,

	ITEM_NONE = -1
};

enum ItemState {
	IS_SPAWN,
	IS_NORM,
	IS_MAGNET,
	IS_MAGNET_SLOW,
	IS_GET
};

struct Item {
	entity_t entity;
	enum ItemType type;
	enum ItemState state;

	float time;

	float ySpeed;
};

struct ItemController {
	float autoCollectY;
	float magnetRange;
	float fallSpeed;
	bool autoCollect;
	float bombCollect;

	struct Texture *tex;
};

void itemNew(float x, float y, enum ItemType type);

void itemStart(struct Danmaku *game);

void itemInit(struct Danmaku *game);
void itemFini(struct Danmaku *game);

#endif
