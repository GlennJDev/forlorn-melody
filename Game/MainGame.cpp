//
// Created by Glenn Jacob on 23/04/16.
//

#include <unistd.h>
#include <list>
#include <sstream>
#include <glm/glm.hpp>
#include <GameEngineBase/Window.h>
#include <GameEngineBase/MasterRenderer.h>
#include <GameEngineBase/NodeBatch.h>
#include <GameEngineBase/TerrainBatch.h>
#include <GameEngineBase/StaticShaderProgram.h>
#include <GameEngineBase/TerrainShader.h>
#include <GameEngineBase/Node.h>
#include <GameEngineBase/Camera.h>
#include <GameEngineBase/Light.h>
#include <GameEngineBase/Scene.h>
#include <GameEngineBase/ResourceCache.h>
#include <GameEngineCustom/Player.h>
#include <GameEngineBase/Chrono.h>
#include <GameEngineBase/Logger.h>
#include <GameEngineBase/FrameRateLimiter.h>
#include <GameEngineBase/Terrain.h>
#include <GameEngineCustom/ImageNode.h>
#include <GameEngineCustom/BackgroundRenderBatch.h>
#include <GameEngineCustom/BackgroundShaderProgram.h>
#include "MainGame.h"

using std::make_shared;
using std::ostringstream;
using std::list;

namespace ForlornMelody {

using GameEngine::Base::Window;
using GameEngine::Base::MasterRenderer;
using GameEngine::Base::NodeBatch;
using GameEngine::Base::TerrainBatch;
using GameEngine::Base::StaticShaderProgram;
using GameEngine::Base::TerrainShader;
using GameEngine::Base::Node;
using GameEngine::Base::Camera;
using GameEngine::Base::Light;
using GameEngine::Base::Scene;
using GameEngine::Base::ResourceCache;
using GameEngine::Base::Chrono;
using GameEngine::Base::Logger;
using GameEngine::Base::Terrain;
using GameEngine::Base::FrameRateLimiter;
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
    this->pCamera->setZ(0.5f);
    this->pCamera->setY(1.f);
    this->pCamera->setPitch(15.f);
    this->pCamera->setYaw(345.f);
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

    auto pTerrainShader = make_shared<TerrainShader>();
    auto pTerrainBatch = make_shared<TerrainBatch>();
    pTerrainBatch->setShaderProgram(pTerrainShader);
    pTerrainBatch->setCamera(pCamera);
    pTerrainBatch->setLight(pLight);

    this->pMasterRenderer = make_shared<MasterRenderer>(vec2(pWindow->getWidth(), pWindow->getHeight()));

    this->pMasterRenderer->setScene(pScene);
    this->pMasterRenderer->addRenderer("3 NodeRenderer", pNodeRenderBatch);
    this->pMasterRenderer->addRenderer("1 BackgroundRenderer", pBackgroundBatch);
    this->pMasterRenderer->addRenderer("2 TerrainBatch", pTerrainBatch);

    // Player
    auto personTexture = this->pResourceCache->loadTexture("./Resources/Textures/playerTexture.png");
    auto objModel = this->pResourceCache->loadModel("Resources/Models/person.obj");
    this->pPlayer = make_shared<Player>("player1", "players", objModel, personTexture);
    pPlayer->setPosition({0.f, 0.0, -5.0f});
    pPlayer->setScale({0.08f,0.08f,0.08f});
    pScene->addChildNode(pPlayer);

    // Frame rate limiter
    this->pFrameRateLimiter = make_shared<FrameRateLimiter>(30);

    this->pMoveChrono = make_shared<Chrono>();
}

