//
// Created by Glenn Jacob on 14/05/16.
//

#ifndef FORLORNMELODY_FRAMERATELIMITER_H
#define FORLORNMELODY_FRAMERATELIMITER_H

#include <memory>
#include <list>

using std::shared_ptr;
using std::make_shared;
using std::list;

namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class Chrono;

class FrameRateLimiter
{
public:
    FrameRateLimiter(int framesPerSecond);
    virtual ~FrameRateLimiter();

    double getFrameRate() const;

    void startFrame();
    void stopFrame();
    void calculateFrameRate();
    void pauseRemainingFrameTime();
private:
    int frameRate;
    double expectedFrameTime;
    double calculatedFrameTime;
    double calculatedFrameRate;
    shared_ptr<Chrono> pChrono;
    list<double> frameTimes;

    const int frameTimesSize = 20;
};
} // Base
} // GameEngine
} // ForlornMelody




#endif //FORLORNMELODY_FRAMERATELIMITER_H
