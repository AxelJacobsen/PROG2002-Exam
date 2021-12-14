/**
 *   @file     include.h
 *
 *	Includes all libraries for other files
 *
 *   @author   Axel Jacobsen
 */

#include "initialize.h"

Camera* cameraHolder;

/*
*   Initializes window
*/
GLFWwindow* initializeWindow() {

    // Initialization of GLFW
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed." << '\n';
        std::cin.get();
    }

    // Setting window hints
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    auto window = glfwCreateWindow(1000, 1000, "Block_Out", nullptr, nullptr);

    glfwSetWindowAspectRatio(window, 1000, 1000);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (window == nullptr)
    {
        std::cerr << "GLFW failed on window creation." << '\n';
        std::cin.get();

        glfwTerminate();

    }

    // Setting the OpenGL context.
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    return window;
}

// -----------------------------------------------------------------------------
// MessageCallback (for debugging purposes)
// -----------------------------------------------------------------------------
void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    std::cerr << "GL CALLBACK:" << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") <<
        "type = 0x" << type <<
        ", severity = 0x" << severity <<
        ", message =" << message << "\n";
}

// -----------------------------------------------------------------------------
// Callback Functions
// -----------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_UP:    cameraHolder->setNewDesDir(2);  break;
        case GLFW_KEY_DOWN:  cameraHolder->setNewDesDir(4);  break;
        case GLFW_KEY_LEFT:  cameraHolder->setNewDesDir(3);  break;
        case GLFW_KEY_RIGHT: cameraHolder->setNewDesDir(9);  break;

        case GLFW_KEY_Q: /*CALL DUMBASS FUCKING GARBAGE*/break;
        case GLFW_KEY_W: /*CALL DUMBASS FUCKING GARBAGE*/break;
        case GLFW_KEY_E: /*CALL DUMBASS FUCKING GARBAGE*/break;
        case GLFW_KEY_A: /*CALL DUMBASS FUCKING GARBAGE*/break;
        case GLFW_KEY_S: /*CALL DUMBASS FUCKING GARBAGE*/break;
        case GLFW_KEY_D: /*CALL DUMBASS FUCKING GARBAGE*/break;
        }
    }
}

void recieveCamera(Camera* camPoint) {
    cameraHolder = camPoint;
}