#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "HoughTransform.h"
#include "GeometricTransform.h"
#include "FourierTransformation.h"
#include "ImageEnhancement.h"

using namespace cv;
using namespace std;

Mat TestCorrelation(Mat image){
  Mat grayImg, h, l, e, result;
  double data_bf[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
  double data_h[3][3] = {{-1,2,-1},{-1,2,-1},{-1,2,-1}};
  double data_l[5][5] = {
                        {0,0,-1,0,0},
                        {0,-1,-2,-1,0},
                        {-1,-2,16,-2,-1},
                        {0,-1,-2,-1,0},
                        {0,0,-1,0,0}};
  double edge[3][3] = {{-1 , -1 , -1}, {-1, -8, -1}, {-1, -1 ,-1}};
  cvtColor(image, grayImg, CV_BGR2GRAY);
  grayImg.convertTo(grayImg, CV_64F);


  ImageProccessing *imgProccess = new ImageProccessing(grayImg);

  h = Mat(3, 3, CV_64F, data_h);
  l = Mat(5, 5, CV_64F, data_l);
  e = Mat(3, 3, CV_64F, &edge);
  //-----Test CrossCorrelation-----
  //Result from CrossCorrelation 2 matrix grayImg and h (BoxFiltering)
  //result = imgProccess->CrossConvolution(l);
  filter2D(grayImg, result, CV_64F, l);
  cout << result;
  result.convertTo(result, CV_8UC1);
  return result;
}

Mat HoughTransformLine(Mat image){
  Mat grayImg, l, edgeMap, result;
  double data_l[5][5] = {
                        {0,0,-1,0,0},
                        {0,-1,-2,-1,0},
                        {-1,-2,16,-2,-1},
                        {0,-1,-2,-1,0},
                        {0,0,-1,0,0}};
  cvtColor(image, grayImg, CV_BGR2GRAY);
  grayImg.convertTo(grayImg, CV_64F);
  l = Mat(5, 5, CV_64F, data_l);
  filter2D(grayImg, edgeMap, CV_64F, l);

  //Line Dectection
  int n = 300, m = 300;
  int k = 20;
  double *theta = new double[m];
  double *p = new double[n];
  cout << "Finish Initial" << endl;
  LineDectect(edgeMap, n, m, k, theta, p);
  //draw Line
  for (int i = 0; i < k; i++)
  {
    CvPoint p1, p2;
    p1.x = 0; p1.y = p[i]/sin(theta[i]);
    p2.x = p[i]/cos(theta[i]); p2.y = 0;
    //cout << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << endl;
    line(image, p1, p2, Scalar(255,255,0), 2, CV_AA);
    //arrowedLine(edgeMap, p1, p2, Scalar(255,255,0) ,5, 8, 0, 0.1);
  }

  //edgeMap.convertTo(edgeMap, CV_8UC1);
  return image;
}

Mat HoughTransformRound(Mat image){
  Mat grayImg, l, edgeMap, result;
  double data_l[5][5] = {
                        {0,0,-1,0,0},
                        {0,-1,-2,-1,0},
                        {-1,-2,16,-2,-1},
                        {0,-1,-2,-1,0},
                        {0,0,-1,0,0}};
  double edge[3][3] = {{-1 , -1 , -1}, {-1, 8, -1}, {-1, -1 ,-1}};
  //Create Edge Map by Laplacian
  cvtColor(image, grayImg, CV_BGR2GRAY);
  grayImg.convertTo(grayImg, CV_64F);
  l = Mat(5, 5, CV_64F, data_l);
  //Canny
  filter2D(grayImg, edgeMap, CV_64F, l);

  int k = 5;
  int *xCenter = new int[k];
  int *yCenter = new int[k];
  int *rRound = new int[k];
  RoundDectect(edgeMap, k, xCenter, yCenter, rRound);
  for (int i = 0; i < k; i++)
  {
    circle(image, Point(xCenter[i], yCenter[i]), rRound[i] , Scalar(255,0,0), 2, 8, 0);
  }
  //Round Dectection
  return image;
}

Mat Polar(Mat image){
  Mat result = Mat::zeros(50, 500, CV_8UC3);
  PolarTransform(image, result, Point(84, 84) , 80);
  return result;
}

Mat Geometric(Mat image){
  Mat grayImg, result, perpective;
  //image.convertTo(image, CV_8U);
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
  //ForwardMapping(image, result, perpective, Size(500,500));
  //BackwardMapping(image, result, perpective, Size(500,500));
  circle(result, Point(450, 280), 2 , Scalar(255,255,0), 2, 8, 0);
  //result.convertTo(result, CV_8UC1);
  return result;
}

Mat RotateImage(Mat image, double theta){
  Mat result;
  //Mat i = getRotationMatrix2D(Point2f(image.rows/2, image.cols/2), theta*180/PI, 1.0);
  //image.convertTo(image, CV_64F);
  double rotate[3][3] = {
    {cos(theta), -sin(theta) , image.rows/2+ 300},
    {sin(theta), cos(theta) , image.cols/2},
    {0, 0, 1}
  };
  Mat rotateMat = Mat(3, 3, CV_64F, rotate);
  warpPerspective(image, result, rotateMat,
    Size(1000, 1000));
  return result;
}

Mat FourierTransform(Mat image){
  image.convertTo(image, CV_BGR2GRAY);
  image.convertTo(image, CV_64F);
  Mat rowMat = rowsDFT(image);
  GammaCorrection(rowMat, 1.0, 0.1);
  Mat colMat = colsDFT(rowMat);
  LogTransform(colMat, 1.0);

  Mat re = rowMat;
  re.convertTo(re, CV_8UC1 );
  return re;
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


    namedWindow("Display Input Image", WINDOW_AUTOSIZE );
    imshow("Display Input Image", Geometric(image));

    waitKey(0);
    return 0;
}
