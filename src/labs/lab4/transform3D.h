#pragma once

#include "utils/glm_utils.h"


namespace transform3D
{
    /**
     * @brief Creates a translation matrix in 3D space.
     *
     * **Initial (Column-major order, before transposition):**
     *
     * | 1  0  0  translateX |
     * | 0  1  0  translateY |
     * | 0  0  1  translateZ |
     * | 0  0  0  1          |
     *
     * **After transposition (Row-major order, OpenGL expects this format):**
     *
     * | 1  0  0  0 |
     * | 0  1  0  0 |
     * | 0  0  1  0 |
     * | translateX  translateY  translateZ  1 |
     *
     * @param translateX Translation along X-axis
     * @param translateY Translation along Y-axis
     * @param translateZ Translation along Z-axis
     * @return glm::mat4 The correctly formatted 4x4 translation matrix
     */
    inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
    {
        return glm::mat4(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            translateX, translateY, translateZ, 1
        );
    }

    /**
     * @brief Creates a scaling matrix in 3D space.
     *
     * **Initial (Column-major order, before transposition):**
     *
     * | scaleX  0       0       0 |
     * | 0       scaleY  0       0 |
     * | 0       0       scaleZ  0 |
     * | 0       0       0       1 |
     *
     * **After transposition (Row-major order, OpenGL expects this format):**
     *
     * | scaleX  0      0      0 |
     * | 0      scaleY  0      0 |
     * | 0      0      scaleZ  0 |
     * | 0      0      0      1 |
     *
     * @param scaleX Scaling factor along X-axis
     * @param scaleY Scaling factor along Y-axis
     * @param scaleZ Scaling factor along Z-axis
     * @return glm::mat4 The correctly formatted 4x4 scaling matrix
     */
    inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
    {
        return glm::mat4(
            scaleX, 0, 0, 0,
            0, scaleY, 0, 0,
            0, 0, scaleZ, 0,
            0, 0, 0, 1
        );
    }

    /**
     * @brief Creates a rotation matrix around the Z-axis.
     *
     * **Initial (Column-major order, before transposition):**
     *
     * | cos(r)  -sin(r)  0  0 |
     * | sin(r)   cos(r)  0  0 |
     * | 0        0       1  0 |
     * | 0        0       0  1 |
     *
     * **After transposition (Row-major order, OpenGL expects this format):**
     *
     * | cos(r)  sin(r)  0  0 |
     * | -sin(r) cos(r)  0  0 |
     * | 0       0       1  0 |
     * | 0       0       0  1 |
     *
     * @param radians Rotation angle in radians
     * @return glm::mat4 The correctly formatted 4x4 rotation matrix around Z-axis
     */
    inline glm::mat4 RotateOZ(float radians)
    {
        return glm::mat4(
            cos(radians), sin(radians), 0, 0,
            -sin(radians), cos(radians), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        );
    }

    /**
     * @brief Creates a rotation matrix around the Y-axis.
     *
     * **Initial (Column-major order, before transposition):**
     *
     * | cos(r)  0  sin(r)  0 |
     * | 0       1  0       0 |
     * | -sin(r) 0  cos(r)  0 |
     * | 0       0  0       1 |
     *
     * **After transposition (Row-major order, OpenGL expects this format):**
     *
     * | cos(r)  0  -sin(r)  0 |
     * | 0       1  0        0 |
     * | sin(r)  0  cos(r)   0 |
     * | 0       0  0        1 |
     *
     * @param radians Rotation angle in radians
     * @return glm::mat4 The correctly formatted 4x4 rotation matrix around Y-axis
     */
    inline glm::mat4 RotateOY(float radians)
    {
        return glm::mat4(
            cos(radians), 0, -sin(radians), 0,
            0, 1, 0, 0,
            sin(radians), 0, cos(radians), 0,
            0, 0, 0, 1
        );
    }

    /**
     * @brief Creates a rotation matrix around the X-axis.
     *
     * **Initial (Column-major order, before transposition):**
     *
     * | 1  0        0         0 |
     * | 0  cos(r)  -sin(r)   0 |
     * | 0  sin(r)   cos(r)   0 |
     * | 0  0        0        1 |
     *
     * **After transposition (Row-major order, OpenGL expects this format):**
     *
     * | 1  0       0        0 |
     * | 0  cos(r)  sin(r)   0 |
     * | 0 -sin(r)  cos(r)   0 |
     * | 0  0       0        1 |
     *
     * @param radians Rotation angle in radians
     * @return glm::mat4 The correctly formatted 4x4 rotation matrix around X-axis
     */
    inline glm::mat4 RotateOX(float radians)
    {
        return glm::mat4(
            1, 0, 0, 0,
            0, cos(radians), sin(radians), 0,
            0, -sin(radians), cos(radians), 0,
            0, 0, 0, 1
        );
    }
}   // namespace transform3D
