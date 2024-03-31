#include "Scene.h"
#include <iostream>
#include "App.h"
#include "shader.h"
#include "assimp/Importer.hpp"
#include "Model.h"

class Player : public BaseComponent {
    int x = 0;
    void OnStart() {
        //std::cout << "Player Start" << std::endl;
    }
    void OnUpdate(double deltaTime, App app) {
        std::cout << "Player Update: " << x << std::endl;
        x++;
    }
    void OnDestroy() {
        //std::cout << "Player Destroy" << std::endl;
    }
};
class Enemy : public BaseComponent {
    void OnStart() {
        //std::cout << "Enemy Start" << std::endl;
    }
    void OnUpdate(double deltaTime, App app) {
        //std::cout << "Enemy Update" << std::endl;
    }
    void OnDestroy() {
        //std::cout << "Enemy Destroy: " << test << std::endl;
    }
};
class Cube : public BaseComponent {
public:
    Model model = Model("res/backpack/backpack.obj");
    float x = 0;
    void OnStart() {
    }
    void OnUpdate(double deltaTime, App app) {
        shader.Bind();
        shader.SetUniformMat4f("model", transform.GetModelMatrix(x += (25.f * deltaTime)));
        shader.SetUniformMat4f("view", glm::lookAt(glm::vec3(4, 0, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
        shader.SetUniformMat4f("proj", app.proj);
        shader.SetUniform1f("material.shininess", 32.0f);
        shader.SetUniform3f("light.position", 2, 0, 2);
        shader.SetUniform3f("light.ambient", 1.0f, 1.0f, 1.0f);
        shader.SetUniform3f("light.diffuse", 1.0f, 1.0f, 1.0f);
        shader.SetUniform3f("light.specular", 1.0f, 1.0f, 1.0f);
        model.Draw(shader);
    }
    void OnDestroy() {
        
    }
};

int main(void) {
    App app;
    if (!app.CreateWindow({640, 480}, "Raptor Unleashed")) {
        return -1;
    }

    Scene scene;
    Player player;
    //Player player2;
    //Enemy enemy;
    Cube cube;

    Shader shader("res/shaders/Camera.hlsl");

    //scene.Register({ &player, &player2, &enemy });
    scene.Register({ &cube, &player });

    scene.Start();

    double x = 0;
    double speed = .1;

    app.Run([&](double deltaTime) {
        glClearColor(1 - x, 0, x, 1);
        scene.Update(deltaTime, app);
        x += speed * deltaTime;
        if (x > 1 || x < 0) {
            speed *= -1;
        }
        std::cout << "dt = " << deltaTime << "\tf = " << 1 / deltaTime << std::endl;
    });

    scene.Destroy();

    glfwTerminate();
    return 0;
}