#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace implemented
{
    class Camera
    {
    public:
        // ---------------------------------------------
        // CONSTRUCTOR & DESTRUCTOR
        // ---------------------------------------------

        Camera()
            : position(0.0f, 2.0f, 5.0f),
            forward(0.0f, 0.0f, -1.0f),
            up(0.0f, 1.0f, 0.0f),
            right(1.0f, 0.0f, 0.0f),
            distanceToTarget(2.0f)
        {
        }

        Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            Set(position, center, up);
        }

        ~Camera() = default;

        // ---------------------------------------------
        // CAMERA SETUP
        // ---------------------------------------------

        void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            this->position = position;
            forward = glm::normalize(center - position);
            right = glm::normalize(glm::cross(forward, up));
            this->up = glm::normalize(glm::cross(right, forward));
        }

        // ---------------------------------------------
        // TRANSLATION FUNCTIONS
        // ---------------------------------------------

        void MoveForward(float distance)
        {
            // Moves forward without affecting the Y-axis
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            // Moves forward along the forward vector
            position += glm::normalize(forward) * distance;
        }

        void TranslateUpward(float distance)
        {
            // Moves along the up vector
            position += glm::normalize(up) * distance;
        }

        void TranslateRight(float distance)
        {
            // Moves along the right vector (projected onto the ground plane)
            glm::vec3 projectedRight = glm::normalize(glm::vec3(right.x, 0, right.z));
            position += projectedRight * distance;
        }

        // ---------------------------------------------
        // FIRST-PERSON ROTATIONS
        // ---------------------------------------------

        void RotateFirstPerson_OX(float angle)
        {
            // Rotates around the OX axis using the right vector
            forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 0)));
            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateFirstPerson_OY(float angle)
        {
            // Rotates around the OY axis (global up)
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
            forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(forward, 0)));
            right = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(right, 0)));
            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateFirstPerson_OZ(float angle)
        {
            // Rotates around the OZ axis
            right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(right, 0)));
            up = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(up, 0)));
        }

        // ---------------------------------------------
        // THIRD-PERSON ROTATIONS
        // ---------------------------------------------

        void RotateThirdPerson_OX(float angle)
        {
            // Rotates the camera around the target on the OX axis
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OY(float angle)
        {
            // Rotates the camera around the target on the OY axis
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(float angle)
        {
            // Rotates the camera around the target on the OZ axis
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        // ---------------------------------------------
        // VIEW MATRIX
        // ---------------------------------------------

        glm::mat4 GetViewMatrix()
        {
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

    public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace implemented
