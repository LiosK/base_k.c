#ifndef BASE62_H
#define BASE62_H

#include "base_k.h"

#define BASE62_RADIX (62)

const char BASE62_DIGITS[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

/** O(1) map from ASCII code points to digit values. */
const uint8_t BASE62_DECODE_MAP[256] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c,
    0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e,
    0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a,
    0x3b, 0x3c, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff};

#ifdef __cplusplus
extern "C" {
#endif

#define BASE62_128_LEN (22)

/** Encodes a byte array to text. */
void base62_128_encode(const uint8_t *bytes, char *text) {
  uint8_t dst[BASE62_128_LEN];
  int err =
      base_k_convert_radix(dst, bytes, BASE62_RADIX, 256, BASE62_128_LEN, 16);
  assert(err == 0);
  for (int i = 0; i < BASE62_128_LEN; i++) {
    text[i] = BASE62_DIGITS[dst[i]];
  }
  text[BASE62_128_LEN] = '\0';
}

/** Decodes text to a byte array. */
int base62_128_decode(const char *text, uint8_t *bytes) {
  uint8_t src[BASE62_128_LEN];
  const uint8_t *uchars = (const uint8_t *)text;
  for (int i = 0; i < BASE62_128_LEN; i++) {
    uint8_t d = BASE62_DECODE_MAP[uchars[i]];
    if (d >= BASE62_RADIX) {
      return -1; // invalid character
    }
    src[i] = d;
  }
  int err =
      base_k_convert_radix(bytes, src, 256, BASE62_RADIX, 16, BASE62_128_LEN);
  assert(err == 0);
  return 0;
}

#ifdef __cplusplus
} /* extern "C" { */
#endif

#endif /* #ifndef BASE62_H */