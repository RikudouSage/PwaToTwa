#include "inputoutput.h"

InputOutput::InputOutput()
{

}

void InputOutput::write(QString text)
{
    std::cout << text.toStdString();
}

void InputOutput::writeln(QString text)
{
    write(text + "\n");
}
