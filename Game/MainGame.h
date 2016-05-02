//
// Created by Glenn Jacob on 23/04/16.
//

#ifndef FORLORNMELODY_MAINGAME_H
#define FORLORNMELODY_MAINGAME_H

#include <memory>

using std::shared_ptr;

namespace ForlornMelody {

namespace GameEngine {
namespace Base {

class Window;
class ResourceCache;
class MasterRenderer;
class Scene;
class Camera;
class Light;

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

    shared_ptr<GameEngine::Base::Window> pWindow = nullptr;
    shared_ptr<GameEngine::Base::MasterRenderer> pMasterRenderer = nullptr;
    shared_ptr<GameEngine::Base::ResourceCache> pResourceCache = nullptr;
    shared_ptr<GameEngine::Base::Camera> pCamera = nullptr;
    shared_ptr<GameEngine::Base::Light> pLight = nullptr;

    shared_ptr<GameEngine::Custom::Player> pPlayer = nullptr;

    GameState gameState = PLAY;
    
};

}
}




#endif //FORLORNMELODY_MAINGAME_H
