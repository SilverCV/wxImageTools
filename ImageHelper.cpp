//
// Created by Jsl on 2023/10/20.
//

#include "ImageHelper.h"
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafilters.hpp>
#include <opencv2/cudawarping.hpp>
#include <opencv2/cudacodec.hpp>
const wxImage ImageHelper::Mat2Image(const cv::Mat &src) {
    if (src.channels() == 1 ){
        return wxImage(src.cols,src.rows,src.data,true);
    }
    if (src.channels() == 3){
        cv::Mat dst;
        cv::cvtColor(src,dst,cv::COLOR_BGR2RGB);
        wxImage image(dst.cols,dst.rows,true);
        size_t size = dst.elemSize() * dst.cols * dst.rows;
        uint8_t* data = (uint8_t *) malloc(size);
        memcpy(data,dst.data,size);
        image.SetData(data,false);
        return image;
    }
    if (src.channels() == 4) {
        cv::Mat rgb,channels[4];
        cv::split(src,channels);
        cv::merge(channels,3,rgb);
        return wxImage(src.cols,src.rows,rgb.data,channels[3].data,false);
    }
    return wxImage();

}


cv::Mat ImageHelper::salt(const cv::Mat &src, int samples) {
    cv::Mat dst = src;
    if (src.channels() == 1)
    {
        for(int i=0;i < samples;++i) {
            int x  = std::rand() % dst.cols;
            int y = std::rand() % dst.rows;
            dst.at<uchar>(y,x) = 255;
        }

    }
    else if (src.channels() == 3)
    {
        for(int i=0;i < samples;++i) {
            int x  = std::rand() % dst.cols;
            int y = std::rand() % dst.rows;
            dst.at<cv::Vec3b>(y,x) = cv::Vec3b(255,255,255);
        }
    }
    return dst;
}

cv::Mat ImageHelper::GaussianNoise(const cv::Mat &src) {
    cv::Mat dst = cv::Mat::zeros(src.size(),src.type());
    cv::RNG rng;
    rng.fill(dst,cv::RNG::NORMAL,10,20);
    dst = dst + src;
    return dst;
}



cv::Mat ImageHelper::PoissonNoise(const cv::Mat &src) {
    cv::Mat dst = src;
    return dst;
}

cv::Mat ImageHelper::soble(const cv::Mat &src) {
    cv::Mat dst;
    auto gpu_dst = convertToGray(src);


    cv::Mat x_grad,y_grad;
    cv::Scharr(gpu_dst,x_grad,-1,1,0);
    cv::Scharr(gpu_dst,y_grad,-1,0,1);
    cv::addWeighted(x_grad,0.5,y_grad,0.5,0,gpu_dst);

    return dst;
}

cv::Mat ImageHelper::scharr(const cv::Mat &src) {
    cv::Mat dst;
    auto gpu_dst = convertToGray(src);
    cv::Mat x_grad,y_grad;
    cv::Sobel(gpu_dst,x_grad,-1,1,0);
    cv::Sobel(gpu_dst,y_grad,-1,0,1);


    cv::addWeighted(x_grad,0.5,y_grad,0.5,0,gpu_dst);

    return dst;
}

cv::Mat ImageHelper::canny(const cv::Mat &src) {
    cv::Mat dst;
    auto gpu_dst = convertToGray(src);

    cv::Canny(gpu_dst,gpu_dst,60,160,3,true);


    return gpu_dst;
}

cv::Mat ImageHelper::Laplacian(const cv::Mat &src) {
    cv::Mat dst;
    auto gpu_dst = convertToGray(src);
    //filter
    cv::medianBlur(gpu_dst,gpu_dst,5);
    //lapalican
    cv::Laplacian(gpu_dst,gpu_dst,-1,3,1);

    return gpu_dst;
}

cv::Mat ImageHelper::erode(const cv::Mat &src) {
    auto gray = convertToGray(src);
    cv::threshold(gray,gray,60,150,cv::THRESH_BINARY);
    auto kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
    cv::erode(gray,gray,kernel);
    return gray;
}

