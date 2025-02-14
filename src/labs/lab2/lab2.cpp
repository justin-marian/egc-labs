#include "labs/lab2/lab2.h"

#include <vector>
#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;


Lab2::Lab2() : cullFace(GL_BACK), polygonMode(GL_FILL) {}

Lab2::~Lab2()
{
    for (auto& mesh : meshes) {
        delete mesh.second;
    }
}


void Lab2::Init()
{
    // Load predefined mesh
    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes["box"] = mesh;

    // Create custom meshes
    CreateCubeMesh("cube_A");
    CreateCubeMesh("cube_B");
    CreateTetrahedronMesh("tetr_A");
    CreateSquareMesh("sqr");
    CreateCircleMesh("filled_circle", 10, 0.4f); // 10 segments for a smoother circle
}


// ------------------------------------------------------
// HELPER FUNCTION TO CREATE A CUBE MESH (WITH COLORS)
void Lab2::CreateCubeMesh(const std::string& name)
{
	// Define cube vertices with positions and colors
    vector<VertexFormat> vertices = {
        { glm::vec3(0, 0, 1), glm::vec3(0, 1, 1), glm::vec3(0.25, 0.85, 0.65) },
        { glm::vec3(1, 0, 1), glm::vec3(1, 1, 1), glm::vec3(0.25, 0.10, 0.35) },
        { glm::vec3(0, 1, 1), glm::vec3(0, 0, 1), glm::vec3(0.25, 0.00, 0.20) },
        { glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0.10, 0.80, 0.70) },
        { glm::vec3(0, 0, 0), glm::vec3(1, 0, 1), glm::vec3(0.10, 0.70, 0.30) },
        { glm::vec3(1, 0, 0), glm::vec3(0, 1, 1), glm::vec3(0.35, 0.70, 0.45) },
        { glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), glm::vec3(0.35, 0.30, 0.35) },
        { glm::vec3(1, 1, 0), glm::vec3(1, 1, 1), glm::vec3(0.10, 0.55, 0.45) }
    };

	// Define indices for the cube
    vector<unsigned int> indices = {
        0, 1, 2, 1, 3, 2,  // Front
        2, 3, 7, 2, 7, 6,  // Top
        1, 7, 3, 1, 5, 7,  // Right
        6, 7, 4, 7, 5, 4,  // Back
        0, 4, 1, 1, 4, 5,  // Bottom
        2, 6, 4, 0, 2, 4   // Left
    };

    CreateMesh(name, vertices, indices);
}


// ------------------------------------------------------
// HELPER FUNCTION TO CREATE A TETRAHEDRON MESH (WITH COLORS)
void Lab2::CreateTetrahedronMesh(const std::string& name)
{
	// Define tetrahedron vertices with positions and colors
    vector<VertexFormat> vertices = {
        { glm::vec3(0, 0, 0), glm::vec3(1, 1, 0), glm::vec3(1, 0, 0) },
        { glm::vec3(1, 0, 1), glm::vec3(1, 1, 1), glm::vec3(0, 1, 0) },
        { glm::vec3(1, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1) },
        { glm::vec3(0, 1, 1), glm::vec3(1, 0, 0), glm::vec3(1, 1, 0) }
    };

	// Define indices for the tetrahedron
    vector<unsigned int> indices = {
		0, 1, 2,  // Front
		0, 2, 3,  // Right
		0, 3, 1,  // Bottom
		3, 2, 1   // Back
    };

    CreateMesh(name, vertices, indices);
}


// ------------------------------------------------------
// HELPER FUNCTION TO CREATE A SQUARE MESH (WITH COLORS)
void Lab2::CreateSquareMesh(const std::string& name)
{
    // Define square vertices with positions and colors
    vector<VertexFormat> vertices = {
        { glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0, 1, 0), glm::vec3(1.0f, 0.0f, 0.0f) }, // Bottom-left (red)
        { glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0, 1, 0), glm::vec3(0.0f, 1.0f, 0.0f) },  // Bottom-right (green)
        { glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0, 1, 0), glm::vec3(0.0f, 0.0f, 1.0f) }, // Top-left (blue)
        { glm::vec3(0.5f, 0.0f,  0.5f), glm::vec3(0, 1, 0), glm::vec3(1.0f, 1.0f, 0.0f) }   // Top-right (yellow)
    };

    // Define indices for two triangles forming the square
    vector<unsigned int> indices = {
        0, 1, 2, // First triangle
        1, 3, 2  // Second triangle
    };

    // Create the mesh and store it
    CreateMesh(name, vertices, indices);
}



