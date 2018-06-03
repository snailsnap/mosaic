#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent, QImage* image, std::vector<Mollusc>* molluscs)
    : QMainWindow(parent)
    , m_molluscs(molluscs)
    , m_classLabel(new QLabel())
    , m_familyLabel(new QLabel())
    , m_genusLabel(new QLabel())
    , m_speciesLabel(new QLabel())
    , m_scientificNameLabel(new QLabel())
    , m_localityLabel(new QLabel())
    , m_dateLabel(new QLabel())
    , m_areaLabel(new QLabel())
    , m_provinceLabel(new QLabel())
    , m_countryLabel(new QLabel())
    , m_subContinentLabel(new QLabel())
    , m_continentLabel(new QLabel())
    , m_image1Label(new QLabel("image1Label"))
    , m_image2Label(new QLabel("image2Label"))
    , m_image3Label(new QLabel("image3Label"))
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
    int selectedMolluscIndex = 0;
    Mollusc selectedMollusc = m_molluscs->at(selectedMolluscIndex);
    
    //TODO: Later change with specific data of highlighted snail
    QSize imageSize = QSize(100, 100);
    QImage image1 = QImage("./../data/ZMB_Mol_100073_1.png").scaled(imageSize, Qt::KeepAspectRatio);
    QImage image2 = QImage("./../data/ZMB_Mol_100073_2.png").scaled(imageSize, Qt::KeepAspectRatio);
    QImage image3 = QImage("./../data/ZMB_Mol_100073_3.png").scaled(imageSize, Qt::KeepAspectRatio);

    const QString classContent = QString::fromStdString(selectedMollusc.m_class);
    const QString familyContent = QString::fromStdString(selectedMollusc.m_family);
    const QString genusContent = QString::fromStdString(selectedMollusc.m_genus);
    const QString speciesContent = QString::fromStdString(selectedMollusc.m_species);
    const QString scientificNameContent = QString::fromStdString(selectedMollusc.m_scientificName);
    const QString localityContent = QString::fromStdString(selectedMollusc.m_locality);
    const QString dateContent = QString::fromStdString(selectedMollusc.m_date);
    const QString areaContent = QString::fromStdString(selectedMollusc.m_area);
    const QString provinceContent = QString::fromStdString(selectedMollusc.m_province);
    const QString countryContent = QString::fromStdString(selectedMollusc.m_country);
    const QString subContinentContent = QString::fromStdString(selectedMollusc.m_subContinent);
    const QString continentContent = QString::fromStdString(selectedMollusc.m_continent);
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
    m_classLabel->setText("Klasse: " + classContent);
    m_familyLabel->setText("Familie: " + familyContent);
    m_genusLabel->setText("Geschlecht: " + genusContent);
    m_speciesLabel->setText("Spezies: " + speciesContent);
    m_scientificNameLabel->setText("Wissenschatflicher Begriff: " + scientificNameContent);
    m_localityLabel->setText("Fundort: " + localityContent);
    m_dateLabel->setText("Datum: " + dateContent);
    m_areaLabel->setText("Gebiet: " + areaContent);
    m_provinceLabel->setText("Provinz: " + provinceContent);
    m_countryLabel->setText("Land: " + countryContent);
    m_subContinentLabel->setText("Teilkontinent: " + subContinentContent);
    m_continentLabel->setText("Kontinent: " + continentContent);

    m_image1Label->setPixmap(QPixmap::fromImage(image1));
    m_image2Label->setPixmap(QPixmap::fromImage(image2));
    m_image3Label->setPixmap(QPixmap::fromImage(image3));

    QGridLayout *layout = new QGridLayout();

    QScrollArea *scrollArea = new QScrollArea();
    QWidget *infoWidget = new QWidget();
    infoWidget->setLayout(layout);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(infoWidget);

    layout->addWidget(m_classLabel);
    layout->addWidget(m_familyLabel);
    layout->addWidget(m_genusLabel);
    layout->addWidget(m_speciesLabel);
    layout->addWidget(m_scientificNameLabel);
    layout->addWidget(m_localityLabel);
    layout->addWidget(m_dateLabel);
    layout->addWidget(m_areaLabel);
    layout->addWidget(m_provinceLabel);
    layout->addWidget(m_countryLabel);
    layout->addWidget(m_subContinentLabel);
    layout->addWidget(m_continentLabel);

    layout->addWidget(m_image1Label);
    layout->addWidget(m_image2Label);
    layout->addWidget(m_image3Label);

    QDockWidget *dwidget = new QDockWidget(this);
    dwidget->setWindowTitle("Informationen");
    dwidget->setWidget(scrollArea);
    dwidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dwidget->setFeatures(QDockWidget::DockWidgetClosable);
    dwidget->setStyleSheet("QDockWidget::title { text-align: left; background: white;}");
    this->addDockWidget(Qt::RightDockWidgetArea, dwidget);
}

