#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "ImageProccessing.h"

using namespace cv;
using namespace std;

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


    Mat grayImg, grayImg_d, h, l, result, result_s;
    double data_bf[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
    double data_h[3][3] = {{-1,2,-1},{-1,2,-1},{-1,2,-1}};
    double data_l[5][5] = {
                          {0,0,-1,0,0},
                          {0,-1,-2,-1,0},
                          {-1,-2,16,-2,-1},
                          {0,-1,-2,-1,0},
                          {0,0,-1,0,0}};
    cvtColor(image, grayImg, CV_BGR2GRAY);
    //cover grayImg to CV_64F (double) before filtering
    grayImg.convertTo(grayImg_d, CV_64F);


    ImageProccessing *imgProccess = new ImageProccessing(grayImg_d);

    //result = rowsDFT(grayImg_d, rows, cols);

    //-----Test CrossCorrelation-----

    h = Mat(3, 3, CV_64F, data_h);
    l = Mat(5, 5, CV_64F, data_l);
    //Result from CrossCorrelation 2 matrix grayImg and h (BoxFiltering)
    double max = 0, min = 0, threshold = 0;
    result = imgProccess->CrossCorrelation(l);
    //zero_cross = Zero_crossing(grayImg_d, l, threshold);
    //cout << zero_cross;
    //result = CrossConvolution(grayImg_d, h, max);
    //-----histogram-----
    //ImageHistogramGray(image);

    result.convertTo(result_s, CV_8UC1);
    namedWindow("Display Input Image", WINDOW_AUTOSIZE );
    imshow("Display Input Image", result_s);

    waitKey(0);

    return 0;
}
