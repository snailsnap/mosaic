#include <string>
#include <limits>
#include <cmath>

#include <QImage>
#include <QVector3D>
#include <QColor>
#include <QPainter>

#include "voronoi.hpp"
#include "../mollusc.hpp"
#include "../mosaic.hpp"

QImage* Voronoi::createMosaic(const QImage& input, int maxNumOfMolluscs)
{
    return new QImage(input);
}
