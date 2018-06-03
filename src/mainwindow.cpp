#include "mainwindow.hpp"

#include <QtWidgets>
#include <QtGui>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent, QImage* image)
    : QMainWindow(parent)
{
    QGraphicsView *view = new QGraphicsView;
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QSize imageSize = image->size();
    QGraphicsScene *scene = new QGraphicsScene(0, 0, imageSize.width(), imageSize.height(), this);

    scene->addPixmap(QPixmap::fromImage(*image));
    view->setScene(scene);

    this->setCentralWidget(view);
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
    if (event->key() == Qt::Key_I) {
        //TODO: Later move it to mouse click on a snail in image
        this->showSnailInfo();
    }
}

void MainWindow::showSnailInfo()
{
    //TODO: Later change with specific data of highlighted snail
    QSize imageSize = QSize(100, 100);
    QImage image1 = QImage("./../data/ZMB_Mol_100073_1.png").scaled(imageSize, Qt::KeepAspectRatio);
    QImage image2 = QImage("./../data/ZMB_Mol_100073_2.png").scaled(imageSize, Qt::KeepAspectRatio);
    QImage image3 = QImage("./../data/ZMB_Mol_100073_3.png").scaled(imageSize, Qt::KeepAspectRatio);

    const QString classContent = QStringLiteral("Gastropoda");
    const QString familyContent = QStringLiteral("Ariophantidae");
    const QString genusContent = QStringLiteral("Asperitas");
    const QString speciesContent = QStringLiteral("trochus polymorpha");
    const QString scientificNameContent = QStringLiteral("Asperitas trochus polymorpha");
    const QString localityContent = QStringLiteral("Rana Mesé");
    const QString dateContent = QStringLiteral("19-29.06.1927");
    const QString areaContent = QStringLiteral("Flores");
    const QString provinceContent = QStringLiteral("East Nusa Tenggara");
    const QString countryContent = QStringLiteral("Indonesia");
    const QString subContinentContent = QStringLiteral(" ");
    const QString continentContent = QStringLiteral("Asia");
    this->showSidebar(
        classContent,
        familyContent,
        genusContent,
        speciesContent,
        scientificNameContent,
        localityContent,
        dateContent,
        areaContent,
        provinceContent,
        countryContent,
        subContinentContent,
        continentContent,
        image1,
        image2,
        image3);
}

void MainWindow::showSidebar(
    const QString &classContent, 
    const QString &familyContent, 
    const QString &genusContent, 
    const QString &speciesContent, 
    const QString &scientificNameContent,
    const QString &localityContent,
    const QString &dateContent,
    const QString &areaContent,
    const QString &provinceContent,
    const QString &countryContent,
    const QString &subContinentContent,
    const QString &continentContent,
    const QImage &image1,
    const QImage &image2,
    const QImage &image3)
{
    QLabel *classLabel = new QLabel("Klasse: " + classContent);
    QLabel *familyLabel = new QLabel("Familie: " + familyContent);
    QLabel *genusLabel = new QLabel("Geschlecht: " + genusContent);
    QLabel *speciesLabel = new QLabel("Spezies: " + speciesContent);
    QLabel *scientificNameLabel = new QLabel("Wissenschatflicher Begriff: " + scientificNameContent);
    QLabel *localityLabel = new QLabel("Fundort: " + localityContent);
    QLabel *dateLabel = new QLabel("Datum: " + dateContent);
    QLabel *areaLabel = new QLabel("Gebiet: " + areaContent);
    QLabel *provinceLabel = new QLabel("Provinz: " + provinceContent);
    QLabel *countryLabel = new QLabel("Land: " + countryContent);
    QLabel *subContinentLabel = new QLabel("Teilkontinent: " + subContinentContent);
    QLabel *continentLabel = new QLabel("Kontinent: " + continentContent);

    QLabel *image1Label = new QLabel("image1Label");
    image1Label->setPixmap(QPixmap::fromImage(image1));
    QLabel *image2Label = new QLabel("image2Label");
    image2Label->setPixmap(QPixmap::fromImage(image2));
    QLabel *image3Label = new QLabel("image3Label");
    image3Label->setPixmap(QPixmap::fromImage(image3));

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

    layout->addWidget(image1Label);
    layout->addWidget(image2Label);
    layout->addWidget(image3Label);

    QDockWidget *dwidget = new QDockWidget(this);
    dwidget->setWindowTitle("Informationen");
    dwidget->setWidget(scrollArea);
    dwidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dwidget->setFeatures(QDockWidget::DockWidgetClosable);
    dwidget->setStyleSheet("QDockWidget::title { text-align: left; background: white;}");
    this->addDockWidget(Qt::RightDockWidgetArea, dwidget);
}

