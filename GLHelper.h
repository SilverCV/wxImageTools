//
// Created by Jsl on 2023/10/21.
//

#ifndef WXIMAGETOOL_GLHELPER_H
#define WXIMAGETOOL_GLHELPER_H

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <string>

bool InitGLEW();
struct GLHandle {
  virtual ~GLHandle() = default;
  virtual GLuint handle() const { return _handle; }

protected:
  GLuint _handle;
};
class GLBuffer : public GLHandle {
public:
  GLBuffer(GLsizeiptr size, const void *data, GLbitfield flags);
  ~GLBuffer();
};

class GLVertexArray : public GLHandle {
public:
  GLVertexArray();
  ~GLVertexArray() override;
  void bind();
  void unbind();
};

class GLTexture : public GLHandle {
public:
  GLTexture(GLenum target);
  ~GLTexture();
};

class GLShader : public GLHandle {
public:
  // GLShader(GLenum type,const std::string& path);
  GLShader(GLenum type, const char *code);
  ~GLShader() override;

protected:
  std::string loadCode(std::string const &path);
};

class GLShaderProgram : public GLHandle {
public:
  GLShaderProgram(std::initializer_list<GLShader> const &shaders);
  GLShaderProgram();
  ~GLShaderProgram() override;
  void AttachShader(GLShader const &shader);
  void DetachShader(GLShader const &shader);
  void Link();
  void use();
  void unuse();
  // set location value
  void SetInt(std::string const &name, int value);
  void SetFloat(std::string const &name, float value);
  void SetDouble(std::string const &name, double v);
  void SetVec2(std::string const &name, glm::vec2 const &v);
  void SetVec3(std::string const &name, glm::vec3 const &v);
  void SetVec4(std::string const &name, glm::vec4 const &v);
  void SetMat2(std::string const &name, glm::mat2 const &v);
  void SetMat3(std::string const &name, glm::mat3 const &v);
  void SetMat4(std::string const &name, glm::mat4 const &v);

protected:
  GLint GetLocation(std::string const &name);
};

#endif // WXIMAGETOOL_GLHELPER_H
