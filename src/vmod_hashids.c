#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "cache/cache.h"

#include "vtim.h"
#include "vrt.h"
#include "vcc_hashids_if.h"
#include "vmod_hashids.h"

VCL_STRING vmod_encode(VRT_CTX, VCL_STRING salt, VCL_INT number) {
  char *buf;
  hashids_t *hashids;
  size_t buf_size;

  CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);
  WS_Assert(ctx->ws);

  if (salt == NULL) {
    return NULL;
  }
  
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
  WS_Assert_Allocated(ctx->ws, buf, buf_size);

  size_t len = hashids_encode_one(hashids, buf, number);
  hashids_free(hashids);

  if (len == 0) {
    return NULL;
  }

  return buf;
}

VCL_INT vmod_decode(VRT_CTX, VCL_STRING salt, VCL_STRING hash) {
  ull_t number;
  size_t len, n_encode, n_decode;
  hashids_t *hashids;
  char *buf;

  CHECK_OBJ_NOTNULL(ctx, VRT_CTX_MAGIC);
  WS_Assert(ctx->ws);

  AN(salt);

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

  buf = WS_Copy(ctx->ws, hash, len + 1);
  if (buf != NULL) {
    buf[len] = '\0';
  } else {
    return ERROR_BUFFER_ALLOC;
  }

  n_decode = hashids_decode(hashids, buf, &number, 1);
  n_encode = hashids_encode(hashids, buf, 1, &number);

  hashids_free(hashids);

  if (n_decode == 0 || n_encode == 0) {
    return ERROR_ZERO_ELEMENTS_RETURNED;
  }

  if (strncmp(hash, buf, len) != 0) {
    return ERROR_INVALID_HASH;
  }

  return number;
}
