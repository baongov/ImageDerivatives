/**
 * @author ngovietbao
 *
 * Created on Oct 30
 */
 #include <stdio.h>
 #include <opencv2/opencv.hpp>
 #include <iostream>
 #include <math.h>

class EdgeDetection{
public:
  Mat ZeroCrossing(Mat input, Mat h, int threshold);
}
