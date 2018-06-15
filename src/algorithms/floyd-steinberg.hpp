#pragma once

#include <vector>

#include <QImage>

#include "../mollusc.hpp"
#include "../molluscpalette.hpp"
#include "../mosaic.hpp"

class FloydSteinberg : MosaicGenerator
{
public:
    FloydSteinberg(MolluscPalette& molluscPalette) : MosaicGenerator(molluscPalette) { };

    std::vector<MolluscPosition> createMosaic(const QImage& input, int maxNumOfMolluscs);
};
