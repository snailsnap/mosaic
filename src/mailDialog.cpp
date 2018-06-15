//
// Created by Leonard Geier on 15.06.18.
//

#include <iostream>
#include "mailDialog.hpp"

MailDialog::MailDialog()
        : QDialog()
        , m_title("<font size=\"6\">Bitte gib deine E-Mail an:</font>")
        , m_buttonOk("OK")
        , m_buttonCancel("Cancel")
        , m_input()
        , m_layout() {

    QObject::connect(&m_buttonOk, &QPushButton::clicked, [=](){
        emit enteredMail(this->m_input.text());
        this->done(0);
    });
    QObject::connect(&m_buttonOk, &QPushButton::clicked, [=](){
        this->done(1);
    });
    m_layout.addWidget(&m_title, 0, 0, 1, 10);
    m_layout.addWidget(&m_input, 1, 0, 1, 10);
    m_layout.addWidget(&m_buttonOk, 2, 0, 1, 1);
    m_layout.addWidget(&m_buttonCancel, 2, 1, 1, 1);
    this->setLayout(&m_layout);
}