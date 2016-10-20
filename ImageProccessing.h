#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

class ImageProccessing
{
  Mat img;
  int rows_i, cols_i;

public:
  ImageProccessing(Mat img)
  {
      this.img = img;
      rows_i = img.rows;
      cols_i = img.cols;
  }
  void ImageHistogramGray();
  Mat ImageCorrelation(Mat h);
  Mat ImageConvolution(Mat h);
  Mat Zero_crossing(Mat h, int threshold);
  void forwardDFT(const double *s, const int &N, double *&a, double *&b);
  void inverseDFT(const double *a, const double *b, const int &N, double *&s);

private:
  int min2k(int num);
  Mat CreateParodayMartrix(int rows_h, int cols_h); //Create Paroday Martrix of image that corresponds to a Matrix H size rows_h x cols_h
  Mat CreateNxNMaxtrix(int N); //CreateNxNMaxtrix from image
  Mat CrossCorrelation(Mat h, double &max_out, double &min_out);
  Mat CrossConvolution(Mat h, double &max_out, double &min_out);
}
