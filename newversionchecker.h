#ifndef NEWVERSIONCHECKER_H
#define NEWVERSIONCHECKER_H

#include <QVersionNumber>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QObject>
#include <QNetworkReply>

class NewVersionChecker
{
public:
    NewVersionChecker();
    bool isNewVersionAvailable();

private:
    QVersionNumber currentVersion();
    QVersionNumber latestVersion();
};

#endif // NEWVERSIONCHECKER_H
