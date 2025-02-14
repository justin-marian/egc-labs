#pragma once

#include "components/simple_scene.h"

#include <vector>
#include <unordered_map>
#include <string>


/**
 * @class Lab2
 * @brief A 3D scene that handles object rendering, culling, and polygon modes.
 */
class Lab2 : public gfxc::SimpleScene
{
public:
    Lab2();
    ~Lab2();

    void Init() override;

private:
    // Rendering functions
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    // Input handling
    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;

    // Mesh creation utility
    void CreateMesh(
        const std::string& name, 
        const std::vector<VertexFormat>& vertices, 
        const std::vector<unsigned int>& indices
    );
    void CreateCubeMesh(const std::string& name);
    void CreateTetrahedronMesh(const std::string& name);
    void CreateSquareMesh(const std::string& name);
    void CreateCircleMesh(const std::string& name, int numSegments, float radius);


private:
    // Class variables
    ///< Determines which faces are culled
    GLenum cullFace;   
    ///< Polygon rendering mode (fill, line, or point)
    GLenum polygonMode;
    ///< Stores mesh objects
    std::unordered_map<std::string, Mesh*> meshes;
};
