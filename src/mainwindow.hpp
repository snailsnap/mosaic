#include "mollusc.hpp"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMoveEvent>
#include <QtWidgets>
#include <QtGui>
#include <QPixmap>

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent, QImage* image, std::vector<Mollusc>* molluscs);
    ~MainWindow();

    void keyPressEvent(QKeyEvent* event);
    void showSnailInfo();
    void showSidebar(
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
        const QImage &image3);

private:
    std::vector<Mollusc>* m_molluscs;
    int m_selectedMolluscIndex;

    QGridLayout *m_layout;
    QScrollArea *m_scrollArea;
    QWidget *m_infoWidget;
    QDockWidget *m_dWidget;

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

    QLabel *m_image1Label;
    QLabel *m_image2Label;
    QLabel *m_image3Label;
};