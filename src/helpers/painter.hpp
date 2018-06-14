#pragma once

#include "../mollusc.hpp"
#include "../mosaic.hpp"
#include "../molluscpalette.hpp"

#include <vector>
#include <tuple>

#include <QImage>

class Painter {
public:
    static std::vector<std::shared_ptr<Mollusc>> paint(const std::vector<MolluscPosition> molluscs, std::shared_ptr<MolluscPalette> palette , QImage& colorImage, QImage& idImage);
};
