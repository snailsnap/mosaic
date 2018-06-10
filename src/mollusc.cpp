#include <string>
#include <sstream>
#include <vector>

#include <QColor>
#include <QPixmap>
#include <QString>
#include <QFile>
#include <iostream>

#include "mollusc.hpp"

Mollusc::Mollusc(const std::string& data)
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
    m_continent = (strings.size() > i) ? strings[i++] : "";
}

Mollusc::Mollusc(const std::string& data, const QString& dir) : Mollusc::Mollusc(data)
{
    m_image = QPixmap(dir + "/" + m_imageName.c_str());
}

std::string Mollusc::description(const std::string &dataDir) {
    // prioritizes family over class. Only shows class if family info doesn't exist
    std::string result = loadString("Family", m_family, dataDir);
    if (result.empty()) {
        result = loadString("Class", m_class, dataDir);
    }
    return result;
}

std::string Mollusc::loadString(const std::string &termType, const std::string &term, const std::string &dataDir) {
    std::string p = dataDir + "/descriptions/" + termType + "_" + term + ".txt";
    QString filepath = QString::fromStdString(p);
    QFile file(filepath);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Warning: Failed to load description file at " << p << ": " << file.errorString().toStdString() << std::endl;
        return "";
    }
    return file.readAll().toStdString();
}
