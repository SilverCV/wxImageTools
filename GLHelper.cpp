//
// Created by Jsl on 2023/10/21.
//

#include "GLHelper.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

bool InitGLEW() {

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  return err == GLEW_OK;
}

GLBuffer::GLBuffer(GLsizeiptr size, const void *data, GLbitfield flags) {
  glCreateBuffers(1, &_handle);
  glNamedBufferStorage(_handle, size, data, flags);
}

GLBuffer::~GLBuffer() { glDeleteBuffers(1, &_handle); }

GLVertexArray::GLVertexArray() { glCreateVertexArrays(1, &_handle); }

GLVertexArray::~GLVertexArray() {
  unbind();
  glDeleteVertexArrays(1, &_handle);
}

void GLVertexArray::bind() { glBindVertexArray(_handle); }

void GLVertexArray::unbind() { glBindVertexArray(0); }

GLTexture::GLTexture(GLenum target) { glCreateTextures(target, 1, &_handle); }

GLTexture::~GLTexture() { glDeleteTextures(1, &_handle); }

// GLShader::GLShader(GLenum type, const std::string &path) {
//     _handle = glCreateShader(type);
//     auto code = loadCode(path);
//     const char* src = code.c_str();
//     glShaderSource(_handle,1,&src, nullptr);
//     glCompileShader(_handle);
//     GLchar info[8192] = {0};
//     GLint status = 0;
//     glGetShaderiv(_handle,GL_COMPILE_STATUS,&status);
//     if (status != GL_TRUE){
//         glGetShaderiv(_handle,GL_INFO_LOG_LENGTH,&status);
//         glGetShaderInfoLog(_handle,status,&status,info);
//         std::cerr  << path << " Compile Error is "  << info << std::endl;
//         exit(EXIT_FAILURE);
//     }
//
// }

GLShader::~GLShader() { glDeleteShader(_handle); }

std::string GLShader::loadCode(const std::string &path) {
  std::string code;
  std::ifstream infile(path, std::ios::in);
  code.assign(std::istreambuf_iterator<char>(infile),
              std::istreambuf_iterator<char>());
  return code;
}

GLShader::GLShader(GLenum type, const char *code) {
  _handle = glCreateShader(type);
  glShaderSource(_handle, 1, &code, nullptr);
  glCompileShader(_handle);
  GLchar info[8192] = {0};
  GLint status = 0;
  glGetShaderiv(_handle, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &status);
    glGetShaderInfoLog(_handle, status, &status, info);
    std::cerr << " Compile Error is " << info << std::endl;
    exit(EXIT_FAILURE);
  }
}

void GLShaderProgram::Link() {
  glLinkProgram(_handle);
  glValidateProgram(_handle);
  GLchar info[8192] = {0};
  GLint status = 0;
  glGetProgramiv(_handle, GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &status);
    glGetProgramInfoLog(_handle, status, &status, info);
    std::cerr << "Link Error is " << info << std::endl;
    exit(EXIT_FAILURE);
  }
}

void GLShaderProgram::AttachShader(const GLShader &shader) {
  glAttachShader(_handle, shader.handle());
}

GLShaderProgram::GLShaderProgram(
    const std::initializer_list<GLShader> &shaders) {
  _handle = glCreateProgram();
  for (auto s : shaders) {
    glAttachShader(_handle, s.handle());
  }
  Link();
  for (auto s : shaders) {
    glDetachShader(_handle, s.handle());
  }
}

GLShaderProgram::GLShaderProgram() { _handle = glCreateProgram(); }

GLShaderProgram::~GLShaderProgram() {
  unuse();
  glDeleteProgram(_handle);
}

void GLShaderProgram::use() { glUseProgram(_handle); }

void GLShaderProgram::unuse() { glUseProgram(0); }

void GLShaderProgram::SetInt(std::string const &name, int value) {
  glUniform1i(GetLocation(name), 1);
}

void GLShaderProgram::DetachShader(const GLShader &shader) {
  glDetachShader(_handle, shader.handle());
}

GLint GLShaderProgram::GetLocation(std::string const &name) {
  return glGetUniformLocation(_handle, name.c_str());
}
void GLShaderProgram::SetDouble(std::string const &name, double v) {
  glUniform1d(GetLocation(name), v);
}

void GLShaderProgram::SetVec2(std::string const &name, glm::vec2 const &v) {
  glUniform2fv(GetLocation(name), 1, glm::value_ptr(v));
}

void GLShaderProgram::SetVec3(std::string const &name, glm::vec3 const &v) {
  glUniform3fv(GetLocation(name), 1, glm::value_ptr(v));
}

void GLShaderProgram::SetVec4(std::string const &name, glm::vec4 const &v) {
  glUniform4fv(GetLocation(name), 1, glm::value_ptr(v));
}

void GLShaderProgram::SetMat2(std::string const &name, glm::mat2 const &v) {
  glUniformMatrix2fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(v));
}

void GLShaderProgram::SetMat3(std::string const &name, glm::mat3 const &v) {
  glUniformMatrix3fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(v));
}

void GLShaderProgram::SetMat4(std::string const &name, glm::mat4 const &v) {

  glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(v));
}
void GLShaderProgram::SetFloat(std::string const &name, float v) {
  glUniform1f(GetLocation(name), v);
}