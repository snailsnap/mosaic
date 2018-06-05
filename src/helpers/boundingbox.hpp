#include "../../dependencies/jc_voronoi/src/jc_voronoi.h"

struct BoundingBox
{
    float centerX;
    float centerY;
    float width;
    float height;
    float rotation;
};

BoundingBox calculateBoundingBox(const jcv_site* site);
