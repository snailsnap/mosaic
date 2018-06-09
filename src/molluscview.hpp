#pragma once

#include "mainwindow.hpp"

#include <QGraphicsView>
#include <QMouseEvent>

class MolluscView : public QGraphicsView {
public:
    MolluscView(MainWindow* window);
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    MainWindow * m_window;
};
