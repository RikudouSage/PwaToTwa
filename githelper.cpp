#include "githelper.h"

#include <QDebug>

GitHelper::GitHelper()
{

}

void GitHelper::clone(QString directory)
{
    clone(QDir(directory));
}

void GitHelper::clone(QDir directory)
{
    directory = QDir(directory);
    if (directory.exists()) {
        throw QString("The directory already exists");
    }

    QProcess process;
    process.start("git", QStringList() << "clone" << URL << directory.absolutePath());
    process.waitForFinished();
}

void GitHelper::checkout(QString directory)
{
    checkout(QDir(directory));
}

void GitHelper::checkout(QDir directory)
{
    if(!directory.exists()) {
        throw QString("The directory does not exist");
    }

    QProcess process;
    process.setWorkingDirectory(directory.absolutePath());
    process.start("git", QStringList() << "checkout" << COMMIT);
    process.waitForFinished();
}

void GitHelper::reinitGitDirectory(QString directory)
{
    reinitGitDirectory(QDir(directory));
}

void GitHelper::reinitGitDirectory(QDir directory)
{
    if(!directory.exists()) {
        throw QString("The directory does not exist");
    }

    QDir gitDir = directory.filePath(".git");
    if(gitDir.exists()) {
        gitDir.removeRecursively();

        QProcess git;
        git.setWorkingDirectory(directory.absolutePath());
        git.start("git", QStringList() << "init");
        git.waitForFinished();
    }
}

void GitHelper::initialCommit(QString directory)
{
    initialCommit(QDir(directory));
}

void GitHelper::initialCommit(QDir directory)
{
    if(!directory.exists()) {
        throw QString("The directory does not exist");
    }

    QProcess git;
    git.setWorkingDirectory(directory.absolutePath());

    git.start("git", QStringList() << "add" << ".");
    git.waitForFinished();

    git.start("git", QStringList() << "commit" << "-am" << "initial commit");
    git.waitForFinished();
}
