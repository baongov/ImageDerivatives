#include "FourierTransformation.h"

void rot90(cv::Mat &matImage, int rotflag){
  //1=CW, 2=CCW, 3=180
  if (rotflag == 1){
    transpose(matImage, matImage);
    flip(matImage, matImage,1); //transpose+flip(1)=CW
  } else if (rotflag == 2) {
    transpose(matImage, matImage);
    flip(matImage, matImage,0); //transpose+flip(0)=CCW
  } else if (rotflag ==3){
    flip(matImage, matImage,-1);    //flip(-1)=180
  } else if (rotflag != 0){ //if not 0,1,2,3:
    cout  << "Unknown rotation flag(" << rotflag << ")" << endl;
  }
}

void forwardDFT(const double *s, const int &N, double *&a, double *&b)
{
    // note: this code is not optimised at all, written for clarity not speed.
    for (int k = 0; k <= N / 2; ++k) {
        a[k] = b[k] = 0;
        for (int x = 0; x < N; ++x) {
            a[k] += s[x] * cos(2 * M_PI / N * k * x);
            b[k] += s[x] * sin(2 * M_PI / N * k * x);
        }
        // normalization
        a[k] *= (k == 0 || k == N / 2) ? 1. / N : 2. / N;
        b[k] *= 2. / N;
    }
}
/************************************************************************/

void inverseDFT(const double *a, const double *b, const int &N, double *&s)
{
    // note: this code is not optimised at all, written for clarity not speed.
    for (int x = 0; x < N; ++x) {
        s[x] = a[0];
        for (int k = 1; k <= N / 2; ++k) {
            s[x] += a[k] * cos(2 * M_PI / N * k * x) + b[k] * sin(2 * M_PI / N * k * x);
        }
    }
}

int min2k(int num)
{
  double log2num = log((double) num) / log(2.0);
  int result = (int) log2num;
  if (log2num - (double)result == 0.0)
    return pow(2, result);
  else
    return pow(2, result + 1);
}

Mat rowsDFT(Mat grayImg)
{
  Mat out;
  int rows = grayImg.rows;
  int cols = grayImg.cols;
  int N;
  if (rows != cols || rows != min2k(rows))
  {
    if (rows > cols)
      N = min2k(rows);
    else
      N = min2k(cols);
  }

  Mat nnMatrix = Mat::zeros(rows, N, CV_64F);
  resize(grayImg,grayImg,Size(rows,N));

  //DFT on each row
  for (int i = 0; i < rows; i++)
  {
    //init relevant varible
    double *s_in = new double [N];
    double *s_out = new double [N];
    double *a = new double [N/2];
    double *b = new double [N/2];
    //read s_in from image at rows_i
    for (int j = 0; j < N; j++)
      *(s_in + j) = grayImg.at<double>(i, j);

    forwardDFT(s_in, N, a, b);

    for (int j = 0; j < N/2; j++)
    {
      nnMatrix.at<double>(i, j) = sqrt(pow(*(a+j),2) + pow(*(b+j),2)) ;
      //cout << nnMatrix.at<double>(i, j) << endl;
      nnMatrix.at<double>(i, N - 1 - j) = sqrt(pow(*(a+j),2) + pow(*(b+j),2));
    }

    delete[] s_in, s_out, a, b;
  }

  //ConvertToPixelMatrix(nnMatrix);
  return nnMatrix;
}

Mat colsDFT(Mat grayImg){
  rot90(grayImg,1);
  Mat dftMat = rowsDFT(grayImg);
  rot90(dftMat,1);
  rot90(dftMat,1);
  rot90(dftMat,1);
  return dftMat;
}
