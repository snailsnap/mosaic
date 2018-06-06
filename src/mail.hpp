#pragma once

#include <string>

#include <QString>

#include "../dependencies/smtpclient/src/SmtpMime"

class MailClient
{
private:
    EmailAddress* m_sender;
    QString* m_defaultRecipient;
    SmtpClient* m_client;
public:
    MailClient(const std::string& credentials);
    MailClient(const QString& server, const QString& user, const QString& password, const QString& defaultRecipient = "");
    ~MailClient();
    void sendImage(const QString& recipient, const QString& subject, const QString& message, const QImage& image);
    void sendImageToDefaultRecipient(const QString& subject, const QString& message, const QImage& image)
    {
        sendImage(*m_defaultRecipient, subject, message, image);
    };
};
