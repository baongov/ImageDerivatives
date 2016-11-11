/**
 * @author ngovietbao
 *
 * Created on Oct 30
 */
 #include <stdio.h>
 #include <opencv2/opencv.hpp>
 #include <iostream>
 #include <math.h>

using namespace cv;

Mat HistogramEqualization(Mat grayImg);
void LogTransform(Mat &grayImg, double a);
void GammaCorrection(Mat &grayImg, double a, double gamma);
