#include "websiteparser.h"

WebsiteParser::WebsiteParser(QString url)
{
    m_url.setUrl(url);
}

const QString WebsiteParser::getWebsiteContent()
{
    if(!m_websiteContent.isNull() && !m_websiteContent.isEmpty()) {
        return m_websiteContent;
    }

    QNetworkAccessManager networkManager;
    QNetworkRequest request(m_url);
    QEventLoop loop;

    QObject::connect(&networkManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    auto reply = networkManager.get(request);

    loop.exec();

    QString response = reply->readAll();

    delete reply;

    m_websiteContent = response;

    return response;
}

const QString WebsiteParser::getManifestUrl()
{
    QRegularExpression regex("(<link.*?rel=['\"]manifest['\"]).*?>");
    auto matches = regex.match(getWebsiteContent());
    if(!matches.hasMatch()) {
        throw QString("The website does not contain a manifest");
    }

    QString linkTag = matches.captured(0);

    QRegularExpression regexManifest("href=[\"'](.+?)[\"']");
    auto matchesManifest = regexManifest.match(linkTag);
    if(!matchesManifest.hasMatch()) {
        throw QString("Could not find href inside the manifest link");
    }

    return getUrl(matchesManifest.captured(1));
}

const QJsonDocument WebsiteParser::getManifestContent()
{
    if(m_manifestContent.isNull() || m_manifestContent.isEmpty()) {
        QUrl manifestUrl = getManifestUrl();

        QNetworkAccessManager networkManager;
        QNetworkRequest request(manifestUrl);
        QEventLoop loop;

        QObject::connect(&networkManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

        auto reply = networkManager.get(request);

        loop.exec();

        QString response = reply->readAll();

        delete reply;

        m_manifestContent = response;
    }

    return QJsonDocument::fromJson(m_manifestContent.toUtf8());
}

const QHash<QString, QString> WebsiteParser::getData()
{
    QHash<QString, QString> result;

    auto json = getManifestContent().object();

    result.insert("hostname", m_url.host());
    result.insert("short_name", json.take("short_name").toString());
    result.insert("theme_color", json.take("theme_color").toString());
    result.insert("background_color", json.take("background_color").toString());
    result.insert("start_url", json.take("start_url").toString());

    return result;
}

const QList<QHash<QString, QString>> WebsiteParser::getImages()
{
    QList<QHash<QString, QString>> result;

    auto json = getManifestContent().object().take("icons").toArray();

    QJsonArray::const_iterator iterator;

    for(iterator = json.constBegin(); iterator != json.constEnd(); ++iterator) {
        auto icon = (*iterator).toObject();

        auto sizes = icon.take("sizes").toString().split("x");

        if(sizes.length() != 2 || sizes.at(0) != sizes.at(1)) {
            throw QString("Invalid size attribute for icon");
        }

        QHash<QString, QString> iconHash;
        iconHash.insert("size", sizes.at(0));
        iconHash.insert("url", getUrl(icon.take("src").toString()));
        result.append(iconHash);
    }


    return result;
}

QString WebsiteParser::getUrl(const QString url)
{
    QString tmp;
    if(url.startsWith("http")) {
        return url;
    }

    if(url.startsWith("//")) {
        return m_url.scheme() + url;
    }

    if(!url.startsWith("/")) {
        tmp = "/" + url;
    }

    QString baseUrl = m_url.toString();
    if(baseUrl.endsWith("/")) {
        baseUrl = baseUrl.left(baseUrl.length() - 1);
    }

    return baseUrl + tmp;
}
