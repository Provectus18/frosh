/* levels.h -- level configuration for Frosh.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Copyright (C) 2026  provectus <freebert@tuta.io>
 *
 * This file is a part of Frosh.
 *
 * Frosh is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * Frosh is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LEVELS_H
#define LEVELS_H

#include "raylib.h"

#define MAX_BLOCKS    50
#define MAX_SPIKES    50
#define MAX_SPONGE    50

typedef struct Sponge {
  Vector2 pos;
  float radius;
} Sponge;

typedef struct {
  Vector2 startPos;
  Rectangle blocks[MAX_BLOCKS];
  int blockCount;
  Rectangle spikes[MAX_SPIKES];
  int spikeCount;
  Sponge sponge[MAX_SPONGE];
  int spongeCount;
  Rectangle finishLine;
} LevelData;

// Define a single level here
static LevelData g_level = {
    .startPos = { 0.0f, -20.0f },
    .blocks = {
      { 100.0f, -100.0f, 1000.0f,100.0f },
      { 100.0f, 100.0f, 1000.0f, 100.0f },
      { 1250.0f, -100.0f, 500.0f, 300.0f }
    },
    .blockCount = 3,
    .spikes = {
      { 100.0f, -250.0f, 50.0f, 100.0f },
      { 1300.0f, -300.0f, 50.0f, 50.0f }
    },
    .spikeCount = 2,
    .sponge = {
      { 1200.0f, -100.0f, 100.0f },
      { 500.0f, -200.0f, 50.0f },
      { 750.0f, -300.0f, 70.0f }
    },
    .spongeCount = 3,
    .finishLine = { 1400.0f, -200.0f, 50.0f, 100.0f },
};

#endif // LEVELS_H