void MainGame::gameLoop()
{
    const string METHOD = "MainGame::gameLoop";
//    auto cloudsTexture = pResourceCache->loadTexture("Resources/Textures/Clouds_BG_Tileable.png");
//    auto pClouds = make_shared<ImageNode>("backgroundCloud1", "backgroundClouds", cloudsTexture);
//    pClouds->setPosition({0.0f,.5f,-50.f});
//    float xScale = glm::tan(glm::radians(70.f)) * -pClouds->getZ()/2;
//    float yScale = xScale * cloudsTexture.height / cloudsTexture.width;
//    pClouds->setXScale(xScale);
//    pClouds->setYScale(yScale);

    auto pScene = this->pMasterRenderer->getScene();

    // Terrain
    auto terrainTexture = pResourceCache->loadTexture("Resources/Textures/grass.png");
    auto pTerrain = make_shared<Terrain>("grass1", "grass", 0, -2, terrainTexture);
    pScene->addChildNode(pTerrain);
    auto pTerrain2 = make_shared<Terrain>("grass2", "grass", -1, -2, terrainTexture);
    pScene->addChildNode(pTerrain2);
    auto pTerrain3 = make_shared<Terrain>("grass3", "grass", 0, -1, terrainTexture);
    pScene->addChildNode(pTerrain3);
    auto pTerrain4 = make_shared<Terrain>("grass4", "grass", -1, -1, terrainTexture);
    pScene->addChildNode(pTerrain4);

    // Scenery
    auto sceneryTexture = pResourceCache->loadTexture("Resources/Textures/layer-1.png");
    auto pScenery = make_shared<ImageNode>("backgroundCloud1", "backgroundClouds", sceneryTexture);
    pScenery->setZOrder(0);
    pScenery->setPosition({0.0f,-0.0f,-8.f});
    float yScale = glm::tan(glm::radians(70.f/2.f)) * fabs(pScenery->getZ()) + 0.1f;
    float xScale = yScale * sceneryTexture.width / sceneryTexture.height;
    pScenery->setXScale(xScale);
    pScenery->setYScale(yScale);
    pScene->addChildNode(pScenery);

    auto pScenery2 = make_shared<ImageNode>("backgroundCloud2", "backgroundClouds", sceneryTexture);
    pScenery2->setZOrder(0);
    pScenery2->setPosition({0.0f, -0.0f, -8.f});
    yScale = glm::tan(glm::radians(70.f/2.f)) * fabs(pScenery2->getZ()) + 0.1f;
    xScale = yScale * sceneryTexture.width / sceneryTexture.height;
    pScenery2->setXScale(xScale);
    pScenery2->setYScale(yScale);
    pScenery2->setX(-pScenery2->getSize()[0]);
    pScene->addChildNode(pScenery2);

//    auto groundTexture = pResourceCache->loadTexture("Resources/Textures/layer-2.png");
//    auto pGround = make_shared<ImageNode>("ground", "ground1", groundTexture);
//    pGround->setZOrder(1);
//    pGround->setPosition({0.0f, -2.75f, -5.0f});
//    xScale = glm::tan(glm::radians(70.f)) * -pGround->getZ();
//    yScale = xScale * groundTexture.height / groundTexture.width;
//    pGround->setScale({xScale, yScale, 1.0f});
//    pScene->addChildNode(pGround);

    srand(time(0));
    auto pTreeModel = pResourceCache->loadModel("Resources/Models/lowPolyTree.obj");
    auto treeTexture = pResourceCache->loadTexture("Resources/Textures/lowPolyTree.png");
    ostringstream oss;
    for (int i=0 ; i<10 ; i++) {
        oss << "tree" << i;
        string treeName = oss.str();
        auto pTree = make_shared<Node>(treeName, "trees", pTreeModel, treeTexture);
        pTree->setX((rand() % 40) - 20);
        pTree->setZ( -5.5f );
        pTree->setY(-0.2f);
        pTree->setScale({0.08f,0.08f,0.08f});
		pTree->setReflectivity(0.0001f);
		pTree->setShineDamper(0.001f);

        pScene->addChildNode(pTree);
    }

    this->pFrameRateLimiter->startFrame();
    pMoveChrono->start();
    int counter = 0;
    bool scaleDown = true;
    while (!this->pWindow->shouldWindowClose() && this->gameState != GameState::EXIT) {
        this->pWindow->pollEvents();
        this->processInput();

        pMoveChrono->stop();
        movePlayer();

        pCamera->setX(pPlayer->getX() + 2.f);

        pMoveChrono->start();

        this->drawGame();

        pFrameRateLimiter->stopFrame();
        pFrameRateLimiter->startFrame();

//        if (counter == 50) {
//            Logger::logMsg(METHOD, "frame rate: ", pFrameRateLimiter->getFrameRate());
//            counter = 0;
//        }

        counter++;
    }
}

void MainGame::movePlayer()
{
    auto seconds = pMoveChrono->duration();
    moveDeltas.push_back(seconds);
    if (moveDeltas.size() > 10)
        moveDeltas.pop_front();
    double totalTime = 0.0;
    for (auto delta : moveDeltas) {
            totalTime += delta;
        }
    double averagedTime = totalTime / (double)moveDeltas.size();

    pPlayer->move(averagedTime);
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
}

void MainGame::drawGame()
{
    // Draw everything here
    this->pMasterRenderer->render();

    this->pWindow->swapBuffers();
}

} // Game
} // ForlornMelody


