//
// Created by Glenn Jacob on 14/05/16.
//

#include <unistd.h>
#include <cmath>
#include <GameEngineBase/Chrono.h>
#include "FrameRateLimiter.h"

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

FrameRateLimiter::FrameRateLimiter(int framesPerSecond)
    : frameRate(framesPerSecond), pChrono(new Chrono())
{
    expectedFrameTime = 1.0 / (double)framesPerSecond;
}

FrameRateLimiter::~FrameRateLimiter()
{
}

double FrameRateLimiter::getFrameRate() const
{
    return this->calculatedFrameRate;
}

void FrameRateLimiter::startFrame()
{
    this->pChrono->start();
}

void FrameRateLimiter::stopFrame()
{
    this->pChrono->stop();
    this->calculateFrameRate();
    this->pauseRemainingFrameTime();
}

void FrameRateLimiter::calculateFrameRate()
{
    if (this->frameTimes.size() == frameTimesSize)
        frameTimes.pop_front();
    frameTimes.push_back(this->pChrono->duration());

    double totalTime = 0.0;
    for (auto frameTime : frameTimes) {
        totalTime += frameTime;
    }
    this->calculatedFrameTime = totalTime / (double)frameTimes.size();
}

void FrameRateLimiter::pauseRemainingFrameTime()
{
    double sleepDuration = (this->expectedFrameTime - this->calculatedFrameTime);
    calculatedFrameRate = 1.0 / (calculatedFrameTime + fabs(sleepDuration));
    if ( sleepDuration > 0.0 )
        usleep((useconds_t)(sleepDuration * 1000000.0));
}
} // Base
} // GameEngine
} // ForlornMelody


