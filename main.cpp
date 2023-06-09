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
#include "initialize.h"
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

    BlockSpawner = new Blockspawner(gGrid->getWHD());
    BlockSpawner->getCameraPointer(cameraAdress);
    BlockSpawner->setXYZshift(XYZshift);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);     
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    double currentTime = 0.0;
    glfwSetTime(0.0);
    float frequency = currentTime,
          deltaTime = 0.0f,	// time between current frame and last frame
          delay     = 0.005f;
    int   dropDelay = 10,
          activeDropDelay = dropDelay,
          initialDelay = dropDelay*10;
    bool  fullscreen = false;

    std::pair<int, int> wihi = cameraAdress->getScreenSize();
    bool first = true;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glGetError();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //update Time
        currentTime = glfwGetTime();
        if (BlockSpawner->isRun(false)){
        gGrid->drawGrid();
        
        if ((currentTime > (frequency + delay)) || first) {
            first = false;
            BlockSpawner->updateBlockLerp();
            BlockSpawner->updateBlockDepthLerp();
            frequency = currentTime;
            bool spacePressed = cameraAdress->updateSpace(false);
            if (initialDelay == 0){
                if (activeDropDelay == 0) {
                    if (spacePressed) { activeDropDelay = 1; }
                    else {
                        activeDropDelay = dropDelay;
                    }
                    BlockSpawner->updateHeight(spacePressed);
                }
                else if (!BlockSpawner->checkForQueue()) { activeDropDelay--; }
                }
            else { initialDelay--; }

        }
        BlockSpawner->drawActiveBlocks();
        if (0 < BlockSpawner->getCurrentBlock()) {
            BlockSpawner->drawDeadBlocks();
        }
        glfwSwapBuffers(window);
        }
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
    BlockSpawner->cleanBlockSpawner();
    delete(gGrid);
    delete(BlockSpawner);
    delete(cameraAdress);
    glfwTerminate();

    return EXIT_SUCCESS;
    
    return 1;
}

