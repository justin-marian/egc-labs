#include "labs/lab6/lab6.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;


Lab6::Lab6() {}

Lab6::~Lab6() {}


void Lab6::Init()
{
    // Load an external mesh (box model)
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a simple cube with custom vertices and colors
    {
        vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(-1, -1,  1), glm::vec3(0, 1, 1), glm::vec3(0.2, 0.8, 0.2)),
            VertexFormat(glm::vec3(1, -1,  1), glm::vec3(1, 0, 1), glm::vec3(0.9, 0.4, 0.2)),
            VertexFormat(glm::vec3(-1,  1,  1), glm::vec3(1, 0, 0), glm::vec3(0.7, 0.7, 0.1)),
            VertexFormat(glm::vec3(1,  1,  1), glm::vec3(0, 1, 0), glm::vec3(0.7, 0.3, 0.7)),
            VertexFormat(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1), glm::vec3(0.3, 0.5, 0.4)),
            VertexFormat(glm::vec3(1, -1, -1), glm::vec3(0, 1, 1), glm::vec3(0.5, 0.2, 0.9)),
            VertexFormat(glm::vec3(-1,  1, -1), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.0, 0.7)),
            VertexFormat(glm::vec3(1,  1, -1), glm::vec3(0, 0, 1), glm::vec3(0.1, 0.5, 0.8)),
        };

        vector<unsigned int> indices = {
            0, 1, 2,  1, 3, 2,
            2, 3, 7,  2, 7, 6,
            1, 7, 3,  1, 5, 7,
            6, 7, 4,  7, 5, 4,
            0, 4, 1,  1, 4, 5,
            2, 6, 4,  0, 2, 4,
        };

        CreateMesh("cube", vertices, indices);
    }

    // Load a custom shader for handling normal-based coloring
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PROJECT_PATH, "lab6", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PROJECT_PATH, "lab6", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


Mesh* Lab6::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0, VBO = 0, IBO = 0;

    // Generate VAO, VBO, and IBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexFormat), vertices.data(), GL_STATIC_DRAW);

    // Bind and set IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Set vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    // Unbind VAO
    glBindVertexArray(0);
    CheckOpenGLError();

    // Store mesh
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


void Lab6::FrameStart()
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab6::Update(float deltaTimeSeconds)
{
    float currentTime = Engine::GetElapsedTime();

    // Render three different meshes with unique shaders
    {
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1), glm::vec3(-5, 0, 0));
        shaders["LabShader"]->Use();
        glUniform1f(glGetUniformLocation(shaders["LabShader"]->GetProgramID(), "Time"), currentTime);
        RenderMesh(meshes["box"], shaders["LabShader"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
        shaders["VertexNormal"]->Use();
        glUniform1f(glGetUniformLocation(shaders["VertexNormal"]->GetProgramID(), "Time"), currentTime);
        RenderSimpleMesh(meshes["cube"], shaders["VertexNormal"], modelMatrix);
    }

    {
        glm::mat4 modelMatrix = glm::translate(glm::mat4(1), glm::vec3(5, 0, 0));
        shaders["VertexColor"]->Use();
        glUniform1f(glGetUniformLocation(shaders["VertexColor"]->GetProgramID(), "Time"), currentTime);
        RenderSimpleMesh(meshes["cube"], shaders["VertexColor"], modelMatrix);
    }
}


void Lab6::FrameEnd()
{
    DrawCoordinateSystem();
}


void Lab6::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID()) return;

    glUseProgram(shader->program);

    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Lab6::OnInputUpdate(float deltaTime, int mods) {}
void Lab6::OnKeyPress(int key, int mods) {}
void Lab6::OnKeyRelease(int key, int mods) {}
void Lab6::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void Lab6::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Lab6::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Lab6::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Lab6::OnWindowResize(int width, int height) {}
