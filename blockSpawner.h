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
    bool    isActive = true,
            run = true;
    float   lerpStart[3],               //Contains start coords of LERP
            lerpStop[3],                //Contains stop  coords of LERP
            speedDiv = 15.0f,           //Higher number = slower speed
            lerpStep = 1.0f / speedDiv, //Speed of LERP, also slowed by frequency in main
            lerpProg = lerpStep,        //defines progress as step to avoid hickups
            Xshift = 0.0f, Yshift = 0.0f, Zshift = 0.0f;

    int     XYZpos[3],
            currentblockNum = -1,
            width, height, depth;

    bool    animFlip = true;            //For ghosts flipflops between frames for pac decides which direction to animate

    GLuint  liveBlockVAO,
            deadBlockVAO,
            blockShader,
            blockSprite;

    std::vector<std::vector<float>>blockList;  //Holds all floats for all cubes
    std::vector<std::vector<int>>blockIntList; //HoldsXYZints of all blocks

    Camera* bCamHolder;
public:
    Blockspawner(std::vector<int> whd) { 
        width  = whd[0];
        height = whd[1];
        depth  = whd[2];
        loadBlockSprite(); 
        compileBlockShader(); 
    };
    ~Blockspawner() {};

    void newBlock();
    int createRandomBlock();
    void drawActiveBlocks();
    void drawDeadBlocks();
    GLuint compileVertices();
    GLuint compileVertices(bool dead);
    void compileBlockShader();

    void  genCube(int x, int y, int z);
    void  genLblock();
    void  genZblock();
    void  genTblock();
    float generateBlockCoord(float x, float y, float z, int mod, int loop);
    void  setXYZshift(std::vector<float> XYZshift) { Xshift = XYZshift[0], Yshift = XYZshift[1], Zshift = XYZshift[2]; }
    void  getCameraPointer(Camera* newCamera) { bCamHolder = newCamera; };
    void  handleBLockTextureCoords(int loop);
    void  loadBlockSprite();
    void  updateBlockLerp();
    void  killBlock() { isActive = false; };
    bool  checkIfLegalDir();
};

#endif
