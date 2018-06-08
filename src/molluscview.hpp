#pragma once

#include "mainwindow.hpp"

#include <QGraphicsView>
#include <QMouseEvent>

class MolluscScene : public QGraphicsView {
public:
    MolluscScene(MainWindow* window);
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    MainWindow * m_window;
};
