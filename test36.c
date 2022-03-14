#include "base36.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  uint8_t bytes[16];
  char encoded[26];
  uint8_t decoded[16];

  for (int i = 0; i < 10000; i++) {
    arc4random_buf(bytes, 16);
    base36_128_encode(bytes, encoded);
    printf("console.assert(0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x"
           "%02x%02x%02x%02xn.toString(36).padStart(25, \"0\") === \"%s\");\n",
           bytes[0], bytes[1], bytes[2], bytes[3], bytes[4], bytes[5], bytes[6],
           bytes[7], bytes[8], bytes[9], bytes[10], bytes[11], bytes[12],
           bytes[13], bytes[14], bytes[15], encoded);

    int err = base36_128_decode(encoded, decoded);
    assert(err == 0);
    for (int j = 0; j < 16; j++) {
      assert(bytes[j] == decoded[j]);
    }

    for (int j = 0; j < 25; j++) {
      encoded[j] = toupper(encoded[j]);
    }
    err = base36_128_decode(encoded, decoded);
    assert(err == 0);
    for (int j = 0; j < 16; j++) {
      assert(bytes[j] == decoded[j]);
    }
  }

  printf("console.log(\"ok\");\n");

  return 0;
}
