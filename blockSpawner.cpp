// -----------------------------------------------------------------------------
// Class function definition
// -----------------------------------------------------------------------------

#include "blockSpawner.h"


void Blockspawner::compileBlockShader() {
    activeBlockShader = CompileShader(blockVertexShaderSrc,
                                    blockFragmentShaderSrc);    
    deadBlockShader = CompileShader(deadBlockVertexShaderSrc,
                                    deadBlockFragmentShaderSrc);

    GLint bposAttrib = glGetAttribLocation(activeBlockShader, "bPosition");
    glEnableVertexAttribArray(bposAttrib);
    glVertexAttribPointer(bposAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    
    GLint dbposAttrib = glGetAttribLocation(activeBlockShader, "dbPosition");
    glEnableVertexAttribArray(dbposAttrib);
    glVertexAttribPointer(dbposAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
}

void Blockspawner::newBlock() {
    std::vector<float> floatVec;
    currentblockNum++;
    isActive = true;
    blockList.push_back(floatVec);
    int bType = createRandomBlock();
    switch (bType)
    {
    case 0: genCube(spawnPoint[0], spawnPoint[1], spawnPoint[2]); printf("Creating Cube \n"); break;
    case 1: genLblock();  printf("Creating L BLock\n");  break;
    case 2: genZblock();  printf("Creating Z BLock\n");  break;
    case 3: genTblock();  printf("Creating T BLock\n");  break;
    default: printf("\nILLEGAL BLOCK TYPE\n");
        break;
    }
    if (currentblockNum != 0) { deadBlockVAO = compileVertices(true); }
    liveBlockVAO = compileVertices();
};

int Blockspawner::createRandomBlock() {
    time_t t;
    srand((unsigned)time(&t));
    int randType = (rand() % 4);
    return randType;
};

void Blockspawner::drawActiveBlocks() {
    GLuint btexAttrib = glGetAttribLocation(activeBlockShader, "bTexcoord");
    glEnableVertexAttribArray(btexAttrib);
    glVertexAttribPointer(btexAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    auto blockTextureLocation     = glGetUniformLocation(activeBlockShader, "u_BlockTexture");

    glUseProgram(activeBlockShader);
    bCamHolder->applycamera(activeBlockShader, width, height);
    transformBlock();
    glBindVertexArray(liveBlockVAO);
    glUniform1i(blockTextureLocation, 1);
    glDrawElements(GL_TRIANGLES, blockList[currentblockNum].size(), GL_UNSIGNED_INT, (const void*)0);
}

void Blockspawner::drawDeadBlocks() {
    GLuint dbtexAttrib = glGetAttribLocation(deadBlockShader, "dbTexcoord");
    glEnableVertexAttribArray(dbtexAttrib);
    glVertexAttribPointer(dbtexAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    auto deadBlockTextureLocation = glGetUniformLocation(deadBlockShader, "u_deadBlockTexture");
    auto deadBlockColorLocation = glGetUniformLocation(deadBlockShader, "u_Color");

    glUseProgram(deadBlockShader);
    bCamHolder->applycamera(deadBlockShader, width, height);
    glBindVertexArray(deadBlockVAO);
    int drawOffset = 0;
    for (int layer = 0; layer < currentblockNum; layer++) {
        glUniform1i(deadBlockTextureLocation, 1);
        glUniform4f(deadBlockColorLocation, abs(blockList[layer][2]), abs(blockList[layer][2]), abs(blockList[layer][2]), 1.0f);
        glDrawElements(GL_TRIANGLES, blockList[currentblockNum].size(), GL_UNSIGNED_INT, (const void*)drawOffset);
        drawOffset += blockList[layer].size();
    }
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
    int stride = 5;
    for (auto& it : blockList[currentblockNum]) {
        tempVert.push_back(it);
    }
    return CreateObject(&tempVert[0], tempVert.size() * sizeof(tempVert[0]), stride);
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
        int stride = 5;
        for (int i = 0; i < currentblockNum; i++) {
            for (auto& it : blockList[i]) {
                tempVert.push_back(it);
            }
        }
        return CreateObject(&tempVert[0], tempVert.size() * sizeof(tempVert[0]), stride);
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
    int pList[4]  { 0 , 3 , 6 , 9 };
    int hList[4]  { 1 , 1 , 1 , 1 };
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
            generateBlockCoord( tempBasePoints[0],
                                tempBasePoints[1],
                                tempBasePoints[2], hList[loop], rep));
            rep++;
        }
        handleBLockTextureCoords(loop);
        loop++;
    }

}
    
void Blockspawner::genLblock() {
    genCube(spawnPoint[0],     spawnPoint[1], spawnPoint[2]);
    genCube(spawnPoint[0] + 1, spawnPoint[1], spawnPoint[2]);
    genCube(spawnPoint[0] + 2, spawnPoint[1], spawnPoint[2]);
    genCube(spawnPoint[0] + 2, spawnPoint[1] - 1, spawnPoint[2]);
}

void Blockspawner::genZblock() {
    genCube(spawnPoint[0],     spawnPoint[1], spawnPoint[2]);
    genCube(spawnPoint[0] + 1, spawnPoint[1], spawnPoint[2]);
    genCube(spawnPoint[0] + 1, spawnPoint[1] - 1, spawnPoint[2]);
    genCube(spawnPoint[0] + 2, spawnPoint[1] - 1, spawnPoint[2]);
}

void Blockspawner::genTblock() {
    genCube(spawnPoint[0],     spawnPoint[1], spawnPoint[2]);
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
    default:  return (float(z) + (Zshift*mod));
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
        else if (lerpStart != lerpStop){ lerpProg += lerpStep; }
    }
    else {
        newBlock();
    }
}

