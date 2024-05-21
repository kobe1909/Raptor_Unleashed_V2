#include "Scene.h"
#include <iostream>
#include "App.h"
#include "shader.h"
#include "Model.h"
#include "Light.h"

class Cube : public BaseComponent {
public:
    Model model = Model("res/raptor_delta/scene.gltf");
    double x = 0;
    double speed = 10;
    bool rotate = false;

    Cube(std::string name) {
        this->name = name;
    }

    void OnStart() {
        std::cout << model.textures_loaded.size() << std::endl;
    }
    void OnUpdate(double deltaTime) {
        if (rotate)
            transform.rotation.y += 15 * deltaTime;
    }
    void OnDraw() {
        shader.Bind();
        scene->AddSceneToShader(shader);
        Draw(model);
    }
    void OnDestroy() {
        
    }
};

class Player : public BaseComponent {
public:
    Camera* camera;

    Player(Camera* camera) : camera(camera) {
        this->transform.position = camera->position;
        this->transform.rotation = camera->rotation;
    }

    void OnStart() {
        
    }
    void OnUpdate(double deltaTime) {
        const float sensitivity = 0.1f;
        glm::vec2 mouseOffset = app->mouseOffset;
        mouseOffset *= sensitivity;

        //transform.rotation.y += mouseOffset.x;
        //transform.rotation.x -= mouseOffset.y;
        //std::cout << scene.camera->rotation.x << '\t' << scene.camera->rotation.y << std::endl;

        if (transform.rotation.x > 89.f) {
            transform.rotation.x = 89.f;
        }
        if (transform.rotation.x < -89.f) {
            transform.rotation.x = -89.f;
        }
        camera->SetTransform(transform);

        const float cameraSpeed = 10;
        if (app->GetKeyState(GLFW_KEY_UP))
            transform.position += cameraSpeed * (float)deltaTime * camera->GetFrontVector();
        if (app->GetKeyState(GLFW_KEY_DOWN))
            transform.position -= cameraSpeed * (float)deltaTime * camera->GetFrontVector();
        if (app->GetKeyState(GLFW_KEY_LEFT))
            transform.position -= glm::normalize(glm::cross(camera->GetFrontVector(), camera->GetUpVector())) * (float)deltaTime * cameraSpeed;
        if (app->GetKeyState(GLFW_KEY_RIGHT))
            transform.position += glm::normalize(glm::cross(camera->GetFrontVector(), camera->GetUpVector())) * (float)deltaTime * cameraSpeed;
        
        camera->SetTransform(transform);
    }
    void OnDraw() {

    }
    void OnDestroy() {

    }
};

int main(void) {
    App app;
    if (!app.CreateWindow({ 640 * 2, 480 * 2 }, "Raptor Unleashed", true)) {
        return -1;
    }
    Cube cube("Backpack");
    //Cube cube2("Backpack2");
    //cube2.transform.position.x += 5;

    DirectionalLight dirLight(glm::normalize(glm::vec3(1.f, -2.f, 1.f)), glm::vec3(.7f, .7f, .7f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 0.f, 0.f));
    PointLight pointLight(glm::vec3(0.f, 0.f, 4.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 0.f, 0.f), 1, 0.35f, 0.44f);

    Camera camera(glm::vec3(-2, 0.5f, 2), glm::vec3(0, -45.f, 0));
    Player player(&camera);

    Scene scene(&app, { cube, player }, { dirLight, pointLight }, camera);

    scene.Start();

    double x = 0;
    double speed = .1;
    bool rotate = false;
    float position = 0;

    app.Run([&](double deltaTime) {
        //glClearColor(1 - x, 0, x, 1);
        glClearColor(1, 1, 1, 1);
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

        scene.Update(deltaTime);
        scene.Draw();

        ImGui::Begin("Debug window");
        ImGui::Text("text");
        ImGui::Checkbox("Rotate Backpack 1", &cube.rotate);
        //ImGui::Checkbox("Rotate Backpack 2", &cube2.rotate);
        ImGui::End();
    });

    scene.Destroy();
    app.CleanUp();

    glfwTerminate();
    return 0;
}