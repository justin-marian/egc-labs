#include "labs/lab3/lab3_vis2D.h"

#include <vector>
#include <iostream>

#include "labs/lab3/transform2D.h"
#include "labs/lab3/object2D.h"

using namespace std;


Lab3_Vis2D::Lab3_Vis2D() {}

Lab3_Vis2D::~Lab3_Vis2D() {}


void Lab3_Vis2D::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Define logical space
    logicSpace = { 0, 0, 4, 4 };  // x, y, width, height

    // Define object properties
    glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
    length = 0.99f;

    // Create and store a square mesh
    Mesh* square1 = object2D::CreateSquare("square1", corner, length, glm::vec3(1, 0, 0));
    AddMeshToList(square1);
}


// ------------------------------------------------------
// VIEWPORT TRANSFORMATIONS
glm::mat3 Lab3_Vis2D::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx = viewSpace.width / logicSpace.width;
    float sy = viewSpace.height / logicSpace.height;
    float tx = viewSpace.x - sx * logicSpace.x;
    float ty = viewSpace.y - sy * logicSpace.y;

    // OpenGL expects column-major matrices
    return glm::mat3(
        sx, 0.0f, 0.0f,
        0.0f, sy, 0.0f,
        tx, ty, 1.0f
    );
}


glm::mat3 Lab3_Vis2D::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx = viewSpace.width / logicSpace.width;
    float sy = viewSpace.height / logicSpace.height;
    float smin = (sx < sy) ? sx : sy;

    float tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    float ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::mat3(
        smin, 0.0f, 0.0f,
        0.0f, smin, 0.0f,
        tx, ty, 1.0f
    );
}


void Lab3_Vis2D::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 color, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glClearColor(color.r, color.g, color.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    auto camera = GetSceneCamera();
    camera->SetOrthographic(viewSpace.x, viewSpace.x + viewSpace.width,
        viewSpace.y, viewSpace.y + viewSpace.height,
        0.1f, 400);
    camera->Update();
}


// ------------------------------------------------------
// Render FUNCTIONS
void Lab3_Vis2D::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Lab3_Vis2D::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Left viewport
    viewSpace = { 0, 0, resolution.x / 2, resolution.y };
    SetViewportArea(viewSpace, glm::vec3(0), true);
    visMatrix = VisualizationTransf2D(logicSpace, viewSpace);
    DrawScene(visMatrix);

    // Right viewport
    viewSpace = { resolution.x / 2, 0, resolution.x / 2, resolution.y };
    SetViewportArea(viewSpace, glm::vec3(0.5f), true);
    visMatrix = VisualizationTransf2DUnif(logicSpace, viewSpace);
    DrawScene(visMatrix);
}


void Lab3_Vis2D::FrameEnd() {}


void Lab3_Vis2D::DrawScene(glm::mat3 visMatrix)
{
    vector<glm::vec2> positions = {
        {0, 0}, {3, 0}, {1.5, 1.5}, {0, 3}, {3, 3}
    };

    for (const auto& pos : positions)
    {
        modelMatrix = visMatrix * transform2D::Translate(pos.x, pos.y);
        RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
    }
}


// ------------------------------------------------------
// INPUT EVENTS
void Lab3_Vis2D::OnInputUpdate(float deltaTime, int mods)
{
    float moveSpeed = 0.5f * deltaTime;
    float scaleSpeed = 5.0f * deltaTime;

    if (window->KeyHold(GLFW_KEY_W)) logicSpace.y += moveSpeed;
    if (window->KeyHold(GLFW_KEY_S)) logicSpace.y -= moveSpeed;
    if (window->KeyHold(GLFW_KEY_A)) logicSpace.x -= moveSpeed;
    if (window->KeyHold(GLFW_KEY_D)) logicSpace.x += moveSpeed;

    if (window->KeyHold(GLFW_KEY_Z))
    {
        logicSpace.x += deltaTime;
        logicSpace.y += deltaTime;
        logicSpace.width -= scaleSpeed;
        logicSpace.height -= scaleSpeed;
    }

    if (window->KeyHold(GLFW_KEY_X))
    {
        logicSpace.x -= deltaTime;
        logicSpace.y -= deltaTime;
        logicSpace.width += scaleSpeed;
        logicSpace.height += scaleSpeed;
    }
}


void Lab3_Vis2D::OnKeyPress(int key, int mods) {}
void Lab3_Vis2D::OnKeyRelease(int key, int mods) {}
void Lab3_Vis2D::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void Lab3_Vis2D::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Lab3_Vis2D::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Lab3_Vis2D::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
