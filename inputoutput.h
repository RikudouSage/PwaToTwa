#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include <QString>
#include <iostream>

class InputOutput
{
public:
    InputOutput();

    void write(QString text);
    void writeln(QString text);
    QString readStdin();
    QString ask(QString question);
};

#endif // INPUTOUTPUT_H
