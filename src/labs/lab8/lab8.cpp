#include "labs/lab8/lab8.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;


Lab8::Lab8()
    : materialShininess(30),
    materialKd(0.5f),
    materialKs(0.5f),
    isSpot(0),
    angleCutOff(glm::radians(30.0f))
{
    for (int i = 0; i < 2; i++) {
        point_light_pos[i] = glm::vec3(0, 1, 1 + i);
        point_light_color[i] = glm::vec3(0, 1, 1);
        point_light_dir[i] = glm::vec3(0, -1, 0);
    }
}

Lab8::~Lab8() {}


void Lab8::Init()
{
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PROJECT_PATH, "lab8", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PROJECT_PATH, "lab8", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    angleCutOff = RADIANS(30);

    for (int i = 0; i < 2; i++)
    {
        point_light_pos[i] = glm::vec3(0, 1, 1 + i);
        point_light_color[i] = glm::vec3(0, 1, 1);
        point_light_dir[i] = glm::vec3(0, -1, 0);
    }

    materialShininess = 30;
    materialKd = 0.5;
    materialKs = 0.5;
}


void Lab8::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab8::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    // Pass transformation matrices
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    GLint loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    GLint loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Send lighting uniforms
    glUniform3fv(glGetUniformLocation(shader->program, "light_position"), 2, glm::value_ptr(point_light_pos[0]));
    glUniform3fv(glGetUniformLocation(shader->program, "light_color"), 2, glm::value_ptr(point_light_color[0]));
    glUniform3fv(glGetUniformLocation(shader->program, "light_direction"), 2, glm::value_ptr(point_light_dir[0]));
    glUniform1i(glGetUniformLocation(shader->program, "light_spot"), isSpot);
    glUniform1f(glGetUniformLocation(shader->program, "angle"), angleCutOff);

    // Pass material properties
    glUniform1i(glGetUniformLocation(shader->program, "material_shininess"), materialShininess);
    glUniform1f(glGetUniformLocation(shader->program, "material_kd"), materialKd);
    glUniform1f(glGetUniformLocation(shader->program, "material_ks"), materialKs);

    // Pass object color
    GLint loc_object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(loc_object_color, color.r, color.g, color.b);

    // Bind and render the mesh
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Lab8::Update(float deltaTimeSeconds)
{
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0, 1));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(1, 0, 0));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 1, 0));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, glm::vec3(0.25, 0.75, 0.75));
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 1));
    }

    for (int i = 0; i < 2; i++)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, point_light_pos[i]);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix);
    }
}


void Lab8::FrameEnd()
{
    DrawCoordinateSystem();
}


void Lab8::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 2;

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

        float angIncCutOff = RADIANS(5);

        for (int i = 0; i < 2; i++) {
            if (window->KeyHold(GLFW_KEY_W)) point_light_pos[i] -= forward * deltaTime * speed;
            if (window->KeyHold(GLFW_KEY_A)) point_light_pos[i] -= right * deltaTime * speed;
            if (window->KeyHold(GLFW_KEY_S)) point_light_pos[i] += forward * deltaTime * speed;
            if (window->KeyHold(GLFW_KEY_D)) point_light_pos[i] += right * deltaTime * speed;
            if (window->KeyHold(GLFW_KEY_E)) point_light_pos[i] += up * deltaTime * speed;
            if (window->KeyHold(GLFW_KEY_Q)) point_light_pos[i] -= up * deltaTime * speed;
        }

        if (window->KeyHold(GLFW_KEY_5)) angleCutOff -= deltaTime * RADIANS(10);
        if (window->KeyHold(GLFW_KEY_6)) angleCutOff += deltaTime * RADIANS(10);
    }
}


void Lab8::OnKeyPress(int key, int mods)
{
    if (window->KeyHold(GLFW_KEY_F)) isSpot = (isSpot + 1) % 2;
}


void Lab8::OnKeyRelease(int key, int mods) {}
void Lab8::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void Lab8::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Lab8::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Lab8::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Lab8::OnWindowResize(int width, int height) {}
