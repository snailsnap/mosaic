#include "molluscview.hpp"

#include <iostream>

MolluscScene::MolluscScene(MainWindow * window)
    : m_window(window)
{
}

void MolluscScene::mouseReleaseEvent(QMouseEvent * event)
{
    m_window->onClick(event);
}
