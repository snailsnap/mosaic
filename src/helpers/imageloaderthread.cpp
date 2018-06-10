#include "imageloaderthread.hpp"

void ImageLoaderThread::run()
{
    for (auto i = m_start; i <= m_end; ++i)
    {
        m_molluscs->at(i)->loadImage(m_dir);
    }
}
