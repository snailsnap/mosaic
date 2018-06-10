#include "mainwindow.hpp"
#include "algorithms/voronoi.hpp"
#include "mail.hpp"
#include "helpers/painter.hpp"
#include "molluscview.hpp"

#include <QFileDialog>
#include <QCameraInfo>
#include <QDesktopWidget>

#include <iostream>

MainWindow::MainWindow(QWidget *parent, MolluscPalette* molluscPalette, bool useCam, QString outputPath, int maxNumOfMolluscs, QString data)
    : QMainWindow(parent)
    , m_molluscPalette(molluscPalette)
    , m_selectedMolluscIndex(0)
    , m_layout(new QGridLayout())
    , m_imageLayout(new QGridLayout())
    , m_scrollArea(new QScrollArea())
    , m_infoWidget(new QWidget())
    , m_dWidget(new QDockWidget(this))
    , m_titleLabel(new QLabel())
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
    , m_useCam(useCam)
    , m_view(new MolluscView(this))
    , m_outputPath(outputPath)
    , m_maxNumOfMolluscs(maxNumOfMolluscs)
    , m_data(data)
    , m_mailClient(m_data.toStdString() + "/credentials.txt")
{
    if(useCam) {
        m_webcam = new Webcam();
        QObject::connect(m_webcam, &Webcam::imageReady, this, &MainWindow::processAndShowPicture);
    }

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setCentralWidget(m_view);
}

MainWindow::~MainWindow() {}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_F11: {
            if (!this->isFullScreen()) {
                this->showFullScreen();
            }
            else {
                this->showMaximized();
            }
            break;
        }
        case Qt::Key_Escape: {
            this->showMaximized();
            break;
        }
        case Qt::Key_I: {
            this->showSnailInfo();
            break;
        }
        case Qt::Key_P: {
            this->takePicture();
            break;
        }
        case Qt::Key_S: {
            if(m_result != nullptr)
                m_result->save(m_outputPath);
            break;
        }
        case Qt::Key_M: {
            if (m_result != nullptr)
                this->sendMail();
            break;
        }
    }
}

