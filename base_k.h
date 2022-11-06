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

/** Converts a digit value array in `src_radix` to that in `dst_radix`. */
int base_k_convert_radix(uint8_t *dst, const uint8_t *src, int dst_radix,
                         int src_radix, int dst_size, int src_size) {
  assert(2 <= dst_radix && dst_radix <= 256 && dst_size >= 0);
  assert(2 <= src_radix && src_radix <= 256 && src_size >= 0);

  for (int i = 0; i < dst_size; i++) {
    dst[i] = 0;
  }

#ifdef BASE_K_USE_NAIVE
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
#else
  // Determine optimum number of digits to read per loop
  int word_size = 0;
  uint64_t word_radix = 1;
  while (word_radix < UINT64_MAX / ((uint64_t)src_radix * dst_radix)) {
    word_size++;
    word_radix *= src_radix;
  }

  int dst_used = dst_size - 1; // storage to memorize dst range already filled

  // Align initial src cursor so it points to heads of words in loop
  int i = src_size % word_size;
  if (i > 0) {
    i -= word_size;
  }
  for (; i < src_size; i += word_size) {
    // Read word_size digits from src to carry
    uint64_t carry = 0;
    for (int j = i < 0 ? 0 : i; j < i + word_size; j++) {
      carry = carry * src_radix + src[j];
    }

    // Iterate over dst from right while carry != 0 or up to place already used
    int j = dst_size - 1;
    for (; carry > 0 || j >= dst_used; j--) {
      if (j < 0) {
        return -1; // dst_size too small
      }
      carry += dst[j] * word_radix;
      dst[j] = carry % dst_radix;
      carry = carry / dst_radix;
    }
    dst_used = j + 1;
    assert(carry == 0 && dst_used >= 0);
  }
#endif /* #ifdef BASE_K_USE_NAIVE */

  return 0; // success
}

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* #ifndef BASE_K_H */
