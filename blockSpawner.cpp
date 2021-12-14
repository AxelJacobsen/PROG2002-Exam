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
    isActive = true;
    blockList.push_back(floatVec);
    int bType = createRandomBlock();
    switch (bType)
    {
    case 0: genCube();   break;
    case 1: genLblock(); break;
    case 2: genZblock(); break;
    case 3: genTblock(); break;
    default: printf("\nILLEGAL BLOCK TYPE\n");
        break;
    }
};

int Blockspawner::createRandomBlock() {
        time_t t;
        srand((unsigned)time(&t));
        int randType = (rand() % 4);        
        //UNTILL FURHTER ADO THIS DONT DO SHIT

        return 0;
};

void Blockspawner::drawBlocks() {
    auto blockVAO = compileVertices();
    
    GLuint btexAttrib = glGetAttribLocation(blockShader, "bTexcoord");
    glEnableVertexAttribArray(btexAttrib);
    glVertexAttribPointer(btexAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    auto blockVertexColorLocation = glGetUniformLocation(blockShader, "u_Color");
    auto blockTextureLocation     = glGetUniformLocation(blockShader, "u_BlockTexture");

    glUseProgram(blockShader);
    glUniform1i(blockTextureLocation, 1);
    bCamHolder->applycamera(blockShader, 5, 5);
    glBindVertexArray(blockVAO);
    glUniform4f(blockVertexColorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
    glDrawElements(GL_TRIANGLES, vertices.size(), GL_UNSIGNED_INT, (const void*)0);
    CleanVAO(blockVAO);
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
    vertices.clear();
    int stride = 5;
    for (int i = 0; i < blockList.size(); i++){
        for (auto& it : blockList[i]) {
            vertices.push_back(it);
        }
    }
    return CreateObject(&vertices[0], vertices.size() * sizeof(vertices[0]), stride);
}

void Blockspawner::genCube() {
    std::vector<float> tempBasePoints;
    for (int i = 0; i < 3; i++) {
        float value = (bCamHolder->getCamFloatMapVal(2, 3, 10, i));
        tempBasePoints.push_back(value);
    }
    int loop = 0, rep = 0, face = 0;
    int pList[4]  { 0 , 3 , 6 , 9 };
    int hList[4]  { 1 , 1 , 1 , 1 };
    for (int b = 1; b < 25; b++) {
        if ((b != 0 && b % 4 == 0)) { 
            loop = 0;
            switch (face) {
            case 0: hList[0] = 1; hList[1] = 1; hList[2] = 1; hList[3] = 1; break;

            case 1: hList[0] = 0; hList[1] = 0; hList[2] = 0; hList[3] = 0; break;

            case 2: pList[0] = 0; pList[1] = 3; pList[2] = 3; pList[3] = 0;
                    hList[0] = 1; hList[1] = 1; hList[2] = 0; hList[3] = 0; break;

            case 3: pList[0] = 9; pList[1] = 6; pList[2] = 6; pList[3] = 9; break;

            case 4: pList[0] = 0; pList[1] = 9; pList[2] = 9; pList[3] = 0; break;

            case 5: pList[0] = 3; pList[1] = 6; pList[2] = 6; pList[3] = 3; break;
            }
            face++;
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
    int bop = 0;
    printf("Begin Cube points print\n");
    for (auto& it : blockList[currentblockNum]) {
        if (bop % 5 == 0) { printf("\n");
        }
        printf("%f\t",it); 
        bop++;
    }
    printf("End Cube points print\n");
}
    
void Blockspawner::genLblock() {

}

void Blockspawner::genZblock() {

}

void Blockspawner::genTblock() {

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