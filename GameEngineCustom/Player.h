//
// Created by Glenn Jacob on 23/04/16.
//

#ifndef FORLORNMELODY_PLAYER_H
#define FORLORNMELODY_PLAYER_H

#include <GameEngineBase/Node.h>
#include <GameEngineBase/GraphicsUtils.h>

namespace ForlornMelody {
namespace GameEngine {

namespace Base {
    class ObjectModel;
}

namespace Custom {

class Player : public Base::Node
{
public:
    static const float ROTATION_SPEED;
    static const float MOVE_SPEED;

    Player(const string& name,
           const string& category,
           const shared_ptr<Base::ObjectModel>& objectModel,
           const Base::GLTexture& texture = Base::GLTexture());
    virtual ~Player();

    void rotateClockwise();
    void rotateCounterClockwise();
    void stopRotating();
    void moveForward();
    void moveBackward();
    void stopMoving();

    void move(const double timeInSeconds);
private:

    double currentSpeed;
    double currentTurnSpeed;
};

} // Custom
} // GameEngine
} // ForlornMelody




#endif //FORLORNMELODY_PLAYER_H
