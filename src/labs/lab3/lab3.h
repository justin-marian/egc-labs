#pragma once

#include "components/simple_scene.h"


/**
 * @class Lab3
 * @brief A 2D animation scene that demonstrates translation, scaling, and rotation.
 *
 * The scene features three animated squares and a bonus solar system-like animation
 * (Sun, Earth, Moon). Objects move, scale, and rotate dynamically.
 */
class Lab3 : public gfxc::SimpleScene
{
public:
    Lab3();
    ~Lab3();

    /**
     * @brief Initializes the scene by setting up the camera, creating objects, and initializing variables.
     */
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

    // Square animation updates
    void UpdateSquare1(float deltaTimeSeconds);
    void UpdateSquare2(float deltaTimeSeconds);
    void UpdateSquare3(float deltaTimeSeconds);
    void UpdateSolarSystem(float deltaTimeSeconds);

private:
    // Transformation matrix for rendering objects
    glm::mat3 modelMatrix;
    // Center coordinates of the square
    float cx, cy;

    // Direction control for translation (1 = forward, 0 = reverse)
    int directionX, directionY;
    int direction;

    // Scaling up/down animation (1 = increase, 0 = decrease)
    int isScalingUp;
    // Distance factor for circular movement (e.g., Sun-Earth distance)
    float squareDistance;
    // Rotation speed for objects
    float rotationSpeed;
    // Radius for rotational movement (solar system effect)
    float rayLength;

    // Translation Variables
    float tx1, ty1;
    float tx3, ty3;

    // Scaling Variables
    float sx2, sy2;

    // Rotation Angles
    float theta_box1;
    float theta_box2;
    float theta_box3;

    // Bonus: Solar System Variables
    float theta_sun;
    float theta_earth;
    float theta_moon;
};
