#ifndef WEBSITEPARSER_H
#define WEBSITEPARSER_H

#include <memory>
#include <QFile>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QEventLoop>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class WebsiteParser
{
public:
    WebsiteParser(QString url, QString manifestPath);
    WebsiteParser(QString url, QFile &manifestFile);
    ~WebsiteParser();

    const QHash<QString, QString> getData();
    const QList<QHash<QString, QString>> getImages();

private:
    QString getUrl(const QString url);
    const QString getWebsiteContent();
    const QString getManifestUrl();
    const QJsonDocument getManifestContent();
    const QString getDownloadedManifestContent();
    const QString getLocalManifestContent();

    QString m_websiteContent;
    QString m_manifestContent;
    QString m_manifestPath;
    QUrl m_url;
    QFile *m_manifestFile = nullptr;
};

#endif // WEBSITEPARSER_H
