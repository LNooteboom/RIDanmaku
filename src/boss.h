#ifndef BOSS_H
#define BOSS_H

#include <ecs.h>
#include <basics.h>

struct Danmaku;
struct Shader;

#define BOSS_FOG_VERTS 8

#define BOSS_MAX_SPELL 256

struct SpellInfo {
	int stage;
	int difficulty;
	char name[64];
};

struct BossController {
	entity_t boss;

	int spellIdx;
	float spellTime;
	int spellBonus;
	entity_t spellBgDvm;
	entity_t spellPortraitDvm;
	entity_t spellNameDvm;
	entity_t spellCircleDvm;

	entity_t distortDvm;
	struct Shader *distortShader;

	float curLife;
	entity_t lifeDvm[17];
	float lifeMarkers[16];
	int lifeColors[16];

	entity_t starDvm[8];
	int stars;

	int timeInterrupt;
	entity_t timeDvm;
	entity_t nameDvm;

	int capture;

	struct SpellInfo *spells;
};

void bossEnd(struct Danmaku *game);

void bossSetTimer(struct Enemy *e, int irq);

void bossInit(struct Danmaku *game);
void bossFini(struct Danmaku *game);

#endif