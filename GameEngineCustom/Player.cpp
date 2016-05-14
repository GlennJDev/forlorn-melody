//
// Created by Glenn Jacob on 23/04/16.
//

#include <GameEngineBase/ObjectModel.h>
#include <GameEngineBase/Logger.h>
#include "Player.h"

namespace ForlornMelody {
namespace GameEngine {

using Base::Node;
using Base::ObjectModel;
using Base::GLTexture;

namespace Custom {

const float Player::ROTATION_SPEED = 360; // °/s
const float Player::MOVE_SPEED = 1.f; // units/s

Player::Player(const string& name,
               const string& category,
               const shared_ptr<ObjectModel>& objectModel,
               const GLTexture& texture)
: Node(name, category, objectModel, texture)
{
}

Player::~Player()
{
}


void Player::rotateClockwise()
{
    currentTurnSpeed = ROTATION_SPEED;
}

void Player::rotateCounterClockwise()
{
    currentTurnSpeed = -ROTATION_SPEED;
}

void Player::moveForward()
{
    currentSpeed = MOVE_SPEED;
}

void Player::moveBackward()
{
    currentSpeed = -MOVE_SPEED;
}

void Player::stopRotating()
{
    currentTurnSpeed = 0;
}

void Player::stopMoving()
{
    currentSpeed = 0;
}

void Player::move(const double timeInSeconds)
{
    this->setYRotation(this->getRotation().y + glm::radians(currentTurnSpeed) * timeInSeconds);
    double distance = currentSpeed * timeInSeconds;

    double x = this->getX() + distance * glm::sin(this->getRotation().y);
    double z = this->getZ() + distance * glm::cos(this->getRotation().y);
    this->setPosition({x, this->getY(), z});

//    static int counter = 0;
//    if (counter == 20) {
//        counter = 0;
//        Base::Logger::logMsg("Player::move", "player move distance: ", distance, " (x,z): (" , x , ",", z, ")");
//    }
//    counter++;

}

} // Custom
} // GameEngine
} // ForlornMelody