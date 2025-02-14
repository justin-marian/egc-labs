#include "labs/lab7/lab7.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;


Lab7::Lab7() :
    lightPosition(glm::vec3(1, 1, 1)), 
    materialShininess(30), materialKd(0.5f), materialKs(0.5f) {}

Lab7::~Lab7() {}


void Lab7::Init()
{
    auto loadMesh = [this](const std::string& meshName, const std::string& path) {
        auto mesh = std::make_unique<Mesh>(meshName);
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, path), meshName + ".obj");
        meshes[mesh->GetMeshID()] = std::move(mesh);
        };

    loadMesh("box", "primitives");
    loadMesh("sphere", "primitives");
    loadMesh("plane50", "primitives");
    loadMesh("bunny", "animals");

    auto shader = std::make_unique<Shader>("LabShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PROJECT_PATH, "lab7", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PROJECT_PATH, "lab7", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = std::move(shader);
}


void Lab7::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


// Update the scene by rendering the objects
void Lab7::Update(float deltaTimeSeconds)
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    // Render sphere at a specific position
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 1));

    // Render bunny with scaling and translation
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f));
    RenderSimpleMesh(meshes["bunny"], shaders["LabShader"], modelMatrix, glm::vec3(1, 1, 1));

    // Render box with transformation (translation, rotation, and scaling)
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix);

    // Render another box with different transformations
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 1, 0));
    RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0, 1, 0));

    // Render ground (plane)
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
    RenderSimpleMesh(meshes["plane50"], shaders["LabShader"], modelMatrix, glm::vec3(0.5, 0.5, 0.5));

    // Render the light source (sphere at light position)
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, lightPosition);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
    RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 1));
}


void Lab7::FrameEnd()
{
    DrawCoordinateSystem();
}


void Lab7::RenderSimpleMesh(
    const std::unique_ptr<Mesh>& mesh,
    const std::unique_ptr<Shader>& shader,
    const glm::mat4& modelMatrix,
    const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    // Set the light and material properties as shader uniforms
    glUniform3fv(glGetUniformLocation(shader->program, "light_position"), 1, glm::value_ptr(lightPosition));
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    glUniform3fv(glGetUniformLocation(shader->program, "eye_position"), 1, glm::value_ptr(eyePosition));

    // Set material properties: shininess, diffuse (kd), and specular (ks) coefficients
    glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), materialShininess);
    glUniform1f(glGetUniformLocation(shader->program, "material_kd"), materialKd);
    glUniform1f(glGetUniformLocation(shader->program, "material_ks"), materialKs);
    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));

    // Bind transformation matrices: Model, View, and Projection
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object using the mesh's VAO
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Lab7::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 2;
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));  // Keep the light on the XZ plane

        if (window->KeyHold(GLFW_KEY_W)) lightPosition -= forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_A)) lightPosition -= right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_S)) lightPosition += forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_D)) lightPosition += right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_E)) lightPosition += up * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_Q)) lightPosition -= up * deltaTime * speed;
    }
}


void Lab7::OnKeyPress(int key, int mods) {}
void Lab7::OnKeyRelease(int key, int mods) {}
void Lab7::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void Lab7::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Lab7::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Lab7::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Lab7::OnWindowResize(int width, int height) {}
