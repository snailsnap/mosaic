#include <string>
#include <limits>
#include <cmath>
#include <random>
#include <algorithm>
#include <queue>
#include <iostream>

#include <QImage>
#include <QVector3D>
#include <QColor>
#include <QPainter>
#include <QtMath>

#include "voronoi.hpp"
#include "../mollusc.hpp"
#include "../mosaic.hpp"
#include "../helpers/boundingbox.hpp"
#include "../helpers/positionGenerator.hpp"

#define JC_VORONOI_IMPLEMENTATION
#include "../../dependencies/jc_voronoi/src/jc_voronoi.h"

std::vector<MolluscPosition> Voronoi::createMosaic(const QImage& input, int maxNumOfMolluscs)
{
    auto width = input.width();
    auto height = input.height();

    // init points

    auto gen = RandomPostionGenerator(0, 0, width, height, width / 2, height / 2, width / 3, height / 3);

    auto points = new jcv_point[maxNumOfMolluscs];

    for (auto i = 0; i < maxNumOfMolluscs; ++i)
    {
        new(points + i) jcv_point();
        gen.getPosition(points + i);
    }

    // generate voronoi diagram

    auto diagram = jcv_diagram();
    memset(&diagram, 0, sizeof(jcv_diagram));
    jcv_diagram_generate(maxNumOfMolluscs, points, 0, &diagram);

    // get cells, calculate positions

    auto sites = jcv_diagram_get_sites(&diagram);

    std::vector<MolluscPosition> positions;
    positions.reserve(diagram.numsites);

    std::vector<bool> floodFillCanvas(height * width, false);

    for (auto i = 0; i < diagram.numsites; ++i)
    {
        auto site = &sites[i];
        
        auto box = calculateBoundingBox(site);

        // throw away cells outside of canvas
        if (box.centerX < 0 || box.centerX >= width || box.centerY < 0 || box.centerY >= height)
        {
            continue;
        }

#ifdef VORONOI_USE_FLOODFILL
        positions.push_back(box.centerX, box.centerY, box.width, box.height, box.rotation);
        getSiteColor(site, input, floodFillCanvas, width, height, &(positions->back()->color));
#else
        positions.push_back(MolluscPosition {box.centerX, box.centerY, box.width, box.height, box.rotation, toVec3(input.pixel(box.centerX, box.centerY))});
#endif // VORONOI_USE_FLOODFILL
    }

    // clean up

    jcv_diagram_free(&diagram);
    delete[] points;

    return positions;
}

struct IntPoint
{
    int x;
    int y;
};

void enqueueIfNeeded(const IntPoint& point, int width, int height, std::vector<bool>& floodFillCanvas, std::queue<IntPoint>& queue)
{
    // check for image boundaries
    if (point.x < 0 || point.x >= width || point.y < 0 || point.y >= height)
    {
        return;
    }

    // ignore pixels which were already used
    if (floodFillCanvas[point.x + point.y * width])
    {
        return;
    }

    queue.push(point);
    floodFillCanvas[point.x + point.y * width] = true;
}

void enqueueNeighbors(const IntPoint& point, int width, int height, std::vector<bool>& floodFillCanvas, std::queue<IntPoint>& queue)
{
    enqueueIfNeeded(IntPoint{ point.x + 1, point.y }, width, height, floodFillCanvas, queue);
    enqueueIfNeeded(IntPoint{ point.x - 1, point.y }, width, height, floodFillCanvas, queue);
    enqueueIfNeeded(IntPoint{ point.x, point.y + 1 }, width, height, floodFillCanvas, queue);
    enqueueIfNeeded(IntPoint{ point.x, point.y - 1 }, width, height, floodFillCanvas, queue);
}

void Voronoi::getSiteColor(const jcv_site* site, const QImage& image, std::vector<bool>& floodFillCanvas, int width, int height, QVector3D* color)
{
    *color = QVector3D();
    auto count = 0;

    // walk over edges first

    auto edge = site->edges;
    int x, y, x0, y0, x1, y1, xDist, yDist, dir;

    while (edge)
    {
        x0 = (int)std::round(edge->pos[0].x);
        y0 = (int)std::round(edge->pos[0].y);
        x1 = (int)std::round(edge->pos[1].x);
        y1 = (int)std::round(edge->pos[1].y);

        xDist = std::abs(x1 - x0);
        yDist = std::abs(y1 - y0);

        if (xDist == 0 && yDist == 0)
        {
            // only one pixel, which is covered by one adjacent edge - skip
            edge = edge->next;
            continue;
        }

        if (xDist > yDist)
        {
            dir = (x1 - x0) / xDist;
            for (x = x0; x != x1; x += dir)
            {
                if (x < 0 || x >= width)
                {
                    continue;
                }

                y = std::round((float)yDist * x / xDist);

                if (y < 0 || y >= height)
                {
                    continue;
                }

                *color += toVec3(image.pixel(x, y));
                ++count;

                floodFillCanvas[x + y * width] = true;
            }
        }
        else
        {
            dir = (y1 - y0) / yDist;
            for (y = y0; y != y1; y += dir)
            {
                if (y < 0 || y >= height)
                {
                    continue;
                }

                x = std::round((float)xDist * y / yDist);

                if (x < 0 || x >= width)
                {
                    continue;
                }

                *color += toVec3(image.pixel(x, y));
                ++count;

                floodFillCanvas[x + y * width] = true;
            }
        }

        edge = edge->next;
    }

    // now floodfill, beginning from site base point

    auto point = IntPoint{ (int)std::round(site->p.x), (int)std::round(site->p.y) };

    auto queue = std::queue<IntPoint>();

    if (floodFillCanvas[point.x + point.y * width])
    {
        std::cout << "IS ALREADY TRUE" << std::endl;

        *color += toVec3(image.pixel(point.x, point.y));
        ++count;

        *color /= count;
        return;
    }
    
    if (point.x < 0 || point.x >= width || point.y < 0 || point.y >= height)
    {
        enqueueNeighbors(point, width, height, floodFillCanvas, queue);
    }
    else
    {
        queue.push(point);
        floodFillCanvas[point.x + point.y * width] = true;
    }

    while (!queue.empty())
    {
        point = queue.front();
        queue.pop();

        *color += toVec3(image.pixel(point.x, point.y));
        ++count;

        enqueueNeighbors(point, width, height, floodFillCanvas, queue);
    }

    *color /= count;
}
