#include "labs/lab9/lab9.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;


Lab9::Lab9() {}

Lab9::~Lab9() {}


void Lab9::Init()
{
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::PROJECT_PATH, "lab9", "textures");

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "grass_bilboard.png").c_str(), GL_REPEAT);
        mapTextures["grass"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "crate.jpg").c_str(), GL_REPEAT);
        mapTextures["crate"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "earth.png").c_str(), GL_REPEAT);
        mapTextures["earth"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "bamboo", "bamboo.png").c_str(), GL_REPEAT);
        mapTextures["bamboo"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "heightmap.png").c_str(), GL_REPEAT);
        mapTextures["mountain"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "ground.jpg").c_str(), GL_REPEAT);
        mapTextures["ground"] = texture;
    }

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
        Mesh* mesh = new Mesh("bamboo");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "bamboo"), "bamboo.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("mountain");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        vector<glm::vec3> vertices
        {
            glm::vec3(0.5f,   0.5f, 0.0f),    // top right
            glm::vec3(0.5f,  -0.5f, 0.0f),    // bottom right
            glm::vec3(-0.5f, -0.5f, 0.0f),    // bottom left
            glm::vec3(-0.5f,  0.5f, 0.0f),    // top left
        };

        vector<glm::vec3> normals
        {
            glm::vec3(0, 1, 1),
            glm::vec3(1, 0, 1),
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0)
        };

        vector<glm::vec2> textureCoords
        {
            glm::vec2(0.f, 0.f),
            glm::vec2(0.f, 1.f),
            glm::vec2(1.f, 1.f),
            glm::vec2(1.f, 0.f)
        };

        vector<unsigned int> indices =
        {
            0, 1, 3,
            1, 2, 3
        };

        Mesh* mesh = new Mesh("square");
        mesh->InitFromData(vertices, normals, textureCoords, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    mapTextures["random_generated"] = CreateRandomTexture(256, 256);

    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PROJECT_PATH, "lab9", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PROJECT_PATH, "lab9", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("MOUNTAIN");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PROJECT_PATH, "lab9", "shaders", "V_Moutain.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::PROJECT_PATH, "lab9", "shaders", "F_Mountain.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Lab9::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab9::Update(float deltaTimeSeconds)
{
    if (randomTexture == nullptr)
    {
        randomTexture = GetRandomTexture();
    }

    {
        glm::vec3 camera = GetSceneCamera()->m_transform->GetWorldPosition();
        glm::vec3 quad = glm::vec3(0.f, 4.f, 0.0f);

        glm::vec3 direction = glm::normalize(camera - quad);
        float angle = atan2(direction.x, direction.z);

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, quad);
        modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 2, 0));

        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, randomTexture, mapTextures["crate"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 3, -3));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, nullptr, mapTextures["earth"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 5.f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["crate"], mapTextures["earth"]);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 8.f, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["random_generated"], nullptr);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.5f, 3.f, 0.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
        RenderSimpleMesh(meshes["square"], shaders["LabShader"], modelMatrix, mapTextures["grass"], nullptr);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 3.f, -3));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
        RenderSimpleMesh(meshes["bamboo"], shaders["LabShader"], modelMatrix, mapTextures["bamboo"], nullptr);
    }

    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0));
        Render(meshes["mountain"], shaders["MOUNTAIN"], modelMatrix, mapTextures["mountain"], mapTextures["ground"]);
    }
}


void Lab9::FrameEnd()
{
    DrawCoordinateSystem();
}


void Lab9::Render(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
    if (!mesh || !shader || !shader->GetProgramID()) return;

    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint locTime = glGetUniformLocation(shader->program, "time");

    if (texture1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture1"), 0);
    }

    if (texture2) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture2"), 1);
    }

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void Lab9::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture1, Texture2D* texture2)
{
    if (!mesh || !shader || !shader->GetProgramID()) return;

    glUseProgram(shader->program);

    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint locTime = glGetUniformLocation(shader->program, "time");

    if (mesh == meshes["sphere"])
    {
        glUniform1f(locTime, (GLfloat)Engine::GetElapsedTime());
    }
    else
    {
        glUniform1f(locTime, -1.f);
    }

    if (texture1) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }
    else {
        if (texture2) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
            glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
        }
        else {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    if (texture2)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
    }
    else {
        if (texture1)
        {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
            glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 1);
        }
        else
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    if (texture1 && texture2)
    {
        glUniform1f(glGetUniformLocation(shader->program, "mix_factor"), 0.4);
    } else
    {
        glUniform1f(glGetUniformLocation(shader->program, "mix_factor"), 1.0);
    }

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


Texture2D* Lab9::GetRandomTexture() {
    if (mapTextures.empty()) return nullptr;
    int randomIndex = rand() % mapTextures.size();
    auto it = std::next(mapTextures.begin(), randomIndex);
    return it->second;
}


Texture2D* Lab9::CreateRandomTexture(unsigned int width, unsigned int height)
{
    srand(static_cast<unsigned int>(time(NULL)));
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    textureID = rand() % mapTextures.size();

    for (unsigned int x = 0; x < width; x++)
    {
        unsigned char r = static_cast<unsigned char>(rand() % 256);
        unsigned char g = static_cast<unsigned char>(rand() % 256);
        unsigned char b = static_cast<unsigned char>(rand() % 256);

        for (unsigned int y = 0; y < height; y++)
        {
            unsigned int index = (x * width + y) * channels;
            if (index + 2 < size) {
                data[index + 0] = r;
                data[index + 1] = g;
                data[index + 2] = b;
            }
        }
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_NEAREST_MIPMAP_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    CheckOpenGLError();

    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}


void Lab9::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    }
}


void Lab9::OnKeyPress(int key, int mods) {}
void Lab9::OnKeyRelease(int key, int mods) {}
void Lab9::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void Lab9::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Lab9::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Lab9::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Lab9::OnWindowResize(int width, int height) {}
