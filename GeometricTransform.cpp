#include "GeometricTransform.h"

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
  cout << a<< endl;
  cout << b << endl;
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

Mat Geometric(Mat image)
{
  Mat grayImg, result, perpective;
  image.convertTo(image, CV_64F);
  //Corresponding pairs pf points
  double input[3][4] = {{86, 454, 20, 398},
                        {100, 32, 315, 353},
                        {1, 1, 1, 1}};
  double output[3][4] = {{0, 500, 0, 500},
                         {0, 0, 500, 500},
                         {1, 1, 1, 1}};
  Mat xy = Mat(3, 4, CV_64F, input);
  Mat xy_ = Mat(3, 4, CV_64F, output);

  perpective = PerpectiveMatrix(xy, xy_);
  warpPerspective(image, result, perpective, Size(500,500));
  result.convertTo(result, CV_8UC1);
  return result;
}
