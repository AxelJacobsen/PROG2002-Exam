/**
 *   Header to the Camera class.
 *
 *   @file     camera.h
 *   @author   Axel Jacobsen
 */

#ifndef __CAMERA_H
#define __CAMERA_H

#include "include.h"

 // -----------------------------------------------------------------------------
 // Camera Class header
 // -----------------------------------------------------------------------------
class Camera {  
private:
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.84f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -0.5f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 0.5f, 0.0f);
    std::vector<std::vector<std::vector<int>>>   gridIntHolder;
    std::vector<std::vector<std::vector<std::vector<float>>>> gridFloatHolder;

    float yaw   = 180.0f;
    float pitch = 0.0f;
    float fov   = 100.0f;
    int   screenWidth = 1000,
          screenHeight = 1000;

    bool  keyCalled = false,
          spaceCalled = false;
    int   blockDesDir = 2;
public:
    Camera() {};
    glm::vec3 getCamPos()   { return cameraPos; };
    glm::vec3 getCamFront() { return cameraFront; };
    glm::vec3 getCamUp()    { return cameraUp; };
    void setCamPos(glm::vec3    newCameraPos)   { cameraPos     = newCameraPos;     };
    void setCamFront(glm::vec3  newCameraFront) { cameraFront   = newCameraFront;   };
    void setCamUp(glm::vec3     newCameraUp)    { cameraUp      = newCameraUp;      };

    int     getCamIntMapVal(int x, int y, int z)   { return gridIntHolder[x][y][z];   };
    void    updateCamIntMap(int x, int y, int z)   { gridIntHolder[x][y][z] = 1; }
    void    printCamIntMap();
    float   getCamFloatMapVal(int x, int y, int z, int XYZ) { return gridFloatHolder[x][y][z][XYZ]; };

    int     getNewDesDir() { if (keyCalled) { keyCalled = false; return blockDesDir; } else return -1; };
    void    setNewDesDir(int newDir) { blockDesDir = newDir; keyCalled = true; }
    GLfloat getCoordsWithInt(int y, int x, int loop, float Xshift, float Yshift, float Zshift);
    void    applycamera(const GLuint shader, const float width, const float height);

    void    recieveIntMap  (std::vector<std::vector<std::vector<int>>> valueMap)   { gridIntHolder = valueMap; }
    void    recieveFloatMap(std::vector<std::vector<std::vector<std::vector<float>>>> floatMap) { gridFloatHolder = floatMap;}

    std::pair<int, int> getScreenSize() { std::pair<int, int> wh = { screenWidth, screenHeight }; return wh; }
    bool    updateSpace(bool update) { if (update) { if (spaceCalled) { spaceCalled = false; } else { spaceCalled = true; printf("Space pressed!\n"); } }  return spaceCalled; };
};

#endif
