#include "mainwindow.hpp"
#include "algorithms/voronoi.hpp"
#include "mail.hpp"
#include "helpers/painter.hpp"
#include "molluscview.hpp"
#include "molluscimage.hpp"

#include <QFileDialog>
#include <QCameraInfo>
#include <QDesktopWidget>
#include <QPointer>

#include <iostream>

MainWindow::MainWindow(QWidget *parent, bool useCam, QString outputPath,
                       int maxNumOfMolluscs, QString data)
    : QMainWindow(parent)
    , m_molluscPalette(std::make_shared<MolluscPalette>())
    , m_painter(m_molluscPalette)
    , m_mosaic(*m_molluscPalette)
    , m_selectedMolluscIndex(0)
    , m_resultLabel(new MolluscImage(this))
    , m_useCam(useCam)
    , m_view(new MolluscView(this))
    , m_diaTimer(new QTimer(this))
    , m_countdownTimer(new QTimer(this))
    , m_dia1(true)
    , m_scene(new QGraphicsScene(m_view))
    , m_mainLayout(new QGridLayout(m_view))
    , m_cameraButton(new QPushButton(this))
    , m_backButton(new QPushButton(this))
    , m_shareButton(new QPushButton(this))
    , m_countdownLabel(new QLabel("text"))
    , m_outputPath(outputPath)
    , m_maxNumOfMolluscs(maxNumOfMolluscs)
    , m_data(data)
    , m_mailClient(MailClient::fromCredentials("resources/credentials.txt"))
{
    m_molluscPalette->loadData(data);

    if (useCam) {
        m_webcam = std::make_unique<Webcam>();
        QObject::connect(m_webcam.get(), &Webcam::imageReady, this, &MainWindow::processAndShowPicture);
    }

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setCentralWidget(m_view);
    m_view->setScene(m_scene);

    m_view->setLayout(m_mainLayout);
    m_mainLayout->setSpacing(3);
    m_mainLayout->setMargin(0);
    m_mainLayout->addWidget(m_resultLabel, 0, 0, 4, 3);

    m_sidebar = Sidebar::newSidebarForScreen(QApplication::desktop()->screenGeometry());

    QObject::connect(m_diaTimer, SIGNAL(timeout()), this, SLOT(diaChange()));
    
    QObject::connect(m_countdownTimer, SIGNAL(timeout()), this, SLOT(countdownChange()));
    auto font = m_countdownLabel->font();
    font.setPointSize(70);
    font.setBold(true);
    m_countdownLabel->setFont(font);
    m_countdownLabel->setStyleSheet("QLabel { background-color : black; color : white; font}");
    m_mainLayout->addWidget(m_countdownLabel, 0, 0, 4, 3, Qt::AlignCenter);

    this->initButtons();
    this->showDia();

    initializeSidebar();
}

MainWindow::~MainWindow() = default;

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
            if(m_countdown < 0) this->takeSelfie();
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
    const auto selectedMollusc = m_molluscs.at(m_selectedMolluscIndex);
    const auto description = selectedMollusc.descr();

    auto lastDotIndex = selectedMollusc.m_imageName.find_last_of('.');
    auto imageName = selectedMollusc.m_imageName.substr(0, lastDotIndex);
   
    auto underscoreIdx1 = imageName.find_last_of('_');
    auto imageNumber = QString::fromStdString(imageName.substr(underscoreIdx1));

    this->showSidebar(
            QString::fromStdString(description->m_class),
            QString::fromStdString(description->m_family),
            QString::fromStdString(description->m_genus),
            QString::fromStdString(description->m_species),
            QString::fromStdString(description->m_scientificName),
            QString::fromStdString(description->m_locality),
            QString::fromStdString(description->m_date),
            QString::fromStdString(description->m_area),
            QString::fromStdString(description->m_province),
            QString::fromStdString(description->m_country),
            QString::fromStdString(description->m_subContinent),
            QString::fromStdString(description->m_continent),
            QImage(m_data + "/" + QString::fromStdString(description->m_inventoryNumber) + "_1" + imageNumber + ".png").scaledToHeight(100),
            QImage(m_data + "/" + QString::fromStdString(description->m_inventoryNumber) + "_2" + imageNumber + ".png").scaledToHeight(100),
            QImage(m_data + "/" + QString::fromStdString(description->m_inventoryNumber) + "_3" + imageNumber + ".png").scaledToHeight(100),
            QString::fromStdString(selectedMollusc.description(m_data.toStdString())));
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
        const QImage &image3,
        const QString &descriptionContent)
{

    m_sidebar->updateContent(
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
            image3,
            descriptionContent);
    this->addDockWidget(m_sidebar->dockArea(), m_sidebar);
}

