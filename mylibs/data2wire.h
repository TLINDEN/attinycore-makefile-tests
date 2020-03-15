/*
  from PCP pcp/libpcp/util.c.
  
  Convert byte arrays from big endian  to numbers and vice versa.  Do
  not  take  care   about  host  endianess.  In   Rob  Pikes'  words:
  https://commandcenter.blogspot.de/2012/04/byte-order-fallacy.html
*/

#ifndef DATA2WIRE_H
#define DATA2WIRE_H

#include <inttypes.h>


inline uint64_t wire_to_data64(uint8_t *data) {
  uint64_t i =
    ((uint64_t)data[7]<<0)  |
    ((uint64_t)data[6]<<8)  |
    ((uint64_t)data[5]<<16) |
    ((uint64_t)data[4]<<24) |
    ((uint64_t)data[3]<<32) |
    ((uint64_t)data[2]<<40) |
    ((uint64_t)data[1]<<48) |
    ((uint64_t)data[0]<<56);
  return i;
}


inline uint32_t wire_to_data32(uint8_t *data) {
  uint32_t i =
    ((uint32_t)data[3]<<0)  |
    ((uint32_t)data[2]<<8)  |
    ((uint32_t)data[1]<<16) |
    ((uint32_t)data[0]<<24);
  return i;
}

inline uint16_t wire_to_data16(uint8_t *data) {
  uint16_t i =
    ((uint16_t)data[1]<<0) |
    ((uint16_t)data[0]<<8);
  return i;
}

inline void data64_to_wire(uint64_t i, uint8_t *data) {
  data[0] = (i >> 56) & 0xFF;
  data[1] = (i >> 48) & 0xFF;
  data[2] = (i >> 40) & 0xFF;
  data[3] = (i >> 32) & 0xFF;
  data[4] = (i >> 24) & 0xFF;
  data[5] = (i >> 16) & 0xFF;
  data[6] = (i >>  8) & 0xFF;
  data[7] =  i        & 0xFF;
}

inline void data32_to_wire(uint32_t i, uint8_t *data) {
  data[0] = (i >> 24) & 0xFF;
  data[1] = (i >> 16) & 0xFF;
  data[2] = (i >>  8) & 0xFF;
  data[3] =  i        & 0xFF;
}

inline void data16_to_wire(uint16_t i, uint8_t *data) {
  data[0] = (i >>  8) & 0xFF;
  data[1] =  i        & 0xFF;
}



#endif
