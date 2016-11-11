#include "HoughTransform.h"

#define M 100
#define N 100
#define P 300

double idx2val(int idx, double min, double del)
{
  return min + idx * del + del/2.0;
}

int val2idx(double val, double min, double del)
{
  return round((val - min)/del);
}

void RoundDectect(Mat edgeMap, int &kl, int *&xCenter, int *&yCenter, int *&rRound)
{
  int rows = edgeMap.rows;
  int cols = edgeMap.cols;
  int numR = (int) min(cols, rows)/2 + 1;

  //Create 3D-Accumulator and set 0 for all cells in accumulator
  int ***accum = new int**[rows];
  for (int i = 0; i < rows; i++)
    accum[i] = new int*[cols];
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
    {
      accum[i][j] = new int[P];
      for (int k = 0; k < P; k++)
      {
        //cout << i << " " << j << " " << k << endl;
          accum[i][j][k] = 0;
      }
    }

  //CompAccumulator
  for (int x = 0; x < rows; x++)
    for (int y = 0; y < cols; y++)
      if (edgeMap.at<double>(x,y) - 10.0 >= 0.0)
      {
        for (double theta = 0; theta < 2*PI; theta += 2*PI/M)
          for (double r = (double)numR/N; r < (double)numR; r += (double)numR/N)
          {
            int xC = (int) (x - r*cos(theta));
            int yC = (int) (y - r*sin(theta));
            //cout << xC << " " <<yC  << " "<< (int) r << " "<< rows << " " << cols << " " << P << endl ;
            if ((xC >= 0 && xC < rows) && (yC >= 0 && yC < cols) && (r >= 0 && r < P))
              accum[xC][yC][(int)r] += 1;
          }
      }

  //Find K Maxima
  int idx = 0;
  int **kmax = new int*[4];
  *(kmax + 0) = new int [kl]; //Val of cells
  *(kmax + 1) = new int [kl]; //i-xc
  *(kmax + 2) = new int [kl]; //j-yc
  *(kmax + 3) = new int [kl]; //k-r

  for (int i = 0; i < kl; i++)
    *(*(kmax + 0) + i) = -1;

  int rMin = 2;

  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      for (int k = 2; k < P; k++)
      {
        if(accum[i][j][k] != 0)
        {
          int idxMin = findMinIdx(*(kmax + 0), kl);
          int min = *(*(kmax + 0) + idxMin);
          int distinctPoint = 1;
          if (idxMin > 0)
          {
            int xC = *(*(kmax + 1)+ idxMin);
            int yC = *(*(kmax + 2)+ idxMin);
            int r = *(*(kmax + 3)+ idxMin - 1);
            distinctPoint = abs(r - k) > 2;
          }
          if ((min == -1 || accum[i][j][k] > min) && distinctPoint)
          {
            //cout << (pow(xC -i,2) + pow(yC -j, 2)) << endl;
            //cout << r - k << endl;
            *(*(kmax + 0)+ idxMin) = accum[i][j][k];
            *(*(kmax + 1)+ idxMin) = i;
            *(*(kmax + 2)+ idxMin) = j;
            *(*(kmax + 3)+ idxMin) = k;
            if (min == -1) idx++;
          }
        }
      }
  printArray(kmax, 4, kl);

  kl = idx;
  xCenter = *(kmax + 1);
  yCenter = *(kmax + 2);
  rRound = *(kmax + 3);
}