void MainWindow::initButton(QPushButton* button, std::string icon, int row, int column, bool visible) {
    const auto iconSize = 100;
    button->setIcon(QIcon("resources/" + QString::fromStdString(icon)));
    button->setIconSize(QSize(iconSize, iconSize));
    button->setFixedSize(iconSize, iconSize);
    button->setStyleSheet("text-align:center; background: black; border: none");
    button->setVisible(visible);
    m_mainLayout->addWidget(button, row, column);
}

void MainWindow::initButtons() {
    initButton(m_backButton, "back.png", 0, 0, false);
    connect(m_backButton, SIGNAL(released()), this, SLOT(showDia()));

    initButton(m_shareButton, "share.png", 2, 2, false);
    connect(m_shareButton, SIGNAL(released()), this, SLOT(shareButtonClick()));

    initButton(m_cameraButton, "camera.png", 3, 2);
    connect(m_cameraButton, SIGNAL(released()), this, SLOT(takeSelfie()));
}

void MainWindow::onClick(QMouseEvent * event)
{
    if (m_idImage != nullptr && !m_imageCaptureInProgress) {
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
    if (m_useCam && m_webcam->supported()) {
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
            m_imageCaptureInProgress = false;
            return;
        }
        m_openImagePath = fileName;

        // read input image
        auto image = QImage(fileName);
        processAndShowPicture(std::make_shared<QImage>(image));
    }
    m_backButton->setVisible(true);
    m_shareButton->setVisible(true);
}

void MainWindow::takeSelfie()
{
    m_imageCaptureInProgress = true;
    m_countdown = 3;
    m_countdownTimer->start(1000);
    m_diaTimer->stop();
}

void MainWindow::countdownChange() {
    if (m_countdown > -1) {
        m_countdownLabel->setText(QStringLiteral("Foto in: ") + QString::number(m_countdown--));
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
        this->processAndShowPicture(std::make_shared<QImage>("resources/dia1.png"));
    }
    else {
        m_dia1 = !m_dia1;
        this->processAndShowPicture(std::make_shared<QImage>("resources/dia2.png"));
    }
    m_backButton->setVisible(false);
    m_shareButton->setVisible(false);
    m_imageCaptureInProgress = false;
    m_diaTimer->start(c_diaTime);
}

void MainWindow::showDia()
{
    m_imageCaptureInProgress = true;
    m_diaTimer->start(0);
}

void MainWindow::stopDia()
{
    if (!m_diaTimer->isActive()) return;
    m_diaTimer->stop();
}

void MainWindow::shareButtonClick() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Schneckenpost"),
        tr("Sende dir das Bild an deine Mailadresse:"), QLineEdit::Normal,
        "", &ok);
    std::string adr = text.toStdString();
    if (ok && !text.isEmpty()) {
        if (!text.contains("@") || !text.contains(".")) {
            //TODO: show user?
            return;
        }
        m_mailClient.sendImage(text, *m_result);
    }
}

void MainWindow::sendMail()
{
    m_mailClient.sendImageToDefaultRecipient(*m_result);
}

void MainWindow::processAndShowPicture(std::shared_ptr<QImage> inputImage) {
    std::cout << "Showing image..." << std::endl;

    m_countdownLabel->setVisible(false);
    m_sidebar->setVisible(false);
    // scale image to screen size
    auto display = QApplication::desktop()->screenGeometry();
    auto scaledImage = inputImage->scaled(display.size(), Qt::KeepAspectRatioByExpanding);
    auto image = scaledImage.copy((scaledImage.width() - display.width()) / 2, (scaledImage.height() - display.height()) / 2, display.width(), display.height());
    
    // process image
    m_result = std::make_unique<QImage>(image.width(), image.height(), QImage::Format::Format_RGB32);
    m_idImage = std::make_unique<QImage>(image.width(), image.height(), QImage::Format::Format_RGB32);
    m_molluscs = m_painter.paint(m_mosaic.createMosaic(image, m_maxNumOfMolluscs), *m_result, *m_idImage);

    m_resultLabel->setFixedSize(display.width(), display.height());
    m_resultLabel->setPixmap(QPixmap::fromImage(*m_result));

    m_imageCaptureInProgress = false;
}

void MainWindow::initializeSidebar() {

}
