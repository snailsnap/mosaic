//
// Created by Leonard Geier on 14.06.18.
//

#ifndef SNAILSNAP_SIDEBAR_H
#define SNAILSNAP_SIDEBAR_H


#include <QDockWidget>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>

class Sidebar : public QDockWidget {
public:
    explicit Sidebar(QWidget *parent = nullptr);

    void update(
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
            const QString &descriptionContent);

private:
    QScrollArea *m_scrollArea;
    QWidget *m_infoWidget;

    QLabel *m_titleLabel;
    QLabel *m_classLabel;
    QLabel *m_familyLabel;
    QLabel *m_genusLabel;
    QLabel *m_speciesLabel;
    QLabel *m_scientificNameLabel;
    QLabel *m_localityLabel;
    QLabel *m_dateLabel;
    QLabel *m_areaLabel;
    QLabel *m_provinceLabel;
    QLabel *m_countryLabel;
    QLabel *m_subContinentLabel;
    QLabel *m_continentLabel;
    QLabel *m_descriptionLabel;

    QLabel *m_image1Label;
    QLabel *m_image2Label;
    QLabel *m_image3Label;

    QVBoxLayout *m_layout;
    QHBoxLayout *m_imageLayout;
};


#endif //SNAILSNAP_SIDEBAR_H
