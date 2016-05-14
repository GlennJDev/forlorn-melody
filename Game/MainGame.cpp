//
// Created by Glenn Jacob on 23/04/16.
//

#include <unistd.h>
#include <sstream>
#include <glm/glm.hpp>
#include <GameEngineBase/Window.h>
#include <GameEngineBase/MasterRenderer.h>
#include <GameEngineBase/NodeBatch.h>
#include <GameEngineBase/StaticShaderProgram.h>
#include <GameEngineBase/Node.h>
#include <GameEngineBase/Camera.h>
#include <GameEngineBase/Light.h>
#include <GameEngineBase/Scene.h>
#include <GameEngineBase/ResourceCache.h>
#include <GameEngineCustom/Player.h>
#include <GameEngineBase/Chrono.h>
#include <GameEngineBase/Logger.h>
#include <GameEngineCustom/ImageNode.h>
#include <GameEngineBase/Terrain.h>
#include <GameEngineCustom/BackgroundRenderBatch.h>
#include <GameEngineCustom/BackgroundShaderProgram.h>
#include "MainGame.h"

using std::make_shared;
using std::ostringstream;

namespace ForlornMelody {

using GameEngine::Base::Window;
using GameEngine::Base::MasterRenderer;
using GameEngine::Base::NodeBatch;
using GameEngine::Base::StaticShaderProgram;
using GameEngine::Base::Node;
using GameEngine::Base::Camera;
using GameEngine::Base::Light;
using GameEngine::Base::Scene;
using GameEngine::Base::ResourceCache;
using GameEngine::Base::Chrono;
using GameEngine::Base::Logger;
using GameEngine::Base::Terrain;
using GameEngine::Custom::Player;
using GameEngine::Custom::ImageNode;
using GameEngine::Custom::BackgroundRenderBatch;
using GameEngine::Custom::BackgroundShaderProgram;

namespace Game {

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
    this->initSystem();

