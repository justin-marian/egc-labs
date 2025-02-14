#pragma once

#include "components/simple_scene.h"
#include "labs/lab5/lab_camera.h"


// Constants for the near and far clipping planes
constexpr auto Z_NEAR = (0.1f);
constexpr auto Z_FAR = (100.0f);


class Lab5 : public gfxc::SimpleScene
{
public:
    // Constructor & Destructor
    Lab5();
    ~Lab5();

    // Initialization Function
    void Init() override;

private:
    // Frame Lifecycle Functions
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    // Rendering Function
    void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

    // Input Handling
    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;

protected:
    // Camera instance
    implemented::Camera* camera;

    // Projection matrix for rendering
    glm::mat4 projectionMatrix;

    // Boolean flag to toggle camera target visualization
    bool renderCameraTarget;

    // Projection type (0 = Orthographic, 1 = Perspective)
    int projectionOrtogonal;

    // Field of View (FOV) for perspective projection
    GLfloat fov;

    // Orthographic Projection Boundaries
    GLfloat left, right;
    GLfloat bottom, top;
};
