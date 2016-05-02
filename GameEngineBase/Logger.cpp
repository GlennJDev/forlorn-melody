//
// Created by Glenn Jacob on 29/03/16.
//


#include "Logger.h"

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

void Logger::logTime(ostream& os)
{
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    os << std::right; // set allignment to right
    os << std::setfill('0') << std::setw(2) <<  now->tm_mday << "-"
        << std::setfill('0') << std::setw(2) << (now->tm_mon + 1) << '-'
        << (now->tm_year + 1900) << " "
        << std::setfill('0') << std::setw(2) << now->tm_hour << ":"
        << std::setfill('0') << std::setw(2) << now->tm_min << ":"
        << std::setfill('0') << std::setw(2) << now->tm_sec << " ";
}

void Logger::logMsgBeginning(const string& functionName)
{
    logTime(cout);
    cout << std::left << std::setw(40) << std::setfill(' ') << functionName ;
}

} // Base
}
}

