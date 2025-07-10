#include "Mesh.h"

using namespace Bim;

Mesh::Mesh() {
    //this->vertices = vertices;
    //this->indices = indices;
    //this->textures = textures;
    //this->m_QOpengGlFunction = m_QOpengGlFunction;
}
void Mesh::Draw(QOpenGLShaderProgram &shader) {
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    //for (unsigned int i = 0; i < textures.size(); i++)
    //{
    //    m_QOpengGlFunction->glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
    //    // retrieve texture number (the N in diffuse_textureN)
    //    string number;
    //    string name = textures[i].type;
    //    if (name == "texture_diffuse")
    //        number = std::to_string(diffuseNr++);
    //    else if (name == "texture_specular")
    //        number = std::to_string(specularNr++); // transfer unsigned int to string
    //    else if (name == "texture_normal")
    //        number = std::to_string(normalNr++); // transfer unsigned int to string
    //    else if (name == "texture_height")
    //        number = std::to_string(heightNr++); // transfer unsigned int to string

    //    // now set the sampler to the correct texture unit
    //    m_QOpengGlFunction->glUniform1i(m_QOpengGlFunction->glGetUniformLocation(shader.programId(), (name + number).c_str()), i);
    //    // and finally bind the texture
    //    m_QOpengGlFunction->glBindTexture(GL_TEXTURE_2D, textures[i].id);
    //}

    // draw mesh
    m_QOpengGlFunction->glBindVertexArray(VAO);
    m_QOpengGlFunction->glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    //m_QOpengGlFunction->glDrawArrays(GL_TRIANGLES,0,vertices.size());
    m_QOpengGlFunction->glBindVertexArray(0);
    m_QOpengGlFunction->glActiveTexture(GL_TEXTURE0);

}
void Mesh::SetupMesh() {
    // create buffers/arrays
    m_QOpengGlFunction->glGenVertexArrays(1, &VAO);
    m_QOpengGlFunction->glBindVertexArray(VAO);

    m_QOpengGlFunction->glGenBuffers(1, &VBO);
    m_QOpengGlFunction->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    m_QOpengGlFunction->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    m_QOpengGlFunction->glGenBuffers(1, &EBO);
    m_QOpengGlFunction->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    m_QOpengGlFunction->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // vertex Positions
    m_QOpengGlFunction->glEnableVertexAttribArray(0);
    m_QOpengGlFunction->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    // vertex normals
    m_QOpengGlFunction->glEnableVertexAttribArray(1);
    m_QOpengGlFunction->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    m_QOpengGlFunction->glEnableVertexAttribArray(2);
    m_QOpengGlFunction->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    for (int i = 0; i < textures.size();++i) {
        textures[i].id = TextureFromFile(textures[i].path,false);
    }
    m_QOpengGlFunction->glBindVertexArray(0);
}
/// <summary>
/// ∂¡»°Õº∆¨
/// </summary>
/// <param name="path"></param>
/// <param name="directory"></param>
/// <param name="gamma"></param>
/// <returns></returns>
unsigned int Mesh::TextureFromFile(const std::string &filename, bool gamma)
{
    unsigned int textureID;
    m_QOpengGlFunction->glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 4);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else {
            std::cerr << "Unsupported nrComponents: " << nrComponents << std::endl;
            stbi_image_free(data);
            return 0;
        }

        m_QOpengGlFunction->glBindTexture(GL_TEXTURE_2D, textureID);
        m_QOpengGlFunction->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        m_QOpengGlFunction->glGenerateMipmap(GL_TEXTURE_2D);

        m_QOpengGlFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        m_QOpengGlFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        m_QOpengGlFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        m_QOpengGlFunction->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
