#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "ImageProccessing.h"
#include "HoughTransform.h"

using namespace cv;
using namespace std;

Mat TestCorrelation(Mat image)
{
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

Mat HoughTransform(Mat image)
{
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

    // if (abs(p1.y) > 10000)
    // {
    //   p1.x = image.rows;
    //   p1.y = p2.x;
    // }
    // if (abs(p2.x) > 10000)
    // {
    //   p2.x = p1.y;
    //   p1.y = image.cols;
    // }

    //cout << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << endl;
    line(image, p1, p2, Scalar(255,255,0), 2, CV_AA);
    //arrowedLine(edgeMap, p1, p2, Scalar(255,255,0) ,5, 8, 0, 0.1);
  }

  //edgeMap.convertTo(edgeMap, CV_8UC1);
  return image;
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
    imshow("Display Input Image", HoughTransform(image));

    waitKey(0);
    return 0;
}
