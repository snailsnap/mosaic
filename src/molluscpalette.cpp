#include "molluscpalette.hpp"

#include <fstream>
#include <array>
#include <iostream>

#include <QString>
#include <QVector3D>

MolluscPalette::MolluscPalette(const QString& dataPath)
{
    this->loadData(dataPath);
    this->fillBuckets();
}

const std::vector<Mollusc*>* MolluscPalette::getMolluscs() const
{
    return m_molluscs;
}

QVector3D MolluscPalette::toVec3(const QColor& color) const
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}

Mollusc* MolluscPalette::getClosestColor(const QVector3D & color) const
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

    auto bucket = *(m_buckets[closestIndex].second);

    std::random_device random;
    std::mt19937_64 generator(random());
    auto distribution = std::uniform_int_distribution<>(0, bucket.size()-1);
    auto idx = distribution(generator);
    return m_buckets[closestIndex].second->at(idx);
}

void MolluscPalette::loadData(const QString& dataPath) {
    // read meta file
    std::ifstream stream(dataPath.toStdString() + "/meta_file.csv");
    std::vector<std::string> strings;
    std::string string;
    while (std::getline(stream, string))
    {
        strings.push_back(string);
    }

    // generate molluscs
    m_molluscs = new std::vector<Mollusc*>();
    for (auto i = 1u; i < strings.size(); ++i)
    {
        m_molluscs->push_back(new Mollusc(strings[i], dataPath));
    }

    // white mollusc for background
    m_molluscs->push_back(new Mollusc("NONE;#FFFFFF;0.0;1.0;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE"));
}

void MolluscPalette::fillBuckets()
{
    auto buckets = std::array<std::vector<Mollusc*>*, 8*8*8>();
    for (auto mollusc : *m_molluscs) {
        auto color = mollusc->m_color;
        auto idx = color.red() / 32
            + (color.green() / 32) * 8
            + (color.blue() / 32) * 8 * 8;
        if (buckets[idx] == nullptr) buckets[idx] = new std::vector<Mollusc*>();
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
        m_buckets.push_back(std::make_pair(color, bucket));
        ++i;
    }
}
