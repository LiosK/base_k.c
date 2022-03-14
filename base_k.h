/**
 * base_k.h - Base-k encoder-decoder in C
 *
 * Copyright 2022 LiosK
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy
 * of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
#ifndef BASE_K_H
#define BASE_K_H

#include <assert.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef BASE_K_USE_NAIVE
/** Converts a digit value array in `src_radix` to that in `dst_radix`. */
int base_k_convert_radix(uint8_t *dst, const uint8_t *src, int dst_radix,
                         int src_radix, int dst_size, int src_size) {
  for (int i = 0; i < dst_size; i++) {
    dst[i] = 0;
  }

  for (int i = 0; i < src_size; i++) {
    uint_fast32_t carry = src[i];
    for (int j = dst_size - 1; j >= 0; j--) {
      carry += dst[j] * src_radix;
      dst[j] = carry % dst_radix;
      carry = carry / dst_radix;
    }
    if (carry != 0) {
      return -1; // dst_size too small
    }
  }
  return 0;
}
#else
/** Converts a digit value array in `src_radix` to that in `dst_radix`. */
int base_k_convert_radix(uint8_t *dst, const uint8_t *src, int dst_radix,
                         int src_radix, int dst_size, int src_size) {
  for (int i = 0; i < dst_size; i++) {
    dst[i] = 0;
  }

  int dst_used = dst_size - 1;
  uint64_t carry = 0;
  for (int i = 0; i < src_size;) {
    // Reset carry to input (read multiple digits for optimization)
    uint64_t power = 1; // Set to src_radix ^ number of digits read
    while (power < ((uint64_t)1 << 48) && i < src_size) {
      carry = carry * src_radix + src[i++];
      power *= src_radix;
    }

    // Iterate over dst from right while carry != 0 or up to place already used
    int j = dst_size - 1;
    for (; carry > 0 || j >= dst_used; j--) {
      if (j < 0) {
        return -1; // dst_size too small
      }
      carry += dst[j] * power;
      dst[j] = carry % dst_radix;
      carry = carry / dst_radix;
    }
    dst_used = j + 1;
    assert(carry == 0 && dst_used >= 0);
  }
  return 0;
}
#endif /* #ifdef BASE_K_USE_NAIVE */

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* #ifndef BASE_K_H */
