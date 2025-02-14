#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    // Define square vertices with positions and colors
    std::vector<VertexFormat> vertices = {
        VertexFormat(corner, color),                                       // Bottom-left  (0)
        VertexFormat(corner + glm::vec3(length, 0, 0), color),             // Bottom-right (1)
        VertexFormat(corner + glm::vec3(length, length, 0), color),        // Top-right    (2)
        VertexFormat(corner + glm::vec3(0, length, 0), color)              // Top-left     (3)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices;

    if (!fill) {
        // Outline mode: GL_LINE_LOOP ensures proper edge drawing
        indices = { 0, 1, 2, 3 };
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Filled mode: Two triangles forming the square
        indices = { 0, 1, 2, 0, 2, 3 };
        square->SetDrawMode(GL_TRIANGLES);
    }

    square->InitFromData(vertices, indices);
    return square;
}
