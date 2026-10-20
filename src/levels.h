#ifndef LEVELS_H
#define LEVELS_H

#include "raylib.h"

#define MAX_BLOCKS    50
#define MAX_SPIKES    50

typedef struct Sponge {
  Vector2 pos;
  float radius;
} Sponge;

typedef struct {
  Rectangle blocks[MAX_BLOCKS];
  int blockCount;
  Rectangle spikes[MAX_SPIKES];
  int spikeCount;
  Sponge sponge;
  Rectangle finishLine;
} LevelData;

// Define a single level here
static LevelData g_level = {
    .blocks = {
      { 100.0f, -100.0f, 1000.0f,100.0f },
      { 100.0f, 100.0f, 1000.0f, 100.0f },
	    { 1250.0f, -100.0f, 500.0f, 300.0f }
    },
    .blockCount = 3,
    .spikes = {
      { 100.0f, -250.0f, 50.0f, 100.0f }
    },
    .spikeCount = 1,
    .sponge = { 1200.0f, -100.0f, 100.0f },
    .finishLine = { 1400.0f, -200.0f, 50.0f, 100.0f },
};

#endif // LEVELS_H
