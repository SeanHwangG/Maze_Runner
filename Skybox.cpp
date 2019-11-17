#include "Skybox.h"

Skybox::Skybox(GLuint shader) : Geometry(shader)
{
    textureID = Skybox::loadCubemap({"objects/right.tga", "objects/left.tga", "objects/top.tga", "objects/base.tga", "objects/front.tga", "objects/back.tga"});;

    /*
     * Skybox indices used below.
     *    4----7
     *   /|   /|
     *  0-+--3 |
     *  | 5--+-6
     *  |/   |/
     *  1----2

     */
    std::vector<glm::vec3> vertices
    {
        glm::vec3(-500.0f, 500.0f, 500.0f),
        glm::vec3(-500.0f, -500.0f, 500.0f),
        glm::vec3(500.0f, -500.0f, 500.0f),
        glm::vec3(500.0f, 500.0f, 500.0f),
        glm::vec3(-500.0f, 500.0f, -500.0f),
        glm::vec3(-500.0f, -500.0f, -500.0f),
        glm::vec3(500.0f, -500.0f, -500.0f),
        glm::vec3(500.0f, 500.0f, -500.0f)
    };
    //for (auto& v : vertices) v *= 500;

    // Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2
    std::vector<glm::ivec3> indices {
        // Front face.
        glm::ivec3(2, 1, 0),
        glm::ivec3(0, 3, 2),
        // Back face.
        glm::ivec3(5, 6, 7),
        glm::ivec3(7, 4, 5),
        // Right face.
        glm::ivec3(6, 2, 3),
        glm::ivec3(3, 7, 6),
        // Left face.
        glm::ivec3(1, 5, 4),
        glm::ivec3(4, 0, 1),
        // Top face.
        glm::ivec3(3, 0, 4),
        glm::ivec3(4, 7, 3),
        // Bottom face.
        glm::ivec3(6, 5, 1),
        glm::ivec3(1, 2, 6),
    };
    

    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbos);
    glBindVertexArray(vao);

    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
        vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
                 indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Skybox::~Skybox()
{
    // Delete the VBO and the VAO.
    glDeleteBuffers(2, vbos);
    glDeleteVertexArrays(1, &vao);
}

void Skybox::draw() {
    glUseProgram(shader);
    glDepthMask(GL_FALSE);
    glEnable(GL_CULL_FACE);
    
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    
}
