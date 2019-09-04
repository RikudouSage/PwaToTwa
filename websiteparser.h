#ifndef WEBSITEPARSER_H
#define WEBSITEPARSER_H

#include <memory>
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

    const QHash<QString, QString> getData();
    const QList<QHash<QString, QString>> getImages();

private:
    QString getUrl(const QString url);
    const QString getWebsiteContent();
    const QString getManifestUrl();
    const QJsonDocument getManifestContent();

    QString m_websiteContent;
    QString m_manifestContent;
    QString m_manifestPath;
    QUrl m_url;
};

#endif // WEBSITEPARSER_H
