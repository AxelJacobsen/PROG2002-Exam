#include "camera.h"

// -----------------------------------------------------------------------------
// Camera Class function definition
// -----------------------------------------------------------------------------

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

bool Camera::updateSpace(bool update) {
    if (update) {
        if (spaceCalled) {
            spaceCalled = false;
        }
        else {
            spaceCalled = true;
        }
    }
    return spaceCalled;
};