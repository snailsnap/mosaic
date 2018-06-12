#include "molluscimage.hpp"

#include <iostream>

MolluscImage::MolluscImage(MainWindow * window)
    : m_window(window)
{
}

void MolluscImage::mouseReleaseEvent(QMouseEvent * event)
{
    std::cout << "MolluscImage::mouseReleaseEvent" << std::endl;
    QLabel::mouseReleaseEvent(event);
    m_window->onClick(event);
}
