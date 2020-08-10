#ifndef COLOREDSTRING_H
#define COLOREDSTRING_H

#include <QString>
#include <QMap>

class ColoredString
{
public:
    explicit ColoredString(const QString text);
    QString toString() const;

    operator QString() const;

private:
    QString m_text;
    static QMap<QString, QString> map;
};

#endif // COLOREDSTRING_H