    this->gameLoop();
}

void MainGame::initSystem()
{
    this->pWindow = make_shared<Window>();
    this->pWindow->create(1280, 800, "Forlorn Melody");
    this->pWindow->enableVsync();

    this->pResourceCache = make_shared<ResourceCache>();

    this->pCamera = make_shared<Camera>();
    this->pCamera->setZ(0.1f);
    this->pCamera->setY(0.5f);
    this->pLight = make_shared<Light>(vec3(0, 500, 500), vec3(255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f));

    auto pScene = make_shared<Scene>("firstScene");

    // Renderers
    // Bij het in commentaar zetten van een batch altijd het shader program mee in commentaar zetten.
    // Het programma kan er niet tegen dat er een ShaderProgram is dat niet gebruikt wordt.
    auto pShaderProgram = make_shared<StaticShaderProgram>();
    auto pNodeRenderBatch = make_shared<NodeBatch>();
    pNodeRenderBatch->setShaderProgram(pShaderProgram);
    pNodeRenderBatch->setCamera(pCamera);
    pNodeRenderBatch->setLight(pLight);
    pNodeRenderBatch->setNodeSortType(GameEngine::Base::NodeSortType::TEXTURE);

    auto pBackgroundShader = make_shared<BackgroundShaderProgram>();
    auto pBackgroundBatch = make_shared<BackgroundRenderBatch>();
    pBackgroundBatch->setShaderProgram(pBackgroundShader);
    pBackgroundBatch->setCamera(pCamera);
    pBackgroundBatch->setLight(pLight);

    this->pMasterRenderer = make_shared<MasterRenderer>(vec2(pWindow->getWidth(), pWindow->getHeight()));

    this->pMasterRenderer->setScene(pScene);
    this->pMasterRenderer->addRenderer("NodeRenderer", pNodeRenderBatch);
    this->pMasterRenderer->addRenderer("BackgroundRenderer", pBackgroundBatch);

    // Player
    auto personTexture = this->pResourceCache->loadTexture("./Resources/Textures/playerTexture.png");
    auto objModel = this->pResourceCache->loadModel("Resources/Models/person.obj");
    this->pPlayer = make_shared<Player>("player1", "players", objModel, personTexture);
    pPlayer->setPosition({0.f, -1.90, -5.0f});
    pPlayer->setScale({0.08f,0.08f,0.08f});

    pScene->addChildNode(pPlayer);
}

void MainGame::gameLoop()
{
    const string METHOD = "MainGame::gameLoop";
    auto pScene = this->pMasterRenderer->getScene();

    auto sceneryTexture = pResourceCache->loadTexture("Resources/Textures/layer-1.png");
    auto pScenery = make_shared<ImageNode>("backgroundCloud1", "backgroundClouds", sceneryTexture);
    pScenery->setZOrder(0);
    pScenery->setPosition({0.0f,.5f,-20.f});
    float yScale = glm::tan(glm::radians(70.f/2.0f)) * -pScenery->getZ() + 0.1f;
    float xScale = yScale * sceneryTexture.width / sceneryTexture.height;
    pScenery->setXScale(xScale);
    pScenery->setYScale(yScale);
    pScene->addChildNode(pScenery);

    auto groundTexture = pResourceCache->loadTexture("Resources/Textures/layer-2.png");
    auto pGround = make_shared<ImageNode>("ground", "ground1", groundTexture);
    pGround->setZOrder(1);
    pGround->setPosition({0.0f, -2.75f, -5.0f});
    xScale = glm::tan(glm::radians(70.f)) * -pGround->getZ();
    yScale = xScale * groundTexture.height / groundTexture.width;
    pGround->setScale({xScale, yScale, 1.0f});
    pScene->addChildNode(pGround);


    srand(time(0));
    auto pTreeModel = pResourceCache->loadModel("Resources/Models/lowPolyTree.obj");
    auto treeTexture = pResourceCache->loadTexture("Resources/Textures/lowPolyTree.png");
    ostringstream oss;
    for (int i=0 ; i<10 ; i++) {
        oss << "tree" << i;
        string treeName = oss.str();
        auto pTree = make_shared<Node>(treeName, "trees", pTreeModel, treeTexture);
        pTree->setX((rand() % 20) - 10);
        pTree->setZ( -5.1f );

        pTree->setY(-1.90f);
        pTree->setScale({0.07f,0.07f,0.07f});
        pScene->addChildNode(pTree);
    }

    auto pCrateModel = pResourceCache->loadModel("Resources/Models/ShippingCrate_SimpleCube.obj");

    Logger::logMsg(METHOD, "crate model: \n", *pCrateModel);

    auto crateTexture = pResourceCache->loadTexture("Resources/Textures/ShippingCrateA_tex.png", false);
    auto pCrate = make_shared<Node>("crate1", "crate", pCrateModel, crateTexture);
    pCrate->setPosition({1.0f, -2.0f, -5.0f});
    pCrate->setScale({1.f/6.f, 1.f/6.f, 1.f/6.f});
    //pCrate->setXRotation(glm::radians(90.f));
    pScene->addChildNode(pCrate);

//    auto pTree = make_shared<Node>("tree1", "trees", pTreeModel, treeTexture);
//    pTree->setPosition({-2.0f, -2.0f, -0.0f});
//    pTree->setScale({0.2f, 0.2f, 0.2f});
//    pScene->addChildNode(pTree);


    Chrono moveChrono;
    Chrono frameRateChrono;
    moveChrono.start();
    frameRateChrono.start();
    int counter = 0;
    bool scaleDown = true;
    double frameRate = (1.0/60.0) * 1000000.0;
    while (!this->pWindow->shouldWindowClose() && this->gameState != GameState::EXIT) {

        this->pWindow->pollEvents();
        this->processInput();

        moveChrono.stop();
        auto seconds = moveChrono.duration();

//        pCrate->setYRotation(pCrate->getYrotation() + 0.01f);
        pCrate->setXRotation(pCrate->getXrotation() + 0.01f);

        this->pPlayer->move(seconds);
//        auto scale = pCrate->getScale();
//        if (scaleDown)
//            pCrate->setScale({scale.x - 0.002f, scale.y - 0.002f, scale.z - 0.002f});
//        else
//            pCrate->setScale({scale.x + 0.002f, scale.y + 0.002f, scale.z + 0.002f});
//
//        if (pCrate->getScale().x <= 0.1f)
//            scaleDown = false;
//        else if (pCrate->getScale().x >= 1.0f)
//            scaleDown = true;
//
//        auto rotation = pTree->getRotation();
//        pTree->setRotation({rotation.x /*+ 0.01f*/, rotation.y /*+ 0.01f*/, rotation.z + 0.01f});
        
        moveChrono.start();

        this->drawGame();

        frameRateChrono.stop();
        auto delta = frameRateChrono.duration() * 1000000.0;
        auto sleepDuration = fmax(0.0, frameRate-delta);

        if ( sleepDuration > 0)
            usleep(sleepDuration);
//        if (counter == 10) {
//            Logger::logMsg(METHOD, "frame rate: ", 1.0/((sleepDuration+delta) / 1000000.0), " duration: ", delta/1000000.0,
//                " sleep duration: ", sleepDuration);
//            counter = 0;
//        }

        frameRateChrono.start();

        counter++;
    }
}

void MainGame::processInput()
{
    static bool fullyRotated = false;
    auto& inputManager = this->pWindow->getInputManager();
    int yRotation = (int)round(glm::degrees(pPlayer->getYrotation()));
    //Logger::logMsg("MainGame::processInput", "Y-rotation = ", yRotation);
    if (inputManager.isKeyPressed(GLFW_KEY_A)) {
        if (yRotation > -90) {
            pPlayer->stopMoving();
            pPlayer->rotateCounterClockwise();
            fullyRotated = false;
        }
        else if (yRotation <= -90) {
            pPlayer->stopRotating();
            pPlayer->setYRotation(glm::radians(-90.0f));

            if (fullyRotated) {
                pPlayer->moveForward();
            }
            fullyRotated = true;
        }
    } else if (inputManager.isKeyPressed(GLFW_KEY_D)) {
        if (yRotation < 90) {
            pPlayer->stopMoving();
            pPlayer->rotateClockwise();
            fullyRotated = false;
        }
        else if (yRotation >= 90) {
            pPlayer->stopRotating();
            pPlayer->setYRotation(glm::radians(90.0f));

            if (fullyRotated) {
                pPlayer->moveForward();
            }
            fullyRotated = true;
        }
    } else {
        pPlayer->stopMoving();
        pPlayer->stopRotating();
    }

    pCamera->setX(pPlayer->getX());
}

void MainGame::drawGame()
{
    // Draw everything here
    this->pMasterRenderer->render();

    this->pWindow->swapBuffers();
}

} // Game
} // ForlornMelody


