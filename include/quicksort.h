#ifndef CILK_QUICKSORT__QUICKSORT_SEQ_H_
#define CILK_QUICKSORT__QUICKSORT_SEQ_H_
#include <algorithm>
#include <omp.h>
#include <cstring>
#include <functional>
#include "parallel_utils.h"

void quicksort_seq(unsigned *array, unsigned start, unsigned finish);

void quicksort_par(unsigned *array, unsigned start, unsigned finish, unsigned *buf1);
#endif //CILK_QUICKSORT__QUICKSORT_SEQ_H_
