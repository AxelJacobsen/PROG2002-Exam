/**
 *   Header for the grid class
 *
 *   @file     grid.h
 *   @author   Axel Jacobsen
 */

#ifndef __GRID_H
#define __GRID_H

#include "include.h"
#include "camera.h"
 /**
  *  Grid class def
  */
class Grid {
private:
    int width = 5, height = 5, depth  = 10;
    float Xshift, Yshift, Zshift;
    bool hasInitVao = false;

    GLuint gridShaderProgram;
    GLuint gridVAO, gridTextureVAO;
    GLuint gridSprite;

    std::vector<std::vector<std::vector<int>>> gridContI =
        std::vector<std::vector<std::vector<int>>>(width, std::vector<std::vector<int>>(height, std::vector<int>(depth)));

    std::vector<std::vector<std::vector<std::vector<float>>>> gridContF =
            std::vector<std::vector<std::vector<std::vector<float>>>>((width+1), 
            std::vector<std::vector<std::vector<float>>>((height+1), 
            std::vector<std::vector<float>>((depth+1), 
            std::vector<float>(3,0))));

    std::vector<float> gridFwText;
    std::vector<float> gridF;

    Camera* gCamHolder;
public:
    Grid();
    void   gridTextureFloatCreate();
    int    findWhatLines(const int x, const int y);
    void   compileGridShader();
    void   callCreateGridVao();
    void   loadGridSprite();
    GLuint createGrid(float size, bool texture);
    void   cleanGrid();
    //Getters
    GLuint getGridShader()   { return gridShaderProgram; };
    GLuint getGridVAO()      { return gridVAO; };
    GLuint getGridTexture()  { return gridSprite; };
    int    getGridSize()     { return gridF.size(); };
    int    getGridVal(int x, int y, int z) { return gridContI[x][y][z]; };

    std::vector<float>getXYZshift() { std::vector<float>XYZshift = { Xshift,Yshift,Zshift }; return XYZshift; };
    std::vector<int>getWHD() { std::vector<int>WidHeiDep = { width,height,depth }; return WidHeiDep; };
    std::vector<std::vector<std::vector<int>>>   getIntGrid()   { return gridContI; }
    std::vector<std::vector<std::vector<std::vector<float>>>> getFloatGrid() { return gridContF; }
    void drawGrid();
    void getGridCameraPointer(Camera* newCamera) { gCamHolder = newCamera; };
    void handleGridTexCoords(int rep);
    void fillGridCoords();
};  

#endif
