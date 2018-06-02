#pragma once

#include <string>

#include <QColor>
#include <QImage>
#include <QString>

struct Mollusc
{
    Mollusc(const std::string& data, const QString& dir);

    std::string m_imageName;
    QImage m_image;
    QColor m_color;
    float m_rotation;
    float m_ratio;
    std::string m_originalImage;
    std::string m_inventoryNumber;
    std::string m_class;
    std::string m_family;
    std::string m_genus;
    std::string m_species;
    std::string m_scientificName;
    std::string m_locality;
    std::string m_date;
    std::string m_area;
    std::string m_province;
    std::string m_country;
    std::string m_subContinent;
    std::string m_continent;
};
