#include "molluscpalette.hpp"

#include <fstream>
#include <array>
#include <iostream>
#include <iterator>
#include <algorithm>

#include <QString>
#include <QVector3D>

MolluscPalette::MolluscPalette(const QString& dataPath)
{
    std::random_device random;
    random_gen = std::mt19937_64 { random() };

    m_mbuckets.max_load_factor(1e6);

    loadData(dataPath);
    fillBuckets();
}

MolluscPalette::~MolluscPalette() {
}

const std::vector<Mollusc> MolluscPalette::getMolluscs() const
{
    return m_molluscs;
}

QVector3D MolluscPalette::toVec3(const QColor& color)
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}
QColor MolluscPalette::fromVec3(const QVector3D& vcolor) {
  QColor color;
  color.setRedF(vcolor.x());
  color.setGreenF(vcolor.y());
  color.setBlueF(vcolor.z());
  return color;
}

std::shared_ptr<Mollusc> MolluscPalette::getClosestColor(const QVector3D & vcolor)
{
    const auto color = fromVec3(vcolor);

    assert(m_mbuckets.bucket_count() != 0);
    const auto bucket = m_mbuckets.bucket(color);

    auto distribution = std::uniform_int_distribution<std::unordered_multimap<QColor, Mollusc>::difference_type>(0, m_mbuckets.bucket_size(bucket)-1);
    auto bucket_it = m_mbuckets.begin(bucket);
    if (bucket_it == m_mbuckets.end(bucket)) {
        const auto comp = [&](auto& lhs, auto& rhs) {
            return (vcolor - toVec3(lhs.first)).lengthSquared() < (vcolor - toVec3(rhs.first)).lengthSquared();
        };
        const auto closest_it = std::min_element(m_mbuckets.cbegin(), m_mbuckets.cend(), comp);
        bucket_it = m_mbuckets.begin(m_mbuckets.bucket(closest_it->first));
    }
    std::advance(bucket_it, distribution(random_gen));
    return std::make_shared<Mollusc>(bucket_it->second);
}

void MolluscPalette::loadData(const QString& dataPath) {
    // read meta file
    std::ifstream stream(dataPath.toStdString() + "/meta_file.csv");
    std::string string;
    std::getline(stream, string);

    // generate molluscs
    while (std::getline(stream, string)) {
        m_molluscs.emplace_back(string, dataPath);
    }

    // white mollusc for background
    m_molluscs.emplace_back("NONE;#FFFFFF;0.0;1.0;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE");
}

void MolluscPalette::fillBuckets() {
    for (const auto& mollusc : m_molluscs) {
        m_mbuckets.emplace(mollusc.m_color, mollusc);
    }
}
