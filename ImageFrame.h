//
// Created by Jsl on 2023/10/20.
//

#ifndef WXIMAGETOOL_IMAGEFRAME_H
#define WXIMAGETOOL_IMAGEFRAME_H
#include "ImageCanvas.h"
#include "typedefs.h"
#include "wx/event.h"

class ImageFrame : public wxFrame {
public:
  ImageFrame();
  ~ImageFrame();

protected:
  void InitUI();
  void InitMenuBar();
  void OnOpenFile(wxCommandEvent &event);
  void OnSave(wxCommandEvent& event);
  void OnSaveAs(wxCommandEvent& event);
  void OnExit(wxCommandEvent &event);
  void OnAbout(wxCommandEvent &event);
  //
protected:
    void OnGaussianFilter(wxCommandEvent& event);
    void OnBoxFilter(wxCommandEvent& event);
    void OnMedianFilter(wxCommandEvent& event);

    void OnGaussianNoise(wxCommandEvent& event);
    void OnSaltNoise(wxCommandEvent& event);
    void OnPoissonNoise(wxCommandEvent& event);

    void OnOpening(wxCommandEvent& event);
    void OnClosing(wxCommandEvent& event);
    void OnErode(wxCommandEvent& event);
    void OnDilate(wxCommandEvent& event);

    void OnSobelEdge(wxCommandEvent& event);
    void OnScharrEdge(wxCommandEvent& event);
    void OnCannyEdge(wxCommandEvent& event);
    void OnLaplacianEdge(wxCommandEvent& event);

    void OnCartoonEffect(wxCommandEvent& event);
    void OnLomographyEffect(wxCommandEvent& event);

    void OnEqualHist(wxCommandEvent& event);

private:
  ImageCanvas *m_ImageCanvas;
private:
  DECLARE_EVENT_TABLE()
};

#endif // WXIMAGETOOL_IMAGEFRAME_H
