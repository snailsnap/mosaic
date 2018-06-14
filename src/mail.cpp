#include <string>
#include <fstream>
#include <iostream>

#include <QString>
#include <QBuffer>
#include <QImage>
#include <QFile>

#include "mail.hpp"
#include "../dependencies/smtpclient/src/SmtpMime"

MailClient MailClient::fromCredentials(const std::string& credentials)
{
    if (!QFile(QString::fromStdString(credentials)).exists())
    {
        std::cerr << "Could not open credentials file. Mailing the result image will not work. Please provide credentials here: " << credentials << std::endl;
        throw std::runtime_error { "could not open credentials" };
    }

    std::ifstream stream(credentials);
    std::vector<QString> strings;
    std::string string;
    while (std::getline(stream, string))
    {
        strings.push_back(QString::fromStdString(string));
    }

    MailClient client { strings[0], strings[1], strings[2], strings.size() > 5 ? strings[5] : "" };
    client.setContents(strings[3], strings[4].replace("\\n", "\n"));
    return client;
}

void MailClient::setContents(const QString &subject, const QString &message) {
    m_subject = subject;
    m_message = message;
}

MailClient::MailClient(const QString& server, const QString& user, const QString& password, const QString& defaultRecipient)
    : m_sender{ user }
    , m_client{ std::make_unique<SmtpClient>(server, 465, SmtpClient::SslConnection) }
    , m_defaultRecipient{ defaultRecipient }
{
    m_client->setUser(user);
    m_client->setPassword(password);
}

MailClient::MailClient(MailClient&& rhs)
    : m_sender{ std::move(rhs.m_sender) }
    , m_client{ std::move(rhs.m_client) }
    , m_defaultRecipient{ rhs.m_defaultRecipient }
    , m_subject{ rhs.m_subject }
    , m_message{ rhs.m_message } {
}

MailClient::~MailClient() = default;

void MailClient::sendImage(const QString& recipient, const QImage& image)
{
    if (!m_setup)
    {
        std::cerr << "Mail client isn't set up, problably because the credentials are missing. Could not send image." << std::endl;
        return;
    }

    MimeMessage mail;

    mail.setSender(new EmailAddress(m_sender));
    mail.addRecipient(new EmailAddress(recipient.toLower()));
    mail.addRecipient(new EmailAddress(m_sender), MimeMessage::Bcc);
    mail.setSubject(m_subject);

    MimeText text;
    text.setText(m_message);
    mail.addPart(&text);

    image.save("SnailSnap.png");
    auto file = new QFile { "SnailSnap.png" };

    mail.addPart(new MimeAttachment(file));

    m_client->connectToHost();
    m_client->login();
    m_client->sendMail(mail);
    m_client->quit();

    file->remove();
}
