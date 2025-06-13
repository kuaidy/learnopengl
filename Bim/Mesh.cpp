#include "Mesh.h"
Mesh::Mesh(QOpenGLFunctions_4_5_Core *m_QOpengGlFunction,vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->m_QOpengGlFunction = m_QOpengGlFunction;
    SetupMesh();
}
void Mesh::Draw(QOpenGLShaderProgram &shader) {
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        m_QOpengGlFunction->glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number = std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number = std::to_string(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        m_QOpengGlFunction->glUniform1i(m_QOpengGlFunction->glGetUniformLocation(shader.programId(), (name + number).c_str()), i);
        // and finally bind the texture
        m_QOpengGlFunction->glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

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
    m_QOpengGlFunction->glGenBuffers(1, &VBO);
    m_QOpengGlFunction->glGenBuffers(1, &EBO);

    m_QOpengGlFunction->glBindVertexArray(VAO);
    m_QOpengGlFunction->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    m_QOpengGlFunction->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    m_QOpengGlFunction->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    m_QOpengGlFunction->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // vertex Positions
    m_QOpengGlFunction->glEnableVertexAttribArray(0);
    m_QOpengGlFunction->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    m_QOpengGlFunction->glEnableVertexAttribArray(1);
    m_QOpengGlFunction->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    m_QOpengGlFunction->glEnableVertexAttribArray(2);
    m_QOpengGlFunction->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    
    m_QOpengGlFunction->glBindVertexArray(0);
}