#include "base36.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
  uint8_t bytes[16];
  char encoded[26];
  uint8_t decoded[16];

  for (int i = 0; i < 100000; i++) {
    arc4random_buf(bytes, 16);
    base36_128_encode(bytes, encoded);

    int err = base36_128_decode(encoded, decoded);
    if (err != 0) {
      fprintf(stderr, "Assertion failed\n");
      return 1;
    }
  }

  return 0;
}
