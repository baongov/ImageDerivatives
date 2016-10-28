/**
 * @author ngovietbao
 *
 * Created on Oct 25
 */

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;

class ImageProccessing
{
  Mat img;
  int rows, cols;

public:
   ImageProccessing(Mat img)
   {
       this->img = img;
       rows = img.rows;
       cols = img.cols;
   }

    Mat CreateParodayMartrix(int height, int weight); //Create Paroday Martrix of image that corresponds to a Matrix H size rows_h x cols_h
    Mat CreatePadingMaxtrix(int N); //CreateNxNMaxtrix from image
    Mat CrossCorrelation(Mat h);
    Mat CrossConvolution(Mat h);
private:
    void ConvertToPixelMatrix(Mat &mat);
};
