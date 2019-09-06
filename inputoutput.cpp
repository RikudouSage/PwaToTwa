#include "inputoutput.h"

InputOutput::InputOutput()
{

}

void InputOutput::write(QString text)
{
    std::cout << text.toStdString();
}

void InputOutput::writeError(QString text)
{
    qWarning("%s", text.toStdString().c_str());
}

void InputOutput::writeln(QString text)
{
    write(text + "\n");
}
