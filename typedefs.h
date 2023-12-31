//
// Created by Jsl on 2023/10/20.
//

#ifndef WXIMAGETOOL_TYPEDEFS_H
#define WXIMAGETOOL_TYPEDEFS_H
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

enum wxWIDGETID {
  ID_IMAGE,
  //filter
  ID_FILTER_GAUSSIAN,
  ID_FILTER_BOX,
  ID_FILTER_MEDIAN,
  ID_FILTER_SHARPEN,
  //noise
  ID_NOISE_SALT,
  ID_NOISE_GAUSSIAN,
  ID_NOISE_POISSON,
  //morphology operate
  ID_MORPH_ERODE,
  ID_MORPH_DILATE,
  ID_MORPH_OPEN,
  ID_MORPH_CLOSE,
  //edge
  ID_EDGE_SOBEL,
  ID_EDGE_SCHARR,
  ID_EDGE_CANNY,
  ID_EDGE_LAPLACIAN,
  //effect
  ID_EFFECT_CARTOON,
  ID_EFFECT_LOMOGRAPHY,

  //hist
  ID_HISTGRAM,
  ID_EQUALHISTGRAM,
};

#endif // WXIMAGETOOL_TYPEDEFS_H
