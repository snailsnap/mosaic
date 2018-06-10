#include "imageloader.hpp"

#include "../globalVars.hpp"

ImageLoader::ImageLoader(std::vector<Mollusc*>* molluscs, const QString& dir)
{
    m_threads = new std::vector<ImageLoaderThread*>;

    auto pos = 0;
    for (auto i = 0; i < g_numThreads; ++i)
    {
        auto start = pos;
        auto end = i == g_numThreads - 1 ? molluscs->size() - 1 : pos + molluscs->size() / g_numThreads;
        pos = end + 1;

        m_threads->push_back(new ImageLoaderThread(molluscs, dir, start, end));
    }
}

ImageLoader::~ImageLoader()
{
    for (auto thread : *m_threads)
    {
        delete thread;
    }

    delete m_threads;
}

void ImageLoader::load()
{
    for (auto thread : *m_threads)
    {
        thread->start();
    }

    for (auto thread : *m_threads)
    {
        thread->wait();
    }
}

