#include "molluscpalette.hpp"

#include <fstream>

#include <QString>

MolluscPalette::MolluscPalette(QString dataPath)
{
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


MolluscPalette::~MolluscPalette()
{
}

std::vector<Mollusc>* MolluscPalette::getMolluscs()
{
    return m_molluscs;
}
