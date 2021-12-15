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
    int width, height, depth;
    float Xshift, Yshift, Zshift;
    bool hasInitVao = false;

    GLuint gridShaderProgram;
    GLuint gridVAO, gridTextureVAO;
    GLuint gridSprite;

    std::vector<std::vector<std::vector<int>>> gridContI;
    std::vector<std::vector<std::vector<std::vector<float>>>> gridContF;

    std::vector<float> gridFwText;
    std::vector<float> gridF;

    Camera* gCamHolder;
public:
    Grid(int w, int h, int d);
    void   gridTextureFloatCreate();
    void   handleGridTexCoords(int rep);
    void   fillGridCoords();
    void   drawGrid();

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
    void getGridCameraPointer(Camera* newCamera) { gCamHolder = newCamera; };

    void compileGridShader();
    void callCreateGridVao();
    void loadGridSprite();

    GLuint createGridVAO(float size, bool texture);
    void   cleanGrid();
};  

#endif
