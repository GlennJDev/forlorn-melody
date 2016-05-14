//
// Created by Glenn Jacob on 05/03/16.
//

#ifndef MUSICBOX_UTILS_H
#define MUSICBOX_UTILS_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <ostream>

using std::string;
using std::vector;
using std::ostream;
using std::endl;

using glm::vec2;
using glm::vec3;
using glm::ivec3;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {
namespace Utils {
namespace String {

extern void split(const string& s , char delimiter, vector<string>& elements);

} // String

namespace glm {

extern ostream& operator<<(ostream& os, const vec2& v);
extern ostream& operator<<(ostream& os, const vec3& v);
//extern bool operator<(const ivec3& first, const ivec3& second);
//extern bool operator==(const ivec3& first, const ivec3& second);
//
//struct Ivec3Hasher {
//    size_t operator()(const ivec3& v);
//};

} // glm

} // Utils
} // Base
}
}


#endif //MUSICBOX_UTILS_H




