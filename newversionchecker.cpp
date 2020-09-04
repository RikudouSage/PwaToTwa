#include "newversionchecker.h"

NewVersionChecker::NewVersionChecker()
{

}

bool NewVersionChecker::isNewVersionAvailable()
{
    return latestVersion() > currentVersion();
}

QVersionNumber NewVersionChecker::currentVersion()
{
    return QVersionNumber::fromString("0.0.14");
}

QVersionNumber NewVersionChecker::latestVersion()
{
    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("https://github.com/RikudouSage/PwaToTwa/releases/latest"));
    QEventLoop loop;

    QObject::connect(&manager, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

    auto reply = manager.get(request);
    loop.exec();

    auto locationHeader = reply->header(QNetworkRequest::KnownHeaders::LocationHeader);
    delete reply;

    return QVersionNumber::fromString(locationHeader.toUrl().path().toUtf8().split('/').last().replace('v', ""));
}
