/* $Id$ */
/* Copyright (c) 1995 Oregon Graduate Institute of Science and Technology
 * P.O.Box 91000-1000, Portland, OR 97291, USA;
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of O.G.I. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  O.G.I. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * O.G.I. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * O.G.I. BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Shanwei Cen
 *         Department of Computer Science and Engineering
 *         email: scen@cse.ogi.edu
 */
/* data structure used by StatFile() */

#ifndef _MPEG_FILE_IO_H
#define _MPEG_FILE_IO_H

struct StatBuf
{
  unsigned short mode;
  long size;
  char uid[32];
  char gid[32];
  char ctime[32];
};

/* protocol of functions defined in shared.c */

int StatFile(char *filename, struct StatBuf * sb);



/* Following read...()'s would exit(1) if requested bytes can not be
   read, and EOF or error other than EINTR, EAGAIN or EWOULDBLOCK is
   encountered */

void read_bytes(int s, char * buf, int size);

void read_byte(int sock, char * data);

void read_short(int sock, short * data);

void read_int(int sock, int * data);

int read_string(int sock, char *data, int maxlen);  /* returns the length of a stream */


/* Following write...()'s would exit(1) if requested bytes can not
   be written to socket because of error has happened*/

void write_bytes(int sock, char * data, int len);

void write_byte(int sock, char data);

void write_short(int sock, short data);

void write_int(int sock, int data);

void write_string(int sock, char *data);


/* following returns: 0 -- succ, -1 -- failed.
   time_read*() will wait at most 5 seconds before they return */

int time_read_bytes(int s, char * buf, int size);

int time_read_int(int sock, int * data);

int time_write_bytes(int sock, char * data, int len);

int time_write_int(int sock, int data);


/* Following read...()'s would return 0/-1 if requested bytes can not
   be read, and EOF (return 0) or error (return -1) other than EINTR,
   EAGAIN or EWOULDBLOCK is encountered */

int wait_read_bytes(int s, char *buf, int size);


/* Following write...()'s would return 0/-1 if requested bytes can not
   be written and error (return -1) other than EINTR, EAGAIN or is
   encountered */

int wait_write_bytes(int s, char *buf, int size);

#endif /* _MPEG_FILE_IO_H */
