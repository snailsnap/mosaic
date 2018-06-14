#pragma once

#include <vector>
#include <tuple>
#include <random>
#include <memory>
#include <unordered_map>

#include <QColor>
#include <QFile>

#include "mollusc.hpp"

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
    ~MolluscPalette();

    static QVector3D toVec3(const QColor & color);
    Mollusc getClosestColor(const QVector3D& color);

    QPixmap& lookup(const std::string& name) const;

    void loadData(const QString& dataPath);

protected:
    std::mt19937_64 generator;

private:
    std::vector<Mollusc> m_molluscs;
    std::vector<std::pair<QColor, std::vector<Mollusc>>> m_buckets;

    std::unique_ptr<MolluscImages> m_images;
};

