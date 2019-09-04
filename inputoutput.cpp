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

/*QString InputOutput::readStdin()
{
    std::string result;
    std::cin >> result;

    return QString::fromStdString(result);
}

QString InputOutput::ask(QString question)
{
    write(question + ": ");
    return readStdin();
}
*/
