#include "mollusc.hpp"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMoveEvent>

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
};