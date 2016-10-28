/**
 * @author ngovietbao
 *
 * Created on Oct 28
 */

#include "ImageProccessing.h"

Mat ImageProccessing::CreateParodayMartrix(int height, int weight)
{
  Mat parody = Mat::zeros(rows + 2*height - 2, cols + 2*weight - 2 , CV_64F);
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      parody.at<double>(i + height - 1,j + weight - 1) = input.at<double>(i,j);

  return parody;
}
