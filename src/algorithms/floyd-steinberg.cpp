#include <string>
#include <limits>
#include <cmath>

#include <QImage>
#include <QVector3D>
#include <QColor>
#include <QPainter>

#include "floyd-steinberg.hpp"
#include "../mollusc.hpp"
#include "../mosaic.hpp"

QImage* FloydSteinberg::createMosaic(const QImage& input, int scale)
{
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

            const Mollusc& mollusc = getClosestColor(m_molluscs, oldVector);
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
