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

MainWindow::MainWindow(QWidget *parent, std::shared_ptr<MolluscPalette> molluscPalette,
                       bool useCam, QString outputPath, int maxNumOfMolluscs, QString data)
    : QMainWindow(parent)
    , m_molluscPalette(molluscPalette)
    , m_painter(m_molluscPalette)
    , m_mosaic(*m_molluscPalette)
    , m_selectedMolluscIndex(0)
    , m_layout(new QVBoxLayout())
    , m_imageLayout(new QHBoxLayout())
    , m_scrollArea(new QScrollArea())
    , m_resultLabel(new MolluscImage(this))
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
    , m_descriptionLabel(new QLabel())
    , m_image1Label(new QLabel("image1Label"))
    , m_image2Label(new QLabel("image2Label"))
    , m_image3Label(new QLabel("image3Label"))
    , m_useCam(useCam)
    , m_view(new MolluscView(this))
    , m_diaTimer(new QTimer(this))
    , m_countdownTimer(new QTimer(this))
    , m_dia1(true)
    , m_scene(new QGraphicsScene())
    , m_mainLayout(new QGridLayout())
    , m_pixmapItem(new QGraphicsPixmapItem())
    , m_cameraButton(new QPushButton())
    , m_backButton(new QPushButton())
    , m_shareButton(new QPushButton())
    , m_countdownLabel(new QLabel("text"))
    , m_outputPath(outputPath)
    , m_maxNumOfMolluscs(maxNumOfMolluscs)
    , m_data(data)
    , m_mailClient(MailClient::fromCredentials("resources/credentials.txt"))
{
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
    std::shared_ptr<Mollusc> selectedMollusc = m_molluscs.at(m_selectedMolluscIndex);

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
            QString::fromStdString(selectedMollusc->m_date), QString::fromStdString(selectedMollusc->m_area),
            QString::fromStdString(selectedMollusc->m_province),
            QString::fromStdString(selectedMollusc->m_country),
            QString::fromStdString(selectedMollusc->m_subContinent),
            QString::fromStdString(selectedMollusc->m_continent),
            QImage(m_data + "/" + QString::fromStdString(selectedMollusc->m_inventoryNumber) + "_1" + imageNumber + ".png").scaledToHeight(100),
            QImage(m_data + "/" + QString::fromStdString(selectedMollusc->m_inventoryNumber) + "_2" + imageNumber + ".png").scaledToHeight(100),
            QImage(m_data + "/" + QString::fromStdString(selectedMollusc->m_inventoryNumber) + "_3" + imageNumber + ".png").scaledToHeight(100),
            QString::fromStdString(selectedMollusc->description(m_data.toStdString())));
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

    m_titleLabel->setText("<b><font size=\"6\">" + scientificNameContent + "</font></b>");
    m_classLabel->setText("<b>Klasse:</b> " + classContent);
    m_familyLabel->setText("<b>Familie:</b> " + familyContent);
    m_genusLabel->setText("<b>Geschlecht:</b> " + genusContent);
    m_speciesLabel->setText("<b>Spezies:</b> " + speciesContent);
    m_scientificNameLabel->setText("<b>Wiss. Begr.:</b> " + scientificNameContent);
    m_localityLabel->setText("<b>Fundort:</b> " + localityContent);
    m_dateLabel->setText("<b>Datum:</b> " + dateContent);
    m_areaLabel->setText("<b>Gebiet:</b> " + areaContent);
    m_provinceLabel->setText("<b>Provinz:</b> " + provinceContent);
    m_countryLabel->setText("<b>Land:</b> " + countryContent);
    m_subContinentLabel->setText("<b>Teilkontinent:</b> " + subContinentContent);
    m_continentLabel->setText("<b>Kontinent:</b> " + continentContent);
    m_descriptionLabel->setText("<b>Zusätzliche Informationen:</b> " + descriptionContent);

    m_image1Label->setPixmap(QPixmap::fromImage(image1));
    m_image2Label->setPixmap(QPixmap::fromImage(image2));
    m_image3Label->setPixmap(QPixmap::fromImage(image3));


    int newWidth = std::max(m_layout->minimumSize().width(), m_imageLayout->minimumSize().width());
    m_scrollArea->setMinimumWidth(newWidth);
    // I don't know why you have to do it like this, but this sets the sidebar to the right size immediately
    m_dWidget->setWidget(m_scrollArea);
    newWidth = std::max(m_layout->minimumSize().width(), m_imageLayout->minimumSize().width());
    m_scrollArea->setMinimumWidth(newWidth);
    m_dWidget->setVisible(true);
    this->addDockWidget(Qt::RightDockWidgetArea, m_dWidget);
}

