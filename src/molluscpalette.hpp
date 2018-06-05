#pragma once

#include <vector>
#include <tuple>

#include <QColor>

#include "mollusc.hpp"

class MolluscPalette
{
public:
    MolluscPalette(QString dataPath);
    ~MolluscPalette();

    std::vector<Mollusc>* getMolluscs() const;

private:
    std::vector<Mollusc>* m_molluscs; //TODO: remove
    std::vector<std::tuple<QColor, std::vector<Mollusc>>> buckets;
};

