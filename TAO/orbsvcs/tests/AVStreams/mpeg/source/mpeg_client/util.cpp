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

#include <stdlib.h>
#include "video.h"
#include "proto.h"
#include "util.h"
#include "ace/OS.h"

ACE_RCSID(mpeg_client, util, "$Id$")

#define TRUE 1
#define FALSE 0

/* Declarations of global variables used. */

unsigned int curBits;
int bitOffset;
int bufLength;
unsigned int *bitBuffer;

/* Bit masks used by bit i/o operations. */

unsigned int nBitMask[] = { 0x00000000, 0x80000000, 0xc0000000, 0xe0000000, 
			    0xf0000000, 0xf8000000, 0xfc000000, 0xfe000000, 
			    0xff000000, 0xff800000, 0xffc00000, 0xffe00000, 
			    0xfff00000, 0xfff80000, 0xfffc0000, 0xfffe0000, 
			    0xffff0000, 0xffff8000, 0xffffc000, 0xffffe000, 
			    0xfffff000, 0xfffff800, 0xfffffc00, 0xfffffe00, 
			    0xffffff00, 0xffffff80, 0xffffffc0, 0xffffffe0, 
			    0xfffffff0, 0xfffffff8, 0xfffffffc, 0xfffffffe};

unsigned int bitMask[] = {  0xffffffff, 0x7fffffff, 0x3fffffff, 0x1fffffff, 
			    0x0fffffff, 0x07ffffff, 0x03ffffff, 0x01ffffff,
			    0x00ffffff, 0x007fffff, 0x003fffff, 0x001fffff,
			    0x000fffff, 0x0007ffff, 0x0003ffff, 0x0001ffff,
			    0x0000ffff, 0x00007fff, 0x00003fff, 0x00001fff,
			    0x00000fff, 0x000007ff, 0x000003ff, 0x000001ff,
			    0x000000ff, 0x0000007f, 0x0000003f, 0x0000001f,
			    0x0000000f, 0x00000007, 0x00000003, 0x00000001};

unsigned int rBitMask[] = { 0xffffffff, 0xfffffffe, 0xfffffffc, 0xfffffff8, 
			    0xfffffff0, 0xffffffe0, 0xffffffc0, 0xffffff80, 
			    0xffffff00, 0xfffffe00, 0xfffffc00, 0xfffff800, 
			    0xfffff000, 0xffffe000, 0xffffc000, 0xffff8000, 
			    0xffff0000, 0xfffe0000, 0xfffc0000, 0xfff80000, 
			    0xfff00000, 0xffe00000, 0xffc00000, 0xff800000, 
			    0xff000000, 0xfe000000, 0xfc000000, 0xf8000000, 
			    0xf0000000, 0xe0000000, 0xc0000000, 0x80000000};

unsigned int bitTest[] = {  0x80000000, 0x40000000, 0x20000000, 0x10000000, 
			    0x08000000, 0x04000000, 0x02000000, 0x01000000,
			    0x00800000, 0x00400000, 0x00200000, 0x00100000,
			    0x00080000, 0x00040000, 0x00020000, 0x00010000,
			    0x00008000, 0x00004000, 0x00002000, 0x00001000,
			    0x00000800, 0x00000400, 0x00000200, 0x00000100,
			    0x00000080, 0x00000040, 0x00000020, 0x00000010,
			    0x00000008, 0x00000004, 0x00000002, 0x00000001};


/*
 *--------------------------------------------------------------
 *
 * correct_underflow --
 *
 *	Called when buffer does not have sufficient data to 
 *      satisfy request for bits.
 *      Calls get_more_data, an application specific routine
 *      required to fill the buffer with more data.
 *
 * Results:
 *      None really.
 *  
 * Side effects:
 *	buf_length and buffer fields in curVidStream structure
 *      may be changed.
 *
 *--------------------------------------------------------------
 */

void 
correct_underflow() {

  int status;

  status = get_more_data(curVidStream->buf_start,
			 curVidStream->max_buf_length,
			 &bufLength, &bitBuffer);

  if (status  < 0) {
    if (!quietFlag) {
      fprintf (stderr, "\n");
      perror("Unexpected read error.");
    }
    exit(1);
  }
  else if ((status == 0) && (bufLength < 1)) {
    if (!quietFlag) {
      fprintf(stderr, "\nImproper or missing sequence end code.\n");
    }
#ifdef ANALYSIS
    PrintAllStats();
#endif
    if (!quietFlag) {
      PrintTimeInfo();
    }
/*
    if (loopFlag) longjmp(env, 1);
    DestroyVidStream(curVidStream);
*/
    exit(0);
  }
#ifdef UTIL2
  curBits = *bitBuffer << bitOffset;
#else
  curBits = *bitBuffer;
#endif

}


/*
 *--------------------------------------------------------------
 *
 * next_bits --
 *
 *	Compares next num bits to low order position in mask.
 *      Buffer pointer is NOT advanced.
 *
 * Results:
 *	TRUE, FALSE, or error code.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

int next_bits(int num, unsigned int mask)
{
  unsigned int stream;
  int ret_value;

  /* If no current stream, return error. */

  if (curVidStream == NULL)
    return NO_VID_STREAM;

  /* Get next num bits, no buffer pointer advance. */

  show_bitsn(num, stream);

  /* Compare bit stream and mask. Set return value toTRUE if equal, FALSE if
     differs. 
  */

  if (mask == stream) {
    ret_value = TRUE;
  } else ret_value = FALSE;

  /* Return return value. */

  return ret_value;
}


