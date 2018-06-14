#pragma once

#include <vector>

#include <QImage>

#include "../../dependencies/jc_voronoi/src/jc_voronoi.h"

#include "../mollusc.hpp"
#include "../mosaic.hpp"

class Voronoi : MosaicGenerator
{
public:
    Voronoi(MolluscPalette& molluscPalette) : MosaicGenerator(molluscPalette) { };

    std::vector<MolluscPosition> createMosaic(const QImage& input, int maxNumOfMolluscs);

private:
    void getSiteColor(const jcv_site* site, const QImage& image, std::vector<bool>& floodFillCanvas, int width, int height, QVector3D* color);
};
