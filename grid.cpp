/**
 *   grid cpp file
 *
 *   @file     grid.h
 *   @author   Axel Jacobsen
 */

#include "grid.h"
#include "globFunc.h"

/**
*  Recieves lvlVect in to Pacman[0]
* 
*   @param filePath - map file filepath
* 
*   @see Map::mapFloatCreate()
*   @see Map::loadMapSpriteSheet()
*/
Grid::Grid() {
    // Read from level file;
    Xshift = 2.0f / float(width);
    Yshift = 2.0f / float(height);
    Zshift = 2.0f / float(depth+1);
    gridTextureFloatCreate();
    loadGridSprite();
    fillGridCoords();
}

/**
 *  Creates map coordinates
 *
 *  @see GLfloat getCoordsWithInt(int y, int x, int loop);
 *  @see Map::findWhatWalls(const int x, const int y)
 *  @see Map::loopOrder(int num)
 *  @see Map::handleMapTexCoords(int rep)
 */
void Grid::gridTextureFloatCreate() {
    int tempTxtRep = 0;
    for (int Zlevel = 0; Zlevel < depth + 1; Zlevel++) {
        for (int inverse = -1; inverse < 2; inverse+=2) {
            for (int Xwall = 0; Xwall < width; Xwall++) {
                tempTxtRep = 0;
                for (int inner = 0; inner < 2; inner++) {
                    for (int outer = 0; outer < 2; outer++) {
                        gridFwText.push_back(((Xshift * Xwall) + (Xshift * outer))-1);
                        gridFwText.push_back(float(inverse));
                        gridFwText.push_back(((Zshift * Zlevel) + (Zshift * inner))-1);
                        handleGridTexCoords(tempTxtRep);
                        tempTxtRep++;
                    }
                }
            }
            
            for (int Ywall = 0; Ywall < height; Ywall++) {
                tempTxtRep = 0;
                for (int inner = 0; inner < 2; inner++) {
                    for (int outer = 0; outer < 2; outer++) {
                        gridFwText.push_back(float(inverse));
                        gridFwText.push_back(((Yshift * Ywall) + (Yshift * outer))-1);
                        gridFwText.push_back(((Zshift * Zlevel) + (Zshift * inner))-1);
                        handleGridTexCoords(tempTxtRep);
                        tempTxtRep++;
                    }
                }
            }
        }
    }
    
    for (int backX = 0; backX < width; backX++) {
        for (int backY = 0; backY < height; backY++) {
            tempTxtRep = 0;
            for (int UD = 0; UD < 2; UD++) {
                for (int LR = 0; LR < 2; LR++) {
                    gridFwText.push_back(((Xshift * backX) + (Xshift * LR)) - 1);
                    gridFwText.push_back(((Yshift * backY) + (Yshift * UD)) - 1);
                    gridFwText.push_back(-1.0f);
                    handleGridTexCoords(tempTxtRep);
                    tempTxtRep++;
                }
            }
        }
    }
}

/**
 *  Loads texture for map Wall
 * 
 *  @see load_opengl_texture(const std::string& filepath, GLuint slot)
 */
void Grid::loadGridSprite() {
    gridSprite = load_opengl_texture("assets/wallTexture.png", 0);
}

/**
 *  Handles Map Texture coords
 *
 *  @param rep - tells us what corner needs coords
 *
 */
void Grid::handleGridTexCoords(int rep) {
    std::pair <int, int> temp = { 0,0 };
    switch (rep) {
    case 0:                     break;
    case 1:  temp.first = 1;    break;
    case 2:  temp.second = 1;   break;
    case 3:  temp = { 1, 1 };   break;
    default: temp = { -1,-1 };  break;
    }
    gridFwText.push_back(temp.first);
    gridFwText.push_back(temp.second);
}

/**
 *  Loads texture for map Wall
 *
 *  @see load_opengl_texture(const std::string& filepath, GLuint slot)
 */
void Grid::fillGridCoords() {
    for (int z = 0; z < depth + 1; z++) {
        for (int y = 0; y < height + 1; y++) {
            for (int x = 0; x < width + 1; x++) {
                gridContF[x][y][z][0] = (x  * Xshift)-1.0f;
                gridContF[x][y][z][1] = (y * Yshift)-1.0f;
                gridContF[x][y][z][2] = (z  * Zshift)-1.0f;
                for (int test = 0; test < 3; test++) {
                    printf("%f\t", gridContF[x][y][z][test]);
                }
                printf("\n");
            }
        }
    }
}

