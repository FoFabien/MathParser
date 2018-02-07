#include "mathlite.hpp"
#include <stack>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cmath>

# define M_PIl 3.141592653589793238462643383279502884L /* pi */

const std::vector<char> math_ops = {'^', '*', '/', '+', '-'};
std::map<std::string, size_t> math_funcs = {
    {"cos", 0},
    {"sin", 1},
    {"tan", 2},
    {"acos", 3},
    {"asin", 4},
    {"atan", 5},
    {"atan2", 6},
    {"cosh", 7},
    {"sinh", 8},
    {"tanh", 9},
    {"acosh", 10},
    {"asinh", 11},
    {"atanh", 12},
    {"exp", 13},
    {"frexp", 14},
    {"ldexp", 15},
    {"log", 16},
    {"log10", 17},
    {"modf", 18},
    {"exp2", 19},
    {"expm1", 20},
    {"ilogb", 21},
    {"log1p", 22},
    {"log2", 23},
    {"logb", 24},
    {"scalbn", 25},
    {"scalbln", 26},
    //{"pow", 27},
    {"sqrt", 28},
    {"cbrt", 29},
    {"hypot", 0},
    {"erf", 30},
    {"erfc", 31},
    {"tgamma", 32},
    {"lgamma", 33},
    {"ceil", 34},
    {"floor", 35},
    {"fmod", 36},
    {"trunc", 37},
    {"round", 38},
    {"lround", 39},
    {"llround", 40},
    {"rint", 41},
    {"lrint", 42},
    {"llrint", 43},
    {"nearbyint", 44},
    {"remainder", 45},
    {"remquo", 46},
    {"copysign", 47},
    {"nan", 48},
    {"nextafter", 49},
    {"nexttoward", 50},
    {"fdim", 51},
    {"fmax", 52},
    {"fmin", 53},
    {"fabs", 54},
    {"abs", 55},
    {"fma", 56},
    // custom
    {"cosd", 100},
    {"sind", 101},
    {"tand", 102},
    {"acosd", 103},
    {"acosd", 104},
    {"acosd", 105}

};


MathLite::MathLite()
{
    //ctor
}

MathLite::~MathLite()
{
    //dtor
}

double MathLite::calc(const std::string &expr, bool &err)
{
    MathStack obj = pushdown(expr);
    deconstructOp(obj);
    /*for(auto& i : obj.stack)
        std::cout << i << std::endl;*/
    obj.result = doMath(obj);
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
    if(obj.errCode > 0) err = true;
    else err = false;
    return obj.result;
}

MathStack MathLite::pushdown(const std::string &expr) const
{
    MathStack obj;
    std::stack<size_t> posStack;
    posStack.push(0);
    obj.stack.push_back("");
    for(auto& c : expr)
    {
        if(c == '(')
        {
            obj.stack[obj.currentPos] += "$" + std::to_string(obj.stack.size());
            posStack.push(obj.currentPos);
            obj.currentPos = obj.stack.size();
            obj.stack.push_back("");
        }
        else if(c == ')')
        {
            if(obj.currentPos == 0 || posStack.empty())
            {
                obj.stack[obj.currentPos] += c;
                obj.errCode = 1;
                obj.errStr = expr;
                return obj;
            }
            else
            {
                obj.currentPos = posStack.top();
                posStack.pop();
            }
        }
        else obj.stack[obj.currentPos] += c;
    }
    if(obj.currentPos != 0)
    {
        obj.errCode = 2;
        obj.errStr = expr;
        return obj;
    }
    for(auto& s: obj.stack)
    {
        std::string::iterator end_pos = std::remove(s.begin(), s.end(), ' ');
        s.erase(end_pos, s.end());
    }
    return obj;
}

