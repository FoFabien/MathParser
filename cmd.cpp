#include "cmd.hpp"
#include <iostream>
#include <string>
#include "mathlite.hpp"

CMD::CMD()
{
    state = 0;
}

CMD::~CMD()
{
    //dtor
}

void CMD::run()
{
    std::string buffer;
    while(state != -1)
    {
        switch(state)
        {
            case 0:
                std::cout << "Hello World" << std::endl;
                std::cout << "type '?' or 'help' for help" << std::endl;
                state = 1;
                break;
            case 1:
                std::cout << ">";
                std::getline(std::cin, buffer);
                if(buffer == "help" || buffer == "?")
                    state = 2;
                else if(buffer == "exit" || buffer == "quit")
                    state = -1;
                else state = 3;
                break;
            case 2:
                std::cout << "type a mathematical expression to get the result" << std::endl;
                std::cout << "type 'exit' or 'quit' to stop" << std::endl;
                std::cout << "type '?' or 'help' again to show this message" << std::endl;
                state = 1;
                break;
            case 3:
            {
                MathLite m;
                bool err;
                double r = m.calc(buffer, err);
                if(err)
                    std::cout << "An error has been returned" << std::endl;
                else std::cout << "= " << r << std::endl;
                state = 1;
                break;
            }
        }
    }
}
