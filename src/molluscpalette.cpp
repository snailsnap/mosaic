#include "molluscpalette.hpp"

#include <fstream>
#include <array>
#include <iostream>

#include <QString>
#include <QVector3D>

MolluscPalette::MolluscPalette(const QString& dataPath)
{
    std::random_device random;
    generator = std::mt19937_64 { random() };

    this->loadData(dataPath);
    this->fillBuckets();
}

QVector3D MolluscPalette::toVec3(const QColor& color)
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}

Mollusc MolluscPalette::getClosestColor(const QVector3D & color)
{
    auto closestIndex = 0;
    auto minDist = std::numeric_limits<float>::max();

    for (auto i = 0u; i < m_buckets.size(); ++i)
    {
        auto dist = (color - toVec3(m_buckets[i].first)).length();
        if (dist < minDist)
        {
            minDist = dist;
            closestIndex = i;
        }
    }

    auto bucket = m_buckets[closestIndex].second;
    auto distribution = std::uniform_int_distribution<>(0, bucket.size()-1);
    auto idx = distribution(generator);
    return m_buckets[closestIndex].second.at(idx);
}

void MolluscPalette::loadData(const QString& dataPath) {
    // read meta file
    std::ifstream stream(dataPath.toStdString() + "/meta_file.csv");
    std::string string;
    std::getline(stream, string);
    while (std::getline(stream, string))
    {
        m_molluscs.emplace_back(string, dataPath);
    }

    // white mollusc for background
    m_molluscs.emplace_back("NONE;#FFFFFF;0.0;1.0;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE");
}

void MolluscPalette::fillBuckets()
{
    auto buckets = std::array<std::vector<Mollusc>*, 8*8*8>();
    for (auto mollusc : m_molluscs) {
        auto color = mollusc.m_color;
        auto idx = color.red() / 32
            + (color.green() / 32) * 8
            + (color.blue() / 32) * 8 * 8;
        if (buckets[idx] == nullptr) buckets[idx] = new std::vector<Mollusc>();
        buckets[idx]->push_back(mollusc);
    }
    
    auto i = 0;
    for (auto bucket : buckets) {
        if (bucket == nullptr) {
            ++i;
            continue;
        }
        auto r = (i & 0x7) * 32 + 15;
        auto g = ((i >> 3) & 0x7) * 32 + 15;
        auto b = ((i >> 6) & 0x7) * 32 + 15;
        auto color = QColor(r,g,b);
        m_buckets.emplace_back(std::make_pair(color, *bucket));
        delete bucket;
        ++i;
    }
}
