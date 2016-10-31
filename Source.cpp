#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "ImageProccessing.h"

using namespace cv;
using namespace std;

Mat TestCorrelation(Mat image)
{
  Mat grayImg, h, l, result;
  double data_bf[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
  double data_h[3][3] = {{-1,2,-1},{-1,2,-1},{-1,2,-1}};
  double data_l[5][5] = {
                        {0,0,-1,0,0},
                        {0,-1,-2,-1,0},
                        {-1,-2,16,-2,-1},
                        {0,-1,-2,-1,0},
                        {0,0,-1,0,0}};
  cvtColor(image, grayImg, CV_BGR2GRAY);
  grayImg.convertTo(grayImg, CV_64F);


  ImageProccessing *imgProccess = new ImageProccessing(grayImg);

  h = Mat(3, 3, CV_64F, data_h);
  l = Mat(5, 5, CV_64F, data_l);
  //-----Test CrossCorrelation-----
  //Result from CrossCorrelation 2 matrix grayImg and h (BoxFiltering)
  result = imgProccess->CrossCorrelation(l);

  result.convertTo(result, CV_8UC1);
  return result;
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
  double input[3][4] = {{86, 454, 20, 398},{100, 32, 315, 353},{1, 1, 1, 1}};
  double output[3][4] = {{0, 500, 0, 500},{0, 0, 500, 500},{1, 1, 1, 1}};
  Mat xy = Mat(3, 4, CV_64F, input);
  Mat xy_ = Mat(3, 4, CV_64F, output);

  perpective = PerpectiveMatrix(xy, xy_);
  warpPerspective(image, result, perpective, Size(500,500));
  result.convertTo(result, CV_8UC1);
  return result;
}

int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image = imread( argv[1], CV_LOAD_IMAGE_COLOR );
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Display Output Image", WINDOW_AUTOSIZE );
    imshow("Display Output Image", image);

    //----------------------------------
    cout << endl << "Please choose one of these options: " << endl;
    cout << "1. Display histogram" << endl;
    cout << "2. Box Filtering" << endl;

    Geometric(image);
    namedWindow("Display Input Image", WINDOW_AUTOSIZE );
    imshow("Display Input Image", Geometric(image));

    waitKey(0);
    return 0;
}
