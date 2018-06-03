#pragma once

#include <vector>
#include <QImage>
#include "../mollusc.hpp"

QImage* createMosaic(const QImage& input, const std::vector<Mollusc>& molluscs, int scale);
