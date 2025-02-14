#include "labs/lab1/lab1.h"

#include <vector>
#include <iostream>
#include <unordered_map>
#include <Core/Engine.h>

using namespace std;


Lab1::Lab1()
{
    object = "box";
    R = G = B = 0;
    A = 1;
    coordX = 4, coordY = 2, coordZ = 2;
}

Lab1::~Lab1()
{
    for (auto& pair : meshes) {
        delete pair.second;
    }
}


void Lab1::LoadMeshHelper(const std::string& name)
{
    Mesh* mesh = new Mesh(name);
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), name + ".obj");
    meshes[name] = mesh;
}

void Lab1::Init()
{
    // Load all meshes
    vector<string> meshNames = { "box", "teapot", "sphere", "quad" };
    for (const auto& name : meshNames) {
        LoadMeshHelper(name);
    }
}

void Lab1::FrameStart() {}

void Lab1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->props.resolution;

    // Set clear color and clear buffers
    glClearColor(R, G, B, A);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set viewport
    glViewport(0, 0, resolution.x, resolution.y);

    // Render objects
    RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));
    RenderMesh(meshes["teapot"], glm::vec3(coordX, coordY, coordZ), glm::vec3(0.5f));
    RenderMesh(meshes["sphere"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));
    RenderMesh(meshes["quad"], glm::vec3(-1, 0.5f, 0), glm::vec3(0.5f));
    RenderMesh(meshes[object], glm::vec3(5, 0.5f, 0), glm::vec3(0.3f));
}

void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}

void Lab1::UpdatePosition(float deltaTime)
{
    if (window->KeyHold(GLFW_KEY_W) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) coordZ += 0.5f * deltaTime;
    if (window->KeyHold(GLFW_KEY_S) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) coordZ -= 0.5f * deltaTime;
    if (window->KeyHold(GLFW_KEY_A) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) coordX += 0.5f * deltaTime;
    if (window->KeyHold(GLFW_KEY_D) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) coordX -= 0.5f * deltaTime;
    if (window->KeyHold(GLFW_KEY_E) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) coordY += 0.5f * deltaTime;
    if (window->KeyHold(GLFW_KEY_Q) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) coordY -= 0.5f * deltaTime;
}

void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    UpdatePosition(deltaTime);
}

void Lab1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_F) {
        // Generate a random RGBA color
        R = rand() / float(RAND_MAX);
        G = rand() / float(RAND_MAX);
        B = rand() / float(RAND_MAX);
        A = 1.0f;  // Keep alpha constant
    }

    if (key == GLFW_KEY_G) {
        static vector<string> objects = { "box", "teapot", "sphere" };
        auto it = find(objects.begin(), objects.end(), object);
        if (it != objects.end()) {
            object = (it + 1 == objects.end()) ? objects.front() : *(it + 1);
        }
    }
}

void Lab1::OnKeyRelease(int key, int mods) {}
void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Lab1::OnWindowResize(int width, int height) {}
