#include <shader.hpp>

Shader::Shader() {
    _program = glCreateProgram();
}

Shader::~Shader() {
    glUseProgram(0);
    glDeleteProgram(_program); _program = 0;
}

Shader& Shader::activate() {
    glUseProgram(_program);
    return *this;
}

void Shader::bind(GLuint loc, glm::mat4 value) {
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::bind(GLuint loc, glm::vec4 value) {
    glUniform4fv(loc, 1, glm::value_ptr(value));
}

void Shader::bind(GLuint loc, glm::vec3 value) {
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::bind(GLuint loc, float value) {
    glUniform1i(loc, value);
}

void Shader::bindMaterial(const std::string name, Material value) {
    bind(name+".ambient", value.ambient);
    bind(name+".diffuse", value.diffuse);
    bind(name+".specular", value.specular);
    bind(name+".shininess", value.shininess);
}

Shader& Shader::attach(std::string const &filename) {
    auto path = PROJECT_SOURCE_DIR "/shaders/";
    std::ifstream t(path + filename);
    
    auto src = std::string(std::istreambuf_iterator<char>(t), std::istreambuf_iterator<char>());
    
    auto shader = create(filename);
    auto source = src.c_str();
    
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    
    int status = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        
        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        
        glDeleteShader(shader);
        
        printf("error compiling shader: %s:\n %s\n", filename.c_str(), &errorLog[0]);
    }
    
    glAttachShader(_program, shader);
    
    return *this;
}

Shader& Shader::link() {
    glLinkProgram(_program);
    
    int status = 0;
    glGetProgramiv(_program, GL_LINK_STATUS, &status);
    
    if(status == GL_FALSE) {
        int maxLength = 0;
        glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &maxLength);
        
        std::vector<GLchar> errorLog(maxLength);
        glGetProgramInfoLog(_program, maxLength, &maxLength, &errorLog[0]);
        
        printf("error %s\n", &errorLog[0]);
    }

    
    return *this;
    
}

GLuint Shader::get() { return _program; }

GLuint Shader::create(std::string const &filename) {
    auto index = filename.rfind(".");
    auto ext = filename.substr(index + 1);

    if(ext == "vert") return glCreateShader(GL_VERTEX_SHADER);
    if(ext == "frag") return glCreateShader(GL_FRAGMENT_SHADER);
    if(ext == "comp") return glCreateShader(GL_COMPUTE_SHADER);
    if(ext == "geom") return glCreateShader(GL_GEOMETRY_SHADER);
    
    return glCreateShader(GL_VERTEX_SHADER);
}
