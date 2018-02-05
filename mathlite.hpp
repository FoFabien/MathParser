#ifndef MATHLITE_H
#define MATHLITE_H

#include <vector>
#include <string>

struct MathStack
{
    std::vector<std::string> stack;
    size_t currentPos = 0;
    size_t errCode = 0;
    std::string errStr = "";
};

class MathLite
{
    public:
        MathLite();
        ~MathLite();
        double calc(const std::string &expr, bool &err);
    protected:
        MathStack pushdown(const std::string &expr);
        void deconstructOp(MathStack &obj);
        double doMath(MathStack &obj);
        static std::vector<std::string> split(const std::string &s, char delim);
        static bool is_double(const std::string &s);
        static double runFunc(const std::string& func, double param, size_t &errCode);
};

#endif // MATHLITE_H