cv::Mat ImageHelper::dilate(const cv::Mat &src) {
    auto gray = convertToGray(src);
    cv::threshold(gray,gray,60,150,cv::THRESH_BINARY);
    auto kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
    cv::dilate(gray,gray,kernel);
    return gray;
}

cv::Mat ImageHelper::opening(const cv::Mat &src) {
    auto gray = convertToGray(src);
    cv::threshold(gray,gray,60,150,cv::THRESH_BINARY);
    auto kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
    cv::erode(gray,gray,kernel);
    cv::dilate(gray,gray,kernel);
    return gray;
}

cv::Mat ImageHelper::closing(const cv::Mat &src) {
    auto gray = convertToGray(src);
    cv::threshold(gray,gray,60,150,cv::THRESH_BINARY);
    auto kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3));
    cv::dilate(gray,gray,kernel);
    cv::erode(gray,gray,kernel);
    return gray;
}

cv::Mat ImageHelper::convertToGray(const cv::Mat &src) {
    cv::Mat  dst;

    if (src.channels()==3){
        cv::cvtColor(src,dst,cv::COLOR_BGR2GRAY);
    }
    else if (src.channels() == 4) {
        cv::cvtColor(src,dst,cv::COLOR_BGRA2GRAY);
    }
    return dst;
}



cv::Mat ImageHelper::CartoonEffect(const cv::Mat &src) {
    cv::Mat dst;
    cv::Mat imageMedian;
    cv::medianBlur(src,imageMedian,7);
    //canny
    cv::Mat canny;
    cv::Canny(imageMedian,canny,50,150);
    //dilate
    auto kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(2,2));
    cv::dilate(canny,canny,kernel);
    //
    canny = canny / 255;
    canny = 1 - canny;
    //
    cv::Mat cannyF;
    canny.convertTo(cannyF,CV_32FC3);
    cv::blur(cannyF,cannyF,cv::Size(5,5));
    cv::Mat bfMat;
    cv::bilateralFilter(src,bfMat,9,150,150);
    dst = bfMat / 25;
    dst = dst * 25;

    //
    cv::Mat image3c;
    cv::Mat imageCanny[] = {cannyF,cannyF,cannyF};
    cv::merge(imageCanny,3,image3c);
    cv::Mat resultf;
    dst.convertTo(resultf,CV_32FC3);
    cv::multiply(resultf,image3c,resultf);
    resultf.convertTo(dst,CV_8UC3);
    return dst;
}

cv::Mat ImageHelper::Lomography(const cv::Mat &src) {
    const float E = std::expf(1.0);
    cv::Mat_<uchar> lut(1,256);
    for(int i=0;i < 256;++i) {
        float x = static_cast<float>(i) / 256.0f;
        lut.at<uchar>(i) = cv::saturate_cast<uchar>(256.0f * (1.0f/ (1.0f + std::powf(E,(-(x - 0.5f) / 0.1f)))));
    }
    std::vector<cv::Mat> bgr;
    cv::split(src,bgr);
    cv::LUT(bgr[2],lut,bgr[2]);
    cv::Mat dst;
    cv::merge(bgr,dst);
    dst.convertTo(dst,CV_32FC3);

    cv::Mat halo(src.size(),CV_32FC3,cv::Scalar::all(0.3f));
    cv::circle(halo,cv::Point(src.cols/2,src.rows/2),src.cols/3,cv::Scalar::all(1.0f),-1);
    cv::blur(halo,halo,cv::Size(src.cols/3,src.rows/3));

    cv::multiply(dst,halo,dst);
    dst.convertTo(dst,CV_8UC3);
    return dst;

}

cv::Mat ImageHelper::EqualHist(const cv::Mat &src) {
   cv::Mat dst;
   if (src.channels() == 1){
       cv::equalizeHist(src,dst);
   }
   else
   {
       std::vector<cv::Mat> channels;
       cv::split(src,channels);
       for(auto& c: channels){
           cv::equalizeHist(c,c);
       }
       cv::merge(channels,dst);
   }
   return dst;
}
