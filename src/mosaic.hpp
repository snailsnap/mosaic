#pragma once

#include <vector>

#include <QImage>
#include <QVector3D>
#include <QColor>

#include "mollusc.hpp"
#include "molluscpalette.hpp"

struct MolluscPosition
{
    int x;
    int y;
    int width;
    int height;
    float rotation;
    QVector3D color;
};

class MosaicGenerator
{
public:
    MosaicGenerator(MolluscPalette& molluscPalette) : m_molluscPalette{ molluscPalette } { };
    virtual std::vector<MolluscPosition> createMosaic(const QImage& input, int maxNumOfMolluscs) = 0;

protected:
    static QVector3D toVec3(const QColor& color);
    static QVector3D toVec3(const QRgb& color);
    const Mollusc& getClosestColor(const std::vector<Mollusc>& molluscs, const QVector3D& color);
    MolluscPalette& m_molluscPalette;
};
