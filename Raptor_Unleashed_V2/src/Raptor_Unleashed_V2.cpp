#include "Scene.h"
#include <iostream>
#include "App.h"

class Player : public BaseComponent {
    int x = 0;
    void OnStart() {
        //std::cout << "Player Start" << std::endl;
    }
    void OnUpdate() {
        //std::cout << "Player Update: " << x << std::endl;
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
    void OnUpdate() {
        //std::cout << "Enemy Update" << std::endl;
    }
    void OnDestroy() {
        //std::cout << "Enemy Destroy: " << test << std::endl;
    }
};

int main(void) {
    App app;
    if (!app.CreateWindow(640, 480, "Raptor Unleashed")) {
        return -1;
    }

    Scene scene;
    Player player;
    Player player2;
    Enemy enemy;

    scene.Register({ &player, &player2, &enemy });

    scene.Start();

    double x = 0;
    double speed = .1;

    app.Run([&](double deltaTime) {
        scene.Update();
        glClearColor(1 - x, 0, x, 1);
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