/*
 *--------------------------------------------------------------
 *
 * get_ext_data --
 *
 *	Assumes that bit stream is at begining of extension
 *      data. Parses off extension data into dynamically 
 *      allocated space until start code is hit. 
 *
 * Results:
 *	Pointer to dynamically allocated memory containing
 *      extension data.
 *
 * Side effects:
 *	Bit stream irreversibly parsed.
 *
 *--------------------------------------------------------------
 */

char *get_ext_data ()
{
  int size, marker;
  char *dataPtr;
  unsigned int data;

  /* Set initial ext data buffer size. */

  size = EXT_BUF_SIZE;

  /* Allocate ext data buffer. */

  dataPtr = (char *) malloc(size);

  /* Initialize marker to keep place in ext data buffer. */

  marker = 0;

  /* While next data is not start code... */
  while (!next_bits(24, 0x000001)) {

    /* Get next byte of ext data. */

    get_bits8(data);

    /* Put ext data into ext data buffer. Advance marker. */

    dataPtr[marker] = (char) data;
    marker++;

    /* If end of ext data buffer reached, resize data buffer. */

    if (marker == size) {
      size += EXT_BUF_SIZE;
      dataPtr = (char *) realloc(dataPtr, size);
    }
  }

  /* Realloc data buffer to free any extra space. */

  dataPtr = (char *) realloc(dataPtr, marker);

  /* Return pointer to ext data buffer. */

  return dataPtr;
}


/*
 *--------------------------------------------------------------
 *
 * next_start_code --
 *
 *	Parses off bitstream until start code reached. When done
 *      next 4 bytes of bitstream will be start code. Bit offset
 *      reset to 0.
 *
 * Results:
 *	Status code.
 *
 * Side effects:
 *	Bit stream irreversibly parsed.
 *
 *--------------------------------------------------------------
 */

int next_start_code()
{
  int state;
  int byteoff;
  unsigned int data;

  /* If no current stream, return error. */

  if (curVidStream == NULL)
    return NO_VID_STREAM;

  /* If insufficient buffer length, correct underflow. */
/*
  if (bufLength < 2) {
    correct_underflow();
  }
  */

  /* If bit offset not zero, reset and advance buffer pointer. */

  byteoff = bitOffset % 8;

  if (byteoff != 0) {
    flush_bits((8-byteoff));
  }

  /* Set state = 0. */

  state = 0;

  /* While buffer has data ... */

  while(bufLength > 0) {

    /* If insufficient data exists, correct underflow. */
/*
    if (bufLength < 2) {
      correct_underflow();
    }
*/
    /* If next byte is zero... */

    get_bits8(data);

    if (data == 0) {

      /* If state < 2, advance state. */

      if (state < 2) state++;
    }

    /* If next byte is one... */

    else if (data == 1) {

      /* If state == 2, advance state (i.e. start code found). */

      if (state == 2) state++;

      /* Otherwise, reset state to zero. */

      else state = 0;
    }

    /* Otherwise byte is neither 1 or 0, reset state to 0. */

    else {
      state = 0;
    }

    /* If state == 3 (i.e. start code found)... */

    if (state == 3) {

      /* Set buffer pointer back and reset length & bit offsets so
	 next bytes will be beginning of start code. 
      */

      bitOffset = bitOffset - 24;

#ifdef ANALYSIS
      bitCount -= 24;
#endif

      if (bitOffset < 0) {
	bitOffset = 32 + bitOffset;
	bufLength++;
	bitBuffer--;
#ifdef UTIL2
	curBits = *bitBuffer << bitOffset;
#else
	curBits = *bitBuffer;
#endif
      }
      else {
#ifdef UTIL2
	curBits = *bitBuffer << bitOffset;
#else
	curBits = *bitBuffer;
#endif
      }

      /* Return success. */

      return OK;
    }
  }

  /* Return underflow error. */

  return UNDERFLOW;
}


/*
 *--------------------------------------------------------------
 *
 * get_extra_bit_info --
 *
 *	Parses off extra bit info stream into dynamically 
 *      allocated memory. Extra bit info is indicated by
 *      a flag bit set to 1, followed by 8 bits of data.
 *      This continues until the flag bit is zero. Assumes
 *      that bit stream set to first flag bit in extra
 *      bit info stream.
 *
 * Results:
 *	Pointer to dynamically allocated memory with extra
 *      bit info in it. Flag bits are NOT included.
 *
 * Side effects:
 *	Bit stream irreversibly parsed.
 *
 *--------------------------------------------------------------
 */

char *get_extra_bit_info ()
{
  int size, marker;
  char *dataPtr;
  unsigned int data;

  /* Get first flag bit. */
  get_bits1(data);

  /* If flag is false, return NULL pointer (i.e. no extra bit info). */

  if (!data) return NULL;

  /* Initialize size of extra bit info buffer and allocate. */

  size = EXT_BUF_SIZE;
  dataPtr = (char *) malloc(size);

  /* Reset marker to hold place in buffer. */

  marker = 0;

  /* While flag bit is true. */

  while (data) {

    /* Get next 8 bits of data. */
    get_bits8(data);

    /* Place in extra bit info buffer. */

    dataPtr[marker] = (char) data;
    marker++;

    /* If buffer is full, reallocate. */

    if (marker == size) {
      size += EXT_BUF_SIZE;
      dataPtr = (char *) realloc(dataPtr, size);
    }

    /* Get next flag bit. */
    get_bits1(data);
  }

  /* Reallocate buffer to free extra space. */

  dataPtr = (char *) realloc(dataPtr, marker);

  /* Return pointer to extra bit info buffer. */

  return dataPtr;
}








