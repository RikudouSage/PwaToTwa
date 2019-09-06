#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include <QString>
#include <iostream>

class InputOutput
{
public:
    InputOutput();

    void write(QString text);
    void writeError(QString text);
    void writeln(QString text);
};

#endif // INPUTOUTPUT_H
