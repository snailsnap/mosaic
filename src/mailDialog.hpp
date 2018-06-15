//
// Created by Leonard Geier on 15.06.18.
//

#ifndef SNAILSNAP_MAILDIALOG_H
#define SNAILSNAP_MAILDIALOG_H


#include <QDialog>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

class MailDialog : public QDialog {

    Q_OBJECT

public:
    MailDialog();

private:
    QLabel m_title;
    QLineEdit m_input;
    QPushButton m_buttonOk;
    QPushButton m_buttonCancel;

    QGridLayout m_layout;

signals:
     void enteredMail(std::string address);
};


#endif //SNAILSNAP_MAILDIALOG_H
