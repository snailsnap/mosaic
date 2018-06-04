#include <QString>

#include "mail.hpp"
#include "../dependencies/smtpclient/src/SmtpMime"

MailClient::MailClient(const QString& server, const QString& user, const QString& password, int port, SmtpClient::ConnectionType connectionType)
{
    m_sender = new EmailAddress(user);

    m_client = new SmtpClient(server, port, connectionType);

    m_client->setUser(user);
    m_client->setPassword(password);
}

MailClient::~MailClient()
{
    delete m_client;
}

void MailClient::sendText(const QString& recipient, const QString& subject, const QString& message)
{
    MimeMessage mail;

    mail.setSender(m_sender);
    mail.addRecipient(new EmailAddress(recipient));
    mail.setSubject(subject);

    MimeText text;

    text.setText(message);

    mail.addPart(&text);

    m_client->connectToHost();
    m_client->login();
    m_client->sendMail(mail);
    m_client->quit();
}
