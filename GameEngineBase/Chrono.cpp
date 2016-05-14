//
// Created by Glenn Jacob on 01/04/16.
//

#include <GLFW/glfw3.h>
#include "Chrono.h"

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

Chrono::Chrono()
{
}

Chrono::~Chrono()
{
}

void Chrono::start()
{
    this->beginTime = glfwGetTime();
}

void Chrono::stop()
{
    this->endTime = glfwGetTime();
}

double Chrono::duration()
{
    return endTime - beginTime;
}
}
}
}



////
//// Created by Glenn Jacob on 01/04/16.
////
//
//#include "Chrono.h"
//
//namespace ForlornMelody {
//namespace GameEngine {
//namespace Base {
//
//Chrono::Chrono()
//{
//}
//
//Chrono::~Chrono()
//{
//}
//
//void Chrono::start()
//{
//    this->beginTime = high_resolution_clock::now();
//}
//
//void Chrono::stop()
//{
//    this->endTime = high_resolution_clock::now();
//}
//
//double Chrono::duration()
//{
//    auto timeSpan = duration_cast<duration<double>>(endTime - beginTime);
//    return timeSpan.count();
//}
//}
//}
//}