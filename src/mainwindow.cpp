#include "mainwindow.hpp"

#include <QtWidgets>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent, QImage& image)
	: QMainWindow(parent)
{
	QGraphicsView *view = new QGraphicsView;
	QGraphicsScene *scene = new QGraphicsScene(0, 0, view->size().width(), view->size().height(), this);

	scene->addPixmap(QPixmap::fromImage(image));
	view->setScene(scene);

	this->setCentralWidget(view);

	QLabel *classLabel = new QLabel("Class: ");
	QLabel *familyLabel = new QLabel("Family: ");
	QLabel *genusLabel = new QLabel("Genus: ");
	QLabel *speciesLabel = new QLabel("Species: ");
	QLabel *scientificLabel = new QLabel("Scientific: ");

	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(classLabel);
	layout->addWidget(familyLabel);
	layout->addWidget(genusLabel);
	layout->addWidget(speciesLabel);
	layout->addWidget(scientificLabel);

	QWidget *infoWidget = new QWidget;
	infoWidget->setWindowTitle("Informationen");
	infoWidget->setLayout(layout);
	QDockWidget *dwidget = new QDockWidget;
	dwidget->setWidget(infoWidget);
	this->addDockWidget(Qt::RightDockWidgetArea, dwidget);
}

MainWindow::~MainWindow() {}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_F11) {
		this->showFullScreen();
	}
	if (event->key() == Qt::Key_Escape) {
		this->showMaximized();
	}
}

