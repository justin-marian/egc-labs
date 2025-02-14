#pragma once

#include <vector>
#include "components/simple_scene.h"


class Lab6 : public gfxc::SimpleScene
{
public:
    Lab6();
    ~Lab6();

    void Init() override;
    Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);

private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;

protected:
    // Projection matrix
    glm::mat4 projectionMatrix;

    // Boolean flag to toggle camera target visualization
    bool renderCameraTarget;

    // Animation time tracking
    float elapsedTime;

    // Scaling and movement factors
    float scaleFactor;
    float moveFactor;
};
