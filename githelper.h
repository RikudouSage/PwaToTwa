#ifndef GITHELPER_H
#define GITHELPER_H

#include <QString>
#include <QDir>

#include <git2.h>

class GitHelper
{
public:
    GitHelper();
    ~GitHelper();

    void clone(QString directory);
    void clone(QDir directory);

    void checkout(QString directory);
    void checkout(QDir directory);

    void reinitGitDirectory(QString directory);
    void reinitGitDirectory(QDir directory);

    void initialCommit(QString directory);
    void initialCommit(QDir directory);

private:
    const QString URL = "https://github.com/GoogleChromeLabs/svgomg-twa";
    const QString COMMIT = "55a8c47056085638e99de307b66e0df9bb81bccf";

    void gitError(int status);
};

#endif // GITHELPER_H
