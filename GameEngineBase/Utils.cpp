//
// Created by Glenn Jacob on 05/03/16.
//

#include <string>
#include <sstream>
#include "Utils.h"

using std::stringstream;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {
namespace Utils {
namespace String {

extern void split(const string& s , char delimiter, vector<string>& elements) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        if (item.length() > 0)
            elements.push_back(item);
    }
}

}
}
} // Base
}
}