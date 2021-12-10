#include "../include/quicksort.h"
#include <iostream>
#include <omp.h>
#include <chrono>
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

unsigned BLOCK;
bool verify(unsigned *array, unsigned n) {
  for(unsigned i = 0; i < n-1; ++i) {
    if(array[i] > array[i+1]) {
      return false;
    }
  }
  return true;
}

void usage() {
  std::cout << "Usage: " << std::endl;
  std::cout << "quicksort size granularity" << std::endl;
  std::cout << "size - size of the array to sort" << std::endl;
  std::cout << "granularity - requlates the size of a block for sequential sorting (block = size / granularity)" << std::endl;
}
int main(int argc, const char **argv) {
  // Init
  if(argc != 3) {
    usage();
    return 1;
  }
  unsigned n = std::atoi(argv[1]);
  BLOCK = std::atoi(argv[2]);
  BLOCK = n / BLOCK;
  std::srand(std::time(nullptr));
  unsigned *array1 = new unsigned[n];
  unsigned *array2 = new unsigned[n];
  for(unsigned i=0; i < n; ++i)
  {
    array1[i] = std::rand();
    array2[i] = std::rand();
  }
  // Seq launch
  auto start = std::chrono::steady_clock::now();
  quicksort_seq(array1,0, n);
  auto stop = std::chrono::steady_clock::now();
  auto seq_dur = stop - start;
  unsigned seq_nano = std::chrono::duration_cast<std::chrono::milliseconds>(seq_dur).count();
  std::cout << "Seq array is sorted: " << verify(array1, n) << std::endl;
  std::cout << "Seq time: " << seq_nano << "ms" << std::endl;
  // Par launch
  // Buffer is needed to avoid memory allocation during sorting
  unsigned *buf1 = new unsigned[n];
  start = std::chrono::steady_clock::now();
#pragma omp parallel
  {
#pragma omp single
    {
      quicksort_par(array2, 0, n, buf1);
    }
  }
  stop = std::chrono::steady_clock::now();
  // Calculating time
  auto par_dur = stop - start;
  unsigned par_nano = std::chrono::duration_cast<std::chrono::milliseconds>(par_dur).count();
  std::cout << "Par array is sorted: " << verify(array2, n) << std::endl;
  std::cout << "Par time: " << par_nano << "ms" << std::endl;
  std::cout << "Speed: " << double(seq_nano) / par_nano << "x" << std::endl;
  delete[] array1;
  delete[] array2;
  delete[] buf1;
  return 0;
}
