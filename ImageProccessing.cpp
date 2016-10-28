/**
 * @author ngovietbao
 *
 * Created on Oct 28
 */

#include "ImageProccessing.h"

Mat ImageProccessing::CreateParodayMartrix(int height, int weight)
{
  Mat parody = Mat::zeros(rows + 2*height - 2, cols + 2*weight - 2 , CV_64F);
  for (int i = 0; i < rows_i; i++)
    for (int j = 0; j < cols_i; j++)
      parody.at<double>(i + rows_h - 1,j + cols_h - 1) = input.at<double>(i,j);

  return parody;
}
