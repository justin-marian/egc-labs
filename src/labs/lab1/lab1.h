#pragma once

#include "components/simple_scene.h"

#include <unordered_map>
#include <string>
#include <vector>


/**
 * @class Lab1
 * @brief A simple 3D scene that allows object rendering, movement, and color changes.
 */
class Lab1 : public gfxc::SimpleScene
{
public:
    Lab1();
    ~Lab1();

    void Init() override;

private:
    // Render loop functions
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

    // Utility functions
    void LoadMeshHelper(const std::string& name);
    void UpdatePosition(float deltaTime);

private:
    // Class variables
    ///< Currently selected object
    std::string object;
    ///< Background color components
    float R, G, B, A;
    ///< Object position
    float coordX, coordY, coordZ;

    // Stores loaded meshes
    std::unordered_map<std::string, Mesh*> meshes;
    std::vector<std::string> objectList = { "box", "teapot", "sphere" };
};
