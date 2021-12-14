/**
 *   OpenGL Pacman Group 34
 *
 *   The Program:
 *     - Draws necessary objects
 *     - Runs wall collision for pacman and ghosts
 *     - Permits consuming pellets
 *     - Utilizes sprites to draw characters
 *     - Has victory conditions with both ghosts and pellets
 *
 *   @file     main.cpp
 *   @author   Axel E.W. Jacobsen, Rafael P. Avalos, Mekides A. Abebe
 */
#define TINYOBJLOADER_IMPLEMENTATION
#include "initialize.h"
//#include "pacman.h"
//#include "ghost.h"
#include "blockSpawner.h"
#include "grid.h"

// -----------------------------------------------------------------------------
// ENTRY POINT
// -----------------------------------------------------------------------------
/**
 *  main function
 */
int main(){
    //Container definition
    Grid* gGrid;
    Blockspawner* BlockSpawner;
    //std::vector<Pacman*>    Pacmans;    ///< Contains only pacman, done for ease of use
    //std::vector<Ghost*>     Ghosts;     ///< Contains ghosts
    //std::vector<Pellet*>    Pellets;    ///< Contains All pellets
    Camera* cameraAdress = new Camera();

    // Creates coordinates for map
    auto window = initializeWindow();
    recieveCamera(cameraAdress);
    if (window == nullptr) { return EXIT_FAILURE; }

    //Init map
    gGrid = new Grid(5, 5, 10);
    std::vector<float>XYZshift = gGrid->getXYZshift();
    gGrid->getGridCameraPointer(cameraAdress);
    gGrid->compileGridShader();

    cameraAdress->recieveIntMap(gGrid->getIntGrid());
    cameraAdress->recieveFloatMap(gGrid->getFloatGrid());
    printf("Map Loaded\n");

    BlockSpawner = new Blockspawner(gGrid->getWHD());
    BlockSpawner->getCameraPointer(cameraAdress);
    BlockSpawner->setXYZshift(XYZshift);
    printf("Blockloader Loaded\n");

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_MULTISAMPLE);

    double currentTime = 0.0;
    glfwSetTime(0.0);
    float frequency = currentTime;
    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;
    float delay     = 0.015f;
    bool fullscreen = false;

    std::pair<int, int> wihi = cameraAdress->getScreenSize();
    bool first = true;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glGetError();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //update Time
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastFrame;
        lastFrame = currentTime;
        
        //printf("Drawing Grid\n");
        gGrid->drawGrid();
        //printf("Grid drawn\n");
        
        if ((currentTime > (frequency + delay) && BlockSpawner->isRun(false)) || first) {
            first = false;
            //printf("Updating Lerp\n");
            BlockSpawner->updateBlockLerp();
            //printf("Lerp updated\n");
            frequency = currentTime;
            
        }

        //printf("Drawing Active blocks\n");
        BlockSpawner->drawActiveBlocks();
        //printf("Blocks drawn\n");
        

        glfwSwapBuffers(window);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            break;
        }

        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            if (!fullscreen) {
                fullscreen = true;
                glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), NULL, NULL, wihi.first, wihi.second, 30);
            }
            else {
                
                fullscreen = false;
                glfwSetWindowMonitor(window, NULL, NULL, NULL, wihi.first, wihi.second, 30);
            }
        }
    }

    glUseProgram(0);
    gGrid->cleanGrid();
    delete(gGrid);
    glfwTerminate();

    return EXIT_SUCCESS;
    
    return 1;
}

