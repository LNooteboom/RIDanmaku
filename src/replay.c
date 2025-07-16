#include "replay.h"
#include <string.h>
#include "redefined.h"

#define REPLAY_SIGNATURE 0x594C5052

static void pushKeys(struct ReplayController *r, uint32_t keys) {
	uint32_t nEvents = r->stages[r->currentStage].nEvents;
	if (nEvents % 512 == 0) {
		r->events[r->currentStage] = globalRealloc(r->events[r->currentStage], (nEvents + 512) * sizeof (struct ReplayEvent));
	}
	struct ReplayEvent *re = &r->events[r->currentStage][nEvents];
	re->time = r->time;
	re->keys = keys;

	r->stages[r->currentStage].nEvents = nEvents + 1;
	//logDebug("keys: %.08x\n", keys);
}

static const enum Keys replayKeys[8] = {
	K_LEFT,
	K_RIGHT,
	K_UP,
	K_DOWN,
	K_FOCUS,
	K_SHOOT,
	K_BOMB,
	K_DIALOG_SKIP
};

static void replayUpdate(struct ReplayController *r) {
	if (r->state == REPLAY_RECORD && r->currentStage != -1) {
		unsigned int keyState = 0;
		for (int i = 0; i < 8; i++) {
			if (keyPressed(replayKeys[i]))
				keyState |= (1 << i);
		}

		if (keyState != r->keys || !r->time) {
			pushKeys(r, keyState);
			r->keys = keyState;
		}
		r->time += 1;
	} else if (r->state == REPLAY_PLAY && r->playIdx < r->stages[r->currentStage].nEvents) {
		struct ReplayEvent *re = &r->events[r->currentStage][r->playIdx];
		if (r->time == re->time) {
			unsigned int changed = r->keys ^ re->keys;
			for (int i = 0; i < 8; i++) {
				unsigned int mask = 1 << i;
				if (changed & mask) {
					if (re->keys & mask) {
						keySimulatePress(replayKeys[i]);
					} else {
						keySimulateRelease(replayKeys[i]);
					}
				}
			}
			r->keys = re->keys;
			r->playIdx += 1;
		}
		r->time += 1;
	}
}

void replayInit(struct Danmaku *dan) {
	addUpdate(UPDATE_PHYS, replayUpdate, &dan->replay);
	replayClearRecording();
}

void replayFini(struct Danmaku *dan) {
	removeUpdate(UPDATE_PHYS, replayUpdate);
	replayClearRecording();
}


void replayStartRecording(void) {
	struct ReplayController *r = &danmaku->replay;
	struct ReplayStage *rs = &r->stages[r->currentStage];
	rs->nEvents = 0;
	rs->randomSeed = randomGetSeed();
	r->events[r->currentStage] = NULL;
	r->time = 0;
	r->keys = 0;
	if (!r->currentStage)
		r->startStage = danmaku->state.stage;

	r->state = REPLAY_RECORD;
}

static void replayStopRecording(struct ReplayController *r) {
	struct ReplayStage *rs = &r->stages[r->currentStage];
	rs->time = r->time;
	r->currentStage += 1;
	r->state = REPLAY_NONE;
}

void replayClearRecording(void) {
	struct ReplayController *r = &danmaku->replay;
	for (int i = 0; i < 8; i++) {
		if (r->events[i]) {
			globalDealloc(r->events[i]);
			r->events[i] = 0;
		}
		struct ReplayStage *rs = &r->stages[i];
		rs->nEvents = 0;
		rs->randomSeed = 0;
	}
	r->currentStage = 0;
}

static struct Asset *openRecording(int idx, bool write) {
	char nameBuf[32];
	snprintf(nameBuf, 32, "thFSW_%.02d.replay", idx);
	logDebug("Open %s, write: %d\n", nameBuf, write);
	return assetUserOpen(nameBuf, write);
}

void replaySaveRecording(int idx, char *name) {
	struct ReplayController *r = &danmaku->replay;
	struct ReplayHeader hdr;
	strncpy(hdr.name, name, 20);
	hdr.signature = REPLAY_SIGNATURE;
	hdr.nStages = r->currentStage;
	hdr.playerType = danmaku->state.player;
	hdr.shotType = danmaku->state.shotType;
	hdr.totalScore = danmaku->state.score;
	hdr.difficulty = danmaku->state.difficulty;
	hdr.startStage = r->startStage;

	struct Asset *a = openRecording(idx, true);
	assetUserWrite(a, &hdr, sizeof(hdr));

	for (int i = 0; i <= r->currentStage; i++) {
		struct ReplayStage *rs = &r->stages[i];
		assetUserWrite(a, rs, sizeof(*rs));
		assetUserWrite(a, r->events[i], rs->nEvents * sizeof(struct ReplayEvent));
	}

	assetClose(a);
}

int replayLoadRecording(int idx) {
	struct ReplayController *r = &danmaku->replay;
	struct Asset *a = openRecording(idx, false);
	if (!a)
		return -1;
	struct ReplayHeader hdr;
	if (assetRead(a, &hdr, sizeof(hdr)) != sizeof(hdr))
		goto failed;

	if (hdr.signature != REPLAY_SIGNATURE)
		goto failed;

	for (int i = 0; i < hdr.nStages; i++) {
		struct ReplayStage *rs = &r->stages[i];
		assetRead(a, rs, sizeof(*rs));
		size_t sz = rs->nEvents * sizeof(struct ReplayEvent);
		r->events[i] = globalAlloc(sz);
		assetRead(a, r->events[i], sz);
	}

	assetClose(a);

	r->nStages = hdr.nStages;
	r->currentStage = 0;
	r->startStage = hdr.startStage;
	danmaku->state.difficulty = hdr.difficulty;
	danmaku->state.player = hdr.playerType;
	danmaku->state.shotType = hdr.shotType;
	danmaku->state.stage = hdr.startStage;

	return 0;

failed:
	logNorm("Failed to load replay %d\n", idx);
	assetClose(a);
	return -1;
}

const char *invalidReplayStr = "---------- --/--/-- --:--";
int replayGetInfo(char info[128], int idx) {
	struct Asset *a = openRecording(idx, false);
	if (!a)
		goto failed;
	struct ReplayHeader hdr;
	if (assetRead(a, &hdr, sizeof(hdr)) != sizeof(hdr))
		goto failed_2;

	if (hdr.signature != REPLAY_SIGNATURE)
		goto failed_2;

	for (int i = 0; i < 10; i++) {
		if (hdr.name[i])
			info[i] = hdr.name[i];
		else
			info[i] = ' ';
	}

	assetClose(a);
	return 0;

failed_2:
	assetClose(a);
failed:
	strcpy(info, invalidReplayStr);
	return -1;
}

void replayStartPlaying(void) {
	struct ReplayController *r = &danmaku->replay;
	r->playIdx = 0;
	r->time = 0;
	r->state = REPLAY_PLAY;
	randomSetSeed(r->stages[r->currentStage].randomSeed);
	keySetSimulation();
}

static void replayStopPlaying(struct ReplayController *r) {
	r->state = REPLAY_NONE;
	keyNoSimulation();
}

void replayStop(void) {
	struct ReplayController *r = &danmaku->replay;
	if (r->state == REPLAY_RECORD) {
		replayStopRecording(r);
		replaySaveRecording(0, "test");
	} else if (r->state == REPLAY_PLAY) {
		replayStopPlaying(r);
	}
}

bool replayHasNextStage(void) {
	struct ReplayController *r = &danmaku->replay;
	return r->state != REPLAY_PLAY || r->currentStage + 1 < r->nStages;
}
