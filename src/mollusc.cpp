#include <string>
#include <sstream>
#include <vector>
#include <cassert>

#include <QColor>
#include <QPixmap>
#include <QString>
#include <QFile>
#include <iostream>

#include "mollusc.hpp"

MolluscDescription::MolluscDescription(const std::vector<std::string>& strings)
{
    m_originalImage = strings[4];
    m_inventoryNumber = strings[5];
    m_class = strings[6];
    m_family = strings[7];
    m_genus = strings[8];
    m_species = strings[9];
    m_scientificName = strings[10];
    m_locality = strings[11];
    m_date = strings[12];
    m_area = strings[13];
    m_province = strings[14];
    m_country = strings[15];
    m_subContinent = strings[16];
    m_continent = (strings.size() == 18) ? strings[17] : "";
}

std::shared_ptr<MolluscDescription> Mollusc::descr() const {
    return m_description;
}

Mollusc::Mollusc(const std::string& data)
{
    std::stringstream stream(data);
    std::vector<std::string> strings;
    std::string string;
    string.reserve(18);
    while (std::getline(stream, string, ';'))
    {
        strings.push_back(string);
    }
    assert(strings.size() == 17 || strings.size() == 18);

    auto i = 0u;
    m_imageName = strings[0];
    m_color = QColor(strings[1].c_str());
    m_rotation = stof(strings[2]);
    m_ratio = stof(strings[3]);

    m_description = std::make_shared<MolluscDescription>(strings);
}

Mollusc::Mollusc(const Mollusc &rhs) = default;

Mollusc::~Mollusc() {
}

std::string Mollusc::description(const std::string &dataDir) const {
    // prioritizes family over class. Only shows class if family info doesn't exist
    std::string result = loadString("Family", m_description->m_family, dataDir);
    if (result.empty()) {
        result = loadString("Class", m_description->m_class, dataDir);
    }
    return result;
}

std::string Mollusc::loadString(const std::string &termType, const std::string &term,
                                const std::string &dataDir) const {
    std::string p = dataDir + "/descriptions/" + termType + "_" + term + ".txt";
    QString filepath = QString::fromStdString(p);
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }
    return file.readAll().toStdString();
}
