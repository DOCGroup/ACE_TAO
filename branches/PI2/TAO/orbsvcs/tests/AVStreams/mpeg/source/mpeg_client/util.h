/* $Id$ */
/*
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

/* Status codes for bit stream i/o operations. */

#ifndef AV_UTIL_H
#define AV_UTIL_H

#define NO_VID_STREAM -1
#define UNDERFLOW -2
#define OK 1
/* Size increment of extension data buffers. */

#define EXT_BUF_SIZE 1024

/* External declarations for bitstream i/o operations. */
extern unsigned int bitMask[];
extern unsigned int nBitMask[];
extern unsigned int rBitMask[];
extern unsigned int bitTest[];

/* External declarations of bitstream global variables. */
extern unsigned int curBits;
extern int bitOffset;
extern int bufLength;
extern unsigned int *bitBuffer;

/* Macro for updating bit counter if analysis tool is on. */
#ifdef ANALYSIS
#define UPDATE_COUNT(numbits) bitCount += numbits
#else
#define UPDATE_COUNT(numbits)
#endif

#ifdef NO_SANITY_CHECKS
#define get_bits1(result)                                                 \
{                                                                         \
  UPDATE_COUNT(1);                                                        \
  result = ((curBits & 0x80000000) != 0);                              \
  curBits <<= 1;                                                          \
  bitOffset++;                                                            \
                                                                          \
  if (bitOffset & 0x20) {                                                 \
    bitOffset = 0;                                                        \
    bitBuffer++;                                                          \
    curBits = *bitBuffer;                                                 \
    bufLength--;                                                          \
  }                                                                       \
}

#define get_bits2(result)                                                 \
{                                                                         \
  UPDATE_COUNT(2);                                                        \
  bitOffset += 2;                                                         \
                                                                          \
  if (bitOffset & 0x20) {                                                 \
    bitOffset -= 32;                                                      \
    bitBuffer++;                                                          \
    bufLength--;                                                          \
    if (bitOffset) {                                                      \
      curBits |= (*bitBuffer >> (2 - bitOffset));                         \
    }                                                                     \
    result = ((curBits & 0xc0000000) >> 30);                           \
    curBits = *bitBuffer << bitOffset;                                    \
  }                                                                       \
                                                                          \
  result = ((curBits & 0xc0000000) >> 30);                             \
  curBits <<= 2;                                                          \
}

#define get_bitsX(num, mask, shift,  result)                              \
{                                                                         \
  UPDATE_COUNT(num);                                                      \
  bitOffset += num;                                                       \
                                                                          \
  if (bitOffset & 0x20) {                                                 \
    bitOffset -= 32;                                                      \
    bitBuffer++;                                                          \
    bufLength--;                                                          \
    if (bitOffset) {                                                      \
      curBits |= (*bitBuffer >> (num - bitOffset));                       \
    }                                                                     \
    result = ((curBits & mask) >> shift);                              \
    curBits = *bitBuffer << bitOffset;                                    \
  }                                                                       \
  else {                                                                  \
    result = ((curBits & mask) >> shift);                              \
    curBits <<= num;                                                      \
  }                                                                       \
}
#else

#define get_bits1(result)                                                 \
{                                                                         \
  /* Check for underflow. */                                              \
  /*                                                                      \
  if (bufLength < 2) {                                                    \
    correct_underflow();                                                  \
  } */                                                                    \
  UPDATE_COUNT(1);                                                        \
  result = ((curBits & 0x80000000) != 0);                              \
  curBits <<= 1;                                                          \
  bitOffset++;                                                            \
                                                                          \
  if (bitOffset & 0x20) {                                                 \
    bitOffset = 0;                                                        \
    bitBuffer++;                                                          \
    curBits = *bitBuffer;                                                 \
    bufLength--;                                                          \
  }                                                                       \
}

#define get_bits2(result)                                                 \
{                                                                         \
  /* Check for underflow. */                                              \
  /*                                                                      \
  if (bufLength < 2) {                                                    \
    correct_underflow();                                                  \
  } */                                                                    \
  UPDATE_COUNT(2);                                                        \
  bitOffset += 2;                                                         \
                                                                          \
  if (bitOffset & 0x20) {                                                 \
    bitOffset -= 32;                                                      \
    bitBuffer++;                                                          \
    bufLength--;                                                          \
    if (bitOffset) {                                                      \
      curBits |= (*bitBuffer >> (2 - bitOffset));                         \
    }                                                                     \
    result = ((curBits & 0xc0000000) >> 30);                           \
    curBits = *bitBuffer << bitOffset;                                    \
  }                                                                       \
                                                                          \
  result = ((curBits & 0xc0000000) >> 30);                             \
  curBits <<= 2;                                                          \
}

