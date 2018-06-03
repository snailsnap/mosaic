#pragma once

#include <vector>

#include <QImage>

#include "../mollusc.hpp"
#include "../mosaic.hpp"

class Voronoi : MosaicGenerator
{
public:
    Voronoi(const std::vector<Mollusc>& molluscs) : MosaicGenerator(molluscs) { };

    QImage * createMosaic(const QImage& input, int maxNumOfMolluscs);
};
