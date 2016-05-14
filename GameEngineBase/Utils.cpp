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

} // String

namespace glm {

extern ostream& operator<<(ostream& os, const vec2& v) {
    os << "{" << v.x << " ; " << v.y << "}";
    return os;
}

extern ostream& operator<<(ostream& os, const vec3& v) {
    os << "{" << v.x << " ; " << v.y << " ; " << v.z << "}";
    return os;
}
// Nog niet getest!
//extern bool operator<(const ivec3& first, const ivec3& second) {
//    bool isLessThan = false;
//    if (first.x < second.x)
//        isLessThan = true;
//    else if (first.x == second.x && first.y < second.y)
//        isLessThan = true;
//    else if (first.x == second.x && first.y < second.y && first.z < second.z)
//        isLessThan = true;
//
//    return isLessThan;
//}
//
//extern bool operator==(const ivec3& first, const ivec3& second) {
//    return first.x == second.x && first.y == second.y && first.z == second.z;
//}
//
//size_t Ivec3Hasher::operator()(const ivec3& v) {
//    using std::hash;
//    return ((hash<int>()(v.x)
//        ^ (hash<int>()(v.y) << 1)) >> 1)
//        ^ (hash<int>()(v.z) << 1);
//}

} // glm

} // Utils
} // Base
} // GameEngine
} // ForlornMelody