void MainWindow::initButton(QPushButton* button, std::string icon, int row, int column, bool visible) {
    const int iconSize = 100;
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
    m_dWidget->setVisible(false);
    // scale image to screen size
    auto display = QApplication::desktop()->screenGeometry();
    auto scaledImage = inputImage->scaled(display.size(), Qt::KeepAspectRatioByExpanding);
    auto image = scaledImage.copy((scaledImage.width() - display.width()) / 2, (scaledImage.height() - display.height()) / 2, display.width(), display.height());
    
    // process image
    m_result = std::make_unique<QImage>(image.width(), image.height(), QImage::Format::Format_RGB32);
    m_idImage = std::make_unique<QImage>(image.width(), image.height(), QImage::Format::Format_RGB32);
    m_molluscs = m_painter.paint(std::move(m_mosaic.createMosaic(image, m_maxNumOfMolluscs)), *m_result, *m_idImage);

    m_resultLabel->setFixedSize(display.width(), display.height());
    m_resultLabel->setPixmap(QPixmap::fromImage(*m_result));

    m_imageCaptureInProgress = false;
}

void MainWindow::initializeSidebar() {
    m_layout->setSpacing(0);
    m_infoWidget->setLayout(m_layout);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setWidget(m_infoWidget);

    // position images side by side
    m_imageLayout->addWidget(m_image1Label);
    m_imageLayout->addWidget(m_image2Label);
    m_imageLayout->addWidget(m_image3Label);
    m_imageLayout->setSpacing(10);

    QMargins spacingMargin(0,0,0,10);
    m_titleLabel->setContentsMargins(spacingMargin);
    m_classLabel->setContentsMargins(spacingMargin);
    m_familyLabel->setContentsMargins(spacingMargin);
    m_genusLabel->setContentsMargins(spacingMargin);
    m_speciesLabel->setContentsMargins(spacingMargin);
    m_scientificNameLabel->setContentsMargins(spacingMargin);
    m_localityLabel->setContentsMargins(spacingMargin);
    m_dateLabel->setContentsMargins(spacingMargin);
    m_areaLabel->setContentsMargins(spacingMargin);
    m_provinceLabel->setContentsMargins(spacingMargin);
    m_countryLabel->setContentsMargins(spacingMargin);
    m_subContinentLabel->setContentsMargins(spacingMargin);
    m_continentLabel->setContentsMargins(spacingMargin);
    m_descriptionLabel->setContentsMargins(spacingMargin);
    m_imageLayout->setContentsMargins(spacingMargin);

    m_layout->addWidget(m_titleLabel);
    m_layout->addLayout(m_imageLayout);
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
    m_layout->addWidget(m_descriptionLabel);
    m_descriptionLabel->setWordWrap(true);
    // remove spaces
    m_layout->insertStretch( -1, 1 );

    m_dWidget->setWidget(m_scrollArea);
    m_dWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    m_dWidget->setFeatures(QDockWidget::DockWidgetClosable);
    m_dWidget->setStyleSheet("QDockWidget::title { text-align: left; background: white;}");
}
