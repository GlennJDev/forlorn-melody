//
// Created by Glenn Jacob on 01/04/16.
//

#ifndef MUSICBOX_CHRONO_H
#define MUSICBOX_CHRONO_H


namespace ForlornMelody {
namespace GameEngine {
namespace Base {

class Chrono
{
public:
    Chrono();
    virtual ~Chrono();

    void start();
    void stop();
    double duration();
private:

    double beginTime;
    double endTime;
};
}
}
}



#endif //MUSICBOX_CHRONO_H


////
//// Created by Glenn Jacob on 01/04/16.
////
//
//#ifndef MUSICBOX_CHRONO_H
//#define MUSICBOX_CHRONO_H
//
//#include <chrono>
//
//using namespace std::chrono;
//
//namespace ForlornMelody {
//namespace GameEngine {
//namespace Base {
//
//class Chrono
//{
//public:
//    Chrono();
//    virtual ~Chrono();
//
//    void start();
//    void stop();
//    double duration();
//private:
//
//    high_resolution_clock::time_point beginTime;
//    high_resolution_clock::time_point endTime;
//};
//}
//}
//}
//
//
//
//#endif //MUSICBOX_CHRONO_H