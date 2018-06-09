#include "molluscview.hpp"

#include <iostream>

MolluscView::MolluscView(MainWindow * window)
    : m_window(window)
{
}

void MolluscView::mouseReleaseEvent(QMouseEvent * event)
{
    m_window->onClick(event);
}
