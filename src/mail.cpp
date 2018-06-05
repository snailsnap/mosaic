#include <QString>
#include <QBuffer>
#include <QImage>

#include "mail.hpp"
#include "../dependencies/smtpclient/src/SmtpMime"

MailClient::MailClient(const QString& server, const QString& user, const QString& password, int port, SmtpClient::ConnectionType connectionType)
    : m_sender{ new EmailAddress(user) }
    , m_client{ new SmtpClient(server, port, connectionType) }
{
    m_client->setUser(user);
    m_client->setPassword(password);
}

MailClient::~MailClient()
{
    delete m_client;
}

void MailClient::sendImage(const QString& recipient, const QString& subject, const QString& message, const QImage& image)
{
    MimeMessage mail;

    mail.setSender(m_sender);
    mail.addRecipient(new EmailAddress(recipient));
    mail.addRecipient(m_sender, MimeMessage::Bcc);
    mail.setSubject(subject);

    MimeText text;
    text.setText(message);
    mail.addPart(&text);

    //Convert QImage to QFile with QByteArray
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QFile * file = new QFile("SnailSnap.png");
    if (file->open(QIODevice::Truncate | QIODevice::WriteOnly)) {
        file->write(ba);
        file->close();
    }

    mail.addPart(new MimeAttachment(file));

    m_client->connectToHost();
    m_client->login();
    m_client->sendMail(mail);
    m_client->quit();
}
