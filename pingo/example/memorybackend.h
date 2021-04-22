#pragma once

#include "../render/backend.h"
#include "../math/vec2.h"

typedef struct Pixel Pixel;
typedef struct Depth Depth;

typedef  struct {
    BackEnd backend;
    Vec2i size;
} MemoryBackend;

void memoryBackendInit(MemoryBackend * ths, Vec2i size);

