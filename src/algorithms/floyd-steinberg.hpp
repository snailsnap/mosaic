#pragma once

#include <vector>
#include <QImage>
#include "../mollusc.hpp"
#include "../mosaic.hpp"

class FloydSteinberg : MosaicGenerator
{
public:
    FloydSteinberg(const std::vector<Mollusc>& molluscs) : MosaicGenerator(molluscs) { };

    QImage * createMosaic(const QImage& input, int scale);
};
