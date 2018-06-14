#pragma once

#include <vector>
#include <tuple>
#include <random>

#include <QColor>

#include "mollusc.hpp"

class MolluscPalette
{
public:
    MolluscPalette(const QString& dataPath);

    static QVector3D toVec3(const QColor & color);
    Mollusc getClosestColor(const QVector3D& color);

protected:
    std::mt19937_64 generator;

private:
    std::vector<Mollusc> m_molluscs;
    std::vector<std::pair<QColor, std::vector<Mollusc>>> m_buckets;

    void loadData(const QString& dataPath);
    void fillBuckets();
};

