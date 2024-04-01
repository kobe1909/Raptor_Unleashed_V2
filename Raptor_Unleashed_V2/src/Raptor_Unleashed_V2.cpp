#include "Scene.h"
#include <iostream>
#include "App.h"
#include "shader.h"
#include "assimp/Importer.hpp"
#include "Model.h"
#include "Light.h"

class Cube : public BaseComponent {
public:
    Model model = Model("res/backpack/backpack.obj");
    double x = 0;
    double speed = 10;

    void OnStart() {
    }
    void OnUpdate(double deltaTime, App& app, Scene& scene) {
        shader.Bind();
        shader.SetUniformMat4f("model", transform.GetModelMatrix());
        shader.SetUniformMat4f("proj", app.proj);
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
    Cube cube;

    DirectionalLight dirLight(glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f));
    PointLight pointLight(glm::vec3(0.f, 0.f, 2.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), 1, 0.35f, 0.44f);

    Camera camera(glm::vec3(0, 4, 10), glm::vec3(0, 0, 0));

    Scene scene({ &cube }, { &dirLight, &pointLight }, camera);

    scene.Start();

    double x = 0;
    double speed = .1;
    bool rotate = false;
    float position = 0;

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

        if (rotate)
            cube.transform.rotation.y += 15 * deltaTime;
        cube.transform.position.x = position;

        ImGui::Begin("Debug window");
        ImGui::Text("text");
        ImGui::Checkbox("Rotate Backpack", &rotate);
        ImGui::SliderFloat("Position", &position, -4, 4);
        ImGui::End();
    });

    scene.Destroy();
    app.CleanUp();

    glfwTerminate();
    return 0;
}