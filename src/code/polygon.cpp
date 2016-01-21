
#include <polygon.hpp>
#include <pipeline.hpp>

Polygon::Polygon(glm::vec3 p, glm::vec4 c,
                 std::vector<glm::vec3> shape,
                 std::vector<glm::vec2> tex,
                 std::vector<unsigned int> e) :
    position(p),
    color(c)
{
    auto normals = Pipeline::calculateNormals(shape, e);
    _verts = Pipeline::createVerts(shape, normals, tex, e);
    this->translate(glm::vec3());
}

Polygon::Polygon(glm::vec3 p, glm::vec4 c,
                 std::vector<glm::vec3> shape,
                 std::vector<glm::vec3> n,
                 std::vector<glm::vec2> t,
                 std::vector<unsigned int> e) :
    position(p),
    color(c)
{
    _verts = Pipeline::createVerts(shape, n, t, e);
    this->translate(glm::vec3());
}

void Polygon::init(Shader *s) {
    shader = s;
    _vao = Pipeline::createVertexArray(_verts);
}

void Polygon::translate(glm::vec3 displacement) {
    position += displacement;
    _translation = glm::translate(glm::mat4(), position);
}

void Polygon::rotate(float angle, glm::vec3 axis) {
    _rotation = glm::rotate(glm::mat4(), glm::radians(angle), axis);
}

void Polygon::orbit(float angle, glm::vec3 axis, glm::vec3 point) {
    glm::vec3 displacement = point - position;
    
    _orbit = glm::translate(glm::mat4(), displacement);
    _orbit = glm::rotate(_orbit, glm::radians(angle), axis);
    _orbit = glm::translate(_orbit, displacement * -1.0f);
}

void Polygon::scale(float s) {
    _scale = glm::scale(glm::mat4(), glm::vec3(s));
}

void Polygon::addTexture(const std::string &textureName, bool alpha)
{
    _textureId = Pipeline::createTexture(textureName, alpha);
}

void Polygon::draw(glm::mat4 view, glm::mat4 proj) {
    
    Pipeline::bind(_vao, _textureId);
    _updateUniforms(view, proj);
    
    glDrawArrays(GL_TRIANGLES, 0, _verts.size());
}

glm::vec3 Polygon::getPosition() {
    auto p = _scale*_translation*_orbit*_rotation*glm::vec4(position, 1.0f);
    return glm::vec3(p.x, p.y, p.z);
}

void Polygon::_updateUniforms(glm::mat4 view, glm::mat4 proj) {
    
    shader->activate()
        .bind("projection", proj)
        .bind("view", view)
        .bind("model", _scale*_translation*_orbit*_rotation)
        .bind("color", color)
        .bindMaterial("material", material);
}