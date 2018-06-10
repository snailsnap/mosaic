#pragma once

#include <vector>

#include <QString>

#include "../mollusc.hpp"
#include "imageloaderthread.hpp"

class ImageLoader
{
public:
    ImageLoader(std::vector<Mollusc*>* molluscs, const QString& dir);
    ~ImageLoader();
    void load();
private:
    std::vector<ImageLoaderThread*>* m_threads;
};
