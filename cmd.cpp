#include "cmd.hpp"
#include <iostream>
#include <string>
#include "mathlite.hpp"

/*******************************************************************
Command Prompt to test the MathLite class.
type 'help' for the command list.
*******************************************************************/

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
    MathLite m;
    bool err;
    double r;
    std::vector<std::string> elems;

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
                else if(buffer == "clear")
                    state = 5;
                else if(buffer == "show")
                    state = 6;
                else state = 3;
                break;
            case 2:
                std::cout << "type a mathematical expression to get the result" << std::endl;
                std::cout << "type 'exit' or 'quit' to stop" << std::endl;
                std::cout << "type '?' or 'help' again to show this message" << std::endl;
                std::cout << "type 'var = expr' to assign an expression to a variable name" << std::endl;
                std::cout << "type 'clear' to clear the variable cache" << std::endl;
                std::cout << "type 'show' to print the variable cache content on screen" << std::endl;
                state = 1;
                break;
            case 3:
                elems = MathLite::split(buffer, '=');
                if(elems.size() > 2)
                {
                    std::cout << "invalid expression, multiple '='" << std::endl;
                    state = 1;
                }
                else if(elems.size() == 2)
                {
                    while(!elems[0].empty() && elems[0].back() == ' ')
                        elems[0].erase(elems[0].end());
                    while(!elems[0].empty() && elems[0].front() == ' ')
                        elems[0].erase(elems[0].begin());
                    if(elems[0].empty() || elems[0].find(' ') != std::string::npos)
                    {
                        std::cout << "Invalid variable name" << std::endl;
                        state = 1;
                    }
                    else
                    {
                        MathStack obj = m.pushdown(elems[1]);
                        m.deconstructOp(obj);
                        obj.result = m.doMath(obj);
                        switch(obj.errCode)
                        {
                            case 0:
                                break;
                            case 1:
                                std::cout << "unexpected bracket ) at '" << obj.errStr << "'" << std::endl;
                                break;
                            case 2:
                                std::cout << "a bracket ) is missing at the end '" << obj.errStr << "'" << std::endl;
                                break;
                            case 3:
                                std::cout << "invalid expression '" << obj.errStr << "'" << std::endl;
                                break;
                            case 4:
                                std::cout << "unknown or invalid function '" << obj.errStr << "'" << std::endl;
                                break;
                            default:
                                std::cout << "undefined error (code " << obj.errCode << ")" << std::endl;
                                break;
                        }
                        r = obj.result;
                        err = (obj.errCode > 0);
                        if(!err)
                        {
                            m.setCache(elems[0], obj);
                            std::cout << elems[0] << " ";
                        }
                        state = 4;
                    }
                }
                else
                {
                    if(buffer.empty())
                    {
                        state = 1;
                    }
                    else
                    {
                        r = m.calc(buffer, err);
                        state = 4;
                    }
                }
                break;
            case 4:
                if(err)
                    std::cout << "An error has occurred" << std::endl;
                else std::cout << "= " << r << std::endl;
                state = 1;
                break;
            case 5:
                m.clearCache();
                std::cout << "Cache cleared" << std::endl;
                state = 1;
                break;
            case 6:
                m.printCache();
                state = 1;
                break;
        }
    }
}
