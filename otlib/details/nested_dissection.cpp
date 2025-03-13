#include <Eigen/Core>
#include <cassert>

using namespace Eigen;

namespace otmap {
  
  void nestdiss_ordering_impl(int i0, int j0, int rows, int cols, int stride, int* &perm)
  {
    int i1 = i0 + rows;
    int j1 = j0 + cols;
    const int th = 4;
  
    if (rows <= th || cols <= th)
    {
      for (int i = i0; i < i1; ++i)
        for (int j = j0; j < j1; ++j)
        {
          *perm = i + j * stride;
          ++perm;
        }
    }
    else
    {
      // Split in 4 quadrants
      int si = i0 + rows / 2;
      int sj = j0 + cols / 2;
  
      for (int i = i0; i < i1; ++i)
      {
        *perm = i + stride * sj;
        ++perm;
      }
      for (int j = j0; j < j1; ++j)
      {
        if (j != sj)
        {
          *perm = si + stride * j;
          ++perm;
        }
      }
  
      // Recursively order each quadrant
      nestdiss_ordering_impl(i0, j0, si - i0, sj - j0, stride, perm);
      nestdiss_ordering_impl(si + 1, j0, i1 - si - 1, sj - j0, stride, perm);
      nestdiss_ordering_impl(si + 1, sj + 1, i1 - si - 1, j1 - sj - 1, stride, perm);
      nestdiss_ordering_impl(i0, sj + 1, si - i0, j1 - sj - 1, stride, perm);
    }
  }
  
  void nestdiss_ordering(int width, int height, int* perm)
  {
    int* p = perm;
    nestdiss_ordering_impl(0, 0, width, height, width, p);
    assert(perm + width * height == p);
  }

} // namespace otmap