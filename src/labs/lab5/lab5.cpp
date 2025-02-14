#include "labs/lab5/lab5.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;


 // ------------------------------------------------------
 // Constructor & Destructor
Lab5::Lab5()
{
    camera = nullptr;
    projectionMatrix = glm::mat4(1.0f);
    renderCameraTarget = false;

    // Default projection mode (1 = Perspective, 0 = Orthographic)
    projectionOrtogonal = 1;
    fov = 60.0f;

    // Define default clipping planes for orthographic projection
    left = -10.0f;
    right = 10.0f;
    bottom = -10.0f;
    top = 10.0f;
}


Lab5::~Lab5()
{
    if (camera)
        delete camera;
}


// ------------------------------------------------------
// Initialization
void Lab5::Init()
{
    // Initialize camera
    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    // Load box mesh
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Load sphere mesh
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Set the initial projection matrix (perspective)
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}


// ------------------------------------------------------
// Frame Start
void Lab5::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set viewport dimensions
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


// ------------------------------------------------------
// Scene Update
void Lab5::Update(float deltaTimeSeconds)
{
    // Render a rotated box at the center
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    // Render a box rotated around the X-axis
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    // Render a simple box without transformations
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
        RenderMesh(meshes["box"], shaders["Simple"], modelMatrix);
    }

    // Render a scaled and rotated box
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(3.f, 2.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(90.f), glm::vec3(1.f, 1.f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.f, .5f, .3f));
        RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
    }

    // Render a rotated and scaled sphere
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2.f, 0.f, 3.f));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(270.f), glm::vec3(1.f, 1.f, 1.f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.8f, 1.9f, 1.9f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }

    // Render the camera target (for visualization purposes)
    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
    }
}


// ------------------------------------------------------
// Frame End
void Lab5::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


// ------------------------------------------------------
// Rendering Function
void Lab5::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


// ------------------------------------------------------
// Input Handling
void Lab5::OnInputUpdate(float deltaTime, int mods)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float cameraSpeed = 10.0f;

        if (window->KeyHold(GLFW_KEY_W))
            camera->TranslateForward(deltaTime * cameraSpeed);

        if (window->KeyHold(GLFW_KEY_A))
            camera->TranslateRight(-deltaTime * cameraSpeed);

        if (window->KeyHold(GLFW_KEY_S))
            camera->TranslateForward(-deltaTime * cameraSpeed);

        if (window->KeyHold(GLFW_KEY_D))
            camera->TranslateRight(deltaTime * cameraSpeed);

        if (window->KeyHold(GLFW_KEY_Q))
            camera->TranslateUpward(-deltaTime * cameraSpeed);

        if (window->KeyHold(GLFW_KEY_E))
            camera->TranslateUpward(deltaTime * cameraSpeed);
    }
}


// ------------------------------------------------------
// Key Press Events

void Lab5::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_T)
        renderCameraTarget = !renderCameraTarget;

    if (key == GLFW_KEY_O)
    {
        projectionOrtogonal = 0;
        projectionMatrix = glm::ortho(left, right, bottom, top, 0.01f, 200.0f);
    }

    if (key == GLFW_KEY_P)
    {
        projectionOrtogonal = 1;
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
    }
}


// ------------------------------------------------------
// Mouse Events
void Lab5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensitivityOX = 0.001f;
        float sensitivityOY = 0.001f;

        camera->RotateFirstPerson_OX(sensitivityOX * -deltaY);
        camera->RotateFirstPerson_OY(sensitivityOY * -deltaX);
    }
}

void Lab5::OnKeyRelease(int key, int mods) {}
void Lab5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Lab5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Lab5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Lab5::OnWindowResize(int width, int height) {}