void Blockspawner::updateBlockDepthLerp() {
    if (isActive && queuedHeightDrop) {
        if (heightLerp >= 1.0f || heightLerp < 0.0f) {
            heightLerp = 1.0f; 
            queuedHeightDrop = false;
        }
        else if (lerpStart != lerpStop) { heightLerp += lerpStep; }
    }
}

void Blockspawner::requestChangeDir() {
    if(requestedDir != 0){
        lerpStart[0] = lerpStop[0];
        lerpStart[1] = lerpStop[1];
        lerpStart[2] = lerpStop[2];
        if (getLerpCoords()) {
            lerpProg = lerpStep / 2.0f;
        }
        requestedDir = 0;
    }
}

void Blockspawner::updateHeight() {
    bool update = true;
    int xDep = 0;
    for (auto& xIt : spatialXYZ) { 
        int yDep = 0;
        for (auto& yIt : xIt) { 
            if (yIt != 0){
                if (bCamHolder->getCamIntMapVal(xDep, yDep, yIt) == 0) {
                    if (0 <= yIt){
                        yIt--;
                        if (yIt == 0) { 
                            yIt--;
                            update = false; 
                        } 
                    }
                }
                else { printf("Crashed at X: %i, Y: %i, Z %i\n", xDep, yDep, yIt); update = false; }
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
                        if (bCamHolder->getCamIntMapVal(x, y + 1, (depth - tempXYZ[x][y + 1]) -1) == 0) {
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
                        if (bCamHolder->getCamIntMapVal(x, y - 1, (depth - tempXYZ[x][y - 1]) -1) == 0){
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
                        if (bCamHolder->getCamIntMapVal(x - 1, y, (depth - tempXYZ[x - 1][y]) -1) == 0) {
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
                        if (bCamHolder->getCamIntMapVal(x + 1, y, (depth - tempXYZ[x + 1][y]) -1) == 0) {
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
    std::vector<float> finalizeLerp = performLerp();
    isActive = false; 
    int coordTracker = 1, counter = 0;
    for (int i = 0; i < blockList[currentblockNum].size(); i+=5) {
        for (int o = 0; o < 3; o++) {
            blockList[currentblockNum][(i + o)] += (finalizeLerp[o]);
        }
    }
    for (int p = 0; p < width; p++) {
        for (int l = 0; l < height; l++) {
            if (spatialXYZ[p][l] != 0) { if (spatialXYZ[p][l] == -1) { 
                bCamHolder->updateCamIntMap(p, l, (spatialXYZ[p][l]+1)); } 
            else {
                bCamHolder->updateCamIntMap(p, l, (spatialXYZ[p][l]));
                }
            }
        }
    }

    //bCamHolder->printCamIntMap();

    lerpStart[0] = 0; lerpStop[0] = 0;
    lerpStart[1] = 0; lerpStop[1] = 0;
    lerpStart[2] = 0; lerpStop[2] = 0;

    for (auto& xIt : spatialXYZ) 
        { for (auto& yIt : xIt) 
            { yIt = 0; } 
    } 
};