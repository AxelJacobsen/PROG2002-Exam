#include "camera.h"

// -----------------------------------------------------------------------------
// Camera Class function definition
// -----------------------------------------------------------------------------
/**
 *  returns coordinates based on give x y and "loop"
 *
 *  @param y     - y coordinate for coords
 *  @param x     - x coordinate for coords
 *  @param loop  - current repitition / desired vertice
 *  @param layer - height of coords
 *  @param shift - width and height of a square, sent in a pair
 *
 *  @return returns float for correct coord
 */
GLfloat Camera::getCoordsWithInt(int y, int x, int loop, float Xshift, float Yshift, float Zshift) {
    GLfloat tempXs, tempYs;
    if (x == 0 && y == 0) { tempXs = 0, tempYs = 0; }
    else { tempXs = (Xshift * float(x)), tempYs = (Yshift * float(y)); }

    switch (loop) {
    case 0:   tempXs;             return (tempXs - 1.0f);  // Bot Left
    case 1:   tempYs;             return (tempYs - 1.0f);  // Bot Left

    case 3:   tempXs;             return (tempXs - 1.0f);  // Top Left
    case 4:   tempYs += Yshift;   return (tempYs - 1.0f);  // Top Left

    case 6:   tempXs += Xshift;   return (tempXs - 1.0f);  // Top Right
    case 7:   tempYs += Yshift;   return (tempYs - 1.0f);  // Top Right

    case 9:   tempXs += Xshift;   return (tempXs - 1.0f);  // Bot Right
    case 10:  tempYs;             return (tempYs - 1.0f);  // Bot Right
    default: return Zshift;
    }
};

/**
 *  Applies camera transformation to supplied shader
 *
 *  @param shader - shader to apply transformation to
 *  @param width  - width of screeen
 *  @param height - height of screen
 *
 */
void Camera::applycamera(const GLuint shader, const float width, const float height) {
    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(fov), (float)(width) / (float)(height), 0.001f, 10.0f);
    GLuint projMat = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(projMat, 1, false, glm::value_ptr(projection));
    
    // camera/view transformation
    glm::mat4 view = glm::lookAt(cameraPos, (cameraPos + cameraFront), cameraUp);
    GLuint viewMat = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(viewMat, 1, false, glm::value_ptr(view));
}

void Camera::printCamIntMap() {
    for (auto& xIt : gridIntHolder) {
        for (auto& yIt : xIt) {
            for (auto& zIt : yIt) {
            
                printf("%i ", zIt);
            }
            printf("\n");
        }
        printf("\n");
    }
}