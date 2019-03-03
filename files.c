#include "files.h"

#define SIX_BITS 63
#define BIT_MOVE 6

const unsigned char base64[65] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *bin_to_64(int value, char *conversion) {
  conversion[1] = base64[value & SIX_BITS];
  value >>= BIT_MOVE;
  conversion[0] = base64[value & SIX_BITS];
  return conversion;
}