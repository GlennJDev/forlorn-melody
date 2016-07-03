//
// Created by Glenn Jacob on 23/04/16.
//

#ifndef FORLORNMELODY_MAINGAME_H
#define FORLORNMELODY_MAINGAME_H

#include <memory>
#include <list>

using std::shared_ptr;
using std::list;

namespace ForlornMelody {

namespace GameEngine {
namespace Base {

class Window;
class ResourceCache;
class MasterRenderer;
class Scene;
class Node;
class Camera;
class Light;
class Chrono;
class FrameRateLimiter;

} // Base

namespace Custom {
class Player;
} // Custom
} // GameEngine

namespace Game {

enum GameState {
    PLAY, EXIT
};

class MainGame
{
public:
    MainGame();
    virtual ~MainGame();
    
    void run();
private:

    void initSystem();
    void gameLoop();
    void processInput();
    void drawGame();

    void movePlayer();

    shared_ptr<GameEngine::Base::Window> pWindow = nullptr;
    shared_ptr<GameEngine::Base::MasterRenderer> pMasterRenderer = nullptr;
    shared_ptr<GameEngine::Base::ResourceCache> pResourceCache = nullptr;
    shared_ptr<GameEngine::Base::Camera> pCamera = nullptr;
    shared_ptr<GameEngine::Base::Light> pLight = nullptr;
    shared_ptr<GameEngine::Base::FrameRateLimiter> pFrameRateLimiter = nullptr;

    shared_ptr<GameEngine::Base::Chrono> pMoveChrono;
    list<double> moveDeltas;
    shared_ptr<GameEngine::Custom::Player> pPlayer = nullptr;

    shared_ptr<GameEngine::Base::Node> pMusicBox = nullptr;

    GameState gameState = PLAY;
};

}
}




#endif //FORLORNMELODY_MAINGAME_H
