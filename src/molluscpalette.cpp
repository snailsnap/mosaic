#include "molluscpalette.hpp"

#include <fstream>
#include <array>
#include <iostream>

#include <QString>
#include <QVector3D>

MolluscPalette::MolluscPalette(QString dataPath)
{
    this->loadData(dataPath);
    this->fillBuckets();
}

MolluscPalette::~MolluscPalette()
{
}

std::vector<Mollusc>* MolluscPalette::getMolluscs() const
{
    return m_molluscs;
}

QVector3D MolluscPalette::toVec3(const QColor& color) const
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}

const Mollusc & MolluscPalette::getClosestColor(const QVector3D & color) const
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

    return m_buckets[closestIndex].second->at(0); //TODO use random
}

void MolluscPalette::loadData(QString dataPath) {
    // read meta file
    std::ifstream stream(dataPath.toStdString() + "/meta_file.csv");
    std::vector<std::string> strings;
    std::string string;
    while (std::getline(stream, string))
    {
        strings.push_back(string);
    }

    // generate molluscs
    m_molluscs = new std::vector<Mollusc>();
    for (auto i = 1u; i < strings.size(); ++i)
    {
        m_molluscs->push_back(Mollusc(strings[i], dataPath));
    }

    // white mollusc for background
    m_molluscs->push_back(Mollusc("NONE;#FFFFFF;0.0;1.0;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE;NONE"));
}

void MolluscPalette::fillBuckets()
{
    auto buckets = std::array<std::vector<Mollusc>*, 8*8*8>();
    for (auto mollusc : *m_molluscs) {
        auto color = mollusc.m_color;
        auto idx = color.red() / 32
            + (color.green() / 32) * 8
            + (color.blue() / 32) * 8 * 8;
        if (buckets[idx] == nullptr) buckets[idx] = new std::vector<Mollusc>();
        buckets[idx]->push_back(mollusc);
    }
    
    int i = 0;
    for (auto bucket : buckets) {
        if (bucket == nullptr) {
            i++;
            continue;
        }
        int r = (i & 0x7) * 32 + 15;
        int g = ((i >> 3) & 0x7) * 32 + 15;
        int b = ((i >> 6) & 0x7) * 32 + 15;
        std::cout << "i = " << i << ": (" << r << "," << g << "," << b << ")" << std::endl;
        auto color = QColor(r,g,b); //TODO use average color
        m_buckets.push_back(std::make_pair(color, bucket));
        /*std::cout << "bucket color: (" << color.red() << "," << color.green() << "," << color.blue() << ")" << std::endl;

        for (auto m : *bucket) {
            auto c = m.m_color;
            std::cout << "  (" << c.red() << "," << c.green() << "," << c.blue() << ")" << std::endl;
        }/**/
        i++;
    }

    auto col = m_buckets[0].first;
    std::cout << "bucket color: (" << col.red() << "," << col.green() << "," << col.blue() << ")" << std::endl;

    for (auto m : *(m_buckets[0].second)) {
        auto c = m.m_color;
        std::cout << "  (" << c.red() << "," << c.green() << "," << c.blue() << ")" << std::endl;
        std::cout << m.m_imageName << std::endl;
    }
}
