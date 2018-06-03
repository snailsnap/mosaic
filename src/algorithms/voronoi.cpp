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

QImage* FloydSteinberg::createMosaic(const QImage& input, int maxNumOfMolluscs
{
    return new QImage(input);
}
