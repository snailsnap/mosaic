#include <vector>

#include <QVector3D>
#include <QColor>

#include "mollusc.hpp"
#include "mosaic.hpp"

QVector3D MosaicGenerator::toVec3(const QColor& color)
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}
