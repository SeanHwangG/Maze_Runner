#include "Geometry.h"

using namespace std;

Geometry::Geometry(){}

Geometry::Geometry(GLuint shader){
    this->shader = shader;
}

Geometry::Geometry(GLuint shader, std::string objFilename) : Geometry(shader)
{
    errno = 0;
    ifstream infile(objFilename);
    // if the file open failed, print out the error message
    if (errno != 0 && infile.is_open()) {
        cerr << strerror(errno) << endl;
    }
    
    // if the file is open and infile is successfully initialized
    if (infile.is_open()) {
        string line;
        while (getline(infile, line)) {
            stringstream ss;
            ss << line;
            string tag, part;
            ss >> tag;
            if (tag == "v" || tag == "vn") {
                glm::vec3 point;
                ss >> point.x >> point.y >> point.z;
                if (tag == "v") {
                    points.push_back(point);
                } else {
                    normals.push_back(point);
                }
            }
            if (tag == "f") {
                ss >> part;
                glm::ivec3 id;
                part = part.substr(0, part.find("//"));
                id.x = stoi(part) - 1;
                ss >> part;
                part = part.substr(0, part.find("//"));
                id.y = stoi(part) - 1;
                ss >> part;
                part = part.substr(0, part.find("//"));
                id.z = stoi(part) - 1;
                indices.push_back(id);
            }
        }
    }
    else {
        cerr << "Can't open the file " << objFilename << endl;
    }
    infile.close();

    model = glm::mat4(1);
    glGenVertexArrays(1, vaos);
    glGenBuffers(2, vbos);
    glBindVertexArray(vaos[0]);     // Bind to the VAO.

    // POINTS
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // NORMALS
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // INDICES
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);
    
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

Geometry::~Geometry()
{
    glDeleteBuffers(2, vbos);
    glDeleteVertexArrays(1, vaos);
}

void Geometry::draw()
{
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE,
                       glm::value_ptr(model));
    glBindVertexArray(vaos[0]);
    glDrawElements(GL_TRIANGLES, (GLsizei) indices.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Geometry::draw(glm::mat4 M, glm::vec3 color)
{
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE,
        glm::value_ptr(M));
    glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));
    glBindVertexArray(vaos[0]);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size() * 3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

GLuint Geometry::loadCubemap(std::vector<std::string> faces){
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return textureID;
}
