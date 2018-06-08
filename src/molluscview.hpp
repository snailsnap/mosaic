#pragma once

#include <QGraphicsView>
#include <QMouseEvent>

class MolluscScene : public QGraphicsView {
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
};
