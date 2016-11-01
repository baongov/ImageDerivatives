#include <opencv2/opencv.hpp>
#include <math.h>
#include <algorithm>    // std::make_heap, std::pop_heap, std::push_heap, std::sort_heap
#include <vector>       // std::vector

using namespace cv;
using namespace std;

#define PI 3.14159265358

void ineDectect(Mat edgeMap, const int m, const int n, int &k, int* &theta, int* &p);
