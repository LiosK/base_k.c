# Base-k encoder-decoder in C

```c
#include "base_k.h"

#include <assert.h>
#include <stdio.h>

int main() {
  int err;
  const uint8_t bytes[16] = {1,   127, 239, 57, 194, 100, 27,  165,
                             106, 148, 131, 24, 136, 65,  224, 90};

  // convert byte array into base-36 digit values
  uint8_t digit_values[25];
  err = base_k_convert_radix(digit_values, bytes, 36, 256, sizeof(digit_values),
                             sizeof(bytes));
  assert(err == 0);

  // convert digit values into textual representation
  static const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char text_buffer[32];
  for (unsigned int i = 0; i < sizeof(digit_values); i++) {
    text_buffer[i] = digits[digit_values[i]];
  }
  text_buffer[sizeof(digit_values)] = '\0';

  // convert digit values back to byte sequence
  uint8_t decoded[sizeof(bytes)];
  err = base_k_convert_radix(decoded, digit_values, 256, 36, sizeof(decoded),
                             sizeof(digit_values));
  assert(err == 0);

  // print results
  printf("Input bytes:   %02x", bytes[0]);
  for (unsigned int i = 1; i < sizeof(bytes); i++) {
    printf(":%02x", bytes[i]);
  }
  printf("\n");

  printf("Encoded text:  %s\n", text_buffer);

  printf("Digit values:  %d", digit_values[0]);
  for (unsigned int i = 1; i < sizeof(digit_values); i++) {
    printf(", %d", digit_values[i]);
  }
  printf("\n");

  printf("Decoded bytes: %02x", decoded[0]);
  for (unsigned int i = 1; i < sizeof(decoded); i++) {
    printf(":%02x", decoded[i]);
  }
  printf("\n");

  return 0;
}
```
