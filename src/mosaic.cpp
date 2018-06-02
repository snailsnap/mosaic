#include <string>
#include <limits>
#include <cmath>

#include <QImage>
#include <QVector3D>
#include <QColor>
#include <QPainter>

#include "mosaic.hpp"
#include "mollusc.hpp"

QVector3D toVec3(const QColor& color)
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}

const Mollusc& getClosestColor(const std::vector<Mollusc>& molluscs, const QVector3D& color)
{
    auto closestIndex = 0;
    auto minDist = std::numeric_limits<float>::max();

    for (auto i = 0u; i < molluscs.size(); ++i)
    {
        auto dist = (color - toVec3(molluscs[i].m_color)).length();
        if (dist < minDist)
        {
            minDist = dist;
            closestIndex = i;
        }
    }

    return molluscs[closestIndex];
}

QImage* createMosaic(const QImage& input, const std::vector<Mollusc>& molluscs)
{
    auto scale = 64;

    auto scaled = input.scaledToWidth(input.width() / scale, Qt::SmoothTransformation);

    auto width = scaled.width();
    auto height = scaled.height();

    std::vector<QVector3D> errorStorage((width + 2) * (height + 1), QVector3D());

    auto result = new QImage(width * scale, height * scale, input.format());
    result->fill(Qt::GlobalColor::white);
    QPainter painter(result);

    for (auto y = 0; y < scaled.height(); ++y)
    {
        for (auto x = 0; x < scaled.width(); ++x)
        {
            auto oldColor = QColor(scaled.pixel(x, y));
            auto oldVector = toVec3(oldColor) + errorStorage[x + 1 + y * width];

            const Mollusc& mollusc = getClosestColor(molluscs, oldVector);
            auto newVector = toVec3(mollusc.m_color);

            if (mollusc.m_imageName.compare("NONE") != 0)
                painter.drawPixmap(x * scale, y * scale, scale, scale, mollusc.m_image);

            auto error = oldVector - newVector;

            errorStorage[x + 2 + y * width] = error * 7 / 16;
            errorStorage[x + (y + 1) * width] = error * 3 / 16;
            errorStorage[x + 1 + (y + 1) * width] = error * 5 / 16;
            errorStorage[x + 2 + (y + 1) * width] = error * 1 / 16;
        }
    }

    return result;
}
