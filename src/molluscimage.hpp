#pragma once

#include "mainwindow.hpp"

#include <QLabel>
#include <QMouseEvent>

class MolluscImage : public QLabel {
public:
    MolluscImage(MainWindow* window);
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    MainWindow * m_window;
};
