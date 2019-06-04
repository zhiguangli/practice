#include <QCoreApplication>
#include "calculator.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    try
    {
        Calculator calculator;
        calculator.calculate("8+(6-3)*(-5)+10/2");
        calculator.output();
    }
    catch(const char *e)
    {
        std::cerr << e << std::endl;
    }


    return a.exec();
}
