#include "HoughTransform.h"

double minP;
double minT;
double delT;
double delP;

void CompAccumulator(Mat &edgeMap, int **&accum, int m, int n)
{
  int rows = edgeMap.rows;
  int cols = edgeMap.cols;

  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
    {
      accum[i][j] = 0;
    }
  double d = sqrt(rows*rows + cols*cols);
  minP = -d;
  minT = -PI/2.0;
  delT = PI/m;
  delP =  d/n;

  for (int x = 0; x < rows; x++)
    for (int y = 0; y < cols; y++)
      if (edgeMap.at<double>(x, y) != 0.0)
      {
        for (int j = 0; j < M; j++)
        {
          double theta = minT + j * delT - delT/2.0;
          double p = x * cos(theta) + y * sin(theta);
          int i = round((p - minP)/delP);
          accum[i][j] += 1;
        }
      }
}

void ApplyNonMaxima(int **&accum, int m, int n)
{
  for (int i = 1; i < m - 1; i++)
    for (int j = 1; j < n - 1; j++)
    {
      int NW = accum[i - 1][j - 1];
      int N = accum[i - 1][j];
      int NE = accum[i - 1][j + 1];
      int E = accum[i][j + 1];
      int SE = accum[i + 1][j + 1];
      int S = accum[i + 1][j - 1];
      int W = accum[i][j - 1];
      int C = accum[i][j]

      int C1 = (C < NW) || (C < N);
      int C2 = (C < NE) || (C < E);
      int C3 = (C < SE) || (C < S);
      int C4 = (C < SW) || (C < W);

      if (C1 || C2 || C3 || C4)
        accum[i][j] = 0;
    }
}

void findMinIdx(int *array, int n)
{
  int min = 0;
  for (int i = 1; i < n; i++)
  {
    if (*(array + i) < *(array + min))
      min = i;
  }
  return min;
}

void FindMaxima(int **&accum, int m, int n, int* &theta, int* &p, int &k)
{
  int max = 0, idx = 0;
  int **kmax = new int*[3];
  *(kmax + 0) = new int [k];
  *(kmax + 1) = new int [k];
  *(kmax + 2) = new int [k];

  for (int i = 0; i < k; i++)
    (*(key + 0)+ i) = -1;

  for (int i = 1; i < m - 1; i++)
    for (int j = 1; j < n - 1; j++)
    {
      if (A[i][j] != 0)
      {
        int idxMin = findMinIdx(*(kmax + 0), k);
        int min = *(*(key + 0) + idxMin);
        if (min == -1 ||(A[i][j] > min))
        {
          *(*(key + 0)+ idxMin) = A[i][j];
          *(*(key + 1)+ idxMin) = minT + j * delT + delT/2.0;
          *(*(key + 2)+ idxMin) = minP + i * delP + delP/2.0;
          if (min == -1) idx++;
        }
      }
    }

  k = idx;
  for (int i = 0 ; i < k; i ++)
  {
    p[i] = *(*(key + 2)+ i);
    theta[i] = *(*(key + 3)+ i);
  }
}

void LineDectect(Mat edgeMap, const int m, const int n, int &k, int* &theta, int* &p)
{
  //Create an accumualator
  int **accum = new int[m][n];
  CompAccumulator(edgeMap, accum, m, n);
  ApplyNonMaxima(accum, m, n)
  FindMaxima(accum, m, n, theta, p, k);

}
