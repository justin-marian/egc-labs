#pragma once

#include "components/simple_scene.h"
#include "utils/glm_utils.h"

#include "labs/lab4/transform3D.h"


class Lab4 : public gfxc::SimpleScene
{
public:
    struct ViewportArea
    {
        ViewportArea() : x(0), y(0), width(1), height(1) {}
        ViewportArea(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {
        }
        int x;
        int y;
        int width;
        int height;
    };

    Lab4();
    ~Lab4();

    void Init() override;

private:
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;

    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
    void OnWindowResize(int width, int height) override;

    void RenderScene();

private:
    glm::mat4 modelMatrix;

    // Object Transformations
    float translateX, translateY, translateZ;
    float translateXEarth, translateXSun, translateXMoon;
    float scaleX, scaleY, scaleZ;
    float angularStepOX, angularStepOY, angularStepOZ;
    float angularStepOZEarth, angularStepOZSun, angularStepOZMoon;

    // Viewport
    float scaleMVPA = 10.f;
    bool moveScreen = false;

    GLenum polygonMode;
    ViewportArea miniViewportArea;
};
