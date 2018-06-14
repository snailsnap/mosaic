#pragma once

#include <string>

#include <QColor>
#include <QPixmap>
#include <QString>

struct Mollusc
{
    Mollusc(const std::string& data);

    std::string m_imageName;
    QColor m_color;
    float m_rotation;
    float m_ratio;
    std::string m_originalImage;
    std::string m_inventoryNumber;
    std::string m_class;
    std::string m_classInfo;
    std::string m_family;
    std::string m_familyInfo;
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

    std::string description(const std::string &dataDir);

private:
    std::string loadString(const std::string &termType, const std::string &term, const std::string &dataDir);
};