// ------------------------------------------------------
// HELPER FUNCTION TO CREATE A CIRCLE MESH (WITH COLORS)
void Lab2::CreateCircleMesh(const std::string& name, int numSegments, float radius)
{
    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    glm::vec3 center(0.0f, 0.0f, 0.5f);
    vertices.emplace_back(center, glm::vec3(0.5f, 0.5f, 1.0f), glm::vec3(1.0f, 0.2f, 0.2f));

	// Add vertices for the circle points and connect them with indices
    for (int i = 0; i <= numSegments; ++i)
    {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float x = center.x + radius * cos(theta);
        float y = center.y + radius * sin(theta);
        vertices.emplace_back(glm::vec3(x, y, 0.5f), glm::vec3(0.5f, 0.5f, 1.0f), glm::vec3(0.2f, 0.8f, 0.2f));
    }

	// Add indices for the circle triangles
    for (int i = 1; i <= numSegments; ++i)
    {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

	// Close the circle
    indices.push_back(0);
    indices.push_back(numSegments);
    indices.push_back(1);

    CreateMesh(name, vertices, indices);
}


void Lab2::CreateMesh(const std::string& name, const vector<VertexFormat>& vertices, const vector<unsigned int>& indices)
{
    unsigned int VAO, VBO, IBO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexFormat), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Define vertex attributes
    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)0);

    glEnableVertexAttribArray(1); // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)sizeof(glm::vec3));

    glEnableVertexAttribArray(2); // Texture Coordinate
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    glEnableVertexAttribArray(3); // Color
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));

    glBindVertexArray(0);

    // Store the mesh
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}


void Lab2::FrameStart()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Lab2::Update(float deltaTimeSeconds)
{
    glLineWidth(3);
    glPointSize(5);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

    glEnable(GL_CULL_FACE);
    glCullFace(cullFace);

    RenderMesh(meshes["box"], shaders["VertexNormal"], glm::vec3(0, 0.5f, -1.5f), glm::vec3(0.75f));
    RenderMesh(meshes["cube_A"], shaders["VertexColor"], glm::vec3(-1.5f, 0.5f, 0), glm::vec3(0.25f));
    RenderMesh(meshes["cube_B"], shaders["VertexColor"], glm::vec3(1.5f, 0.5f, 0), glm::vec3(0.25f));
    RenderMesh(meshes["tetr_A"], shaders["VertexColor"], glm::vec3(1.5f, 1.5f, 0), glm::vec3(0.5f));
    RenderMesh(meshes["sqr"], shaders["VertexColor"], glm::vec3(1.5f, 0.0f, 0), glm::vec3(0.5f));
    RenderMesh(meshes["filled_circle"], shaders["VertexColor"], glm::vec3(0, 0, 0), glm::vec3(1.0f));

    glDisable(GL_CULL_FACE);
}


void Lab2::FrameEnd()
{
    DrawCoordinateSystem();
}


void Lab2::OnInputUpdate(float deltaTime, int mods) {}


void Lab2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE)
    {
        polygonMode = (polygonMode == GL_FILL) ? GL_LINE :
                      (polygonMode == GL_LINE) ? GL_POINT :
                      GL_FILL;
    }

    if (key == GLFW_KEY_F1)
    {
        cullFace = (cullFace == GL_FRONT) ? GL_BACK : GL_FRONT;
    }
}


void Lab2::OnKeyRelease(int key, int mods) {}
void Lab2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void Lab2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Lab2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Lab2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Lab2::OnWindowResize(int width, int height) {}