#define get_bitsX(num, mask, shift,  result)                              \
{                                                                         \
  /* Check for underflow. */                                              \
  /*                                                                      \
  if (bufLength < 2) {                                                    \
    correct_underflow();                                                  \
  } */                                                                    \
  UPDATE_COUNT(num);                                                      \
  bitOffset += num;                                                       \
                                                                          \
  if (bitOffset & 0x20) {                                                 \
    bitOffset -= 32;                                                      \
    bitBuffer++;                                                          \
    bufLength--;                                                          \
    if (bitOffset) {                                                      \
      curBits |= (*bitBuffer >> (num - bitOffset));                       \
    }                                                                     \
    result = ((curBits & mask) >> shift);                              \
    curBits = *bitBuffer << bitOffset;                                    \
  }                                                                       \
  else {                                                                  \
   result = ((curBits & mask) >> shift);                               \
   curBits <<= num;                                                       \
  }                                                                       \
}
#endif

#define get_bits3(result) get_bitsX(3,   0xe0000000, 29, result)
#define get_bits4(result) get_bitsX(4,   0xf0000000, 28, result)
#define get_bits5(result) get_bitsX(5,   0xf8000000, 27, result)
#define get_bits6(result) get_bitsX(6,   0xfc000000, 26, result)
#define get_bits7(result) get_bitsX(7,   0xfe000000, 25, result)
#define get_bits8(result) get_bitsX(8,   0xff000000, 24, result)
#define get_bits9(result) get_bitsX(9,   0xff800000, 23, result)
#define get_bits10(result) get_bitsX(10, 0xffc00000, 22, result)
#define get_bits11(result) get_bitsX(11, 0xffe00000, 21, result)
#define get_bits12(result) get_bitsX(12, 0xfff00000, 20, result)
#define get_bits14(result) get_bitsX(14, 0xfffc0000, 18, result)
#define get_bits16(result) get_bitsX(16, 0xffff0000, 16, result)
#define get_bits18(result) get_bitsX(18, 0xffffc000, 14, result)
#define get_bits32(result) get_bitsX(32, 0xffffffff,  0, result)

#define get_bitsn(num, result) get_bitsX((num), nBitMask[num], (32-(num)), result)

#ifdef NO_SANITY_CHECKS
#define show_bits32(result)                                             \
{                                                                       \
  if (bitOffset) {                                                      \
    result = curBits | (*(bitBuffer+1) >> (32 - bitOffset));            \
  }                                                                     \
  else {                                                                \
    result = curBits;                                                   \
  }                                                                     \
}

#define show_bitsX(num, mask, shift,  result)                           \
{                                                                       \
  int bO;                                                               \
  bO = bitOffset + num;                                                 \
  if (bO > 32) {                                                        \
    bO -= 32;                                                           \
    result = ((curBits & mask) >> shift) |                              \
                (*(bitBuffer+1) >> (shift + (num - bO)));               \
  }                                                                     \
  else {                                                                \
    result = ((curBits & mask) >> shift);                               \
  }                                                                     \
}

#else
#define show_bits32(result)                                             \
{                                                                       \
  /* Check for underflow. */                                            \
  /* if (bufLength < 2) {                                               \
    correct_underflow();                                                \
  }  */                                                                 \
  if (bitOffset) {                                                      \
    result = curBits | (*(bitBuffer+1) >> (32 - bitOffset));            \
  }                                                                     \
  else {                                                                \
    result = curBits;                                                   \
  }                                                                     \
}

#define show_bitsX(num, mask, shift, result)                            \
{                                                                       \
  int bO;                                                               \
                                                                        \
  /* Check for underflow. */                                            \
  /* if (bufLength < 2) {                                               \
    correct_underflow();                                                \
  }  */                                                                 \
  bO = bitOffset + num;                                                 \
  if (bO > 32) {                                                        \
    bO -= 32;                                                           \
    result = ((curBits & mask) >> shift) |                              \
                (*(bitBuffer+1) >> (shift + (num - bO)));               \
  }                                                                     \
  else {                                                                \
    result = ((curBits & mask) >> shift);                               \
  }                                                                     \
}
#endif

