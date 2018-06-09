#pragma once

#include "../mollusc.hpp"
#include "../mosaic.hpp"
#include "../molluscpalette.hpp"

#include <vector>
#include <tuple>

#include <QImage>

class Painter {
public:
    static std::vector<Mollusc*>* paint(const std::vector<MolluscPosition*>* molluscs, MolluscPalette* palette , QImage& colorImage, QImage& idImage);
};