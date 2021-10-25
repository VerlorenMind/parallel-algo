#ifndef CILK_QUICKSORT__PARALLEL_UTILS_H_
#define CILK_QUICKSORT__PARALLEL_UTILS_H_

#include <functional>

template<typename I, typename O>
void map(I *in, O *out, unsigned start, unsigned finish, std::function<O(I)> &f) {
#pragma omp parallel for
  for(unsigned i=start; i < finish; ++i) {
    out[i] = f(in[i]);
  }
}

template<typename T>
void upward(T *array, unsigned start, unsigned finish) {
  if (start - finish == 0) {
    return;
  }
  else if (finish - start == 1) {
    array[finish] += array[start];
  }
  else {
    unsigned m = (finish + start) / 2;
#pragma omp task
    { upward(array, start, m); }
#pragma omp task
    { upward(array, m+1, finish); }
#pragma omp taskwait
    array[finish] += array[m];
  }
}

template<typename T>
void downward(T *array, unsigned start, unsigned finish) {
  if (start - finish == 0) {
    return;
  }
  else {
    unsigned m = (finish + start) / 2;
    array[finish] += array[m];
    array[m] = array[finish] - array[m];
#pragma omp task
    { downward(array, start, m); }
#pragma omp task
    { downward(array, m+1, finish); }
#pragma omp taskwait
  }
}

template<typename T>
void scan(T *array, unsigned start, unsigned finish) {
  upward(array, start, finish-1);
  T temp = array[finish-1];
  array[finish - 1] = 0;
  downward(array, start, finish-1);
  memmove(array+start, array+start+1, (finish - start - 1) * sizeof(T));
  array[finish-1] = temp;
}

template<typename T>
unsigned filter_seq(T *in, T *out, unsigned start, unsigned finish, std::function<bool(T)> &pred) {
  unsigned ind = 0;
  for(unsigned i=start; i < finish; ++i) {
    if (pred(in[i])) {
      out[start + ind] = in[i];
      ++ind;
    }
  }
  return ind;
}

template<typename T>
unsigned filter_par(T *in, T *out, bool* buf1, T* buf2, unsigned start, unsigned finish, std::function<bool(T)> &pred) {
  map(in, buf1, start, finish, pred);
#pragma omp parallel for
  for (unsigned i = start; i < finish; ++i) {
    buf2[i] = buf1[i] ? 1 : 0;
  }
  scan(buf2, start, finish);
#pragma omp parallel for
  for (unsigned i = start; i < finish; ++i) {
    if (buf1[i] && buf2[i] > 0) {
      out[start + buf2[i] - 1] = in[i];
    }
  }
  return buf2[finish - 1];
}

#endif //CILK_QUICKSORT__PARALLEL_UTILS_H_

