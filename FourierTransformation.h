/**
 * @author ngovietbao
 *
 * Created on Oct 28
 */
 #include <stdio.h>
 #include <opencv2/opencv.hpp>
 #include <iostream>
 #include <math.h>

class ImageProccessing{
public:
  void forwardDFT(const double *s, const int &N, double *&a, double *&b);
  void inverseDFT(const double *a, const double *b, const int &N, double *&s);
}
