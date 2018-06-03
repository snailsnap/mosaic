#pragma once

#include <vector>
#include <QImage>
#include "mollusc.hpp"

class MosaicGenerator
{
public:
    MosaicGenerator(const std::vector<Mollusc>& molluscs) : m_molluscs{ molluscs } { };

    virtual QImage * createMosaic(const QImage& input, int scale) = 0;

protected:
    const std::vector<Mollusc>& m_molluscs;
};
