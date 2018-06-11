#include <string>
#include <fstream>

#include <QString>
#include <QBuffer>
#include <QImage>

#include "mail.hpp"
#include "../dependencies/smtpclient/src/SmtpMime"

MailClient::MailClient(const std::string& credentials)
{
    std::ifstream stream(credentials);
    std::vector<std::string> strings;
    std::string string;
    while (std::getline(stream, string))
    {
        strings.push_back(string);
    }

    m_client = new SmtpClient(QString::fromStdString(strings[0]), 465, SmtpClient::SslConnection);
    m_client->setUser(QString::fromStdString(strings[1]));
    m_client->setPassword(QString::fromStdString(strings[2]));

    m_sender = new EmailAddress(QString::fromStdString(strings[1]));

    m_subject = QString::fromStdString(strings[3]);
    m_message = QString::fromStdString(strings[4]).replace("\\n", "\n");

    m_defaultRecipient = QString(strings.size() > 5 ? QString::fromStdString(strings[5]) : "");
}

MailClient::MailClient(const QString& server, const QString& user, const QString& password, const QString& defaultRecipient)
    : m_sender{ new EmailAddress(user) }
    , m_client{ new SmtpClient(server, 465, SmtpClient::SslConnection) }
    , m_defaultRecipient{ QString(defaultRecipient) }
{
    m_client->setUser(user);
    m_client->setPassword(password);
}

MailClient::~MailClient()
{
    delete m_client;
}

void MailClient::sendImage(const QString& recipient, const QImage& image)
{
    MimeMessage mail;

    mail.setSender(m_sender);
    mail.addRecipient(new EmailAddress(recipient.toLower()));
    mail.addRecipient(m_sender, MimeMessage::Bcc);
    mail.setSubject(m_subject);

    MimeText text;
    text.setText(m_message);
    mail.addPart(&text);

    image.save("SnailSnap.png");
    auto file = new QFile("SnailSnap.png");

    mail.addPart(new MimeAttachment(file));

    m_client->connectToHost();
    m_client->login();
    m_client->sendMail(mail);
    m_client->quit();

    file->remove();
    delete file;
}
