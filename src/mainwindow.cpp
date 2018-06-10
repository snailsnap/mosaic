#include "mainwindow.hpp"
#include "algorithms/voronoi.hpp"
#include "mail.hpp"
#include "helpers/painter.hpp"
#include "molluscview.hpp"

#include <QFileDialog>
#include <QCameraInfo>
#include <QDesktopWidget>
#include <QPointer>

#include <iostream>

MainWindow::MainWindow(QWidget *parent, MolluscPalette* molluscPalette, bool useCam, QString outputPath, int maxNumOfMolluscs, QString data)
    : QMainWindow(parent)
    , m_molluscPalette(molluscPalette)
    , m_selectedMolluscIndex(0)
    , m_layout(new QGridLayout())
    , m_scrollArea(new QScrollArea())
    , m_resultLabel(new QLabel())
    , m_infoWidget(new QWidget())
    , m_dWidget(new QDockWidget(this))
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
    , m_data(data)
    , m_useCam(useCam)
    , m_view(new MolluscView(this))
    , m_diaTimer(new QTimer(this))
    , m_countdownTimer(new QTimer(this))
    , m_dia1(true)
    , m_scene(new QGraphicsScene())
    , m_pixmapItem(new QGraphicsPixmapItem())
    , m_cameraButton(new QPushButton())
    , m_countdownLabel(new QLabel("text"))
    , m_outputPath(outputPath)
    , m_maxNumOfMolluscs(maxNumOfMolluscs)
    , m_mailClient(m_data.toStdString() + "/credentials.txt")
{
    if(useCam) {
        m_webcam = new Webcam();
        QObject::connect(m_webcam, &Webcam::imageReady, this, &MainWindow::processAndShowPicture);
    }

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setCentralWidget(m_view);
    m_view->setScene(m_scene);

    m_scene->addWidget(m_resultLabel);

    QObject::connect(m_diaTimer, SIGNAL(timeout()), this, SLOT(diaChange()));
    
    QObject::connect(m_countdownTimer, SIGNAL(timeout()), this, SLOT(countdownChange()));
    auto font = m_countdownLabel->font();
    font.setPointSize(70);
    font.setBold(true);
    m_countdownLabel->setFont(font);
    m_countdownLabel->setStyleSheet("QLabel { background-color : black; color : white; font}");
    m_scene->addWidget(m_countdownLabel);

    this->showCameraButton();
    this->showDia();
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
            if (m_result != nullptr)
                m_result->save(m_outputPath);
            break;
        }
        case Qt::Key_M: {
            if (m_result != nullptr)
                this->sendMail();
            break;
        }
        case Qt::Key_D: {
            this->showDia();
            break;
        }
        case Qt::Key_X: {
            this->stopDia();
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


    m_infoWidget->setLayout(m_layout);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setWidget(m_infoWidget);

    m_layout->addWidget(m_classLabel);
    m_layout->addWidget(m_familyLabel);
    m_layout->addWidget(m_genusLabel);
    m_layout->addWidget(m_speciesLabel);
    m_layout->addWidget(m_scientificNameLabel);
    m_layout->addWidget(m_localityLabel);
    m_layout->addWidget(m_dateLabel);
    m_layout->addWidget(m_areaLabel);
    m_layout->addWidget(m_provinceLabel);
    m_layout->addWidget(m_countryLabel);
    m_layout->addWidget(m_subContinentLabel);
    m_layout->addWidget(m_continentLabel);

    m_layout->addWidget(m_image1Label);
    m_layout->addWidget(m_image2Label);
    m_layout->addWidget(m_image3Label);

    m_dWidget->setWindowTitle("Informationen");
    m_dWidget->setWidget(m_scrollArea);
    m_dWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    m_dWidget->setFeatures(QDockWidget::DockWidgetClosable);
    m_dWidget->setStyleSheet("QDockWidget::title { text-align: left; background: white;}");
    m_dWidget->setVisible(true);
    this->addDockWidget(Qt::RightDockWidgetArea, m_dWidget);
}

void MainWindow::showCameraButton() {
    m_cameraButton->setIcon(QIcon(m_data + "/camera.png"));
    m_cameraButton->setIconSize(QSize(100, 100));
    m_cameraButton->setStyleSheet("text-align:center; background: black; border: none");
    m_scene->addWidget(m_cameraButton);
    connect(m_cameraButton, SIGNAL(released()), this, SLOT(takeSelfie()));
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

        if (fileName == "") {
            m_diaTimer->setInterval(c_diaTime);
            m_countdownLabel->setVisible(false);
            return;
        }
        m_openImagePath = fileName;

        // read input image
        auto image = QImage(fileName);
        processAndShowPicture(std::make_shared<QImage>(image));
    }
}

void MainWindow::takeSelfie()
{
    m_countdown = 3;
    m_countdownTimer->start(1000);
    m_diaTimer->stop();
}

void MainWindow::countdownChange() {
    if (m_countdown > -1) {
        m_countdownLabel->setText(QStringLiteral("lächeln in: ") + QString::number(m_countdown--));
        m_countdownLabel->adjustSize();

        auto display = QApplication::desktop()->screenGeometry();
        m_countdownLabel->move((display.width() - m_countdownLabel->width()) / 2, (display.height() - m_countdownLabel->height()) / 2);
        m_countdownLabel->setVisible(true);
        return;
    }
    
    if (m_countdown == -1) {
        takePicture();
        m_countdownTimer->stop();
        m_diaTimer->start(c_photoTime);
    }
}

void MainWindow::diaChange() {
    if (m_dia1) {
        m_dia1 = !m_dia1;
        this->processAndShowPicture(std::make_shared<QImage>(QString::fromStdString(m_data.toStdString() + "/dia1.png")));
    }
    else {
        m_dia1 = !m_dia1;
        this->processAndShowPicture(std::make_shared<QImage>(QString::fromStdString(m_data.toStdString() + "/dia2.png")));
    }
    m_diaTimer->start(c_diaTime);
}

void MainWindow::showDia()
{
    m_diaTimer->start(0);
}

void MainWindow::stopDia()
{
    if (!m_diaTimer->isActive()) return;
    m_diaTimer->stop();
}

void MainWindow::sendMail()
{
    m_mailClient.sendImageToDefaultRecipient(*m_result);
}

void MainWindow::processAndShowPicture(std::shared_ptr<QImage> inputImage) {
    std::cout << "Showing image..." << std::endl;

    m_countdownLabel->setVisible(false);
    m_dWidget->setVisible(false);
    // scale image to screen size
    auto display = QApplication::desktop()->screenGeometry();
    auto scaledImage = inputImage->scaled(display.size(), Qt::KeepAspectRatioByExpanding);
    auto image = scaledImage.copy((scaledImage.width() - display.width()) / 2, (scaledImage.height() - display.height()) / 2, display.width(), display.height());
    
    // process image
    auto mosaic = Voronoi(*m_molluscPalette);
    auto molluscPositions = mosaic.createMosaic(image, m_maxNumOfMolluscs);

    m_result = new QImage(image.width(), image.height(), image.format());
    m_idImage = new QImage(image.width(), image.height(), image.format());
    m_molluscs = Painter::paint(molluscPositions, m_molluscPalette, *m_result, *m_idImage);

    auto offset = (display.width() - m_result->width()) / 2;
    m_resultLabel->setFixedSize(display.width(), display.height());
    m_resultLabel->setPixmap(QPixmap::fromImage(*m_result));

    m_cameraButton->move(display.width() - m_cameraButton->iconSize().width() - offset, display.height() - m_cameraButton->iconSize().height());
}
