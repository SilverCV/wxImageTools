//
// Created by Jsl on 2023/10/21.
//
#include "ImageCanvas.h"
#include "wx/event.h"
#include "wx/log.h"

ImageCanvas::ImageCanvas(wxWindow *parent, int id,
                         const wxGLAttributes &canvasAttrs)
    : wxGLCanvas(parent, canvasAttrs, id),_fovy(45.0f),_first(true) {
  InitContext();
}
ImageCanvas::~ImageCanvas() {}
void ImageCanvas::OnPaint(wxPaintEvent &event) {
  _ctx->SetCurrent(*this);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  _shaderProgram->use();
  _vertexArray->bind();
  auto mvp = _projection * _view * _model;
  _shaderProgram->SetMat4("mvp", mvp);
  glBindTextureUnit(0, _tex->handle());
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  _vertexArray->unbind();
  _shaderProgram->unuse();
  SwapBuffers();
}

void ImageCanvas::InitContext() {
  // initialize opengl
  wxGLContextAttrs attrs;
  attrs.PlatformDefaults().CoreProfile().OGLVersion(4, 6).EndList();
  _ctx = std::make_shared<wxGLContext>(this, nullptr, &attrs);
  if (!_ctx->IsOK()) {
    wxLogError("fail to create opengl context");
    exit(EXIT_FAILURE);
  }
  _ctx->SetCurrent(*this);
  // intitalize glew
  InitGLEW();
  // shader
  const char *vertexCode = R"(
        #version 460 core
        layout(location = 0) in vec3 pos;
        layout(location = 1) in vec2 tc;
        layout(location = 0) out vec2 uv;
        uniform mat4 mvp;
        void main()
        {
            uv = tc;
            gl_Position = mvp * vec4(pos,1.0);
        }
    )";
  const char *FragCode = R"(
        #version 460 core
        layout(location = 0) out vec4 FragColor;
        layout(location = 0) in vec2 uv;
        layout(binding=0) uniform sampler2D tex;
        void main()
        {
            FragColor = texture(tex,uv);
        }
    )";
  GLShader vertexShader(GL_VERTEX_SHADER, vertexCode);
  GLShader fragmetShader(GL_FRAGMENT_SHADER, FragCode);
  _shaderProgram = std::make_shared<GLShaderProgram>(
      std::initializer_list<GLShader>{vertexShader, fragmetShader});

  //
  struct Vertex {
    float pos[3];
    float tc[2];
  };
  std::vector<Vertex> vertices = {{{-1.0, -1.0, 0.0}, {0.0, 0.0}},
                                  {{-1.0, 1.0, 0.0}, {0.0, 1.0}},
                                  {{1.0, 1.0, 0.0}, {1.0, 1.0}},
                                  {{1.0, -1.0, 0.0}, {1.0, 0.0}}};
  unsigned int indices[] = {0, 1, 2, 0, 2, 3};
  _vertexArray = std::make_shared<GLVertexArray>();
  GLBuffer vbo(sizeof(Vertex) * vertices.size(),
               (const void *)(vertices.data()), 0);
  GLBuffer ebo(sizeof(indices), indices, 0);
  glVertexArrayElementBuffer(_vertexArray->handle(), ebo.handle());
  glVertexArrayVertexBuffer(_vertexArray->handle(), 0, vbo.handle(), 0,
                            sizeof(Vertex));
  glEnableVertexArrayAttrib(_vertexArray->handle(), 0);
  glEnableVertexArrayAttrib(_vertexArray->handle(), 1);
  glVertexArrayAttribFormat(_vertexArray->handle(), 0, 3, GL_FLOAT, GL_FALSE,
                            offsetof(Vertex, pos));
  glVertexArrayAttribFormat(_vertexArray->handle(), 1, 2, GL_FLOAT, GL_FALSE,
                            offsetof(Vertex, tc));
  glVertexArrayAttribBinding(_vertexArray->handle(), 0, 0);
  glVertexArrayAttribBinding(_vertexArray->handle(), 1, 0);
  _tex = std::make_shared<GLTexture>(GL_TEXTURE_2D);
  glTextureParameteri(_tex->handle(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(_tex->handle(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTextureParameteri(_tex->handle(), GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_NEAREST);
  glTextureParameteri(_tex->handle(), GL_TEXTURE_MAG_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
  glTextureParameteri(_tex->handle(), GL_TEXTURE_BASE_LEVEL, 0);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  //
  _model = glm::mat4(1.0f);
  _view = glm::lookAt(glm::vec3(0, 0, 1.0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  _projection = glm::mat4(1.0f);

  //
  Bind(wxEVT_PAINT, &ImageCanvas::OnPaint, this);
  Bind(wxEVT_SIZE, &ImageCanvas::OnResize, this);
  Bind(wxEVT_MOUSEWHEEL, &ImageCanvas::OnWheel, this);
  Bind(wxEVT_MOTION,&ImageCanvas::OnMotion,this);
}

void ImageCanvas::LoadImageFromPath(const std::string &fileName) {
  auto src = cv::imread(fileName);
  LoadImageFromMat(src);
}

void ImageCanvas::LoadImageFromMat(const cv::Mat &src) {
  //
  if (!src.data)
    return;
  _ImageMat = src.clone();
  cv::Mat dst;
  cv::flip(src, dst, 0);
  if (dst.channels() == 3)
    cv::cvtColor(dst, dst, cv::COLOR_BGR2RGB);
  else if (dst.channels() == 4) {
    cv::cvtColor(dst, dst, cv::COLOR_BGRA2RGBA);
  }
  GLenum internalFormat = GL_RGB8;
  GLenum format = GL_RGB;
  if (dst.channels() == 1) {
    internalFormat = GL_R8;
    format = GL_RED;
  }
  if (dst.channels() == 3) {
    internalFormat = GL_RGB8;
    format = GL_RGB;
  }
  if (dst.channels() == 4) {
    internalFormat = GL_RGBA8;
    format = GL_RGBA;
  }
  GLenum type = GL_UNSIGNED_BYTE;
  if (dst.elemSize1() == 2) type = GL_UNSIGNED_SHORT;
  if (dst.elemSize1() == 4) {type = GL_FLOAT;internalFormat = GL_RGB32F;format = GL_RGB;}
  auto level = GetLevel(src.cols, src.rows);
  glTextureParameteri(_tex->handle(), GL_TEXTURE_MAX_LEVEL, level);
  glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, dst.cols, dst.rows, 0, format,
               type, dst.data);
  glGenerateTextureMipmap(_tex->handle());
  Refresh(true);

}

int ImageCanvas::GetLevel(int w, int h) {
  int level = 1;
  while ((w | h) >> level)
    level++;
  return level;
}

void ImageCanvas::OnResize(wxSizeEvent &event) {
  int w = event.GetSize().GetWidth();
  int h = event.GetSize().GetHeight();
  glViewport(0, 0, w, h);

 //_projection = glm::perspective(_fovy, static_cast<float>(w) / h, 0.001f, 500.0f);
}

void ImageCanvas::OnWheel(wxMouseEvent &event) {
    if (event.GetWheelAxis() == wxMOUSE_WHEEL_HORIZONTAL)
        event.Skip();
    float scale = _model[0][0];
    auto delta = event.GetWheelRotation() / event.GetWheelDelta() * 0.1f;
    scale += delta;
    if (scale < 0.1f) return;
    float tx = _model[3][0];
    float ty = _model[3][1];
    auto translate = glm::translate(glm::mat4(1.0f),glm::vec3(tx * (1.0f - delta),ty * (1.0f - delta),0));
    _model = glm::scale(translate,glm::vec3(scale));
    Refresh(true);
}

void ImageCanvas::OnMotion(wxMouseEvent &event) {
    if (event.Dragging())
    {
        if (_first)
        {
            _lastX = event.GetX();
            _lastY = event.GetY();
            _first = false;
        }
        float tx = _model[3][0];
        float ty = _model[3][1];
        int w = GetSize().GetWidth();
        int h = GetSize().GetHeight();
        auto scale = _model[0][0];
        const float coeff = 1.0f / scale;
        float delta_x = tx + (event.GetX() - _lastX )  / static_cast<float>(w)  * coeff ;
        float delta_y = ty + (_lastY - event.GetY()  ) / static_cast<float>(h)  * coeff;
        _model = glm::translate(glm::scale(glm::mat4(1.0f),glm::vec3(scale)),glm::vec3(delta_x,delta_y,0));
        Refresh(true);
    } else{
        _first = true;
    }

}

cv::Mat ImageCanvas::GetImageMat() {
    wxMutexLocker locker(_mtx);
    return _ImageMat;
}
