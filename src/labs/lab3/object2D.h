#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{
    /**
     * @brief Creates a square mesh with the given parameters.
     *
     * @param name Unique name for the mesh.
     * @param leftBottomCorner The bottom-left corner position (world coordinates).
     * @param length The length of the square's sides.
     * @param color The color of the square (applied uniformly to all vertices).
     * @param fill If true, renders a filled square using GL_TRIANGLES. Otherwise, renders only the outline using GL_LINE_LOOP.
     * @return Mesh* A pointer to the created square mesh.
     */
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

} // namespace object2D
