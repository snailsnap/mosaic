#pragma once

#include "../mollusc.hpp"
#include "../mosaic.hpp"
#include "../molluscpalette.hpp"

#include <vector>
#include <tuple>
#include <memory>

#include <QImage>

class Painter {
    std::shared_ptr<MolluscPalette> m_palette;
public:
    Painter(std::shared_ptr<MolluscPalette> palette);
    std::vector<Mollusc> paint(
        std::vector<MolluscPosition>&& molluscs,
        QImage& colorImage, QImage& idImage);
};
