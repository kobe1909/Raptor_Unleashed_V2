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

    Cube(std::string name) {
        this->name = name;
    }

    void OnStart() {
        std::cout << "hello world" << std::endl;
    }
    void OnUpdate(double deltaTime) {
    }
    void OnDraw() {
        shader.Bind();
        shader.SetUniformMat4f("model", transform.GetModelMatrix());
        shader.SetUniformMat4f("proj", app->proj);
        scene->AddLightsToShader(shader);
        scene->AddCameraToShader(shader);
        model.Draw(shader);
    }
    void OnDestroy() {
        
    }
};

int main(void) {
    App app;
    if (!app.CreateWindow({ 640, 480 }, "Raptor Unleashed")) {
        return -1;
    }
    Cube cube("Backpack");
    Cube cube2("Backpack2");
    cube2.transform.position.x += 5;

    DirectionalLight dirLight(glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f));
    PointLight pointLight(glm::vec3(0.f, 0.f, 2.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), 1, 0.35f, 0.44f);

    Camera camera(glm::vec3(0, 4, 10), glm::vec3(0, -90.f, 0));

    Scene scene(&app, { cube, cube2 }, { dirLight, pointLight }, camera);

    scene.Start();

    double x = 0;
    double speed = .1;
    bool rotate = false;
    float position = 0;

    //std::cout << scene.GetObjectByName<Cube>("backpack").speed << std::endl;

    app.Run([&](double deltaTime) {
        glClearColor(1 - x, 0, x, 1);
        x += speed * deltaTime;
        if (x > 1) {
            x = 1;
            speed *= -1;
        }
        if (x < 0) {
            x = 0;
            speed *= -1;
        }
        //std::cout << "dt = " << deltaTime << "\tf = " << 1 / deltaTime << "\t" << x << std::endl;
        rotate = app.GetKeyState(GLFW_KEY_R);
        if (rotate)
            cube.transform.rotation.y += 15 * deltaTime;
        cube.transform.position.x = position;

        const float sensitivity = 0.1f;
        glm::vec2 mouseOffset = app.mouseOffset;
        mouseOffset *= sensitivity;

        scene.camera.rotation.y += mouseOffset.x;
        scene.camera.rotation.x -= mouseOffset.y;
        std::cout << scene.camera.rotation.x << '\t' << scene.camera.rotation.y << std::endl;

        if (scene.camera.rotation.x > 89.f) {
            scene.camera.rotation.x = 89.f;
        }   
        if (scene.camera.rotation.x < -89.f) {
            scene.camera.rotation.x = -89.f;
        }

        const float cameraSpeed = 10;
        if (app.GetKeyState(GLFW_KEY_UP))
            scene.camera.position += cameraSpeed * (float)deltaTime * scene.camera.GetFrontVector();
        if (app.GetKeyState(GLFW_KEY_DOWN))
            scene.camera.position -= cameraSpeed * (float)deltaTime * scene.camera.GetFrontVector();
        if (app.GetKeyState(GLFW_KEY_LEFT))
            scene.camera.position -= glm::normalize(glm::cross(scene.camera.GetFrontVector(), scene.camera.GetUpVector())) * (float)deltaTime * cameraSpeed;
        if (app.GetKeyState(GLFW_KEY_RIGHT))
            scene.camera.position += glm::normalize(glm::cross(scene.camera.GetFrontVector(), scene.camera.GetUpVector())) * (float)deltaTime * cameraSpeed;

        scene.Update(deltaTime);
        scene.Draw();

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