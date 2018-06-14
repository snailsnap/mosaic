//
// Created by Leonard Geier on 14.06.18.
//

#include "sidebar.hpp"

Sidebar::Sidebar(QWidget *parent)
        : QDockWidget(parent)
        , m_layout(new QVBoxLayout())
        , m_imageLayout(new QHBoxLayout())
        , m_scrollArea(new QScrollArea())
        , m_infoWidget(new QWidget())
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
        , m_image3Label(new QLabel("image3Label")) {

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

    setWidget(m_scrollArea);
    setFeatures(QDockWidget::NoDockWidgetFeatures);
    setFeatures(QDockWidget::DockWidgetClosable);
    setStyleSheet("QDockWidget::title { text-align: left; background: white;}");
}

void Sidebar::update(
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
        const QString &descriptionContent) {

    // update widgets
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

    // resize sidebar to fit content
    int newWidth = std::max(m_layout->minimumSize().width(), m_imageLayout->minimumSize().width());
    m_scrollArea->setMinimumWidth(newWidth);
    // I don't know why you have to do it like this, but this sets the sidebar to the right size immediately
    setWidget(m_scrollArea);
    newWidth = std::max(m_layout->minimumSize().width(), m_imageLayout->minimumSize().width());
    m_scrollArea->setMinimumWidth(newWidth);
    setVisible(true);
}