/**
 *  calls CompileShader for maps shader, also sets map verticie values
 *
 *
 *  @see CompileShader(const std::string& vertexShaderSrc,
            const std::string& fragmentShaderSrc)
 *  @see Map::callCreateMapVao()
 *  
 */
void Grid::compileGridShader(){
    gridShaderProgram = CompileShader(   gridVertexShaderSrc,
                                         gridFragmentShaderSrc);

    GLint gPosAttrib = glGetAttribLocation(gridShaderProgram, "gPosition");
    glEnableVertexAttribArray(gPosAttrib);
    glVertexAttribPointer(gPosAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    callCreateGridVao();

    GLuint gtexAttrib = glGetAttribLocation(gridShaderProgram, "gTexcoord");
    glEnableVertexAttribArray(gtexAttrib);
    glVertexAttribPointer(gtexAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    
}

void Grid::callCreateGridVao() {
    int gridSize = gridFwText.size();
    gridTextureVAO = createGrid(gridSize, true);
}

// -----------------------------------------------------------------------------
//  CREATE MAP
// -----------------------------------------------------------------------------
/**
 *  Creates map VAO
 *
 *  @param size - size of map to be drawn
 *
 *  @see GLuint getIndices(int out, int mid, int in)
 *
 *  @return returns vao of map
 */
GLuint Grid::createGrid(float size, bool texture) {
    std::vector<GLuint> mapIndices;
    if (texture){
        for (int o = 0; o < (size / 3); o += 4) {
            for (int m = 0; m < 2; m++) {
                for (int i = o; i < (o + 3); i++) {
                    int hold = getIndices(o, m, i);
                    if ((i - o) == 2 && m != 1) { hold++; }
                    mapIndices.push_back(hold);
                }
            }
        };
    }
    else {
        for (int i = 0; i < size/4; i+=4) {
            mapIndices.push_back((i));
            mapIndices.push_back((i+1));
            
            mapIndices.push_back((i+1));
            mapIndices.push_back((i+3));
            
            mapIndices.push_back((i+3));
            mapIndices.push_back((i+2));
            
            mapIndices.push_back((i+2));
            mapIndices.push_back((i));
        }
    }

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
        size * sizeof((gridFwText)[0]),
        (&gridFwText[0]),
        GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (const void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mapIndices.size() * sizeof(mapIndices[0]), (&mapIndices[0]), GL_STATIC_DRAW);

    return vao;
}

/**
 *  Cleans shader, texturesheet & mapVAO
 *
 *  @see CleanVAO(GLuint& vao)
 */
void Grid::cleanGrid() {
    glDeleteProgram(gridShaderProgram);
    glDeleteTextures(1, &gridSprite);
    CleanVAO(gridTextureVAO);
    CleanVAO(gridVAO);
}

/**
 *  Draws map
 *
 *  @param shader - shaderprogram to use for drawing
 *  @param vao    - vao of object
 * 
 *  @see Camera::applycamera(const GLuint shader, const float width, const float height)
 */
void Grid::drawGrid() {
    auto gridTextureLocation = glGetUniformLocation(gridShaderProgram, "u_gridTexture");
    auto gridIsTextured = glGetUniformLocation(gridShaderProgram, "boolText");
    auto gridVertexColorLocation = glGetUniformLocation(gridShaderProgram, "u_Color");

    glUseProgram(gridShaderProgram);
    glUniform1i(gridTextureLocation, 0);
    gCamHolder->applycamera(gridShaderProgram, width, height);
    glBindVertexArray(gridTextureVAO);
    glUniform1f(gridIsTextured, 1.0f);
    glDrawElements(GL_TRIANGLES, gridFwText.size(), GL_UNSIGNED_INT, (const void*)0);
    
    if (!hasInitVao) { gridVAO = createGrid(gridFwText.size(), false); hasInitVao = true; }

    glUseProgram(gridShaderProgram);
    gCamHolder->applycamera(gridShaderProgram, width, height);
    glBindVertexArray(gridVAO);
    glUniform4f(gridVertexColorLocation, 0.1f, 0.9f, 0.1f, 1.0f);
    glUniform1f(gridIsTextured, 0.0f);
    glDrawElements(GL_LINES, gridFwText.size(), GL_UNSIGNED_INT, (const void*)0);
}