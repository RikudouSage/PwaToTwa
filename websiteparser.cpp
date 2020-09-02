#include "websiteparser.h"

WebsiteParser::WebsiteParser(QString url, QString manifestPath)
{
    m_url.setUrl(url);
    m_manifestPath = manifestPath;
}

WebsiteParser::WebsiteParser(QString url, QFile &manifestFile)
{
    m_url.setUrl(url);
    m_manifestFile = &manifestFile;
}

WebsiteParser::~WebsiteParser()
{
    if (m_manifestFile != nullptr) {
        delete m_manifestFile;
    }
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
    QString manifestUrl;

    if(!m_manifestPath.isNull() && !m_manifestPath.isEmpty()) {
        manifestUrl = m_manifestPath;
    } else {
        QRegularExpression regex("(<link[^<>]*?rel=['\"]manifest['\"]).*?>");
        auto matches = regex.match(getWebsiteContent());
        if(!matches.hasMatch()) {
            throw QString("Could not detect the manifest automatically, try using --manifest option");
        }

        QString linkTag = matches.captured(0);

        QRegularExpression regexManifest("href=[\"'](.+?)[\"']");
        auto matchesManifest = regexManifest.match(linkTag);
        if(!matchesManifest.hasMatch()) {
            throw QString("Could not find href inside the manifest link");
        }

        manifestUrl = matchesManifest.captured(1);
    }

    return getUrl(manifestUrl);
}

const QJsonDocument WebsiteParser::getManifestContent()
{
    if (m_manifestContent.isNull() || m_manifestContent.isEmpty()) {
        if (m_manifestFile == nullptr) {
            m_manifestContent = getDownloadedManifestContent();
        } else {
            m_manifestContent = getLocalManifestContent();
        }
    }

    return QJsonDocument::fromJson(m_manifestContent.toUtf8());
}

const QString WebsiteParser::getDownloadedManifestContent()
{
    QUrl manifestUrl = getManifestUrl();

    QNetworkAccessManager networkManager;
    QNetworkRequest request(manifestUrl);
    QEventLoop loop;

    QObject::connect(&networkManager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    auto reply = networkManager.get(request);

    loop.exec();

    QString response = reply->readAll();

    delete reply;

    return response.toUtf8();
}

const QString WebsiteParser::getLocalManifestContent()
{
    if (!m_manifestFile->open(QIODevice::ReadOnly)) {
        throw QString("Could not open local manifest file for writing");
    }
    auto result = m_manifestFile->readAll();
    m_manifestFile->close();

    return result;
}

const QHash<QString, QString> WebsiteParser::getData()
{
    QHash<QString, QString> result;

    auto json = getManifestContent().object();

    QStringList missingKeys;

    if (!json.contains("short_name")) {
        missingKeys << "short_name";
    }

    if (!json.contains("theme_color")) {
        missingKeys << "theme_color";
    }

    if (!json.contains("background_color")) {
        missingKeys << "background_color";
    }

    if (!json.contains("start_url")) {
        missingKeys << "start_url";
    }

    if(!missingKeys.empty()) {
        throw QString("The manifest at url '" + getManifestUrl() + "' is missing these keys: " + missingKeys.join(", "));
    }

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

    if (json.empty()) {
        throw QString("The manifest at url '" + getManifestUrl() + "' does not contain any icons");
    }

    QJsonArray::const_iterator iterator;

    for(iterator = json.constBegin(); iterator != json.constEnd(); ++iterator) {
        auto icon = (*iterator).toObject();

        auto sizes = icon.take("sizes").toString().split("x");
        auto url = icon.take("src").toString();

        if(sizes.length() != 2 || sizes.at(0) != sizes.at(1)) {
            qWarning() << "[Warning] The icon with url '" + url + "' does not have a valid size attribute, ignoring";
            continue;
        }

        QHash<QString, QString> iconHash;
        iconHash.insert("size", sizes.at(0));
        iconHash.insert("url", getUrl(url));
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
    } else {
        tmp = url;
    }

    QString baseUrl = m_url.toString();
    if(baseUrl.endsWith("/")) {
        baseUrl = baseUrl.left(baseUrl.length() - 1);
    }

    return baseUrl + tmp;
}
