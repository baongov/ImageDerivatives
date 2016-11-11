/**
 * @author ngovietbao
 *
 * Created on Oct 28
 */
 #include <stdio.h>
 #include <opencv2/opencv.hpp>
 #include <iostream>
 #include <math.h>
 #include "ImageProccessing.h"

using namespace cv;
using namespace std;

void forwardDFT(const double *s, const int &N, double *&a, double *&b);
void inverseDFT(const double *a, const double *b, const int &N, double *&s);
Mat rowsDFT(Mat grayImg);
Mat colsDFT(Mat grayImg);
