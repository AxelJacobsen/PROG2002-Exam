/**
 *   Header to the blockSpawner class
 *
 *   @file     blockSpawner.h
 *   @author   Axel Jacobsen
 */

#ifndef __BLOCKSPAWNER_H
#define __BLOCKSPAWNER_H

#include "globFunc.h"
#include "camera.h"

 // -----------------------------------------------------------------------------
 // Blockspawner Class
 // -----------------------------------------------------------------------------'

class Blockspawner {
private:
    bool    isActive = false,
            run = true,
            failDelay = false;
    float   lerpStart[3]{0},               //Contains start coords of LERP
            lerpStop[3]{0},                //Contains stop  coords of LERP
            speedDiv   = 10.0f,            //Higher number = slower speed
            lerpStep   = 1.0f / speedDiv,  //Speed of LERP, also slowed by frequency in main
            lerpProg   = lerpStep,         //defines progress as step to avoid hickups
            heightLerp = lerpStep,
            Xshift = 0.0f, 
            Yshift = 0.0f, 
            Zshift = 0.0f;

    int     currentblockNum = -1,
            width, height, depth,
            requestedDir = 1,
            spawnPoint[3] = { 0, 4, 9 },
            deadSize = 0;

    bool    animFlip = true,            //For ghosts flipflops between frames for pac decides which direction to animate
            heightUpdated = true,
            queuedHeightDrop = false;

    GLuint  liveBlockVAO = -1,
            deadBlockVAO = -1,
            activeBlockShader = -1,
            deadBlockShader   = -1,
            blockSprite;

    std::vector<std::vector<float>>blockList;  //Holds all floats for all cubes
    std::vector<std::vector<int>> spatialXYZ;  //holds position of shape correctly in the XYZ space

    Camera* bCamHolder;
public:
    Blockspawner(std::vector<int> whd) { 
        width  = whd[0];
        height = whd[1];
        depth  = whd[2];
        spatialXYZ =
                std::vector<std::vector<int>>(width,
                std::vector<int>(height));
        loadBlockSprite();
    };
    ~Blockspawner() {};

    
    
    void  newBlock();
    int   createRandomBlock();
    void  genCube(int x, int y, int z);
    void  genLblock();
    void  genZblock();
    void  genTblock();
    float generateBlockCoord(float x, float y, float z, int mod, int loop);
    void  handleBLockTextureCoords(int loop);
    bool  generateLerpCoords();

    void  requestChangeDir();
    void  updateBlockLerp();
    void  updateBlockDepthLerp();
    void  updateHeight(bool space);
    void  killBlock();

    void   drawActiveBlocks();
    void   drawDeadBlocks();
    void   transformBlock();
    GLuint compileVertices();
    GLuint compileVertices(bool dead);
    void   compileActiveBlockShader();
    void   compileDeadBlockShader();
    
    std::vector<float> performLerp();
    std::vector<float> getColorsWithFloat(float colorDepth);
    void printBlockLContent(int desLayer);
    void loadBlockSprite();
    
    void  setNewDir(int newDir) { requestedDir = newDir; }
    void  setXYZshift(std::vector<float> XYZshift) { Xshift = XYZshift[0], Yshift = XYZshift[1], Zshift = XYZshift[2]; }

    bool  isRun(bool stop) { if (stop) { run = false; } return run; }
    bool  checkForQueue() { return queuedHeightDrop; }
    int   getCurrentBlock() { return currentblockNum; };
    void  getCameraPointer(Camera* newCamera) { bCamHolder = newCamera; };
    bool  getHeightUpdated() { if (heightUpdated) { heightUpdated = false; return true; } return false; }

    void cleanBlockSpawner() {
        glDeleteProgram(activeBlockShader);
        glDeleteProgram(deadBlockShader);
        glDeleteTextures(1, &blockSprite);
        CleanVAO(liveBlockVAO);
        CleanVAO(deadBlockVAO);
    }
};  

#endif
