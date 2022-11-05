#include "base_k.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char BASE36_DIGITS[] = "0123456789abcdefghijklmnopqrstuvwxyz";

// Tests round-trip conversion
int main() {
  uint8_t src[0x100];
  uint8_t encoded[0x800];
  uint8_t decoded[0x100];
  uint8_t src_hex[0x200];

  for (int i = 0; i < 20000; i++) {
    int base = arc4random_uniform(255) + 2;
    int src_len = arc4random_uniform(0x100);
    int dst_len = ceil((src_len * 8.0) / log2(base));

    arc4random_buf(src, src_len);
    int err = base_k_convert_radix(encoded, src, base, 256, dst_len, src_len);
    assert(err == 0);
    assert(base != 256 || memcmp(src, encoded, src_len) == 0);

    err = base_k_convert_radix(decoded, encoded, 256, base, src_len, dst_len);
    assert(err == 0);

    assert(memcmp(src, decoded, src_len) == 0);

    // Where possible, tests encoded string against JavaScript implementation
    if (base <= 36 && src_len > 0) {
      for (int j = 0; j < dst_len; j++) {
        encoded[j] = BASE36_DIGITS[encoded[j]];
      }
      encoded[dst_len] = '\0';

      for (int j = 0; j < src_len; j++) {
        src_hex[j * 2] = BASE36_DIGITS[src[j] >> 4];
        src_hex[j * 2 + 1] = BASE36_DIGITS[src[j] & 0xf];
      }
      src_hex[src_len * 2] = '\0';

      printf("console.assert(0x%sn.toString(%d).padStart(%d, \"0\") === "
             "\"%s\");\n",
             src_hex, base, dst_len, encoded);
    }
  }

  printf("console.log(\"ok\");\n");

  return 0;
}
