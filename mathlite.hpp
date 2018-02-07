#ifndef MATHLITE_H
#define MATHLITE_H

#include <vector>
#include <map>
#include <string>

struct MathStack
{
    std::vector<std::string> stack;
    size_t currentPos = 0;
    size_t errCode = 0;
    std::string errStr = "";
    double result;
};

class MathLite
{
    public:
        MathLite();
        ~MathLite();

        // parsing
        double calc(const std::string &expr, bool &err); // take a math expression and return the result. set the err flag if an error happens.
        MathStack pushdown(const std::string &expr) const; // construct a stack from the expression
        void deconstructOp(MathStack &obj) const; // parse the operators
        double doMath(MathStack &obj) const; // calculate the result

        // variable
        void setCache(const std::string &name, const MathStack &obj);
        MathStack getCache(const std::string &name) const;
        void clearCache();
        void printCache() const;
        std::map<std::string, MathStack> exportCache() const;
        void importCache(const std::map<std::string, MathStack>& cache);

        // misc
        bool isValid(const std::string &varName) const;
        bool isVariable(const std::string &name) const;
        static std::vector<std::string> split(const std::string &s, char delim);
        static bool isDouble(const std::string &s);
        static double runFunc(const std::string& func, double param, size_t &errCode);
    protected:
        std::map<std::string, MathStack> varCache;
};

#endif // MATHLITE_H
