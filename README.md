# Forlorn Melody
Code repository for the game Forlorn Melody. The repository contains two libraries for the game engine and one for the logic of the game itself. OpenGL and GLFW3 is used for the graphics and user input. GLM is used for the math in the game.

## GameEngineBase
This is the library which contains the code for reuse in every game. The components in this library are the components that every game engine will need.

## GameEngineCustom
This library contains the components that will only be used in this game.

## Game
The Game library contains all logic of the game.

# Building
This project uses Cmake for building a make project or Visual Studio project. Before building make sure that GLFW3 and GLM can be found on your system. 

On Mac OS X the header files must be in the /usr/local/include folder. For linking the game the libraries must be found in /usr/local/lib. 

For building the project on Windows the libraries and the include files must in the C:\Program Files (x86)\glew, C:\Program Files (x86)\glfw or C:\Program Files (x86)\glm. Then change the working directory for the ForlormMelody project to $(SolutionDir)$(Configuration)\ so that it can find the required resources.

