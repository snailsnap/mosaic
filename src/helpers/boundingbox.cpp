#include <vector>
#include <utility>
#include <limits>
#include <algorithm>

#include "../../dependencies/jc_voronoi/src/jc_voronoi.h"

#include "boundingbox.hpp"

// bounding box algorithm based on https://github.com/cansik/LongLiveTheSquare
BoundingBox calculateBoundingBox(const jcv_site* site)
{
    auto edge = site->edges;

    auto box = BoundingBox();
    auto minArea = std::numeric_limits<float>::max();

    // collect all edge points
    auto points = std::vector<std::pair<float, float>>();

    while (edge)
    {
        points.push_back(std::make_pair(edge->pos[0].x, edge->pos[0].y));

        edge = edge->next;
    }

    for(auto i = 0; i < points.size(); ++i)
    {
        // calculate edge segment angle
        auto p0 = points[i];
        auto p1 = points[(i + 1) % points.size()];

        auto deltaX = p0.first - p1.first;
        auto deltaY = p0.second - p1.second;
        auto angle = -std::atan(deltaY / deltaX);

        // rotate points so that edge segment is parallel to x axis
        auto rotated = std::vector<std::pair<float, float>>();

        for (auto j = 0; j < points.size(); ++j)
        {
            auto newX = points[j].first * std::cos(angle) - points[j].second * std::sin(angle);
            auto newY = points[j].first * std::sin(angle) + points[j].second * std::cos(angle);
            rotated.push_back(std::make_pair(newX, newY));
        }

        // find x/y range of rotated points
        auto minX = std::numeric_limits<float>::max();
        auto maxX = std::numeric_limits<float>::min();
        auto minY = std::numeric_limits<float>::max();
        auto maxY = std::numeric_limits<float>::min();

        for (auto j = 0; j < rotated.size(); ++j)
        {
            minX = std::min(minX, rotated[j].first);
            maxX = std::max(maxX, rotated[j].first);
            minY = std::min(minY, rotated[j].second);
            maxY = std::max(maxY, rotated[j].second);
        }

        auto width = maxX - minX;
        auto height = maxY - minY;

        if (width * height >= minArea)
        {
            continue;
        }

        auto centerX = (minX + maxX) / 2;
        auto centerY = (minY + maxY) / 2;
        auto rotatedBackX = centerX * std::cos(-angle) - centerY * std::sin(-angle);
        auto rotatedBackY = centerX * std::sin(-angle) + centerY * std::cos(-angle);

        box = BoundingBox{ rotatedBackX, rotatedBackY, maxX - minX, maxY - minY, angle };
        minArea = width * height;
    }

    return box;
}
