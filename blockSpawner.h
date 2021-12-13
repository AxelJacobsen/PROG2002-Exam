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
            lerpProg = lerpStep;        //defines progress as step to avoid hickups
    float   Xshift = 0.0f, Yshift = 0.0f, Zshift = 0.0f;

    int     XYZpos[3],
            currentblockNum = -1;

    bool    animFlip = true;            //For ghosts flipflops between frames for pac decides which direction to animate

    GLuint blockVAO;
    GLuint blockShader;

    std::vector<std::vector<float>>blockList;  //HoldsXYZ of one corner for a new block
    std::vector<std::vector<int>>blockIntList; //HoldsXYZints of all blocks
    std::vector<GLfloat> vertices;

    Camera* bCamHolder;
public:
    Blockspawner() {};
    ~Blockspawner() {};

    //virtual void changeDir();
    //virtual void updateLerp();
    void compilePacShader();
    void updateDir(int outDir);
    void pacAnimate();
    void updateCard(int newDir);
    int  getCard();
    void loadPacSpriteSheet();
    void deletePacSpriteSheet();
    bool updatePelletState(bool change);
    void updateAnimDel(int val, bool set);
    bool getRun()       { return run; };
    float getLerpProg() { return lerpProg; }
    void setRun(const bool runUpdate) { run = runUpdate; };
    GLuint compilePacman();
    void drawPacman();
    void transformPacman();
    void moveCamera(float x, float y);
    void checkForKeyUpdate();

    void newBlock();
    int createRandomBlock();
    void drawBlocks();
    GLuint compileVertices();
    void compileBlockShader();

    void  genCube();
    void  genLblock();
    void  genZblock();
    void  genTblock();
    float generateBlockCoord(int x, int y, int z, int mod, int loop);
    void  setXYZshift(std::vector<float> XYZshift) { Xshift = XYZshift[0], Yshift = XYZshift[1], Zshift = XYZshift[2]; }
    void  getCameraPointer(Camera* newCamera) { bCamHolder = newCamera; };
};

#endif
