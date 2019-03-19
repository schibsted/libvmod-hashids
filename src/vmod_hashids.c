#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "cache/cache.h"
#include "vtim.h"
#include "vrt.h"
#include "vcc_hashids_if.h"
#include "hashids.h"

/* Max size of hash we will allow to decode. */
#define HASH_MAX_SIZE 64

/* Returned error codes. */
enum HASHIDS_ERROR_CODES {
  ERROR_EMPTY_HASH             = -1,
  ERROR_MAX_SIZE_OVERFLOW      = -2,
  ERROR_ZERO_ELEMENTS_RETURNED = -3,
  ERROR_INVALID_HASH           = -4,
  ERROR_HASHID_INIT            = -5
};

typedef unsigned long long ull_t;

/* Encode function. */
VCL_STRING vmod_encode(VRT_CTX, VCL_STRING salt, VCL_INT number) {
  char *buf;
  hashids_t *hashids;
  size_t buf_size;

  CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);
  WS_Assert(ctx->ws);
  AN(salt);
  
  hashids = hashids_init(salt);
  if (hashids == NULL) {
    return NULL;
  }

  buf_size = hashids_estimate_encoded_size(hashids, 1, (ull_t*)&number);
  if (buf_size == 0 || buf_size > HASH_MAX_SIZE) {
    hashids_free(hashids);
    return NULL;
  }

  buf = WS_Alloc(ctx->ws, buf_size);
  if (buf == NULL) {
    hashids_free(hashids);
    return NULL;
  }

  size_t len = hashids_encode_one(hashids, buf, number);
  hashids_free(hashids);

  if (len == 0) {
    return NULL;
  }

  return buf;
}

/* Decode function. */
VCL_INT vmod_decode(VRT_CTX, VCL_STRING salt, VCL_STRING hash) {
  ull_t number;
  size_t len, n_encode, n_decode;
  hashids_t *hashids;
  char buf[HASH_MAX_SIZE];

  CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);
  AN(salt);
  AN(hash);

  len = strlen(hash);
  if (len == 0) {
    return ERROR_EMPTY_HASH;
  }

  if (len > HASH_MAX_SIZE) {
    return ERROR_MAX_SIZE_OVERFLOW;
  }

  hashids = hashids_init(salt);
  if (hashids == NULL) {
    return ERROR_HASHID_INIT;
  }

  memcpy(buf, hash, len);
  buf[len] = '\0';

  n_decode = hashids_decode(hashids, buf, &number, 1);
  if (n_decode == 0) {
    hashids_free(hashids);
    return ERROR_ZERO_ELEMENTS_RETURNED;
  }

  /* We reencode the decoded number and compare the returned hash with the input hash
     so we can validate the hash and salt combination.
  */
  if (hashids_estimate_encoded_size(hashids, 1, &number) > HASH_MAX_SIZE) {
    hashids_free(hashids);
    return ERROR_MAX_SIZE_OVERFLOW;
  }

  n_encode = hashids_encode(hashids, buf, 1, &number);
  if (n_encode == 0) {
    hashids_free(hashids);
    return ERROR_ZERO_ELEMENTS_RETURNED;
  }

  hashids_free(hashids);

  if (strncmp(hash, buf, len) != 0) {
    return ERROR_INVALID_HASH;
  }

  return number;
}
