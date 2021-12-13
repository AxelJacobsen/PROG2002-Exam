// -----------------------------------------------------------------------------
// Class function definition
// -----------------------------------------------------------------------------

#include "blockSpawner.h"

/**

Pacman::Pacman(std::pair<int, int> XY, std::pair<float, float> xyshift) {
    dir = 9, prevDir = 3;
    XYshift = xyshift;
    XYpos[0] = XY.first, XYpos[1] = XY.second;
    lerpProg = 1.0f - lerpStep;
    Character::characterInit();
    pacAnimate();
};


void Pacman::changeDir() {
    bool legal = true;
    if (prevDir == 0) { prevDir = dir; }    //Ensures that prevDir can never be 0 even if you hard-code it :)
    legal = Character::getLegalDir(dir);

    int modDir = prevDir;                   //New mod value used to ensure snappier movement
    if (legal && (dir < prevDir) && (prevDir % dir == 0)) { modDir = dir; } //This fixes problem where smaller numbers got mod'd by a larger number

    if (legal && (dir % modDir == 0) && dir != prevDir) {   //Incase you are trying to turn 180 degrees this procs
        float coordHolder[2];

        coordHolder[0] = lerpStop[0];   coordHolder[1] = lerpStop[1];
        lerpStop[0] = lerpStart[0];     lerpStop[1] = lerpStart[1];
        lerpStart[0] = coordHolder[0];  lerpStart[1] = coordHolder[1];

        if (lerpProg < 0.0f) { lerpProg = 1.0f; }            //This stops skiping a tile if lerpProg is over 1 or under 0
        else if (1.0f < lerpProg) { lerpProg = lerpStep / 2.0f; }
        else { lerpProg = (1 - lerpProg); }

        Character::getLerpCoords();
        prevDir = dir;
    }
    if (legal && (lerpProg <= 0 || lerpProg >= 1)) {  //else this handles updating lerp
        lerpStart[0] = lerpStop[0];
        lerpStart[1] = lerpStop[1];
        Character::getLerpCoords();
        lerpProg = lerpStep / 2.0f;
        prevDir  = dir;
    }
};


void Pacman::updateLerp() {
    if (lerpProg > 1 || lerpProg < 0) { changeDir(); }
    else { lerpProg += lerpStep; }
}


void Pacman::compilePacShader() {
    shaderProgram = CompileShader(  playerVertexShaderSrc,
                                    playerFragmentShaderSrc);

    GLint pposAttrib = glGetAttribLocation(shaderProgram, "pPosition");
    glEnableVertexAttribArray(pposAttrib);
    glVertexAttribPointer(pposAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
}


void Pacman::updateDir(int outDir) {
    dir = outDir;
}


void Pacman::pacAnimate() {
    if (animFlip) { animVal++; }
    else { animVal--; }
    float wMod = (1.0f / 6.0f);
    float hMod = (1.0f / 4.0f);
    float mhMod = hMod;
    float mwMod = (wMod * animVal) + wMod;
    wMod *= animVal;

    Character::characterAnimate(hMod, wMod, mhMod, mwMod);
    if (animVal == 3) animFlip = false;
    else if (animVal == 0) animFlip = true;
}


void Pacman::updateCard(int newDir) {
    cardDir = newDir;
};


int  Pacman::getCard() {
    return cardDir;
}


void Pacman::loadPacSpriteSheet() {
    pacSpriteSheet = load_opengl_texture("assets/pacman.png", 0);
}


void Pacman::deletePacSpriteSheet() {
    glDeleteTextures(1, &pacSpriteSheet);
}


bool Pacman::updatePelletState(bool change) {
    if (change) {
        if (updatePellet) {
            updatePellet = false;
            return true;
        }
        else { updatePellet = true; return false; }
    }
    return updatePellet;
}


void Pacman::updateAnimDel(int val, bool set) {
    if (set)    { animDelay = val; }
    else        { animDelay += val; }
}


GLuint Pacman::compilePacman() {
    std::vector<GLfloat> veticieList;
    int stride = 5;
    for (int i = 0; i < 20; i++) {
        veticieList.push_back(vertices[i]);
    }
    return CreateObject(&veticieList[0], veticieList.size() * sizeof(veticieList[0]), stride);
}


void Pacman::drawPacman() {
    transformPacman();
}


void Pacman::transformPacman() {
    float newX = (((1 - lerpProg) * lerpStart[0]) + (lerpProg * lerpStop[0]));
    float newY = (((1 - lerpProg) * lerpStart[1]) + (lerpProg * lerpStop[1]));

    //LERP performed in the shader for the pacman object
    glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(newX, newY, 0.01f));
    moveCamera(newX - 1, newY);
    GLuint transformationmat = glGetUniformLocation(shaderProgram, "u_TransformationMat");

    glUniformMatrix4fv(transformationmat, 1, false, glm::value_ptr(translation));
}


void Pacman::moveCamera(float x, float y) {
    glm::vec3 oldCam = CamHolder->getCamPos();
    oldCam[0] = (x + (XYshift.first  / 2));
    oldCam[1] = (y + (XYshift.second / 2));
    CamHolder->setCamPos(oldCam);
}

void Pacman::checkForKeyUpdate() {
    setCard(CamHolder->getCard());

    int newPotDir = CamHolder->getNewDesDir();
    if (newPotDir != -10){
        if (getLegalDir(newPotDir)) {
            updateDir(newPotDir);
            changeDir();
        };
    }
}*/