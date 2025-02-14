#include "labs/lab3/lab3.h"

#include <vector>
#include <iostream>

#include "labs/lab3/transform2D.h"
#include "labs/lab3/object2D.h"

using namespace std;


Lab3::Lab3()
{
    // Initialize center coordinates
    cx = 0.0f;
    cy = 0.0f;

    // Initialize translation variables
    tx1 = 0.0f;
    ty1 = 0.0f;
    tx3 = 0.0f;
    ty3 = 0.0f;

    // Initialize scaling variables
    sx2 = 1.0f;
    sy2 = 1.0f; // Start with normal scale (1.0f)

    // Initialize rotation angles
    theta_box1 = 0.0f;
    theta_box2 = 0.0f;
    theta_box3 = 0.0f;

    // Initialize solar system rotation angles
    theta_sun = 0.0f;
    theta_earth = 0.0f;
    theta_moon = 0.0f;

    // Initialize movement-related parameters
    squareDistance = 60.0f;
    rotationSpeed = 0.5f;
    rayLength = 100.0f;

    // Initialize state variables
    isScalingUp = true;
    direction = 1;
    directionX = 1;
    directionY = 1;

    // Initialize transformation matrix as identity
    modelMatrix = glm::mat3(1.0f);
}

Lab3::~Lab3() {}


void Lab3::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, static_cast<float>(resolution.x), 0, static_cast<float>(resolution.y), 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100.0f;

    // Compute square center
    cx = corner.x + squareSide / 2.0f;
    cy = corner.y + squareSide / 2.0f;

    // Create Squares
    vector<pair<string, glm::vec3>> squares = {
        {"square1", glm::vec3(1, 0, 0)}, // Red
        {"square2", glm::vec3(0, 1, 0)}, // Green
        {"square3", glm::vec3(0, 0, 1)}, // Blue
        {"sun", glm::vec3(1, 1, 0)},     // Yellow
        {"earth", glm::vec3(0, 1, 1)},   // Cyan
        {"moon", glm::vec3(1, 1, 1)}     // White
    };

    for (const auto& squareData : squares) {
        const std::string& name = squareData.first;
        const glm::vec3& color = squareData.second;

        Mesh* square = object2D::CreateSquare(name, corner, squareSide, color, true);
        AddMeshToList(square);
    }
}


// ------------------------------------------------------
// Render FUNCTIONS
void Lab3::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab3::Update(float deltaTimeSeconds)
{
    UpdateSquare1(deltaTimeSeconds);
    UpdateSquare2(deltaTimeSeconds);
    UpdateSquare3(deltaTimeSeconds);
    UpdateSolarSystem(deltaTimeSeconds);
}


void Lab3::FrameEnd() {}


// ------------------------------------------------------
// OBJECT UPDATES
void Lab3::UpdateSquare1(float deltaTimeSeconds)
{
    modelMatrix = glm::mat3(1.0f);
    modelMatrix *= transform2D::Translate(250.0f, 250.0f);

    // Vertical movement
    ty1 += (directionY ? 1.0f : -1.0f) * deltaTimeSeconds * 100.0f;
    if (ty1 >= 365.0f || ty1 <= -255.0f) directionY = ~directionY;

    modelMatrix *= transform2D::Translate(tx1, ty1);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
}


void Lab3::UpdateSquare2(float deltaTimeSeconds)
{
    modelMatrix = glm::mat3(1.0f);
    modelMatrix *= transform2D::Translate(400.0f, 400.0f);

    // Scaling animation
    float scaleSpeed = 0.3f * deltaTimeSeconds;
    sx2 += (isScalingUp ? scaleSpeed : -scaleSpeed);
    sy2 += (isScalingUp ? scaleSpeed : -scaleSpeed);
    if (sx2 >= 1.0f || sx2 <= 0.1f) isScalingUp = ~isScalingUp;

    // Rotation animation
    theta_box2 += deltaTimeSeconds * 0.5f;
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Rotate(8.0f * theta_box2);
    modelMatrix *= transform2D::Translate(-cx, -cy);

    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
}


void Lab3::UpdateSquare3(float deltaTimeSeconds)
{
    modelMatrix = glm::mat3(1.0f);
    modelMatrix *= transform2D::Translate(500.0f, 50.0f);

    // Diagonal movement
    float moveSpeed = 50.0f * deltaTimeSeconds;
    tx3 += (direction ? moveSpeed : -moveSpeed);
    ty3 += (direction ? moveSpeed : -moveSpeed);
    if (tx3 >= 400.0f || tx3 <= 50.0f) direction = ~direction;

    // Scaling animation
    modelMatrix *= transform2D::Translate(cx, cy);
    modelMatrix *= transform2D::Scale(sx2, sy2);
    modelMatrix *= transform2D::Translate(-cx, -cy);

    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
}


// ------------------------------------------------------
// BONUS: SOLAR SYSTEM ANIMATION
void Lab3::UpdateSolarSystem(float deltaTimeSeconds)
{
    // Get screen resolution to center the system
    glm::ivec2 resolution = window->GetResolution();
    float centerX = static_cast<float>(resolution.x) / 2.0f;
    float centerY = static_cast<float>(resolution.y) / 2.0f;

    // Sun (stationary at screen center)
    modelMatrix = glm::mat3(1.0f);
    modelMatrix *= transform2D::Translate(centerX, centerY);
    modelMatrix *= transform2D::Scale(0.8f, 0.8f);
    RenderMesh2D(meshes["sun"], shaders["VertexColor"], modelMatrix);

    // Earth (orbits around Sun)
    theta_earth += deltaTimeSeconds * 0.5f;
    modelMatrix = glm::mat3(1.0f);
    modelMatrix *= transform2D::Translate(centerX, centerY);
    modelMatrix *= transform2D::Rotate(4.0f * theta_earth);
    modelMatrix *= transform2D::Translate(squareDistance * 3.5f, squareDistance * 3.5f);
    modelMatrix *= transform2D::Scale(0.4f, 0.4f);
    RenderMesh2D(meshes["earth"], shaders["VertexColor"], modelMatrix);

    // Moon (orbits around Earth)
    theta_moon += deltaTimeSeconds * 0.5f;
    modelMatrix *= transform2D::Rotate(6.0f * theta_moon);
    modelMatrix *= transform2D::Translate(squareDistance * 2.0f, squareDistance * 2.0f);
    modelMatrix *= transform2D::Scale(0.6f, 0.6f);
    RenderMesh2D(meshes["moon"], shaders["VertexColor"], modelMatrix);
}


void Lab3::OnInputUpdate(float deltaTime, int mods) {}
void Lab3::OnKeyPress(int key, int mods) {}
void Lab3::OnKeyRelease(int key, int mods) {}
void Lab3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void Lab3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Lab3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Lab3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Lab3::OnWindowResize(int width, int height) {}
