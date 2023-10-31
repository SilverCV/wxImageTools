//
// Created by Jsl on 2023/10/20.
//

#include "ImageFrame.h"
ImageFrame::ImageFrame()
    : wxFrame(nullptr, wxID_ANY, wxT("ImageTool"), wxDefaultPosition,
              wxSize(1280, 960)) {
  InitUI();
}
ImageFrame::~ImageFrame() {}

void ImageFrame::OnOpenFile(wxCommandEvent &event) {
  wxFileDialog dialog(this, wxT("select image file"), wxT(""), wxT(""),
                      wxT("image (*.jpg)|*.jpg|(*.png)|*.png"),
                      wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (dialog.ShowModal() == wxID_OK) {
    auto fileName = dialog.GetPath().utf8_string();
    auto dst = cv::imread(fileName);
    m_ImageCanvas->LoadImageFromMat(dst);
  }
}
void ImageFrame::OnExit(wxCommandEvent &event) { Close(true); }

void ImageFrame::OnAbout(wxCommandEvent &event) {
  wxMessageBox(wxT("the image tool for image process"), wxT("About"),
               wxOK | wxCENTER);
}
void ImageFrame::OnSave(wxCommandEvent &event) {

}
void ImageFrame::OnSaveAs(wxCommandEvent &event) {

}


void ImageFrame::InitUI() {
  InitMenuBar();
  // set menu
  wxPanel *mainPanel = new wxPanel(this, wxID_ANY);
  //
  wxGLAttributes attributes;
  attributes.PlatformDefaults()
      .MinRGBA(8, 8, 8, 8)
      .DoubleBuffer()
      .Depth(24)
      .Stencil(8)
      .EndList();
  m_ImageCanvas = new ImageCanvas(mainPanel, ID_IMAGE, attributes);
  // image process

  // horizontalSizer->Add(m_ImagePanel);
  wxBoxSizer *verticalSizer = new wxBoxSizer(wxVERTICAL);

  verticalSizer->Add(m_ImageCanvas, 1, wxEXPAND | wxALL, 3);
  mainPanel->SetSizer(verticalSizer);

  Centre();
  CreateStatusBar();
  SetStatusText(wxT("ImageProcess"));
  // center pannel
}

void ImageFrame::InitMenuBar() {
  // file menu
  wxMenu *fileMenu = new wxMenu;
  fileMenu->Append(wxID_NEW, wxT("&New...\tCtrl-N"));
  fileMenu->Append(wxID_OPEN, wxT("&Open...\tCtrl-O"));
  fileMenu->Append(wxID_CLOSE, wxT("&Close...\tCtrl-W"));
  fileMenu->Append(wxID_SAVE,wxT("&Save...\tCtrl-S"));
  fileMenu->Append(wxID_SAVEAS,wxT("&Save as...\tCtrl-Shift-S"));
  fileMenu->AppendSeparator();
  fileMenu->Append(wxID_EXIT, wxT("&Exit...\tCtrl-Q"));
  //process
  wxMenu* algorithmMenu = new wxMenu;
  wxMenu* filterMenu = new wxMenu;

  filterMenu->Append(ID_FILTER_GAUSSIAN,wxT("&GaussianBlur"));
  filterMenu->Append(ID_FILTER_BOX,wxT("&BoxBlur"));
  filterMenu->Append(ID_FILTER_MEDIAN,wxT("&MedianBlur"));
    algorithmMenu->AppendSubMenu(filterMenu,wxT("&Filter"));
  wxMenu* noiseMenu = new wxMenu;
  noiseMenu->Append(ID_NOISE_SALT,wxT("&SaltNoise"));
  noiseMenu->Append(ID_NOISE_GAUSSIAN,wxT("&GaussianNoise"));
  noiseMenu->Append(ID_NOISE_POISSON,wxT("&PassionNoise"));
  algorithmMenu->AppendSubMenu(noiseMenu,wxT("&Noise"));
  wxMenu* MorphMenu = new wxMenu;
  MorphMenu->Append(ID_MORPH_ERODE,wxT("&Erode"));
  MorphMenu->Append(ID_MORPH_DILATE,wxT("&Dilate"));
  MorphMenu->Append(ID_MORPH_OPEN,wxT("&Opening"));
  MorphMenu->Append(ID_MORPH_CLOSE,wxT("&Closing"));
  algorithmMenu->AppendSubMenu(MorphMenu,wxT("&Morphology"));
  wxMenu* edgeMenu = new wxMenu;
  edgeMenu->Append(ID_EDGE_SOBEL,wxT("&Sobel"));
  edgeMenu->Append(ID_EDGE_SCHARR,wxT("&Scharr"));
  edgeMenu->Append(ID_EDGE_CANNY,wxT("&Canny"));
  edgeMenu->Append(ID_EDGE_LAPLACIAN,wxT("&Laplacian"));
  algorithmMenu->AppendSubMenu(edgeMenu,wxT("&Edge"));
  //effect
  wxMenu* effectMenu = new wxMenu;
  effectMenu->Append(ID_EFFECT_CARTOON,wxT("&Cartoon"));
  effectMenu->Append(ID_EFFECT_LOMOGRAPHY,wxT("&Lomography"));

  algorithmMenu->AppendSubMenu(effectMenu,wxT("&Effect"));
  //histgram
  wxMenu* histMenu = new wxMenu;
  histMenu->Append(ID_HISTGRAM,wxT("&Histgram"));
  histMenu->Append(ID_EQUALHISTGRAM,wxT("&EqualHist"));
  algorithmMenu->AppendSubMenu(histMenu,wxT("&histgram"));
  // help menu
  wxMenu *helpMenu = new wxMenu;
  helpMenu->Append(wxID_ABORT, wxT("&About..."));
  helpMenu->AppendSeparator();
  wxMenuBar *menuBar = new wxMenuBar;

  menuBar->Append(fileMenu, wxT("&File"));
  menuBar->Append(algorithmMenu,wxT("&Image"));
  menuBar->Append(helpMenu, wxT("&Help"));
  SetMenuBar(menuBar);
}


void ImageFrame::OnGaussianFilter(wxCommandEvent &event) {
    auto dst = m_ImageCanvas->GetImageMat();
    cv::GaussianBlur(dst,dst,cv::Size(3,3),1.0,1.0);
    m_ImageCanvas->LoadImageFromMat(dst);
}

void ImageFrame::OnBoxFilter(wxCommandEvent &event) {
    auto dst = m_ImageCanvas->GetImageMat();
    cv::boxFilter(dst,dst,dst.depth(),cv::Size(3,3));
    m_ImageCanvas->LoadImageFromMat(dst);
}

void ImageFrame::OnMedianFilter(wxCommandEvent &event) {
    auto dst = m_ImageCanvas->GetImageMat();
    cv::medianBlur(dst,dst,3);
    m_ImageCanvas->LoadImageFromMat(dst);
}
void ImageFrame::OnGaussianNoise(wxCommandEvent &event) {
    auto src = m_ImageCanvas->GetImageMat();
    auto dst = ImageHelper::GaussianNoise(src);
    m_ImageCanvas->LoadImageFromMat(dst);
}

void ImageFrame::OnSaltNoise(wxCommandEvent &event) {
    auto src = m_ImageCanvas->GetImageMat();
    auto dst = ImageHelper::salt(src,3000);
    m_ImageCanvas->LoadImageFromMat(dst);
}

void ImageFrame::OnPoissonNoise(wxCommandEvent &event) {
    auto src = m_ImageCanvas->GetImageMat();
    auto dst = ImageHelper::PoissonNoise(src);
    m_ImageCanvas->LoadImageFromMat(dst);
}
void ImageFrame::OnOpening(wxCommandEvent &event) {
    auto src = m_ImageCanvas->GetImageMat();
    auto dst = ImageHelper::opening(src);
    m_ImageCanvas->LoadImageFromMat(dst);
}

void ImageFrame::OnClosing(wxCommandEvent &event) {
    auto src = m_ImageCanvas->GetImageMat();
    auto dst = ImageHelper::closing(src);
    m_ImageCanvas->LoadImageFromMat(dst);
}

void ImageFrame::OnErode(wxCommandEvent &event) {
    auto src = m_ImageCanvas->GetImageMat();
    auto dst = ImageHelper::erode(src);
    m_ImageCanvas->LoadImageFromMat(dst);
}

void ImageFrame::OnDilate(wxCommandEvent &event) {
    auto src = m_ImageCanvas->GetImageMat();
    auto dst = ImageHelper::dilate(src);
    m_ImageCanvas->LoadImageFromMat(dst);
}

void ImageFrame::OnSobelEdge(wxCommandEvent &event) {
    auto src = m_ImageCanvas->GetImageMat();
    auto dst = ImageHelper::soble(src);
    m_ImageCanvas->LoadImageFromMat(dst);
}

void ImageFrame::OnScharrEdge(wxCommandEvent &event) {
    auto src = m_ImageCanvas->GetImageMat();
    auto dst = ImageHelper::scharr(src);
    m_ImageCanvas->LoadImageFromMat(dst);
}

void ImageFrame::OnCannyEdge(wxCommandEvent &event) {
    auto src = m_ImageCanvas->GetImageMat();
    auto dst = ImageHelper::canny(src);
    m_ImageCanvas->LoadImageFromMat(dst);
}

void ImageFrame::OnLaplacianEdge(wxCommandEvent &event) {
    auto src = m_ImageCanvas->GetImageMat();
    auto dst = ImageHelper::Laplacian(src);
    m_ImageCanvas->LoadImageFromMat(dst);
}

void ImageFrame::OnCartoonEffect(wxCommandEvent &event) {
    auto src = m_ImageCanvas->GetImageMat();
    auto dst = ImageHelper::CartoonEffect(src);
    m_ImageCanvas->LoadImageFromMat(dst);
}
void ImageFrame::OnLomographyEffect(wxCommandEvent &event) {
    auto src = m_ImageCanvas->GetImageMat();
    auto dst = ImageHelper::Lomography(src);
    m_ImageCanvas->LoadImageFromMat(dst);
}

void ImageFrame::OnEqualHist(wxCommandEvent &event) {
    auto src = m_ImageCanvas->GetImageMat();
    auto dst = ImageHelper::EqualHist(src);
    m_ImageCanvas->LoadImageFromMat(dst);
}
BEGIN_EVENT_TABLE(ImageFrame, wxFrame)
                EVT_MENU(wxID_CLOSE, ImageFrame::OnExit)
                EVT_MENU(wxID_OPEN, ImageFrame::OnOpenFile)
                EVT_MENU(wxID_ABORT, ImageFrame::OnAbout)
                EVT_MENU(wxID_SAVE,ImageFrame::OnSave)
                EVT_MENU(wxID_SAVEAS,ImageFrame::OnSaveAs)

                EVT_MENU(ID_FILTER_GAUSSIAN,ImageFrame::OnGaussianFilter)
                EVT_MENU(ID_FILTER_BOX,ImageFrame::OnBoxFilter)
                EVT_MENU(ID_FILTER_MEDIAN,ImageFrame::OnMedianFilter)

                EVT_MENU(ID_NOISE_GAUSSIAN,ImageFrame::OnGaussianNoise)
                EVT_MENU(ID_NOISE_SALT,ImageFrame::OnSaltNoise)
                EVT_MENU(ID_NOISE_POISSON,ImageFrame::OnPoissonNoise)

                EVT_MENU(ID_MORPH_OPEN,ImageFrame::OnOpening)
                EVT_MENU(ID_MORPH_CLOSE,ImageFrame::OnClosing)
                EVT_MENU(ID_MORPH_ERODE,ImageFrame::OnErode)
                EVT_MENU(ID_MORPH_DILATE,ImageFrame::OnDilate)


                EVT_MENU(ID_EDGE_SOBEL,ImageFrame::OnSobelEdge)
                EVT_MENU(ID_EDGE_SCHARR,ImageFrame::OnScharrEdge)
                EVT_MENU(ID_EDGE_CANNY,ImageFrame::OnCannyEdge)
                EVT_MENU(ID_EDGE_LAPLACIAN,ImageFrame::OnLaplacianEdge)

                EVT_MENU(ID_EFFECT_CARTOON,ImageFrame::OnCartoonEffect)
                EVT_MENU(ID_EFFECT_LOMOGRAPHY,ImageFrame::OnLomographyEffect)

                EVT_MENU(ID_EQUALHISTGRAM,ImageFrame::OnEqualHist)
END_EVENT_TABLE()












