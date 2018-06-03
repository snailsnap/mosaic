#pragma once

#include <vector>

#include <QImage>
#include <QVector3D>
#include <QColor>

#include "mollusc.hpp"

class MosaicGenerator
{
public:
    MosaicGenerator(const std::vector<Mollusc>& molluscs) : m_molluscs{ molluscs } { };
    virtual QImage * createMosaic(const QImage& input, int maxNumOfMolluscs) = 0;

protected:
    static QVector3D toVec3(const QColor& color);
    const Mollusc& getClosestColor(const std::vector<Mollusc>& molluscs, const QVector3D& color);
    const std::vector<Mollusc>& m_molluscs;
};
