#include "HoughTransform.h"

double minP;
double minT;
double delT;
double delP;

#define M 300
#define N 300

void printArray(int **accum, int m, int n)
{
  for (int i = 0; i < m; i++)
  {
    for (int j = 0; j < n; j++)
      cout << accum[i][j] << " ";
    cout << endl;
  }
}

void CompAccumulator(Mat &edgeMap, int **&accum, int m, int n)
{
  int rows = edgeMap.rows;
  int cols = edgeMap.cols;
  double t = 10.0;
  for (int i = 0; i < m; i++)
    for (int j = 0; j < n; j++)
    {
      accum[i][j] = 0;
    }
  double d = sqrt(rows*rows + cols*cols);
  minP = -d;
  minT = -PI/2.0;
  delT = PI/m;
  delP =  2*d/n;

  cout << "Voting... " << endl;

  for (int x = 0; x < rows; x++)
    for (int y = 0; y < cols; y++)
      if (edgeMap.at<double>(x, y) - t >= 0.0)
      {
        for (int j = 0; j < M; j++)
        {
          double theta = minT + j * delT + delT/2.0;
          double p = x * cos(theta) + y * sin(theta);
          int i = round((p - minP)/delP);
          if (i >= N) continue;
          accum[i][j] += 1;
        }
      }
  //printArray(accum, m, n);
}

void ApplyNonMaxima(int **&accum, int m, int n)
{
  for (int i = 1; i < m - 1; i++)
    for (int j = 1; j < n - 1; j++)
    {
      int NW = accum[i - 1][j - 1];
      int _N = accum[i - 1][j];
      int NE = accum[i - 1][j + 1];
      int E = accum[i][j + 1];
      int SE = accum[i + 1][j + 1];
      int S = accum[i + 1][j];
      int SW = accum[i + 1][j - 1];
      int W = accum[i][j - 1];
      int C = accum[i][j];

      int C1 = (C < NW) || (C < _N);
      int C2 = (C < NE) || (C < E);
      int C3 = (C < SE) || (C < S);
      int C4 = (C < SW) || (C < W);

      if (C1 || C2 || C3 || C4)
        accum[i][j] = 0;
    }
}

int findMinIdx(int *array, int n)
{
  int min = 0;
  for (int i = 1; i < n; i++)
  {
    if (*(array + i) < *(array + min))
      min = i;
  }
  return min;
}

void FindMaxima(int **&accum, int m, int n, double* &theta, double* &p, int &k)
{
  int max = 0, idx = 0;
  int **kmax = new int*[3];
  *(kmax + 0) = new int [k];
  *(kmax + 1) = new int [k];
  *(kmax + 2) = new int [k];

  for (int i = 0; i < k; i++)
    *(*(kmax + 0) + i) = -1;

  for (int i = 1; i < m - 1; i++)
    for (int j = 1; j < n - 1; j++)
    {
      if (accum[i][j] != 0)
      {
        int idxMin = findMinIdx(*(kmax + 0), k);
        int min = *(*(kmax + 0) + idxMin);
        if (min == -1 ||(accum[i][j] > min))
        {
          *(*(kmax + 0)+ idxMin) = accum[i][j];
          *(*(kmax + 1)+ idxMin) = j;
          *(*(kmax + 2)+ idxMin) = i;
          if (min == -1) idx++;
        }
      }
    }

  k = idx;
  for (int i = 0 ; i < k; i ++)
  {
    theta[i] = minT + *(*(kmax + 1)+ i) * delT + delT/2.0;
    p[i] = minP + *(*(kmax + 2)+ i) * delP + delP/2.0;
  }
}

void LineDectect(Mat edgeMap, int m, int n, int &k, double* &theta, double* &p)
{
  //Create an accumualator
  int **accum = new int*[M];
  for (int i = 0; i < M; i++)
    *(accum + i) = new int [N];
  cout << "CompAccumulator" << endl;
  CompAccumulator(edgeMap, accum, m, n);
  cout << "ApplyNonMaxima" << endl;
  ApplyNonMaxima(accum, m, n);
  cout << "FindMaxima" << endl;
  FindMaxima(accum, m, n, theta, p, k);

}
