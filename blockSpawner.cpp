// -----------------------------------------------------------------------------
// Class function definition
// -----------------------------------------------------------------------------

#include "blockSpawner.h"


void Blockspawner::compileActiveBlockShader() {
    activeBlockShader = CompileShader(blockVertexShaderSrc,
                                      blockFragmentShaderSrc);

    GLint bposAttrib = glGetAttribLocation(activeBlockShader, "bPosition");
    glEnableVertexAttribArray(bposAttrib);
    glVertexAttribPointer(bposAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    
    GLint dbtextAttrib = glGetAttribLocation(activeBlockShader, "bTexture");
    glEnableVertexAttribArray(dbtextAttrib);
    glVertexAttribPointer(dbtextAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    
    GLuint dcolorAttrib = glGetAttribLocation(activeBlockShader, "bColor");
    glEnableVertexAttribArray(dcolorAttrib);
    glVertexAttribPointer(dcolorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
}

void Blockspawner::compileDeadBlockShader() {
    deadBlockShader = CompileShader(deadBlockVertexShaderSrc,
                                    deadBlockFragmentShaderSrc);

    GLint dbposAttrib = glGetAttribLocation(deadBlockShader, "dbPosition");
    glEnableVertexAttribArray(dbposAttrib);
    glVertexAttribPointer(dbposAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);

    GLint dbtextAttrib = glGetAttribLocation(deadBlockShader, "dbTexture");
    glEnableVertexAttribArray(dbtextAttrib);
    glVertexAttribPointer(dbtextAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    
    GLuint dbcolorAttrib = glGetAttribLocation(deadBlockShader, "dbColor");
    glEnableVertexAttribArray(dbcolorAttrib);
    glVertexAttribPointer(dbcolorAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
}

void Blockspawner::newBlock() {
    std::vector<float> floatVec;
    currentblockNum++;
    isActive = true;
    blockList.push_back(floatVec);
    int bType = createRandomBlock();
    switch (bType)
    {
    case 0: genCube(spawnPoint[0], spawnPoint[1], spawnPoint[2]); break;
    case 1: genLblock();  break;
    case 2: genZblock();  break;
    case 3: genTblock();  break;
    default: printf("\nILLEGAL BLOCK TYPE\n");
        break;
    }
    liveBlockVAO = compileVertices();
    compileActiveBlockShader();

    if (currentblockNum != 0) { deadBlockVAO = compileVertices(true); }
    compileDeadBlockShader();
    
};

int Blockspawner::createRandomBlock() {
    time_t t;
    srand((unsigned)time(&t));
    int randType = (rand() % 4);
    return randType;
};

void Blockspawner::drawActiveBlocks() {
    if (activeBlockShader == -1) { compileActiveBlockShader(); }

    auto activeBlockColorLocation = glGetUniformLocation(activeBlockShader, "u_Color");
    glUseProgram(activeBlockShader);
    bCamHolder->applycamera(activeBlockShader, width, height);
    transformBlock();
    glBindVertexArray(liveBlockVAO);
    glUniform4f(activeBlockColorLocation, 0.2f, 0.2f, 1.0f, 1.0f);
    glLineWidth(3);
    glDrawElements(GL_LINES, blockList[currentblockNum].size(), GL_UNSIGNED_INT, (const void*)0);
    glLineWidth(1);
}

void Blockspawner::drawDeadBlocks() {
    if (deadBlockShader == -1) { compileDeadBlockShader(); }
    auto deadBlockTextureLocation = glGetUniformLocation(deadBlockShader, "u_deadBlockTexture");
    
    glUseProgram(deadBlockShader);
    glUniform1i(deadBlockTextureLocation, 1);
    bCamHolder->applycamera(deadBlockShader, width, height);
    glBindVertexArray(deadBlockVAO);
    glDrawElements(GL_TRIANGLES, deadSize, GL_UNSIGNED_INT, (const void*)0);
}

/**
 *  Compiles all Pellet verticie lists into a large vector and calls CreateObject
 *
 *  @param itObj - which type of object to iterate, here always pellet
 *
 *  @see Pellet::getVertCoord(int index);
 *  @see GLuint CreateObject(GLfloat* object, int size, const int stride);
 *
 *  @return returns VAO gotten from CreateObject func
 */
GLuint Blockspawner::compileVertices() {
    std::vector<GLfloat> tempVert;
    int stride = 8;
    for (auto& it : blockList[currentblockNum]) {
        tempVert.push_back(it);
    }
    return CreateObject(&tempVert[0], tempVert.size() * sizeof(tempVert[0]), stride, true);
}

/**
 *  Compiles all Pellet verticie lists into a large vector and calls CreateObject
 *
 *  @param itObj - which type of object to iterate, here always pellet
 *
 *  @see Pellet::getVertCoord(int index);
 *  @see GLuint CreateObject(GLfloat* object, int size, const int stride);
 *
 *  @return returns VAO gotten from CreateObject func
 */
GLuint Blockspawner::compileVertices(bool dead) {
    std::vector<GLfloat> tempVert;
    if (dead) {
        int stride = 8;
        for (int i = 0; i < currentblockNum; i++) {
            for (auto& it : blockList[i]) {
                tempVert.push_back(it);
            }
        }
        deadSize = tempVert.size();
        return CreateObject(&tempVert[0], tempVert.size() * sizeof(tempVert[0]), stride, false);
    }
    else return -1;
}

void Blockspawner::genCube(int x, int y, int z) {
    spatialXYZ[x][y] = z;
    std::vector<float> tempBasePoints;
    for (int i = 0; i < 3; i++) {
        float value = (bCamHolder->getCamFloatMapVal(x, y, z, i));
        tempBasePoints.push_back(value);
    }
    int loop = 0, rep = 0, face = 0;
    int pList[4]{ 0 , 3 , 6 , 9 };
    int hList[4]{ 1 , 1 , 1 , 1 };
    for (int b = 0; b < 24; b++) {
        if ((b != 0 && b % 4 == 0)) {
            loop = 0;
            face++;
            switch (face) {
            case 0: hList[0] = 1; hList[1] = 1; hList[2] = 1; hList[3] = 1; break;  //FRONT

            case 1: hList[0] = 0; hList[1] = 0; hList[2] = 0; hList[3] = 0; break;  //BACK

            case 2: pList[0] = 0; pList[1] = 3; pList[2] = 3; pList[3] = 0;         //LEFT
                    hList[0] = 1; hList[1] = 1; hList[2] = 0; hList[3] = 0; break;

            case 3: pList[0] = 9; pList[1] = 6; pList[2] = 6; pList[3] = 9; break;  //RIGHT

            case 4: pList[0] = 0; pList[1] = 9; pList[2] = 9; pList[3] = 0; break;  //TOP

            case 5: pList[0] = 3; pList[1] = 6; pList[2] = 6; pList[3] = 3; break;  //BOT
            }
        }
        rep = pList[loop];
        for (int XYZ = 0; XYZ < 3; XYZ++) {
            blockList[currentblockNum].push_back(
                generateBlockCoord(tempBasePoints[0],
                    tempBasePoints[1],
                    tempBasePoints[2], hList[loop], rep));
            rep++;
        }
        //adds texturecoords
        handleBLockTextureCoords(loop);
        //Color coords, arent used untill dead
        blockList[currentblockNum].push_back(0.6f);
        blockList[currentblockNum].push_back(0.6f);
        blockList[currentblockNum].push_back(0.6f);
        loop++;
    }
}

void Blockspawner::printBlockLContent(int desLayer) {
    int spacer = 1;
    for (auto& it : blockList[desLayer]) {
        printf("%f\t", it);
        if (spacer % 8 == 0) { printf("\n"); }
        spacer++;
    }
}

void Blockspawner::genLblock() {
    genCube(spawnPoint[0], spawnPoint[1], spawnPoint[2]);
    genCube(spawnPoint[0] + 1, spawnPoint[1], spawnPoint[2]);
    genCube(spawnPoint[0] + 2, spawnPoint[1], spawnPoint[2]);
    genCube(spawnPoint[0] + 2, spawnPoint[1] - 1, spawnPoint[2]);
}

void Blockspawner::genZblock() {
    genCube(spawnPoint[0], spawnPoint[1], spawnPoint[2]);
    genCube(spawnPoint[0] + 1, spawnPoint[1], spawnPoint[2]);
    genCube(spawnPoint[0] + 1, spawnPoint[1] - 1, spawnPoint[2]);
    genCube(spawnPoint[0] + 2, spawnPoint[1] - 1, spawnPoint[2]);
}

void Blockspawner::genTblock() {
    genCube(spawnPoint[0], spawnPoint[1], spawnPoint[2]);
    genCube(spawnPoint[0] + 1, spawnPoint[1], spawnPoint[2]);
    genCube(spawnPoint[0] + 2, spawnPoint[1], spawnPoint[2]);
    genCube(spawnPoint[0] + 1, spawnPoint[1] - 1, spawnPoint[2]);
}

float Blockspawner::generateBlockCoord(float x, float y, float z, int mod, int loop) {

    switch (loop) {
    case 0:   return x;            // Top Left
    case 1:   return y;            // Top Left

    case 3:   return x;            // Bot Left
    case 4:   return (y + Yshift); // Bot Left

    case 6:   return (x + Xshift); // Bot Right
    case 7:   return (y + Yshift); // Bot Right

    case 9:   return (x + Xshift); // Top Right
    case 10:  return y;            // Top Right
    default:  return (float(z) + (Zshift * mod));
    }
} 

void Blockspawner::handleBLockTextureCoords(int loop) {
    std::pair <int, int> temp = { 0,0 };
    switch (loop) {
    case 0:                     break;
    case 3:  temp.first = 1;    break;
    case 1:  temp.second = 1;   break;
    case 2:  temp = { 1, 1 };   break;
    default: temp = { -1,-1 };  break;
    }
    blockList[currentblockNum].push_back(temp.first);
    blockList[currentblockNum].push_back(temp.second);
}

/**
 *  Loads texture for map Wall
 *
 *  @see load_opengl_texture(const std::string& filepath, GLuint slot)
 */
void Blockspawner::loadBlockSprite() {
    blockSprite = load_opengl_texture("assets/ghostModelShader.png", 1);
}


void Blockspawner::updateBlockLerp() {
    if (isActive) {
        if (lerpProg >= 1.0f || lerpProg <= 0.0f) {
            lerpProg = 1.0f;
            int newDir = bCamHolder->getNewDesDir();
            if (newDir != -1) { setNewDir(newDir); }
            else { requestChangeDir(); }
        }
        else if (lerpStart != lerpStop) { lerpProg += lerpStep; }
    }
    else {
        newBlock();
    }
}

void Blockspawner::updateBlockDepthLerp() {
    int speedMod = 1;
    if (isActive && queuedHeightDrop) {
        if (heightLerp >= 1.0f || heightLerp < 0.0f) {
            heightLerp = 1.0f;
            queuedHeightDrop = false;
        }
        else if (lerpStart != lerpStop) { if (bCamHolder->updateSpace(false)) { speedMod = 5; } heightLerp += lerpStep * speedMod; }
    }
}

void Blockspawner::requestChangeDir() {
    if (requestedDir != 0) {
        lerpStart[0] = lerpStop[0];
        lerpStart[1] = lerpStop[1];
        lerpStart[2] = lerpStop[2];
        if (getLerpCoords()) {
            lerpProg = lerpStep / 2.0f;
        }
        requestedDir = 0;
    }
}

void Blockspawner::updateHeight(bool space) {
    bool update = true;
    int xDep = 0;
    for (auto& xIt : spatialXYZ) {
        int yDep = 0;
        for (auto& yIt : xIt) {
            if (yIt != 0) {
                if (bCamHolder->getCamIntMapVal(xDep, yDep, yIt - 1) == 0) {
                    yIt--;
                    if (yIt == 0) {
                        yIt--;
                        update = false;
                    }
                }
                else {
                    if (yIt == (depth - 1)) {
                        if (failDelay) { run = false; printf("\n\n\nGame Over\n\n\n"); }
                        failDelay = true;
                    }
                    update = false;
                }
            }
            yDep++;
        }
        xDep++;
    }
    lerpStart[2] = lerpStop[2];
    lerpStop[2] -= Zshift;
    heightLerp = 0.0f;
    if (update) {
        queuedHeightDrop = true;
    }
    else { killBlock(); };
}

bool Blockspawner::getLerpCoords() {
    std::vector<std::vector<int>> tempXYZ = spatialXYZ;
    bool legalmove = true;
    switch (requestedDir) {
    case 2:
        for (int x = tempXYZ.size() - 1; 0 <= x; x--) {
            for (int y = tempXYZ[x].size() - 1; 0 <= y; y--) {
                if (tempXYZ[x][y] != 0) {
                    if ((y + 1) < tempXYZ[x].size()) {
                        if (bCamHolder->getCamIntMapVal(x, y + 1, (depth - tempXYZ[x][y + 1]) - 1) == 0) {
                            tempXYZ[x][y + 1] = tempXYZ[x][y];
                            tempXYZ[x][y] = 0;
                        }
                        else { legalmove = false; }
                    }
                    else { legalmove = false; }
                }
            }
        }
        if (legalmove) lerpStop[1] = lerpStart[1] + Yshift;
        break;     //UP
    case 4:
        for (int x = 0; x < tempXYZ.size(); x++) {
            for (int y = 0; y < tempXYZ[x].size(); y++) {
                if (tempXYZ[x][y] != 0) {
                    if (0 <= (y - 1)) {
                        if (bCamHolder->getCamIntMapVal(x, y - 1, (depth - tempXYZ[x][y - 1]) - 1) == 0) {
                            tempXYZ[x][y - 1] = tempXYZ[x][y];
                            tempXYZ[x][y] = 0;
                        }
                        else { legalmove = false; }
                    }
                    else { legalmove = false; }
                }
            }
        }
        if (legalmove) lerpStop[1] = lerpStart[1] - Yshift;
        break;     //DOWN
    case 3:
        for (int x = 0; x < spatialXYZ.size(); x++) {
            for (int y = 0; y < spatialXYZ[x].size(); y++) {
                if (tempXYZ[x][y] != 0) {
                    if (0 <= (x - 1)) {
                        if (bCamHolder->getCamIntMapVal(x - 1, y, (depth - tempXYZ[x - 1][y]) - 1) == 0) {
                            tempXYZ[x - 1][y] = tempXYZ[x][y];
                            tempXYZ[x][y] = 0;
                        }
                        else { legalmove = false; }
                    }
                    else { legalmove = false; }
                }
            }
        }
        if (legalmove) lerpStop[0] = lerpStart[0] - Xshift;
        break;     //LEFT
    case 9:
        for (int x = spatialXYZ.size() - 1; 0 <= x; x--) {
            for (int y = spatialXYZ[x].size() - 1; 0 <= y; y--) {
                if (tempXYZ[x][y] != 0) {
                    if ((x + 1) < tempXYZ.size()) {
                        if (bCamHolder->getCamIntMapVal(x + 1, y, (depth - tempXYZ[x + 1][y]) - 1) == 0) {
                            tempXYZ[x + 1][y] = tempXYZ[x][y];
                            tempXYZ[x][y] = 0;
                        }
                        else { legalmove = false; }
                    }
                    else { legalmove = false; }
                }
            }
        }
        if (legalmove) lerpStop[0] = lerpStart[0] + Xshift;
        break;     //Right
    default: break;
    }

    if (legalmove) { spatialXYZ = tempXYZ; }
    return legalmove;
}

/**
*  Performs shader transformation for Pacman
*
*  @param shaderprogram - pacmans shaderprogram
*  @param lerpProg      - current pacman lerpprog
*  @param lerpStart     - pacman lerpstartXY
*  @param lerpStop      - pacman lerpstopXY
*/
void Blockspawner::transformBlock() {
    std::vector<float> transformLerpCoords = performLerp();
    glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(transformLerpCoords[0], transformLerpCoords[1], transformLerpCoords[2]));
    GLuint transformationmat = glGetUniformLocation(activeBlockShader, "u_TransformationMat");

    glUniformMatrix4fv(transformationmat, 1, false, glm::value_ptr(translation));
}

std::vector<float> Blockspawner::performLerp() {
    std::vector<float> tempLerpHolder;
    tempLerpHolder.push_back(((1.0f - lerpProg) * lerpStart[0]) + (lerpProg * lerpStop[0]));
    tempLerpHolder.push_back(((1.0f - lerpProg) * lerpStart[1]) + (lerpProg * lerpStop[1]));
    tempLerpHolder.push_back(((1.0f - heightLerp) * lerpStart[2]) + (heightLerp * lerpStop[2]));
    return tempLerpHolder;
}

void Blockspawner::killBlock() {
    if (bCamHolder->updateSpace(false)) { bCamHolder->updateSpace(true); }
    heightLerp = 1.0f;
    std::vector<float> finalizeLerp = performLerp();
    isActive = false;
    int coordTracker = 1, counter = 0;
    for (int i = 0; i < blockList[currentblockNum].size(); i += 8) {
        for (int o = 0; o < 3; o++) {
            blockList[currentblockNum][(i + o)] += (finalizeLerp[o]);
        }
    }
    for (int p = 0; p < width; p++) {
        for (int l = 0; l < height; l++) {
            if (spatialXYZ[p][l] != 0) {
                if (spatialXYZ[p][l] == -1) {
                    bCamHolder->updateCamIntMap(p, l, (spatialXYZ[p][l] + 1));
                }
                else {
                    bCamHolder->updateCamIntMap(p, l, (spatialXYZ[p][l]));
                }
            }
        }
    }
    for (int k = 0; k < blockList[currentblockNum].size(); k += (8*8)) {
        float activeZColor = blockList[currentblockNum][k + 2];
        std::vector<float> newColors = getColorsWithFloat(activeZColor);
        for (int j = k; j < (k + (8*8)); j+=8) { 
            int colorIt = 0;
            for (int h = (j + 5); h < (j + 8); h++) {
                blockList[currentblockNum][h] = newColors[colorIt];
                colorIt++;
            }
        }
    }
    lerpStart[0] = 0; lerpStop[0] = 0;
    lerpStart[1] = 0; lerpStop[1] = 0;
    lerpStart[2] = 0; lerpStop[2] = 0;

    for (auto& xIt : spatialXYZ)
    {
        for (auto& yIt : xIt)
        {
            yIt = 0;
        }
    }
};

std::vector<float> Blockspawner::getColorsWithFloat(float colorDepth) {
    switch (int(colorDepth * 10.0f)) {
    case -10: {std::vector<float> tempColors{ 0.9f,0.0f,0.0f }; return tempColors; } break;
    case -8:  {std::vector<float> tempColors{ 0.5f,0.5f,0.0f }; return tempColors; } break;
    case -6:  {std::vector<float> tempColors{ 0.0f,0.9f,0.0f }; return tempColors; } break;
    case -3:  {std::vector<float> tempColors{ 0.0f,0.5f,0.5f }; return tempColors; } break;
    case -1:  {std::vector<float> tempColors{ 0.0f,0.0f,0.9f }; return tempColors; } break;
    case  0:  {std::vector<float> tempColors{ 0.0f,0.9f,0.9f }; return tempColors; } break;
    case  2:  {std::vector<float> tempColors{ 0.9f,0.9f,0.9f }; return tempColors; } break;
    case  4:  {std::vector<float> tempColors{ 0.9f,0.0f,0.9f }; return tempColors; } break;
    case  6:  {std::vector<float> tempColors{ 0.9f,0.9f,0.0f }; return tempColors; } break;
    case  8:  {std::vector<float> tempColors{ 0.0f,0.0f,0.0f }; return tempColors; } break;
    default: printf("Couldnt find coords\n"); {std::vector<float> tempColors{ 1.0f,1.0f,1.0f }; return tempColors; } break;
    }
}