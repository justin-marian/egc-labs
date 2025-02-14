#pragma once

#include "utils/glm_utils.h"


namespace transform2D
{
    /**
     * @brief Creates a 2D translation matrix.
     *
     * @param translateX Translation along the X-axis.
     * @param translateY Translation along the Y-axis.
     * @return glm::mat3 The translation matrix.
     *
     * @note The original row-major matrix is:
     *      [ 1   0   translateX ]
     *      [ 0   1   translateY ]
     *      [ 0   0   1          ]
     *
     *       This is transposed for OpenGL (column-major):
     *      [ 1   0   0          ]
     *      [ 0   1   0          ]
     *      [ translateX  translateY  1 ]
     */
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        return glm::mat3(
            1, 0, 0,
            0, 1, 0,
            translateX, translateY, 1  // Transposed (column-major order)
        );
    }

    /**
     * @brief Creates a 2D scaling matrix.
     *
     * @param scaleX Scaling factor along the X-axis.
     * @param scaleY Scaling factor along the Y-axis.
     * @return glm::mat3 The scaling matrix.
     *
     * @note The original row-major matrix is:
     *      [ scaleX   0   0 ]
     *      [ 0   scaleY   0 ]
     *      [ 0     0      1 ]
     *
     *       This is transposed for OpenGL (column-major):
     *      [ scaleX   0      0 ]
     *      [ 0     scaleY   0 ]
     *      [ 0        0      1 ]
     */
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        return glm::mat3(
            scaleX, 0, 0,
            0, scaleY, 0,
            0, 0, 1  // Transposed (column-major order)
        );
    }

    /**
     * @brief Creates a 2D rotation matrix.
     *
     * @param radians Rotation angle in radians (counterclockwise).
     * @return glm::mat3 The rotation matrix.
     *
     * @note The original row-major matrix is:
     *      [ cos(θ)  -sin(θ)  0 ]
     *      [ sin(θ)   cos(θ)  0 ]
     *      [ 0        0       1 ]
     *
     *       This is transposed for OpenGL (column-major):
     *      [ cos(θ)   sin(θ)   0 ]
     *      [ -sin(θ)  cos(θ)   0 ]
     *      [ 0        0        1 ]
     */
    inline glm::mat3 Rotate(float radians)
    {
        float cosTheta = cos(radians);
        float sinTheta = sin(radians);

        return glm::mat3(
            cosTheta, -sinTheta, 0,
            sinTheta, cosTheta, 0,
            0, 0, 1  // Transposed (column-major order)
        );
    }

} // namespace transform2D
