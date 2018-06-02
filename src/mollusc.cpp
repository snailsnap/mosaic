#include <string>
#include <sstream>
#include <vector>

#include <QColor>
#include <QImage>
#include <QString>

#include "mollusc.hpp"

Mollusc::Mollusc(const std::string& data, const QString& dir)
{
    std::stringstream stream(data);
    std::vector<std::string> strings;
    std::string string;
    while (std::getline(stream, string, ';'))
    {
        strings.push_back(string);
    }

    auto i = 0u;
    m_imageName = strings[i++];
    m_color = QColor(strings[i++].c_str());
    m_rotation = stof(strings[i++]);
    m_ratio = stof(strings[i++]);
    m_originalImage = strings[i++];
    m_inventoryNumber = strings[i++];
    m_class = strings[i++];
    m_family = strings[i++];
    m_genus = strings[i++];
    m_species = strings[i++];
    m_scientificName = strings[i++];
    m_locality = strings[i++];
    m_date = strings[i++];
    m_area = strings[i++];
    m_province = strings[i++];
    m_country = strings[i++];
    m_subContinent = strings[i++];
    m_continent = strings[i++];

    m_image = QImage(dir + "/" + m_imageName.c_str());
}
