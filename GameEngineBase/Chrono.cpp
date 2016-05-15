//
// Created by Glenn Jacob on 01/04/16.
//

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
    this->beginTime = high_resolution_clock::now();
}

void Chrono::stop()
{
    this->endTime = high_resolution_clock::now();
}

double Chrono::duration()
{
    auto diff = endTime - beginTime;
    auto timeSpan = duration_cast<seconds>(diff);
    return timeSpan.count();
}
}
}
}