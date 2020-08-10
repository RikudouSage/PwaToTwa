#include "coloredstring.h"

QMap<QString, QString> ColoredString::map = QMap<QString, QString>({
    {"green", "32"}
});

ColoredString::ColoredString(const QString text)
{
    m_text = text;
}

QString ColoredString::toString() const
{
    enum Mode { // todo come up with better names
        String,
        Tag,
        TagString,
        TagTag
    };
    QString out;

    auto mode = Mode::String;
    QString currentTag;

    for (int i = 0; i < m_text.length(); ++i) {
        auto letter = m_text.at(i);
        switch (mode) {
        case String:
            if (letter == "<") {
                mode = Tag;
                continue;
            }
            out += letter;
            break;
        case Tag:
            if (letter == ">") {
                if (!map.contains(currentTag)) {
                    mode = String;
                    continue;
                }
                out += "\033[0;" + map.value(currentTag) + "m";
                mode = TagString;
            } else {
                currentTag += letter;
            }
            break;
        case TagString:
            if (letter == "<") {
                mode = TagTag;
                continue;
            }
            out += letter;
            break;
        case TagTag:
            if (letter != ">") {
                continue;
            }
            out += "\033[0m";
            currentTag = "";
            mode = String;
            break;
        }
    }

    return out;
}

ColoredString::operator QString() const
{
    return toString();
}
