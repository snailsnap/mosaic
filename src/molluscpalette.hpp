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

    const std::vector<Mollusc>* getMolluscs() const;
    QVector3D toVec3(const QColor & color) const;
    const Mollusc& getClosestColor(const QVector3D& color) const;

private:
    std::vector<Mollusc>* m_molluscs;
    std::vector<std::pair<QColor, std::vector<Mollusc>*>> m_buckets;

    void loadData(const QString& dataPath);
    void fillBuckets();
};