void MainWindow::showSnailInfo()
{
    if (m_molluscs == nullptr) return;
    Mollusc* selectedMollusc = m_molluscs->at(m_selectedMolluscIndex);

    auto lastDotIndex = selectedMollusc->m_imageName.find_last_of(".");
    auto imageName = selectedMollusc->m_imageName.substr(0, lastDotIndex);
    
    auto underscoreIdx1 = imageName.find_last_of("_");
    auto imageNumber = QString::fromStdString(imageName.substr(underscoreIdx1));
    
    this->showSidebar(
        QString::fromStdString(selectedMollusc->m_class),
        QString::fromStdString(selectedMollusc->m_family),
        QString::fromStdString(selectedMollusc->m_genus),
        QString::fromStdString(selectedMollusc->m_species),
        QString::fromStdString(selectedMollusc->m_scientificName),
        QString::fromStdString(selectedMollusc->m_locality),
        QString::fromStdString(selectedMollusc->m_date),
        QString::fromStdString(selectedMollusc->m_area),
        QString::fromStdString(selectedMollusc->m_province),
        QString::fromStdString(selectedMollusc->m_country),
        QString::fromStdString(selectedMollusc->m_subContinent),
        QString::fromStdString(selectedMollusc->m_continent),
        QImage(m_data + "/" + QString::fromStdString(selectedMollusc->m_inventoryNumber) + "_1" + imageNumber + ".png").scaledToHeight(100),
        QImage(m_data + "/" + QString::fromStdString(selectedMollusc->m_inventoryNumber) + "_2" + imageNumber + ".png").scaledToHeight(100),
        QImage(m_data + "/" + QString::fromStdString(selectedMollusc->m_inventoryNumber) + "_3" + imageNumber + ".png").scaledToHeight(100));
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
    m_titleLabel->setText("<b><font size=\"6\">" + scientificNameContent + "</font></b>");
    m_classLabel->setText("<b>Klasse:</b> " + classContent);
    m_familyLabel->setText("<b>Familie:</b> " + familyContent);
    m_genusLabel->setText("<b>Geschlecht:</b> " + genusContent);
    m_speciesLabel->setText("<b>Spezies:</b> " + speciesContent);
    m_scientificNameLabel->setText("<b>Wissenschatflicher Begriff:</b> " + scientificNameContent);
    m_localityLabel->setText("<b>Fundort:</b> " + localityContent);
    m_dateLabel->setText("<b>Datum:</b> " + dateContent);
    m_areaLabel->setText("<b>Gebiet:</b> " + areaContent);
    m_provinceLabel->setText("<b>Provinz:</b> " + provinceContent);
    m_countryLabel->setText("<b>Land:</b> " + countryContent);
    m_subContinentLabel->setText("<b>Teilkontinent:</b> " + subContinentContent);
    m_continentLabel->setText("<b>Kontinent:</b> " + continentContent);

    m_image1Label->setPixmap(QPixmap::fromImage(image1));
    m_image2Label->setPixmap(QPixmap::fromImage(image2));
    m_image3Label->setPixmap(QPixmap::fromImage(image3));


    m_infoWidget->setLayout(m_layout);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setWidget(m_infoWidget);

    // position images side by side
    m_imageLayout->addWidget(m_image1Label, 0, 0);
    m_imageLayout->addWidget(m_image2Label, 0, 1);
    m_imageLayout->addWidget(m_image3Label, 0, 2);

    m_layout->addWidget(m_titleLabel, 0, 0);
    m_layout->addLayout(m_imageLayout, 1, 0);
    m_layout->addWidget(m_classLabel, 2, 0);
    m_layout->addWidget(m_familyLabel, 3, 0);
    m_layout->addWidget(m_genusLabel, 4, 0);
    m_layout->addWidget(m_speciesLabel, 5, 0);
    m_layout->addWidget(m_scientificNameLabel, 6, 0);
    m_layout->addWidget(m_localityLabel, 7, 0);
    m_layout->addWidget(m_dateLabel, 8, 0);
    m_layout->addWidget(m_areaLabel, 9, 0);
    m_layout->addWidget(m_provinceLabel, 10, 0);
    m_layout->addWidget(m_countryLabel, 11, 0);
    m_layout->addWidget(m_subContinentLabel, 12, 0);
    m_layout->addWidget(m_continentLabel, 13, 0);

    m_dWidget->setWidget(m_scrollArea);
    m_dWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    m_dWidget->setFeatures(QDockWidget::DockWidgetClosable);
    m_dWidget->setStyleSheet("QDockWidget::title { text-align: left; background: white;}");
    this->addDockWidget(Qt::RightDockWidgetArea, m_dWidget);
}

void MainWindow::onClick(QMouseEvent * event)
{
    if (m_idImage != nullptr) {
        auto x = event->x();
        auto y = event->y();
        auto color = m_idImage->pixelColor(x, y);
        if (color != Qt::white) {
            m_selectedMolluscIndex = color.red() + (color.green() << 8) + (color.blue() << 16);
            this->showSnailInfo();
        }
    }
}

void MainWindow::takePicture() {
    if (m_useCam && QCameraInfo::availableCameras().size() > 0) {
        std::cout << "Capturing image..." << std::endl;
        m_webcam->captureImage();
    }
    else {
        auto fileName = QFileDialog::getOpenFileName(this, tr("Open Image"),
            m_openImagePath,
            tr("Images (*.png *.jpg)"));

        if (fileName == "") return;
        m_openImagePath = fileName;

        // read input image
        auto image = QImage(fileName);

        processAndShowPicture(std::make_shared<QImage>(image));
    }
}

void MainWindow::sendMail()
{
    m_mailClient.sendImageToDefaultRecipient(*m_result);
}

void MainWindow::processAndShowPicture(std::shared_ptr<QImage> inputImage) {
    std::cout << "Showing image..." << std::endl;
    // scale image to screen size
    auto display = QApplication::desktop()->screenGeometry();
    auto image = inputImage->scaled(display.size(), Qt::KeepAspectRatioByExpanding);
    
    // process image
    auto mosaic = Voronoi(*m_molluscPalette);
    auto molluscPositions = mosaic.createMosaic(image, m_maxNumOfMolluscs);

    m_result = new QImage(image.width(), image.height(), image.format());
    m_idImage = new QImage(image.width(), image.height(), image.format());
    m_molluscs = Painter::paint(molluscPositions, m_molluscPalette, *m_result, *m_idImage);

    auto imageSize = m_result->size();
    auto scene = new QGraphicsScene(0, 0, imageSize.width(), imageSize.height(), this);

    scene->addPixmap(QPixmap::fromImage(*m_result));
    m_view->setScene(scene);
}