#define show_bits1(result)  show_bitsX(1,  0x80000000, 31, result)
#define show_bits2(result)  show_bitsX(2,  0xc0000000, 30, result)
#define show_bits3(result)  show_bitsX(3,  0xe0000000, 29, result)
#define show_bits4(result)  show_bitsX(4,  0xf0000000, 28, result)
#define show_bits5(result)  show_bitsX(5,  0xf8000000, 27, result)
#define show_bits6(result)  show_bitsX(6,  0xfc000000, 26, result)
#define show_bits7(result)  show_bitsX(7,  0xfe000000, 25, result)
#define show_bits8(result)  show_bitsX(8,  0xff000000, 24, result)
#define show_bits9(result)  show_bitsX(9,  0xff800000, 23, result)
#define show_bits10(result) show_bitsX(10, 0xffc00000, 22, result)
#define show_bits11(result) show_bitsX(11, 0xffe00000, 21, result)
#define show_bits12(result) show_bitsX(12, 0xfff00000, 20, result)
#define show_bits13(result) show_bitsX(13, 0xfff80000, 19, result)
#define show_bits14(result) show_bitsX(14, 0xfffc0000, 18, result)
#define show_bits15(result) show_bitsX(15, 0xfffe0000, 17, result)
#define show_bits16(result) show_bitsX(16, 0xffff0000, 16, result)
#define show_bits17(result) show_bitsX(17, 0xffff8000, 15, result)
#define show_bits18(result) show_bitsX(18, 0xffffc000, 14, result)
#define show_bits19(result) show_bitsX(19, 0xffffe000, 13, result)
#define show_bits20(result) show_bitsX(20, 0xfffff000, 12, result)
#define show_bits21(result) show_bitsX(21, 0xfffff800, 11, result)
#define show_bits22(result) show_bitsX(22, 0xfffffc00, 10, result)
#define show_bits23(result) show_bitsX(23, 0xfffffe00,  9, result)
#define show_bits24(result) show_bitsX(24, 0xffffff00,  8, result)
#define show_bits25(result) show_bitsX(25, 0xffffff80,  7, result)
#define show_bits26(result) show_bitsX(26, 0xffffffc0,  6, result)
#define show_bits27(result) show_bitsX(27, 0xffffffe0,  5, result)
#define show_bits28(result) show_bitsX(28, 0xfffffff0,  4, result)
#define show_bits29(result) show_bitsX(29, 0xfffffff8,  3, result)
#define show_bits30(result) show_bitsX(30, 0xfffffffc,  2, result)
#define show_bits31(result) show_bitsX(31, 0xfffffffe,  1, result)

#define show_bitsn(num,result) show_bitsX((num), (0xffffffff << (32-(num))), (32-(num)), result)

#ifdef NO_SANITY_CHECKS
#define flush_bits32                                                  \
{                                                                     \
  UPDATE_COUNT(32);                                                   \
                                                                      \
  bitBuffer++;                                                        \
  bufLength--;                                                        \
  curBits = *bitBuffer  << bitOffset;                                 \
}

#define flush_bits(num)                                               \
{                                                                     \
  bitOffset += num;                                                   \
                                                                      \
  UPDATE_COUNT(num);                                                  \
                                                                      \
  if (bitOffset & 0x20) {                                             \
    bitOffset -= 32;                                                  \
    bitBuffer++;                                                      \
    bufLength--;                                                      \
    curBits = *bitBuffer  << bitOffset;                               \
  }                                                                   \
  else {                                                              \
    curBits <<= num;                                                  \
  }                                                                   \
}
#else
#define flush_bits32                                                  \
{                                                                     \
  if (curVidStream == NULL) {                                         \
    /* Deal with no vid stream here. */                               \
  }                                                                   \
  /*                                                                  \
  if (bufLength < 2) {                                                \
    correct_underflow();                                              \
  }                                                                   \
  */                                                                  \
  UPDATE_COUNT(32);                                                   \
                                                                      \
  bitBuffer++;                                                        \
  bufLength--;                                                        \
  curBits = *bitBuffer  << bitOffset;                                 \
}

#define flush_bits(num)                                               \
{                                                                     \
  if (curVidStream == NULL) {                                         \
    /* Deal with no vid stream here. */                               \
  }                                                                   \
  /*                                                                  \
  if (bufLength < 2) {                                                \
    correct_underflow();                                              \
  }                                                                   \
  */                                                                  \
  UPDATE_COUNT(num);                                                  \
                                                                      \
  bitOffset += num;                                                   \
                                                                      \
  if (bitOffset & 0x20) {                                             \
    bufLength--;                                                      \
    bitOffset -= 32;                                                  \
    bitBuffer++;                                                      \
    curBits = *bitBuffer << bitOffset;                                \
  }                                                                   \
  else {                                                              \
    curBits <<= num;                                                  \
  }                                                                   \
}
#endif

#define UTIL2

#endif /* if !defined (AV_UTIL_H) */

