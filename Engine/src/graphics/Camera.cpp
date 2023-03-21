#include "Camera.h"

glm::vec3 Engine::Camera::foreward()
{
    return glm::normalize(rotationMat * glm::vec4(0, 0, 1, 0));
}

glm::vec3 Engine::Camera::right()
{
    return glm::normalize(rotationMat * glm::vec4(1, 0, 0, 0));
}

glm::vec3 Engine::Camera::up()
{
    return glm::normalize(rotationMat * glm::vec4(0, 1, 0, 0));
}

void Engine::Camera::Translate(glm::vec3 offset)
{
    this->position += right() * offset.x + up() * offset.y + foreward() * offset.z;
}

void Engine::Camera::Translate(float x, float y, float z)
{
    this->position += right() * x + up() * y + foreward() * z;
}

void Engine::Camera::Rotate(glm::vec3 offset)
{
    this->rotation += offset;
}

void Engine::Camera::Rotate(float x, float y, float z)
{
    this->rotation.x += x;
    this->rotation.y += y;
    this->rotation.z += z;
}

void Engine::Camera::UpdateUniformVariables(unsigned int shaderProgram, glm::vec2 screenScale)
{
    //camPos
    int uniformLocation = glGetUniformLocation(shaderProgram, "camPos");
    glUniform3f(uniformLocation, position.x, position.y, position.z);

    //camRot
    SetRotationMatrix();
    uniformLocation = glGetUniformLocation(shaderProgram, "camRot");
    glUniformMatrix4fv(uniformLocation, 1, false, &rotationMat[0][0]);

    //screenScale
    uniformLocation = glGetUniformLocation(shaderProgram, "screenScale");
    glUniform2f(uniformLocation, screenScale.x, screenScale.y);

    //fov
    uniformLocation = glGetUniformLocation(shaderProgram, "fov");
    glUniform1f(uniformLocation, fov);


    //sunDir <--------- Temp, move to future world class
    uniformLocation = glGetUniformLocation(shaderProgram, "sunDir");
    glUniform3f(uniformLocation, -0.5f, 1, -0.75f);
    
}

void Engine::Camera::SetRotationMatrix()
{
    rotationMat = glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
}
