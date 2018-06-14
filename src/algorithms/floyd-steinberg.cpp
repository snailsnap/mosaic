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

std::vector<MolluscPosition> FloydSteinberg::createMosaic(const QImage& input, int maxNumOfMolluscs)
{
    auto sizeRatio = (float)input.width() / input.height();
    auto horizontal = sizeRatio >= 1;
    if (!horizontal)
    {
        sizeRatio = 1 / sizeRatio;
    }
    auto numPerEdge = (int)std::sqrt(maxNumOfMolluscs / sizeRatio);

    auto targetWidth = (int)(horizontal ? numPerEdge * sizeRatio : numPerEdge);
    auto molluscSize = input.width() / targetWidth;

    auto scaled = input.scaledToWidth(targetWidth, Qt::SmoothTransformation);

    auto width = scaled.width();
    auto height = scaled.height();

    std::vector<QVector3D> errorStorage((width + 2) * (height + 1), QVector3D());

    auto result = new QImage(width * molluscSize, height * molluscSize, input.format());
    result->fill(Qt::GlobalColor::white);
    QPainter resultPainter(result);
    auto idTexture = new QImage(width * molluscSize, height * molluscSize, input.format());
    idTexture->fill(Qt::GlobalColor::black);
    QPainter idPainter(idTexture);

    std::vector<MolluscPosition> positions;
    MolluscPosition pos;
    pos.rotation = 0.0;
    pos.width = pos.height = molluscSize;

    for (auto y = 0; y < scaled.height(); ++y)
    {
        for (auto x = 0; x < scaled.width(); ++x)
        {
            auto oldColor = QColor(scaled.pixel(x, y));
            auto oldVector = toVec3(oldColor) + errorStorage[x + 1 + y * width];

            auto mollusc = m_molluscPalette.getClosestColor(oldVector);
            auto newVector = toVec3(mollusc->m_color);

            if (mollusc->m_imageName.compare("NONE") != 0) {
                pos.color = newVector;
                pos.x = x * molluscSize;
                pos.y = y * molluscSize;
                positions.push_back(pos);
            }

            auto error = oldVector - newVector;

            errorStorage[x + 2 + y * width] = error * 7 / 16;
            errorStorage[x + (y + 1) * width] = error * 3 / 16;
            errorStorage[x + 1 + (y + 1) * width] = error * 5 / 16;
            errorStorage[x + 2 + (y + 1) * width] = error * 1 / 16;
        }
    }

    return positions;
}
