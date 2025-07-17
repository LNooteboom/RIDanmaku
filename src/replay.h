#ifndef REPLAY_H
#define REPLAY_H

#include <stdint.h>
#include <stdbool.h>

struct Danmaku;

enum ReplayState {
	REPLAY_NONE,
	REPLAY_RECORD,
	REPLAY_PLAY
};

struct ReplayHeader {
	uint32_t signature;
	char name[20];
	uint64_t totalScore;

	uint8_t nStages;
	uint8_t startStage;
	uint8_t playerType;
	uint8_t shotType;

	uint8_t difficulty;
};

struct ReplayStage {
	uint32_t randomSeed;
	uint32_t nEvents;
	uint32_t time;
};

struct ReplayEvent {
	uint32_t time;
	uint32_t keys;
};

struct ReplayController {
	enum ReplayState state;

	unsigned int time;
	unsigned int keys;

	int startStage, currentStage;
	int nStages;
	unsigned int playIdx;

	struct ReplayStage stages[8];
	struct ReplayEvent *events[8];
};

void replayInit(struct Danmaku *dan);
void replayFini(struct Danmaku *dan);

void replayStop(void);

void replayStartRecording(void);
void replayClearRecording(void);
void replaySaveRecording(int idx, char *name);

int replayGetInfo(char info[128], int idx);
int replayLoadRecording(int idx);
void replayStartPlaying(void);

bool replayIsPlaying(void);
bool replayHasNextStage(void);

#endif