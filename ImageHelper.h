//
// Created by Jsl on 2023/10/20.
//

#ifndef WXIMAGETOOL_IMAGEHELPER_H
#define WXIMAGETOOL_IMAGEHELPER_H
#include <opencv2/opencv.hpp>
#include <wx/image.h>

struct ImageHelper {


  static const wxImage Mat2Image(cv::Mat const &src);
  static cv::Mat convertToGray(const cv::Mat& src);
  static cv::Mat salt(const cv::Mat& src,int samples);

  static cv::Mat GaussianNoise(const cv::Mat& src);
  static cv::Mat PoissonNoise(const cv::Mat& src);

  static cv::Mat erode(const cv::Mat& src);
  static cv::Mat dilate(const cv::Mat& src);
  static cv::Mat opening(const cv::Mat& src);
  static cv::Mat closing(const cv::Mat& src);

  static cv::Mat soble(const cv::Mat& src);
  static cv::Mat scharr(const cv::Mat& src);
  static cv::Mat canny(const cv::Mat& src);
  static cv::Mat Laplacian(const cv::Mat& src);

  static cv::Mat CartoonEffect(const cv::Mat& src);
  static cv::Mat Lomography(const cv::Mat& src);

  static cv::Mat EqualHist(const cv::Mat& src);
};

#endif // WXIMAGETOOL_IMAGEHELPER_H