void MathLite::deconstructOp(MathStack &obj) const
{
    if(obj.errCode > 0) return;
    std::vector<std::vector<char> > op_list = {{'^'}, {'*', '/'}, {'+', '-'}};
    int lastOp;
    size_t nextOp, count;
    bool found;
    for(size_t z = 0; z < obj.stack.size(); ++z)
    {
        count = 0;
        for(auto& c: obj.stack[z])
            for(auto& t: math_ops)
                if(c == t) ++count;
        if(count < 2) continue;

        for(auto& ops: op_list)
        {
            lastOp = -1;
            for(size_t i = 0; i < obj.stack[z].size(); ++i)
            {
                found = false;
                for(auto& t: ops)
                    if(obj.stack[z][i] == t)
                    {
                        found = true;
                        break;
                    }
                if(!found)
                {
                    for(auto& t: math_ops)
                        if(obj.stack[z][i] == t) lastOp = i;
                }
                else
                {
                    found = false;
                    for(nextOp = i+1; nextOp < obj.stack[z].size(); ++nextOp)
                    {
                        for(auto& t: math_ops)
                            if(obj.stack[z][nextOp] == t) found = true;
                        if(found) break;
                    }
                    std::string str = obj.stack[z].substr(lastOp+1, nextOp-lastOp-1);
                    obj.stack[z].replace(lastOp+1, nextOp-lastOp-1, "$" + std::to_string(obj.stack.size()));
                    obj.stack.push_back(str);
                    i = nextOp-1;
                }
            }
        }
        --z;
    }
}

double MathLite::doMath(MathStack &obj) const
{
    if(obj.errCode > 0) return 0;
    size_t pos = obj.currentPos;
    int op_pos = -1;
    bool minus_flag = false;
    std::string &str = obj.stack[pos];
    std::string func = "";
    std::string var = "";
    std::vector<std::string> strs;
    std::vector<double> vs = {0.f};
    if(str.empty()) return 0;
    for(int i = 0; i < (int)str.size(); ++i)
        for(auto& t: math_ops)
            if(str[i] == t) op_pos = i;
    if(op_pos != -1)
    {
        if(op_pos == 0)
        {
            if(str[op_pos] != '-')
            {
                obj.errCode = 3;
                obj.errStr = str;
                return 0;
            }
            minus_flag = true;
            str.erase(str.begin());
            op_pos = -1;
            strs = {str};
        }
        else
        {
            std::string A = str.substr(0, op_pos);
            std::string B = str.substr(op_pos+1, str.size()-op_pos-1);
            strs = {A, B};
            vs = {0.f, 0.f};
        }
    }
    else strs = {str};

    for(size_t i = 0; i < strs.size(); ++i)
    {
        if(strs[i].find("$") != std::string::npos)
        {
            std::vector<std::string> tokens = split(strs[i], '$');
            switch(tokens.size())
            {
                case 2:
                    func = tokens[0];
                    var = tokens[1];
                    break;
                case 1:
                    func = "";
                    var = tokens[0];
                    break;
                default:
                    obj.errCode = 3;
                    obj.errStr = strs[i];
                    return 0;
            }
            obj.currentPos = std::stoi(var);
            vs[i] = doMath(obj);
            if(obj.errCode > 0) return 0;
            vs[i] = runFunc(func, vs[i], obj.errCode);
            if(obj.errCode > 0)
            {
                obj.errStr = func;
                return 0;
            }
        }
        else
        {
            if(strs[i] == "pi")
                vs[i] = M_PIl;
            else if(isDouble(strs[i]))
                vs[i] = std::stod(strs[i]);
            else if(isVariable(strs[i]))
                vs[i] = varCache.at(strs[i]).result;
        }
    }
    if(minus_flag)
    {
        vs[0] = - vs[0];
    }
    if(op_pos != -1)
    {
        switch(str[op_pos])
        {
            case '^': return pow(vs[0], vs[1]);
            case '*': return vs[0]*vs[1];
            case '/': return vs[0]/vs[1];
            case '+': return vs[0]+vs[1];
            case '-': return vs[0]-vs[1];
        }
    }
    else return vs[0];
    obj.errCode = 3;
    obj.errStr = str;
    return 0;
}

void MathLite::setCache(const std::string &name, const MathStack &obj)
{
    if(obj.errCode == 0)
        varCache[name] = obj;
}

