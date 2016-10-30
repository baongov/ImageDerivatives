#include "FourierTransformation.h"

void FourierTransformation::forwardDFT(const double *s, const int &N, double *&a, double *&b)
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

void FourierTransformation::inverseDFT(const double *a, const double *b, const int &N, double *&s)
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

Mat rowsDFT(Mat grayImg, int rows, int cols)
{
  int N = 0;
  if (rows > cols)
    N = min2k(rows);
  else
    N = min2k(cols);
  cout << "min2k = " << N << endl;
  //Transfer normal image to a NxN image
  Mat nnMatrix = CreateNxNMaxtrix(grayImg, N);
  //DFT on each row
  for (int i = 0; i < N; i++)
  {
    //init relevant varible
    double *s_in = new double [N];
    double *s_out = new double [N];
    double *a = new double [N/2 + 1];
    double *b = new double [N/2 + 1];
    //read s_in from image at rows_i
    for (int j = 0; j < N; j++)
      *(s_in + j) = nnMatrix.at<double>(i, j);

    forwardDFT(s_in, N, a, b);

    for (int j = 0; j < N/2; j++)
    {
      nnMatrix.at<double>(i, j) = 5*sqrt(pow(*(a+j),2) + pow(*(b+j),2));
      nnMatrix.at<double>(i, N - 1 - j) = 5*sqrt(pow(*(a+j),2) + pow(*(b+j),2));
    }

    inverseDFT(a, b, N, s_out);

    //for (int j = 0; j < N; j++)
      //nnMatrix.at<double>(i, j) = *(s_out + j);

    delete[] s_in, s_out, a, b;
  }
  return nnMatrix;
}
