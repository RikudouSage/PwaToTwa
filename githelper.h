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
    const QString COMMIT = "a6ccc67577f335bda13532257138ef68cac901d8";

    void gitError(int status);
};

#endif // GITHELPER_H
