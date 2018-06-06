#pragma once

#include <vector>

#include <QImage>

#include "../mollusc.hpp"
#include "../molluscpalette.hpp"
#include "../mosaic.hpp"

class FloydSteinberg : MosaicGenerator
{
public:
    FloydSteinberg(const MolluscPalette& molluscPalette) : MosaicGenerator(molluscPalette) { };

    QImage * createMosaic(const QImage& input, int maxNumOfMolluscs);
};
