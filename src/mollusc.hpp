#pragma once

#include <string>
#include <vector>
#include <memory>

#include <QColor>
#include <QPixmap>
#include <QString>

struct MolluscDescription
{
    MolluscDescription(const std::vector<std::string> &data);

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
};


struct Mollusc
{
    Mollusc() = delete;
    Mollusc(const std::string& data);
    Mollusc(const Mollusc& rhs);
    ~Mollusc();

    std::string m_imageName;
    QColor m_color;
    float m_rotation;
    float m_ratio;

    std::shared_ptr<MolluscDescription> descr() const;

    std::string description(const std::string &dataDir) const;

private:
    std::shared_ptr<MolluscDescription> m_description;
    std::string loadString(const std::string &termType, const std::string &term, const std::string &dataDir) const;
};
