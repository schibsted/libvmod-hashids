#include "hashids.h"

#define HASH_MAX_SIZE 64

enum HASHIDS_ERROR_CODES {
  ERROR_EMPTY_HASH             = -1,
  ERROR_MAX_SIZE_OVERFLOW      = -2,
  ERROR_BUFFER_ALLOC           = -3,
  ERROR_ZERO_ELEMENTS_RETURNED = -4,
  ERROR_INVALID_HASH           = -5,
  ERROR_HASHID_INIT            = -6
};

typedef unsigned long long ull_t;
