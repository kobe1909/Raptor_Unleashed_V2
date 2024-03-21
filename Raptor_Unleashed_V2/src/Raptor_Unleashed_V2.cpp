#include <GLFW/glfw3.h>
#include "Scene.h"
#include <iostream>

class Player : public BaseComponent {
    int x = 0;
    void OnStart() {
        std::cout << "Player Start" << std::endl;
    }
    void OnUpdate() {
        std::cout << "Player Update" << " : " << x << std::endl;
        x++;
    }
    void OnDestroy() {
        std::cout << "Player Destroy" << std::endl;
    }
};
class Enemy : public BaseComponent {
    void OnStart() {
        std::cout << "Enemy Start" << std::endl;
    }
    void OnUpdate() {
        std::cout << "Enemy Update" << std::endl;
    }
    void OnDestroy() {
        std::cout << "Enemy Destroy" << std::endl;
    }
};

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glClearColor(1, 0, 0, 1);

    Scene scene;
    Player player;
    Enemy enemy;

    scene.Register({ &player, &enemy });

    scene.Start();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        scene.Update();
    }

    scene.Destroy();

    glfwTerminate();
    return 0;
}