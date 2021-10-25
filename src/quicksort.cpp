#include "../include/quicksort.h"

unsigned partition(unsigned *array, unsigned start, unsigned finish)
{
  unsigned pivot = array[finish-1];
  unsigned i = start-1;
  for(unsigned j=start; j < finish-1; ++j) {
    if(array[j] <= pivot) {
      ++i;
      std::swap(array[j], array[i]);
    }
  }
  std::swap(array[i+1], array[finish-1]);
  return i + 1;
}

void quicksort_seq(unsigned *array, unsigned start, unsigned finish)
{
  if(start >= 0 && finish >= 0 && start < finish)
  {
    unsigned p = partition(array, start, finish);
    quicksort_seq(array, start, p);
    quicksort_seq(array, p+1, finish);
  }
}

extern unsigned BLOCK;

void quicksort_par(unsigned *array, unsigned start, unsigned finish, unsigned *buf1) {
  if (finish - start < BLOCK) {
    quicksort_seq(array, start, finish);
  }
  else {
    unsigned pivot = array[finish - 1];
    std::function<bool(unsigned)> less = [&pivot] (unsigned x) {return x < pivot; };
    std::function<bool(unsigned)> more = [&pivot] (unsigned x) {return x >= pivot; };
    unsigned left_num = filter_seq(array, buf1, start, finish-1, less);
    unsigned right_num = filter_seq(array, buf1 + left_num, start, finish-1, more);
    memcpy(array + start, buf1 + start, (finish - start - 1) * sizeof(unsigned));
    std::swap(array[start + left_num], array[finish-1]);
#pragma omp task
    { quicksort_par(array, start, start + left_num, buf1); }
#pragma omp task
    { quicksort_par(array, start + left_num + 1, finish, buf1); }
  }
}
