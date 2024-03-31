#include "Scene.h"
#include <iostream>
#include "App.h"
#include "shader.h"
#include "assimp/Importer.hpp"
#include "Model.h"
#include "Light.h"

class Player : public BaseComponent {
    int x = 0;
    void OnStart() {
        //std::cout << "Player Start" << std::endl;
    }
    void OnUpdate(double deltaTime, App& app, Scene& scene) {
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
    void OnUpdate(double deltaTime, App& app, Scene& scene) {
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
    void OnUpdate(double deltaTime, App& app, Scene& scene) {
        shader.Bind();
        shader.SetUniformMat4f("model", transform.GetModelMatrix(x += (25.f * deltaTime)));
        shader.SetUniformMat4f("proj", app.proj);
        shader.SetUniform1f("material.shininess", 32.0f);
        scene.AddLightsToShader(shader);
        scene.AddCameraToShader(shader);
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

    DirectionalLight dirLight(glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f));
    PointLight pointLight(glm::vec3(0.f, 0.f, 2.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), 1, 0.35f, 0.44f);

    Shader shader("res/shaders/Camera.hlsl");

    //scene.Register({ &player, &player2, &enemy });
    scene.Register({ &cube });
    scene.AddLight({ &dirLight, &pointLight });
    scene.camera = Camera(glm::vec3(0, 4, 10), glm::vec3(0, 0, 0));

    scene.Start();

    double x = 0;
    double speed = .1;

    app.Run([&](double deltaTime) {
        glClearColor(1 - x, 0, x, 1);
        scene.Update(deltaTime, app, scene);
        x += speed * deltaTime;
        if (x > 1) {
            x = 1;
            speed *= -1;
        }
        if (x < 0) {
            x = 0;
            speed *= -1;
        }
        std::cout << "dt = " << deltaTime << "\tf = " << 1 / deltaTime << "\t" << x << std::endl;
    });

    scene.Destroy();

    glfwTerminate();
    return 0;
}