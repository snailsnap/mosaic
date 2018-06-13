#pragma once

#include <vector>
#include <tuple>
#include <random>
#include <memory>
#include <unordered_map>
#include <functional>

#include <QColor>
#include <QFile>

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

class MolluscImages {
    std::shared_ptr<std::unordered_map<std::string, QPixmap>> images;
public:
    QPixmap& lookup(const std::string& name) const;

    MolluscImages() = delete;
    MolluscImages(const QString datadir, std::vector<QString> &&filenames);
    ~MolluscImages();
};

class MolluscPalette
{
public:
    MolluscPalette();
    MolluscPalette(const MolluscPalette&) = delete;
    ~MolluscPalette();

    static QVector3D toVec3(const QColor & color);
    static QColor fromVec3(const QVector3D& vcolor);
    std::shared_ptr<Mollusc> getClosestColor(const QVector3D& color);

    QPixmap& lookup(const std::string& name) const;

    void loadData(const QString& dataPath);
protected:
    const std::vector<Mollusc> getMolluscs() const;
    std::mt19937_64 random_gen;

private:
    std::vector<Mollusc> m_molluscs;
    std::unordered_multimap<QColor, Mollusc> m_mbuckets;

    std::unique_ptr<MolluscImages> m_images;
};

