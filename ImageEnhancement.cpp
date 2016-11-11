#include "ImageEnhancement.h"
#include "ImageProccessing.h"
Mat HistogramEqualization(Mat grayImg)
{
  int histSize = 256;
  float range[] = {0, 256};
  const float* histRange = {range};
  Mat result, histOut, grayImg_d;
  int rows = grayImg.rows;
  int cols = grayImg.cols;
  grayImg.convertTo(grayImg_d, CV_64F);
  calcHist(&grayImg, 1, 0, Mat(), histOut, 1, &histSize, &histRange, true, false); // Confingure histogram for grayscale
  histOut.convertTo(histOut, CV_64F);
  result = Mat::zeros(rows, cols, CV_64F);
  for (int i = 0; i < 256; i++)
  {
    histOut.at<double>(i, 0) = histOut.at<double>(i, 0)/(rows*cols)*255;
  }
  for (int i = 1; i < 256; i++)
  {
    histOut.at<double>(i, 0) += histOut.at<double>(i-1, 0);
  }
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
    {
      result.at<double>(i,j) = (int) histOut.at<double>((int)grayImg_d.at<double>(i,j), 0);
    }
  return result;
}

void LogTransform(Mat &grayImg, double a)
{
    for (int i = 0; i < grayImg.rows; i++)
      for (int j = 0; j < grayImg.cols; j++)
      {
         int val = grayImg.at<double>(i,j);
         grayImg.at<double>(i,j) = a * log(1 + val);
      }

    ConvertToPixelMatrix(grayImg);
}

void GammaCorrection(Mat &grayImg, double a, double gamma)
{
  for (int i = 0; i < grayImg.rows; i++)
    for (int j = 0; j < grayImg.cols; j++)
    {
       int val = grayImg.at<double>(i,j);
       grayImg.at<double>(i,j) = a * pow(val, gamma);
    }

  ConvertToPixelMatrix(grayImg);
}
