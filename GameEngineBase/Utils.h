//
// Created by Glenn Jacob on 05/03/16.
//

#ifndef MUSICBOX_UTILS_H
#define MUSICBOX_UTILS_H

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {
namespace Utils {
namespace String {
extern void split(const string& s , char delimiter, vector<string>& elements);
}
}
} // Base
}
}

#endif //MUSICBOX_UTILS_H




