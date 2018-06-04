#include <string>
#include <limits>
#include <cmath>
#include <random>
#include <algorithm>

#include <QImage>
#include <QVector3D>
#include <QColor>
#include <QPainter>

#define JC_VORONOI_IMPLEMENTATION
#include "../../dependencies/jc_voronoi/src/jc_voronoi.h"

#include "voronoi.hpp"
#include "../mollusc.hpp"
#include "../mosaic.hpp"

QImage* Voronoi::createMosaic(const QImage& input, int maxNumOfMolluscs)
{
    // init points

    std::random_device random;
    std::mt19937_64 generator(random());
    // todo: choose better generator or manipulate the generator based on face detection
    std::uniform_real_distribution<float> horizontal(0.0, input.width());
    std::uniform_real_distribution<float> vertical(0.0, input.height());

    auto points = new jcv_point[maxNumOfMolluscs];

    for (auto i = 0; i < maxNumOfMolluscs; ++i)
    {
        new(points + i) jcv_point{ horizontal(generator), vertical(generator) };
    }

    // generate voronoi diagram

    jcv_diagram diagram;
    memset(&diagram, 0, sizeof(jcv_diagram));
    jcv_diagram_generate(maxNumOfMolluscs, points, 0, &diagram);

    // get cells, calculate positions

    auto sites = jcv_diagram_get_sites(&diagram);

    auto positions = new MolluscPosition[diagram.numsites];

    for (auto i = 0; i < diagram.numsites; ++i)
    {
        auto site = &sites[i];
        auto edge = site->edges;

        auto x = 0.0;
        auto y = 0.0;
        auto count = 0;

        auto minX = std::numeric_limits<double>::max();
        auto maxX = std::numeric_limits<double>::min();
        auto minY = std::numeric_limits<double>::max();
        auto maxY = std::numeric_limits<double>::min();

        // todo: check anisotropy, set angle accordingly, consider when calculating dimensions
        // todo: average color in cell, don't just use center
        while (edge)
        {
            x += edge->pos[0].x;
            y += edge->pos[1].y;
            ++count;

            minX = std::min<double>(minX, edge->pos[0].x);
            maxX = std::max<double>(maxX, edge->pos[0].x);
            minY = std::min<double>(minY, edge->pos[0].y);
            maxY = std::max<double>(maxY, edge->pos[0].y);

            edge = edge->next;
        }

        x /= count;
        y /= count;

        auto dimX = (int)(maxX - minX);
        auto dimY = (int)(maxY - minY);
        auto dim = std::min<int>(dimX, dimY);

        new(positions + i) MolluscPosition{ (int)x, (int)y, dim, dim, 0, QColor(input.pixel((int)x, (int)y)) };
    }

    // draw molluscs

    auto result = new QImage(input.width(), input.height(), input.format());
    result->fill(Qt::GlobalColor::white);
    QPainter painter(result);

    for (auto i = 0; i < diagram.numsites; ++i)
    {
        auto pos = &positions[i];

        auto mollusc = getClosestColor(m_molluscs, toVec3(pos->color));

        // todo: better drawing with save/translate/rotate/restore
        if (mollusc.m_imageName.compare("NONE") != 0)
            painter.drawPixmap(pos->x - pos->width / 2, pos->y - pos->height / 2, pos->width, pos->height, mollusc.m_image);
    }

    // clean up

    jcv_diagram_free(&diagram);
    delete[] points;

    return result;
}
