#pragma once

#include <QString>

#include "../dependencies/smtpclient/src/SmtpMime"

class MailClient
{
public:
    MailClient(const QString& server, const QString& user, const QString& password, int port = 465, SmtpClient::ConnectionType connectionType = SmtpClient::SslConnection);
    ~MailClient();
    void sendText(const QString& recipient, const QString& subject, const QString& message);
private:
    EmailAddress* m_sender;
    SmtpClient* m_client;
};