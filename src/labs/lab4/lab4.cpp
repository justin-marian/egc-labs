#include "labs/lab4/lab4.h"

#include <vector>
#include <string>
#include <iostream>

#include "labs/lab4/transform3D.h"

using namespace std;


Lab4::Lab4() {
    // Initialize translation variables
    translateX = 0.0f;
    translateY = 0.0f;
    translateZ = 0.0f;
    translateXEarth = 0.0f;
    translateXSun = 0.0f;
    translateXMoon = 0.0f;

    // Initialize scaling variables
    scaleX = 1.0f;
    scaleY = 1.0f;
    scaleZ = 1.0f;

    // Initialize rotation variables
    angularStepOX = 0.0f;
    angularStepOY = 0.0f;
    angularStepOZ = 0.0f;
    angularStepOZEarth = 0.0f;
    angularStepOZSun = 0.0f;
    angularStepOZMoon = 0.0f;

    // Initialize viewport settings
    scaleMVPA = 10.0f;
    moveScreen = false;

    // Initialize rendering settings
    polygonMode = GL_FILL;
    miniViewportArea = ViewportArea(50, 50, 200, 200);
}

Lab4::~Lab4() {}


void Lab4::Init()
{
    polygonMode = GL_FILL;

    // Load box mesh
    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    // Set small viewport resolution
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);
}


void Lab4::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Lab4::RenderScene()
{
    // Static Boxes (Translation, Scaling, Rotation)
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(-2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Translate(translateX, translateY, translateZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(0.0f, 0.5f, -1.5f);
    modelMatrix *= transform3D::Scale(scaleX, scaleY, scaleZ);
    RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(2.5f, 0.5f, -1.5f);
    modelMatrix *= transform3D::RotateOX(angularStepOX);
    modelMatrix *= transform3D::RotateOY(angularStepOY);
    modelMatrix *= transform3D::RotateOZ(angularStepOZ);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // Solar System (Sun, Earth, Moon)
    glm::vec3 sunCenter(0.0f, 2.0f, 2.0f);

    // Sun (Static)
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(sunCenter.x, sunCenter.y, sunCenter.z);
    modelMatrix *= transform3D::RotateOZ(angularStepOZSun);
    modelMatrix *= transform3D::Scale(scaleX + 3, scaleY + 3, scaleZ + 3);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // Earth (Orbits Sun)
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(sunCenter.x, sunCenter.y, sunCenter.z);
    modelMatrix *= transform3D::RotateOZ(angularStepOZEarth);
    modelMatrix *= transform3D::Translate(0.0f, 7.0f, 0.0f);
    modelMatrix *= transform3D::Scale(scaleX + 0.1f, scaleY + 0.1f, scaleZ + 0.1f);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);

    // Moon (Orbits Earth)
    modelMatrix *= transform3D::RotateOZ(angularStepOZMoon);
    modelMatrix *= transform3D::Translate(3.0f, 0.0f, 0.0f);
    modelMatrix *= transform3D::Scale(scaleX - 0.5f, scaleY - 0.5f, scaleZ - 0.5f);
    RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
}


void Lab4::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    // Solar System Rotations
    angularStepOZSun -= deltaTimeSeconds;
    angularStepOZEarth -= deltaTimeSeconds * 2.0f;
    angularStepOZMoon += deltaTimeSeconds * 5.0f;

    // Render full viewport
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
    RenderScene();
    DrawCoordinateSystem();

    // Render Mini Viewport
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    RenderScene();
    DrawCoordinateSystem();
}


void Lab4::FrameEnd() {}


// ------------------------------------------------------
// INPUT EVENTS
void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    float moveSpeed = deltaTime;

    if (moveScreen) {
        if (window->KeyHold(GLFW_KEY_W)) translateZ -= moveSpeed;
        if (window->KeyHold(GLFW_KEY_S)) translateZ += moveSpeed;
        if (window->KeyHold(GLFW_KEY_A)) translateX -= moveSpeed;
        if (window->KeyHold(GLFW_KEY_D)) translateX += moveSpeed;
        if (window->KeyHold(GLFW_KEY_R)) translateY += moveSpeed;
        if (window->KeyHold(GLFW_KEY_F)) translateY -= moveSpeed;
    }

    if (window->KeyHold(GLFW_KEY_1)) scaleX += moveSpeed, scaleY += moveSpeed, scaleZ += moveSpeed;
    if (window->KeyHold(GLFW_KEY_2)) scaleX -= moveSpeed, scaleY -= moveSpeed, scaleZ -= moveSpeed;
    if (window->KeyHold(GLFW_KEY_3)) angularStepOX += moveSpeed;
    if (window->KeyHold(GLFW_KEY_4)) angularStepOX -= moveSpeed;
    if (window->KeyHold(GLFW_KEY_5)) angularStepOY += moveSpeed;
    if (window->KeyHold(GLFW_KEY_6)) angularStepOY -= moveSpeed;
    if (window->KeyHold(GLFW_KEY_7)) angularStepOZ += moveSpeed;
    if (window->KeyHold(GLFW_KEY_8)) angularStepOZ -= moveSpeed;
}


void Lab4::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE) polygonMode = (polygonMode == GL_FILL) ? GL_LINE : GL_FILL;
    if (key == GLFW_KEY_P) moveScreen = !moveScreen;
    if (key == GLFW_KEY_U) miniViewportArea.width += scaleMVPA, miniViewportArea.height += scaleMVPA;
    if (key == GLFW_KEY_O) miniViewportArea.width -= scaleMVPA, miniViewportArea.height -= scaleMVPA;
}


void Lab4::OnKeyRelease(int key, int mods) {}
void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Lab4::OnWindowResize(int width, int height) {}
