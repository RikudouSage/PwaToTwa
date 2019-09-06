#ifndef ANDROIDPROJECTMODIFIER_H
#define ANDROIDPROJECTMODIFIER_H

#include <QString>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QHash>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QStandardPaths>
#include <QProcess>

class AndroidProjectModifier
{
public:
    AndroidProjectModifier(QString path);
    void addSupportLibrary();
    void setBasicData(QHash<QString, QString> data);
    void addImages(QList<QHash<QString, QString>> images);

private:
    QString getFileContent(const QString filepath);
    void updateFileContent(const QString filepath, const QString content);
    QString downloadImage(const QString url);
    QString randomString(int length = 15);
    QString resizeImage(const QHash<QString, QString> imageData, QString size);
    void addIcons(QHash<QString, QString> image);
    void addSplash(QHash<QString, QString> image);

    QDir m_directory;
};

#endif // ANDROIDPROJECTMODIFIER_H
