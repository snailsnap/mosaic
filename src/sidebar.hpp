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
    // use this to get a sidebar that fits the screen orientation
    static Sidebar *newSidebarForScreen(QRect screenSize, QWidget *parent = nullptr);

    virtual Qt::DockWidgetArea dockArea() = 0;

    virtual void updateContent(
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

protected:
    explicit Sidebar(QWidget *parent = nullptr);

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

    QMargins m_spacingMargin = QMargins(0,0,0,10);
};


class RightSidebar : public Sidebar {
public:
    explicit RightSidebar(QWidget *parent);

    Qt::DockWidgetArea dockArea() override;

    void updateContent(
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
            const QString &descriptionContent) override;

private:
    QVBoxLayout *m_layout;
    QHBoxLayout *m_imageLayout;
};

class BottomSidebar : public Sidebar {
public:
    explicit BottomSidebar(QWidget *parent);

    Qt::DockWidgetArea dockArea() override;

private:
    QHBoxLayout *m_layout;
    QVBoxLayout *m_imageLayout;
    QVBoxLayout *m_leftLayout;
    QVBoxLayout *m_rightLayout;
};


#endif //SNAILSNAP_SIDEBAR_H
