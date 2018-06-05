#pragma once

#include <vector>
#include <tuple>

#include <QColor>

#include "mollusc.hpp"

class MolluscPalette
{
public:
    MolluscPalette();
    ~MolluscPalette();

private:
    std::vector<std::tuple<QColor, std::vector<Mollusc>>> buckets;
};

