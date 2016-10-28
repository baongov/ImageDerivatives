/**
 * @author ngovietbao
 *
 * Created on Oct 28
 */

#include "ImageProccessing.h"

void ImageProccessing::ConvertToPixelMatrix(Mat &mat){
  /**
   * Format matrix by setting its value to be in [0, 255]
   */
  int minPixel, maxPixel; //Min and max value after doing correlation
  //Init value
  minPixel = 0;
  maxPixel = 0;
  //Identify min and max from given matrix
  for (int i = 0; i < mat.rows; i++)
    for (int j = 0; j < mat.cols; j++){
      if (mat.at<double>(i,j) > max_out)
        maxPixel = mat.at<double>(i,j;
      if (mat.at<double>(i,j) < min_out)
        minPixel = mat.at<double>(i,j;
    }

  //Covert matrix to pixel colors
  for (int i = 0; i < mat.rows; i++)
    for (int j = 0; j < mat.cols; j++)
      mat.at<double>(i,j) = int((mat.at<double>(i,j) - minPixel)/ (maxPixel - minPixel) * 255);
}

Mat ImageProccessing::CreateParodayMartrix(int height, int weight){
  Mat parody = Mat::zeros(rows + 2*height - 2, cols + 2*weight - 2 , CV_64F);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      parody.at<double>(i + height - 1,j + weight - 1) = input.at<double>(i,j);

  return parody;
}

Mat ImageProccessing::CreatePadingMaxtrix(int n){
  Mat result = Mat::zeros(n, n, CV_64F);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      result.at<double>(i,j) = input.at<double>(i,j);
  return result;
}

Mat ImageProccessing::CrossCorrelation(Mat h){
  int rows_h = h.rows;
  int cols_h = h.cols;
  Mat result = Mat::zeros(rows + rows_h - 1, cols + cols_h - 1, CV_64F);
  Mat parody_I = CreateParodayMartrix(rows_h, cols_h);

  for (int i = 0; i < result.rows; i++)
    for (int j = 0; j < result.cols; j++)
    {
      int sum = 0;
      //Sum of multiplications from operands in matrix parody_I and h
      for (int x = 0; x < rows_h; x++)
        for (int y = 0; y < cols_h; y++)
          sum += h.at<double>(x,y)*parody_I.at<double>(i+x,j+y);
      result.at<double>(i,j) = sum;
      //Store max value for calculating pixel colors
      if (sum > max_out)
        maxPixel = sum;
      if (sum < min_out)
        minPixel = sum;
    }

  ConvertToPixelMatrix(result);
  return result;
}

Mat ImageProccessing::CrossConvolution(Mat h){
  Mat hFlip;
  flip(h, hFlip, 1);
  return CrossCorrelation(hFlip);
}
