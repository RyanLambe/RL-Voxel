#include "Camera.h"

Engine::Camera::Camera()
{
    prevPos = position;
    prevRot = rotation;
    rotationMat = glm::mat4(1);
}

Engine::Camera::~Camera()
{
    glDeleteTextures(1, &prevFrame);
}

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

void Engine::Camera::UpdateUniformVariables(unsigned int shaderProgram)
{
    //Transformation data
    //camPos
    int uniformLocation = glGetUniformLocation(shaderProgram, "camPos");
    glUniform3f(uniformLocation, position.x, position.y, position.z);

    //camRot
    SetRotationMatrix();
    uniformLocation = glGetUniformLocation(shaderProgram, "camRot");
    glUniformMatrix4fv(uniformLocation, 1, false, &rotationMat[0][0]);


    //Camera Settings
    //screenScale
    uniformLocation = glGetUniformLocation(shaderProgram, "screenScale");
    glUniform2f(uniformLocation, screenScale.x, screenScale.y);

    //fov
    uniformLocation = glGetUniformLocation(shaderProgram, "fov");
    glUniform1f(uniformLocation, fov);


    //Progressive rendering
    if (position != prevPos || rotation != prevRot) {
        frame = 1;
        prevPos = position;
        prevRot = rotation;
    }

    if (frame == 1) {
        uniformLocation = glGetUniformLocation(shaderProgram, "prevFrame");
        glUniform1i(uniformLocation, 0);
        frame = 1;
    }

    uniformLocation = glGetUniformLocation(shaderProgram, "frame");
    glUniform1i(uniformLocation, frame);

    uniformLocation = glGetUniformLocation(shaderProgram, "time");
    glUniform1f(uniformLocation, (GLfloat)glfwGetTime());


    //sunDir <--------- Temp, move to future world class
    uniformLocation = glGetUniformLocation(shaderProgram, "sunDir");
    glUniform3f(uniformLocation, -0.5f, 1, -0.75f);
    
}

void Engine::Camera::Enable(glm::vec2 size)
{
    screenScale = size;

    //create prevFrame texture
    glGenTextures(1, &prevFrame);
    glBindTexture(GL_TEXTURE_2D, prevFrame);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)size.x, (GLsizei)size.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);

    //create frame buffer
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, prevFrame, 0);
}

void Engine::Camera::Resize(glm::vec2 size)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (GLsizei)size.x, (GLsizei)size.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);
    frame = 1;
}

void Engine::Camera::Render(unsigned int shaderProgram)
{
    glBindTexture(GL_TEXTURE_2D, prevFrame);

    //render to fbo
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    int uniformLocation = glGetUniformLocation(shaderProgram, "render");
    glUniform1i(uniformLocation, 1);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //display fbo
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    uniformLocation = glGetUniformLocation(shaderProgram, "render");
    glUniform1i(uniformLocation, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    frame++;
}

void Engine::Camera::SetRotationMatrix()
{
    rotationMat = glm::eulerAngleYXZ(rotation.y, rotation.x, rotation.z);
}
