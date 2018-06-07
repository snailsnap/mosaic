#include <string>
#include <limits>
#include <cmath>
#include <random>
#include <algorithm>
#include <queue>

#include <QImage>
#include <QVector3D>
#include <QColor>
#include <QPainter>
#include <QtMath>

#include "voronoi.hpp"
#include "../mollusc.hpp"
#include "../mosaic.hpp"
#include "../helpers/boundingbox.hpp"

#define JC_VORONOI_IMPLEMENTATION
#include "../../dependencies/jc_voronoi/src/jc_voronoi.h"

QImage* Voronoi::createMosaic(const QImage& input, int maxNumOfMolluscs)
{
    auto width = input.width();
    auto height = input.height();

    // init points

    std::random_device random;
    std::mt19937_64 generator(random());
    // todo: choose better generator or manipulate the generator based on face detection
    std::uniform_real_distribution<float> horizontal(0.0, width);
    std::uniform_real_distribution<float> vertical(0.0, height);

    auto points = new jcv_point[maxNumOfMolluscs];

    for (auto i = 0; i < maxNumOfMolluscs; ++i)
    {
        new(points + i) jcv_point{ horizontal(generator), vertical(generator) };
    }

    // generate voronoi diagram

    auto diagram = jcv_diagram();
    memset(&diagram, 0, sizeof(jcv_diagram));
    jcv_diagram_generate(maxNumOfMolluscs, points, 0, &diagram);

    // get cells, calculate positions

    auto sites = jcv_diagram_get_sites(&diagram);

    auto positions = std::vector<MolluscPosition>();

    auto floodFillCanvas = std::vector<bool>(height * width, false);

    for (auto i = 0; i < diagram.numsites; ++i)
    {
        auto site = &sites[i];
        
        auto box = calculateBoundingBox(site);

        positions.push_back(MolluscPosition{ (int)std::round(box.centerX), (int)std::round(box.centerY), (int)box.width, (int)box.height, box.rotation });

#ifdef VORONOI_USE_FLOODFILL
        getSiteColor(site, input, floodFillCanvas, width, height, &(positions.back().color));
#else
        positions.back().color = toVec3(input.pixel((int)std::round(box.centerX), (int)std::round(box.centerY)));
#endif // VORONOI_USE_FLOODFILL
    }

    // draw molluscs

    auto result = new QImage(width, height, input.format());
    result->fill(Qt::GlobalColor::white);
    QPainter painter(result);

    for (auto i = 0; i < diagram.numsites; ++i)
    {
        auto pos = positions[i];

        auto mollusc = m_molluscPalette.getClosestColor(pos.color);

        // todo: better drawing with save/translate/rotate/restore
        if (mollusc.m_imageName.compare("NONE") != 0)
        {
            painter.save();
            painter.translate(pos.x, pos.y);
            painter.rotate(-qRadiansToDegrees(pos.rotation));
            painter.translate(-pos.width / 2, -pos.height / 2);
            painter.drawPixmap(0, 0, pos.width, pos.height, mollusc.m_image);
            painter.restore();
        }
    }

    // clean up

    jcv_diagram_free(&diagram);
    delete[] points;

    return result;
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
