#pragma once

#include "../../dependencies/jc_voronoi/src/jc_voronoi.h"

struct BoundingBox
{
    int centerX;
    int centerY;
    int width;
    int height;
    float rotation;
};

BoundingBox calculateBoundingBox(const jcv_site* site);
