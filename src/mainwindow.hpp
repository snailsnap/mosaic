#pragma once

#include "mollusc.hpp"
#include "molluscpalette.hpp"
#include "Webcam.hpp"
#include "mail.hpp"
#include "sidebar.hpp"

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QMoveEvent>
#include <QtWidgets>
#include <QtGui>
#include <QPixmap>
#include <QObject>
#include <QTimer>
#include <QGraphicsProxyWidget>

#include "helpers/painter.hpp"
#include "algorithms/voronoi.hpp"
#include "mailDialog.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent, bool useCam, QString outputPath, int maxNumOfMolluscs, QString data);
    ~MainWindow();

    void sendMail();
    void keyPressEvent(QKeyEvent* event);
    void showSnailInfo();
    void showSidebar(const QString &classContent,
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
    void onClick(QMouseEvent * event);

private:
    const int c_diaTime = 5000;
    const int c_photoTime = 300000;

    bool m_imageCaptureInProgress = false;
    int m_countdown = -1;
    bool m_useCam;
    bool m_dia1;
    int m_maxNumOfMolluscs;
    std::shared_ptr<QImage> m_result;
    std::shared_ptr<QImage> m_idImage;
    QString m_outputPath;
    std::unique_ptr<Webcam> m_webcam;
    QString m_openImagePath = "C:/";
    QString m_data;

    std::shared_ptr<MolluscPalette> m_molluscPalette;
    std::vector<Mollusc> m_molluscs;
    Painter m_painter;
    Voronoi m_mosaic;
    int m_selectedMolluscIndex;

    QPushButton *m_cameraButton;
    QPushButton *m_backButton;
    QPushButton *m_shareButton;
    QLabel *m_resultLabel;
    QLabel *m_countdownLabel;

    Sidebar *m_sidebar;

    QGraphicsView *m_view;
    QGraphicsScene *m_scene;
    QGridLayout* m_mainLayout;

    MailClient m_mailClient;
    QTimer *m_diaTimer;
    QTimer *m_countdownTimer;

    void takePicture();
    void stopDia();
    void initButton(QPushButton* button, std::string icon, int row, int column, bool visible = true);
    void initButtons();
    void processAndShowPicture(std::shared_ptr<QImage> image);
    void initializeSidebar();

    MailDialog m_mailDialog;

public slots:
    void shareButtonClick();
    void showDia();
    void diaChange();
    void takeSelfie();
    void countdownChange();
};
