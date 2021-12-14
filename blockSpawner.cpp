// -----------------------------------------------------------------------------
// Class function definition
// -----------------------------------------------------------------------------

#include "blockSpawner.h"


void Blockspawner::compileBlockShader() {
    blockShader = CompileShader(blockVertexShaderSrc,
                                blockFragmentShaderSrc);

    GLint bposAttrib = glGetAttribLocation(blockShader, "bPosition");
    glEnableVertexAttribArray(bposAttrib);
    glVertexAttribPointer(bposAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
}

void Blockspawner::newBlock() {
    std::vector<float> floatVec;
    currentblockNum++;
    if (currentblockNum != 0) { deadBlockVAO = compileVertices(true); };
    isActive = true;
    blockList.push_back(floatVec);
    int bType = createRandomBlock();
    switch (bType)
    {
    case 0: genCube(0,4,9); break;
    case 1: genLblock();    break;
    case 2: genZblock();    break;
    case 3: genTblock();    break;
    default: printf("\nILLEGAL BLOCK TYPE\n");
        break;
    }
};

int Blockspawner::createRandomBlock() {
    time_t t;
    srand((unsigned)time(&t));
    int randType = (rand() % 4);
    return randType;
};

void Blockspawner::drawActiveBlocks() {
    liveBlockVAO = compileVertices();
    
    GLuint btexAttrib = glGetAttribLocation(blockShader, "bTexcoord");
    glEnableVertexAttribArray(btexAttrib);
    glVertexAttribPointer(btexAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    auto blockTextureLocation     = glGetUniformLocation(blockShader, "u_BlockTexture");

    glUseProgram(blockShader);
    bCamHolder->applycamera(blockShader, width, height);
    glBindVertexArray(liveBlockVAO);
    glUniform1i(blockTextureLocation, 1);
    glDrawElements(GL_TRIANGLES, blockList[currentblockNum].size(), GL_UNSIGNED_INT, (const void*)0);
    CleanVAO(liveBlockVAO);
}

void Blockspawner::drawDeadBlocks() {

    GLuint btexAttrib = glGetAttribLocation(blockShader, "bTexcoord");
    glEnableVertexAttribArray(btexAttrib);
    glVertexAttribPointer(btexAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    auto blockTextureLocation = glGetUniformLocation(blockShader, "u_BlockTexture");

    glUseProgram(blockShader);
    bCamHolder->applycamera(blockShader, width, height);
    glBindVertexArray(deadBlockVAO);
    int drawOffset = 0;
    for (int layer = 0; layer < currentblockNum; layer++) {
        printf("Wild Wild West Textures: %i", int(blockList[layer][2]));
        glUniform1i(blockTextureLocation, int(blockList[layer][2]));
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
            for (auto& it : blockList[currentblockNum]) {
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
    genCube(0, 4, 9);
    genCube(1, 4, 9);
    genCube(2, 4, 9);
    genCube(2, 3, 9);
}

void Blockspawner::genZblock() {
    genCube(0, 4, 9);
    genCube(1, 4, 9);
    genCube(1, 3, 9);
    genCube(2, 3, 9);
}

void Blockspawner::genTblock() {
    genCube(0, 4, 9);
    genCube(1, 4, 9);
    genCube(2, 4, 9);
    genCube(1, 3, 9);
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
        if (lerpProg > 1 || lerpProg < 0) { requestChangeDir(); }
        else if (lerpStart != lerpStop) { lerpProg += lerpStep; }
        if (lerpProg < 0.6f && lerpProg > 0.5f) {
            updateHeight();
        }
    }
    else {
        newBlock();
    }
}

void Blockspawner::requestChangeDir() {
    if(requestedDir != 0){
        if (checkIfLegalDir(requestedDir)) {  //else this handles updating lerp
            lerpStart[0] = lerpStop[0];
            lerpStart[1] = lerpStop[1];
            lerpStart[2] = lerpStop[2];
            Blockspawner::getLerpCoords();
            lerpProg = lerpStep / 2.0f;
        }
    }
}

bool Blockspawner::checkIfLegalDir(int newDir) {
    return true;
}

void Blockspawner::updateHeight() {
    for (auto& xIt : spatialXYZ) { 
        for (auto& yIt : xIt) { 
            if (0 < yIt) { yIt--; }
        } 
    }
}

void Blockspawner::getLerpCoords() {

}

void Blockspawner::checkIfHitEnd() {

}