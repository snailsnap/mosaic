#pragma once

#include <string>
#include <memory>

#include <QString>

#include "../dependencies/smtpclient/src/SmtpMime"

class MailClient
{
    std::unique_ptr<SmtpClient> m_client;
    QString m_sender;
    QString m_defaultRecipient;
    QString m_subject;
    QString m_message;
public:
    MailClient() = delete;
    MailClient(const MailClient&) = delete;
    MailClient(MailClient&& rhs);
    MailClient(const QString& server, const QString& user, const QString& password, const QString& defaultRecipient = "");
    ~MailClient();

    static MailClient fromCredentials(const std::string& credentials);

    void setContents(const QString& subject, const QString& message);
    void sendImage(const QString& recipient, const QImage& image);
    void sendImageToDefaultRecipient(const QImage& image)
    {
        sendImage(m_defaultRecipient, image);
    };
};
