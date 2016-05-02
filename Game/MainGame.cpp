//
// Created by Glenn Jacob on 23/04/16.
//

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
    this->pCamera->setZ(0.1);
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
    pPlayer->setPosition({0.f, -1.0, -2.0f});
    pPlayer->setScale({0.08f,0.08f,0.08f});

    pScene->addChildNode(pPlayer);
}

void MainGame::gameLoop()
{
    auto cloudsTexture = pResourceCache->loadTexture("Resources/Textures/Clouds_BG_Tileable.png");
    auto pClouds = make_shared<ImageNode>("backgroundCloud1", "backgroundClouds", cloudsTexture);
    pClouds->setPosition({0.0f,.5f,-50.f});
    float xScale = glm::tan(glm::radians(70.f)) * -pClouds->getZ()/2;
    float yScale = xScale * cloudsTexture.height / cloudsTexture.width;
    pClouds->setXScale(xScale);
    pClouds->setYScale(yScale);

    auto grassTexture = this->pResourceCache->loadTexture("./Resources/Textures/grass.png");
    auto pTerrain = make_shared<Terrain>("t1", "terrain", 0, -1, grassTexture);
    pTerrain->setY(-1.0f);
    auto pTerrain2 = make_shared<Terrain>("t2", "terrain", -1, -1, grassTexture);
    pTerrain2->setY(-1.0f);
    auto pTerrain3 = make_shared<Terrain>("t3", "terrain", 0, 0, grassTexture);
    pTerrain3->setY(-1.0f);
    auto pTerrain4 = make_shared<Terrain>("t4", "terrain", -1, 0, grassTexture);
    pTerrain4->setY(-1.0f);

    auto pScene = this->pMasterRenderer->getScene();
    pScene->addChildNode(pTerrain);
    pScene->addChildNode(pTerrain2);
    pScene->addChildNode(pTerrain3);
    pScene->addChildNode(pTerrain4);

//    srand(time(0));
    auto pTreeModel = pResourceCache->loadModel("Resources/Models/lowPolyTree.obj");
    auto treeTexture = pResourceCache->loadTexture("Resources/Textures/lowPolyTree.png");
    ostringstream oss;
    for (int i=0 ; i<100 ; i++) {
        oss << "tree" << i;
        string treeName = oss.str();
        auto pTree = make_shared<Node>(treeName, "trees", pTreeModel, treeTexture);
        pTree->setX((rand() % 100) - 50);
        pTree->setZ( -( (rand() % 40) + 2 ) );
        pTree->setY(-1);
        pTree->setScale({0.2f,0.2f,0.2f});
        pScene->addChildNode(pTree);
    }

//    auto pCrateModel = pResourceCache->loadModel("Resources/Models/Crate1.obj");
//    auto crateTexture = pResourceCache->loadTexture("Resources/Textures/crate_1.png", false);
//    auto pCrate = make_shared<Node>("crate1", "crate", pCrateModel, crateTexture);
//    pCrate->setPosition({2.0f, -2.0f, -0.0f});
//    pCrate->setScale({0.5f, 0.5f, 0.5f});
//    pScene->addChildNode(pCrate);

//    auto pTree = make_shared<Node>("tree1", "trees", pTreeModel, treeTexture);
//    pTree->setPosition({-2.0f, -2.0f, -0.0f});
//    pTree->setScale({0.2f, 0.2f, 0.2f});
//    pScene->addChildNode(pTree);

    pScene->addChildNode(pClouds);

    Chrono chrono;
    chrono.start();
    bool scaleDown = true;
    while (!this->pWindow->shouldWindowClose() && this->gameState != GameState::EXIT) {

        this->processInput();
        chrono.stop();

        this->pPlayer->move(chrono.duration());
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
        chrono.start();

        this->drawGame();

        this->pWindow->pollEvents();
    }
}

void MainGame::processInput()
{
    auto& inputManager = this->pWindow->getInputManager();
    int yRotation = (int)round(glm::degrees(pPlayer->getYrotation()));
    //Logger::logMsg("MainGame::processInput", "Y-rotation = ", yRotation);
    if (inputManager.isKeyPressed(GLFW_KEY_A)) {
        if (yRotation > -90) {
            pPlayer->stopMoving();
            pPlayer->rotateCounterClockwise();
        }
        else if (yRotation <= -90) {
            pPlayer->stopRotating();
            pPlayer->setYRotation(glm::radians(-90.0f));
            pPlayer->moveForward();

            pCamera->setX(pPlayer->getX());
        }
    } else if (inputManager.isKeyPressed(GLFW_KEY_D)) {
        if (yRotation < 90) {
            pPlayer->stopMoving();
            pPlayer->rotateClockwise();
        }
        else if (yRotation >= 90) {
            pPlayer->stopRotating();
            pPlayer->setYRotation(glm::radians(90.0f));
            pPlayer->moveForward();

            pCamera->setX(pPlayer->getX());
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


