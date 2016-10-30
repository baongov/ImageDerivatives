#include "EdgeDetection.h"

Mat EdgeDetection::Zero_crossing(Mat input, Mat h, int threshold){
  int rows_i = input.rows;
  int cols_i = input.cols;
  int rows_h = h.rows;
  int cols_h = h.cols;
  Mat result = Mat::zeros(rows_i + rows_h - 1, cols_i + cols_h - 1, CV_64F);
  Mat result_r = Mat::zeros(rows_i + rows_h - 1, cols_i + cols_h - 1, CV_64F);
  int min_out = 0, max_out = 0;
  Mat parody_I = CreateParodayMartrix(input, rows_h, cols_h);

  for (int i = 0; i < result.rows; i++)
    for (int j = 0; j < result.cols; j++)
    {
      int sum = 0;
      //Sum of multiplications from operands in matrix parody_I and h
      for (int x = 0; x < rows_h; x++)
        for (int y = 0; y < cols_h; y++)
          sum += h.at<double>(x,y)*parody_I.at<double>(i+x,j+y);
      result.at<double>(i,j) = sum;

      if (sum > max_out)
        max_out = sum;
      if (sum < min_out)
        min_out = sum;
    }
  //Covert matrix
  for (int i = 0; i < result.rows; i++)
    for (int j = 0; j < result.cols; j++)
      if (result.at<double>(i,j) < 0 && abs(result.at<double>(i,j)) > 0.04*max_out)
        result.at<double>(i,j) = -1;
      else if (result.at<double>(i,j) > 0 && abs(result.at<double>(i,j)) > 0.04*max_out)
        result.at<double>(i,j) = 1;
      else
        result.at<double>(i,j) = 0;
  //Apply mask 3x3 to detect edge point
  for (int i = 1; i < result.rows - 1; i++)
    for (int j = 1; j < result.cols - 1; j++)
    {
      double nw = result.at<double>(i-1,j-1);
      double n = result.at<double>(i-1,j);
      double ne = result.at<double>(i-1,j+1);
      double w = result.at<double>(i,j-1);
      double e = result.at<double>(i,j+1);
      double sw = result.at<double>(i+1,j-1);
      double s = result.at<double>(i+1,j);
      double se = result.at<double>(i+1,j+1);
      if (w*e < 0 || n*s < 0 || nw*se < 0 || sw*ne < 0)
        result_r.at<double>(i,j) = 255;
      else
        result_r.at<double>(i,j) = 0;
    }
  return result_r;
}
