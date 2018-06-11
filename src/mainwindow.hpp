#pragma once

#include "mollusc.hpp"
#include "molluscpalette.hpp"
#include "Webcam.hpp"
#include "mail.hpp"

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

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent, MolluscPalette* molluscPalette, bool useCam, QString outputPath, int maxNumOfMolluscs, QString data);
    ~MainWindow();

    void sendMail();
    void keyPressEvent(QKeyEvent* event);
    void showSnailInfo();
    void showCameraButton();
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
    const int c_photoTime = 30000;

    bool m_imageCaptureInProgress = false;
    int m_countdown = -1;
    bool m_useCam;
    bool m_dia1;
    int m_maxNumOfMolluscs;
    QImage* m_result = nullptr;
    QImage* m_idImage = nullptr;
    QString m_outputPath;
    Webcam* m_webcam;
    QString m_openImagePath = "C:/";
    QString m_data;

    MolluscPalette* m_molluscPalette;
    std::vector<Mollusc*>* m_molluscs;
    int m_selectedMolluscIndex;

    QGridLayout *m_layout;
    QScrollArea *m_scrollArea;
    QWidget *m_infoWidget;
    QDockWidget *m_dWidget;
    QPushButton *m_cameraButton;
    QPushButton *m_backButton;
    QPushButton *m_shareButton;
    QLabel *m_resultLabel;
    QLabel *m_countdownLabel;

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

    QGraphicsView *m_view;
    QGraphicsScene *m_scene;
    QGridLayout* m_mainLayout;
    QGraphicsPixmapItem *m_pixmapItem;

    MailClient m_mailClient;
    QTimer *m_diaTimer;
    QTimer *m_countdownTimer;

    void takePicture();
    void stopDia();
    void processAndShowPicture(std::shared_ptr<QImage> image);

public slots:
    void shareButtonClick();
    void showDia();
    void diaChange();
    void takeSelfie();
    void countdownChange();
};