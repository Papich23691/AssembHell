#include "files.h"

const unsigned char base64_digset[B64_DIG_LEN] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *bin_to_64(int value, char *conversion) {
  conversion[1] = base64_digset[value & SIX_BITS];
  value >>= BIT_MOVE;
  conversion[0] = base64_digset[value & SIX_BITS];
  
  return conversion;
}