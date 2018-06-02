#include "mainwindow.hpp"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent, QImage& image)
	: QMainWindow(parent)
{
	QGraphicsView *view = new QGraphicsView;
	QGraphicsScene *scene = new QGraphicsScene(0, 0, view->size().width(), view->size().height(), this);

	scene->addPixmap(QPixmap::fromImage(image));
	view->setScene(scene);

	this->setCentralWidget(view);
}

MainWindow::~MainWindow() {}