MathStack MathLite::getCache(const std::string &name) const
{
    std::map<std::string, MathStack>::const_iterator it = varCache.find(name);
    if(it != varCache.end())
        return it->second;
    MathStack obj;
    obj.errCode = 5;
    return obj;
}

void MathLite::clearCache()
{
    varCache.clear();
}

void MathLite::printCache() const
{
    for(std::map<std::string, MathStack>::const_iterator it = varCache.begin(); it != varCache.end(); ++it)
    {
        std::cout << it->first << " = ";
        if(it->second.errCode == 0) std::cout << it->second.result << std::endl;
        else std::cout << "error" << std::endl;
    }
}

std::map<std::string, MathStack> MathLite::exportCache() const
{
    return varCache;
}

void MathLite::importCache(const std::map<std::string, MathStack>& cache)
{
    varCache = cache;
}

bool MathLite::isValid(const std::string &varName) const
{
    if(varName == "pi") return false;
    if(math_funcs.find(varName) != math_funcs.end()) return false;
    return true;
}

bool MathLite::isVariable(const std::string &name) const
{
    if(varCache.find(name) != varCache.end()) return true;
    return false;
}

std::vector<std::string> MathLite::split(const std::string &s, char delim)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim))
        elems.push_back(std::move(item));
    return elems;
}

bool MathLite::isDouble(const std::string &s)
{
    if(s.empty()) return false;
    std::string::const_iterator it = s.begin();
    bool dot = false;
    while(it != s.end())
    {
        if(!dot && (*it == ',' || *it == '.')) dot = true;
        else if(!std::isdigit(*it)) break;
        ++it;
    }
    return (!s.empty() && it == s.end());
}

double MathLite::runFunc(const std::string& func, double param, size_t &errCode)
{
    if(func.empty()) return param;
    if(math_funcs.find(func) == math_funcs.end())
    {
        errCode = 4;
        return 0;
    }
    switch(math_funcs[func])
    {
        case 0: return cos(param);
        case 1: return sin(param);
        case 2: return tan(param);
        case 3: return acos(param);
        case 4: return asin(param);
        case 5: return atan(param);
        //case 6: return atan2(param, );
        case 7: return cosh(param);
        case 8: return sinh(param);
        case 9: return tanh(param);
        case 10: return acosh(param);
        case 11: return asinh(param);
        case 12: return atanh(param);
        case 13: return exp(param);

        case 16: return log(param);

        case 28: return sqrt(param);

        // custom
        case 100: return cos(param*M_PIl/180.f);
        case 101: return sin(param*M_PIl/180.f);
        case 102: return tan(param*M_PIl/180.f);
        case 103: return acos(param*M_PIl/180.f);
        case 104: return asin(param*M_PIl/180.f);
        case 105: return atan(param*M_PIl/180.f);
        default: errCode = 4; return param;
    }

    /*{"frexp", 14},
    {"ldexp", 15},
    ////{"log", 16},
    {"log10", 17},
    {"modf", 18},
    {"exp2", 19},
    {"expm1", 20},
    {"ilogb", 21},
    {"log1p", 22},
    {"log2", 23},
    {"logb", 24},
    {"scalbn", 25},
    {"scalbln", 26},
    //{"pow", 27},
    ////{"sqrt", 28},
    {"cbrt", 29},
    {"hypot", 0},
    {"erf", 30},
    {"erfc", 31},
    {"tgamma", 32},
    {"lgamma", 33},
    {"ceil", 34},
    {"floor", 35},
    {"fmod", 36},
    {"trunc", 37},
    {"round", 38},
    {"lround", 39},
    {"llround", 40},
    {"rint", 41},
    {"lrint", 42},
    {"llrint", 43},
    {"nearbyint", 44},
    {"remainder", 45},
    {"remquo", 46},
    {"copysign", 47},
    {"nan", 48},
    {"nextafter", 49},
    {"nexttoward", 50},
    {"fdim", 51},
    {"fmax", 52},
    {"fmin", 53},
    {"fabs", 54},
    {"abs", 55},
    {"fma", 56}*/
}
