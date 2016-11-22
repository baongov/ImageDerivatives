#include "GeometricTransform.h"

void PolarTransform(Mat image, Mat &result, Point pCen , int rMax)
{
  int rows_r = result.rows;
  int cols_r = result.cols;
  int rMin = rMax - result.rows; //70 - 50 = 20
  cout << "map points" <<endl;
  for (int i = 0; i < cols_r; i++)
  {
    double theta = 2*PI/cols_r*i;
    for (int j = 0; j < rows_r; j++)
    {
      int x = pCen.x + (j + rMin)*cos(theta);
      int y = pCen.y + (j + rMin)*sin(theta);
      //cout << x << " " << y << " " << image.rows << " " <<image.cols << endl;
      //cout << j << " " << i << " " << result.rows << " " << result.cols << endl;
      result.at<cv::Vec3b>(j,i) = image.at<cv::Vec3b>(x, y);
    }
  }
  cout << "finish map points" <<endl;
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

void ForwardMapping(Mat input, Mat &output, Mat perpective, Size size){
  int rows = input.rows;
  int cols = input.cols;
  int px, py;

  output = Mat::zeros(size, CV_8UC3);

  for (int i = 0; i < rows ; i++)
    for (int j = 0; j < cols; j++)
    {
      double dataP[3][1] =  {{i},{j},{1}};
      Mat P = Mat(3, 1 , CV_64F, dataP);
      Mat P_ = perpective * P ;

        px = round(P_.at<double>(0, 0)/P_.at<double>(2, 0));
        py = round(P_.at<double>(1, 0)/P_.at<double>(2, 0));
        //cout << P_ << endl;

      if (px >= 0 && px <= output.rows && py >= 0 && py <= output.cols)
      {
        // Vec3b &intensityI = input.at<Vec3b>(i, j);
        // Vec3b &intensityO = output.at<Vec3b>(px, py);
        //cout << px << " " << py << endl;
        output.at<Vec3b>(px, py) = input.at<Vec3b>(i, j);
        // intensityO.val[0] = intensityI.val[0];
        // intensityO.val[1] = intensityI.val[1];
        // intensityO.val[2] = intensityI.val[2];
        //cout << input.at<Vec3b>(i, j) << endl;
      }
      if (i == 20 && j == 315)
      {
        cout << px << " " << py;
        cout << input.at<Vec3b>(i, j) << endl;
        cout << output.at<Vec3b>(px, py) << endl;
      }
    }
}

void BackwardMapping(Mat input, Mat &output, Mat perpective, Size size){
  int rows = size.width;
  int cols = size.height;
  int px, py;
  cout << rows << " " << cols << endl;
  output = Mat::zeros(size, CV_8UC3);
  for (int i = 0; i < rows ; i++)
    for (int j = 0; j < cols; j++)
    {
      double dataP_[3][1] =  {{i},{j},{1}};
      Mat P_ = Mat(3, 1 , CV_64F, dataP_);
      Mat P = perpective.inv(0) * P_ ;


      py = round(P.at<double>(0, 0)/P.at<double>(2, 0));
      px = round(P.at<double>(1, 0)/P.at<double>(2, 0));

      if (px >= 0 && px <= input.rows && py >= 0 && py <= input.cols)
      {
        output.at<Vec3b>(j, i) = input.at<Vec3b>(px, py);
      }

      if (i == 450 && j == 280)
      {
        cout << px << " " << py;
        cout << output.at<Vec3b>(i, j) << endl;
        cout << input.at<Vec3b>(px, py) << endl;
      }
    }
}
