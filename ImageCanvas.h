//
// Created by Jsl on 2023/10/21.
//

#ifndef WXIMAGETOOL_IMAGECANVAS_H
#define WXIMAGETOOL_IMAGECANVAS_H
#include "GLHelper.h"
#include "ImageHelper.h"
#include "typedefs.h"
#include "wx/event.h"
#include <wx/glcanvas.h>

class ImageCanvas : public wxGLCanvas {
public:
  explicit ImageCanvas(wxWindow *parent, int id,
                       const wxGLAttributes &canvasAttrs);
  ~ImageCanvas();

  void LoadImageFromPath(const std::string &fileName);
  void LoadImageFromMat(const cv::Mat &src);
  cv::Mat GetImageMat();
protected:
  void InitContext();
  void OnPaint(wxPaintEvent &event);
  void OnResize(wxSizeEvent &event);
  void OnWheel(wxMouseEvent &event);
  void OnMotion(wxMouseEvent& event);
  int GetLevel(int w, int h);

private:
  std::shared_ptr<wxGLContext> _ctx;
  std::shared_ptr<GLShaderProgram> _shaderProgram;
  std::shared_ptr<GLVertexArray> _vertexArray;
  std::shared_ptr<GLTexture> _tex;
  // model projection view
  glm::mat4 _view;
  glm::mat4 _model;
  glm::mat4 _projection;
  float _fovy;

  bool _first;
  float _lastX;
  float _lastY;

  cv::Mat _ImageMat;
  wxMutex _mtx;
};

#endif // WXIMAGETOOL_IMAGECANVAS_H
