#pragma once

#include "components/simple_scene.h"


/**
 * @class Lab3_Vis2D
 * @brief A 2D visualization class that handles logical-to-viewport transformations.
 */
class Lab3_Vis2D : public gfxc::SimpleScene
{
public:
    /**
     * @struct ViewportSpace
     * @brief Represents the viewport area in screen space (pixel coordinates).
     */
    struct ViewportSpace
    {
        ViewportSpace() : x(0), y(0), width(1), height(1) {}
        ViewportSpace(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {
        }

        int x;      ///< X position in screen space
        int y;      ///< Y position in screen space
        int width;  ///< Width of the viewport
        int height; ///< Height of the viewport
    };

    /**
     * @struct LogicSpace
     * @brief Defines the logical space for rendering in normalized coordinates.
     */
    struct LogicSpace
    {
        LogicSpace() : x(0), y(0), width(1), height(1) {}
        LogicSpace(float x, float y, float width, float height)
            : x(x), y(y), width(width), height(height) {
        }

        float x;      ///< X position in logical space
        float y;      ///< Y position in logical space
        float width;  ///< Width of the logical rendering space
        float height; ///< Height of the logical rendering space
    };

    // Constructor & Destructor
    Lab3_Vis2D();
    ~Lab3_Vis2D();

    void Init() override;

private:
    // Core rendering functions
    void FrameStart() override;
    void Update(float deltaTimeSeconds) override;
    void FrameEnd() override;
    void DrawScene(glm::mat3 visMatrix);

    // Input handling
    void OnInputUpdate(float deltaTime, int mods) override;
    void OnKeyPress(int key, int mods) override;
    void OnKeyRelease(int key, int mods) override;
    void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
    void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
    void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

    // Transformation functions
    glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
    glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

    /**
     * @brief Configures the viewport area.
     *
     * @param viewSpace Defines the viewport area in screen space.
     * @param backgroundColor Color used to clear the viewport background.
     * @param clear If true, clears the viewport before rendering.
     */
    void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 backgroundColor = glm::vec3(0), bool clear = true);

protected:
    float length;            ///< Size of rendered objects
    ViewportSpace viewSpace; ///< Current viewport space
    LogicSpace logicSpace;   ///< Current logic space
    glm::mat3 modelMatrix;   ///< Transformation matrix for objects
    glm::mat3 visMatrix;     ///< Visualization transformation matrix
};
