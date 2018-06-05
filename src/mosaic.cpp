#include <vector>

#include <QVector3D>
#include <QColor>

#include "mollusc.hpp"
#include "mosaic.hpp"

QVector3D MosaicGenerator::toVec3(const QColor& color)
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}

QVector3D MosaicGenerator::toVec3(const QRgb& rgb)
{
    return QVector3D(qRed(rgb) / 255.0, qGreen(rgb) / 255.0, qBlue(rgb) / 255.0);
}

const Mollusc& MosaicGenerator::getClosestColor(const std::vector<Mollusc>& molluscs, const QVector3D& color)
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
