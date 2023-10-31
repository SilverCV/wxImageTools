//
// Created by Jsl on 2023/10/20.
//

#include "ImageApp.h"
bool ImageApp::OnInit() {
  wxInitAllImageHandlers();
  ImageFrame *frame = new ImageFrame;
  frame->Show();
  // load image

  return true;
}
