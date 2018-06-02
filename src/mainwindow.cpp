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
	this->showSidebar();
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

void MainWindow::showSidebar()
{
	QLabel *classLabel = new QLabel("Klasse: ");
	QLabel *familyLabel = new QLabel("Familie: ");
	QLabel *genusLabel = new QLabel("Geschlecht: ");
	QLabel *speciesLabel = new QLabel("Spezies: ");
	QLabel *scientificNameLabel = new QLabel("Wissenschatfliche Begriff: ");
	QLabel *localityLabel = new QLabel("Fundort: ");
	QLabel *dateLabel = new QLabel("Datum: ");
	QLabel *areaLabel = new QLabel("Gebiet: ");
	QLabel *provinceLabel = new QLabel("Provinz: ");
	QLabel *countryLabel = new QLabel("Land: ");
	QLabel *subContinentLabel = new QLabel("Teilkontinent: ");
	QLabel *continentLabel = new QLabel("Kontinent: ");

	QGridLayout *layout = new QGridLayout();

	QScrollArea *scrollArea = new QScrollArea();
	QWidget *infoWidget = new QWidget();
	infoWidget->setLayout(layout);
	scrollArea->setWidgetResizable(true);
	scrollArea->setWidget(infoWidget);

	layout->addWidget(classLabel);
	layout->addWidget(familyLabel);
	layout->addWidget(genusLabel);
	layout->addWidget(speciesLabel);
	layout->addWidget(scientificNameLabel);
	layout->addWidget(localityLabel);
	layout->addWidget(dateLabel);
	layout->addWidget(areaLabel);
	layout->addWidget(provinceLabel);
	layout->addWidget(countryLabel);
	layout->addWidget(subContinentLabel);
	layout->addWidget(continentLabel);

	QDockWidget *dwidget = new QDockWidget(this);
	dwidget->setWindowTitle("Informationen");
	dwidget->setWidget(scrollArea);
	dwidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
	this->addDockWidget(Qt::RightDockWidgetArea, dwidget);
}

