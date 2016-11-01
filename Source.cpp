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
  e = Mat(3, 3, CV_64F, edge);
  //-----Test CrossCorrelation-----
  //Result from CrossCorrelation 2 matrix grayImg and h (BoxFiltering)
  result = imgProccess->CrossCorrelation(e);

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

    namedWindow("Display Input Image", WINDOW_AUTOSIZE );
    imshow("Display Input Image", TestCorrelation(image));

    waitKey(0);
    return 0;
}
