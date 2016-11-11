#include "GeometricTransform.h"

void PolarTransform(Mat image, Mat &result, Point pCen , int rMax)
{
  int rows_r = result.rows;
  int cols_r = result.cols;
  int rMin = rMax - result.rows; //70 - 50 = 20
  cout << "map points" <<endl;
  for (int i = 0; i < cols_r; i++)
  {
    double theta = 2*PI/cols_r*i;
    for (int j = 0; j < rows_r; j++)
    {
      int x = pCen.x + (j + rMin)*cos(theta);
      int y = pCen.y + (j + rMin)*sin(theta);
      //cout << x << " " << y << " " << image.rows << " " <<image.cols << endl;
      //cout << j << " " << i << " " << result.rows << " " << result.cols << endl;
      result.at<cv::Vec3b>(j,i) = image.at<cv::Vec3b>(x, y);
    }
  }
  cout << "finish map points" <<endl;
}

Mat PerpectiveMatrix(Mat input, Mat output)
{
  //if (input.cols != 4 && input.cols != output.cols) return null;
  Mat perpective = Mat::zeros(3, 3, CV_64F);
  Mat a = Mat::zeros(8, 8, CV_64F);
  Mat b = Mat::zeros(8, 1, CV_64F);
  Mat c;

  for (int i = 0; i < 8; i+=2)
  {
    b.at<double>(i,0) = output.at<double>(0,i/2); //=x'
    b.at<double>(i+1,0) = output.at<double>(1,i/2); //=y'
    //x'
    a.at<double>(i,0) = input.at<double>(0,i/2); //=x
    a.at<double>(i,1) = input.at<double>(1,i/2); //=y
    a.at<double>(i,2) = 1;
    a.at<double>(i,3) = 0;
    a.at<double>(i,4) = 0;
    a.at<double>(i,5) = 0;
    a.at<double>(i,6) = - output.at<double>(0,i/2) * input.at<double>(0,i/2);
    a.at<double>(i,7) = - output.at<double>(0,i/2) * input.at<double>(1,i/2);
    //y'
    a.at<double>(i+1,0) = 0;
    a.at<double>(i+1,1) = 0;
    a.at<double>(i+1,2) = 0;
    a.at<double>(i+1,3) = input.at<double>(0,i/2); //=x
    a.at<double>(i+1,4) = input.at<double>(1,i/2); //=y
    a.at<double>(i+1,5) = 1;
    a.at<double>(i+1,6) = - output.at<double>(1,i/2) * input.at<double>(0,i/2);
    a.at<double>(i+1,7) = - output.at<double>(1,i/2) * input.at<double>(1,i/2);
  }
  c = a.inv(0)*b;

  for (int i = 0; i < 8; i++)
  {
    int rows = i/3;
    int cols = i % 3;
    perpective.at<double>(rows, cols) = c.at<double>(i,0);
  }
  perpective.at<double>(2, 2) = 1;

  return perpective;
}
