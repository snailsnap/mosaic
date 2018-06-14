#pragma once

#include <vector>
#include <tuple>
#include <random>
#include <memory>
#include <unordered_map>
#include <functional>

#include <QColor>

#include "mollusc.hpp"

namespace std {
template<>
struct hash<QColor> {
  std::size_t operator()(const QColor& color) const {
    auto idx = color.red() >> 5
            | (color.green() >> 5) << 3
            | (color.blue() >> 5) << 6;
    return idx & 0x1ff;
  }
};
}

class MolluscPalette
{
public:
    MolluscPalette(const QString& dataPath);
    ~MolluscPalette();

    static QVector3D toVec3(const QColor & color);
    static QColor fromVec3(const QVector3D& vcolor);
    std::shared_ptr<Mollusc> getClosestColor(const QVector3D& color);

protected:
    const std::vector<Mollusc> getMolluscs() const;
    std::mt19937_64 random_gen;

private:
    std::vector<Mollusc> m_molluscs;
    std::unordered_multimap<QColor, Mollusc> m_mbuckets;

    void loadData(const QString& dataPath);
    void fillBuckets();
};

