//
// Created by Glenn Jacob on 29/03/16.
//

#ifndef MUSICBOX_LOGGER_H
#define MUSICBOX_LOGGER_H

#include <string>
#include <ostream>
#include <iostream>
#include <iomanip>
#include "Utils.h"

using std::string;
using std::ostream;
using std::cout;
using std::cerr;
using std::endl;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {
class Logger
{
public:
    template<typename ... TmsgParts>
    static void logMsg(const string& functionName, const TmsgParts& ... msgParts)
    {
        logMsgBeginning(functionName);
        Logger::logMsgParts(cout, msgParts...);
        cout << endl;
    }

    template<typename ... TmsgParts>
    static void logError(const string& functionName, const TmsgParts& ... msgParts)
    {
        logMsgBeginning(functionName);
        logMsgParts(cerr, msgParts...);
        cerr << endl;
    }

private:
    static void logTime(ostream& os);

    template<typename Tfirst,typename ... TmsgParts>
    static void logMsgParts(std::ostream& os, const Tfirst& firstPart, const TmsgParts& ... msgParts) {
        os << firstPart;
        logMsgParts(os, msgParts...);
    }

    template<typename TmsgPart>
    static void logMsgParts(std::ostream& os, const TmsgPart& msgPart) {
        os << msgPart;
    }

    static void logMsgBeginning(const string& functionName);
};

} // Base
}
}



#endif //MUSICBOX_LOGGER_H
