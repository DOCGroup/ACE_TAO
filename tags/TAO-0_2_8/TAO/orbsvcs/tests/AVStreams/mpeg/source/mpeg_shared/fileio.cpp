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
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include "routine.h"
#include "fileio.h"

ACE_RCSID(mpeg_shared, fileio, "$Id$")

/*
   Statfile(filename, sb)
     input: filename - path name of a directory entry;
     output: sb - StatBuf structure of the given directory entry;
     returns: 0 - successful, -1 - failed, with errno indicating error
     */
int StatFile(char *filename, struct StatBuf * sb)
{
  struct stat statbuf;
  struct passwd *pw;
  struct group *grp;

  if (stat(filename, &statbuf) == -1)
    return -1;

  sb->mode = statbuf.st_mode;
  sb->size = statbuf.st_size;
  
  strcpy(sb->ctime, ctime(&(statbuf.st_ctime)));
  sb->ctime[strlen(sb->ctime)-1] = 0;  /* remove CR */

  if ((pw = getpwuid(statbuf.st_uid)) == NULL)
    sprintf(sb->uid, "%-10d", statbuf.st_uid);
  else
    strncpy(sb->uid, pw->pw_name, 32);

  if ((grp = getgrgid(statbuf.st_gid)) == NULL)
    sprintf(sb->gid, "%-10d", statbuf.st_gid);
  else
    strncpy(sb->gid, grp->gr_name, 32);
}

/*
   read_bytes(s, buf, size)
     input: s - socket to read from
            buf - address of a buffer for the read
	    size - number of bytes to read
     output: non
     action: try read given number of bytes, all errors are reported, and
             cause program to exit.
*/
void read_bytes(int s, char * buf, int size)
{
  int val, remain = size;
  char * ptr = buf;

  if (size == 0) return;
  
  for (;;)
  {
    val = read(s, ptr, remain);
    
    if (val == -1 && (errno == EINTR || errno == EAGAIN | errno == EWOULDBLOCK))
    {   /* interrupted or need to wait, try again */
      if (errno == EAGAIN | errno == EWOULDBLOCK) usleep(10000);
      errno = 0;
      continue;
    }
    if (val == -1)
    {
      perror("Error -- Read from socket");
      exit(1);
    }
    if (val == 0) /* EOF encountered */
    {
      fprintf(stderr, "Error -- EOF reached while trying to read %d bytes.\n", size);
      exit(1);
    }
    ptr += val;
    remain -= val;
    if (remain < 0)  /* weird thing is happening */
    {
      fprintf(stderr, "Error: read too much from socket, %d out of %d bytes.\n",
	      size-remain, size);
      exit(1);
    }
    if (remain == 0)
      break;
  }
}

int wait_read_bytes(int s, char *buf, int size)
{
  int val, remain = size;
  char * ptr = buf;

  if (size == 0) return 1;
  
  for (;;)
  {
    val = read(s, ptr, remain);
    
    if (val == -1 && (errno == EINTR || errno == EAGAIN | errno == EWOULDBLOCK))
    {   /* interrupted or need to wait, try again */
      if (errno == EAGAIN | errno == EWOULDBLOCK) usleep(10000);
      errno = 0;
      continue;
    }
    if (val == -1)
    {
      /*
      perror("Error -- wait_read from socket");
      */
      return -1;
    }
    if (val == 0) /* EOF encountered */
    {
      /*
      fprintf(stderr, "Warn pid %d -- EOF on wait_read %d bytes.\n",
	      getpid(), size);
      */
      return 0;
    }
    ptr += val;
    remain -= val;
    if (remain < 0)  /* weird thing is happening */
    {
      fprintf(stderr,
	      "Error: weird, read too much from socket, %d out of %d bytes.\n",
	      size-remain, size);
      return -1;
    }
    if (remain == 0)
      break;
  }
  return size;
}

#define WAIT_DURATION 5000000

int time_read_bytes(int s, char * buf, int size)
{
  int val, remain = size;
  char * ptr = buf;
  long start = get_usec();

  int times = 0;
  
  if (size == 0) return 0;
  
  for (;;) {
    struct fd_set read_mask;
    struct timeval tval = {0, 0};

    FD_ZERO(&read_mask);
    FD_SET(s, &read_mask);
#ifdef _HPUX_SOURCE
    if (select(s+1, (int *)&read_mask, NULL, NULL, &tval) <= 0)
#else
    if (select(s+1, &read_mask, NULL, NULL, &tval) <= 0)
#endif
    {
      /*
      if (get_duration(start, get_usec()) < WAIT_DURATION) {
	usleep(10000);
	continue;
      }
      */
      if (times < WAIT_DURATION / 10000) {
	usleep(10000);
        times ++;
        continue;
      }
      else {
	fprintf(stderr, "time_read_bytes() %d seconds expired\n",
		WAIT_DURATION/1000000);
	return -1;
      }
    }

    if (!(FD_ISSET(s, &read_mask))) continue;
    
    val = read(s, ptr, 1);
    
    if (val == -1 && (errno == EINTR))
    {   /* interrupted or need to wait, try again */
      errno = 0;
      continue;
    }
    if (val == -1)
    {
      perror("Error -- time_read_bytes() from socket");
      return -1;
    }
    if (val == 0) /* EOF encountered */
    {
      /*
      fprintf(stderr, "Error -- EOF reached while trying to read %d bytes.\n", size);
      */
      return -1;
      /* 'continue' causes infinite loop if the pipe is closed by peer
      continue;
      */
    }
    ptr += val;
    remain -= val;
    if (remain == 0)
      return 0;
  }
}

/*
   write_bytes(sock, data, len):
     try write len byte of data to sock, report all errors by
     warning message.
*/
void write_bytes(int sock, char * data, int len)
{
  while (len > 0) {
    int res = write(sock, data, len);
    if (res == -1) {
      if (errno == EINTR || errno == EAGAIN) continue;
      fprintf(stderr, "Error pid %d", getpid());
      perror(" -- failed to write all bytes to socket");
      exit(1);
    }
    len -= res;
    data += res;
  }
}

int time_write_bytes(int sock, char * data, int plen)
{
  int len = plen;
  while (len > 0) {
    int res = write(sock, data, len);
    if (res == -1) {
      if (errno == EINTR || errno == EAGAIN) continue;
      fprintf(stderr, "Error pid %d", getpid());
      perror(" -- failed to time_write all bytes to socket");
      return -1;
    }
    len -= res;
    data += res;
  }
  return plen;
}

int wait_write_bytes(int sock, char * data, int plen)
{
  int len = plen;
  while (len > 0) {
    int res = write(sock, data, len);
    if (res == -1) {
      if (errno == EINTR || errno == EAGAIN) continue;
      /*
      fprintf(stderr, "Error pid %d", getpid());
      perror(" -- failed to wait_write all bytes to socket");
      */
      return -1;
    }
    len -= res;
    data += res;
  }
  return plen;
}

/* read_byte:
     read one byte from sock to buffer data */

void read_byte(int sock, char * data)
{
  read_bytes(sock, data, 1);
}

/* read_short:
     read a short integer from socket sock to buffer data,
     byte order conversion is also done.
*/
void read_short(int sock, short * data)
{
  read_bytes(sock, (char *)data, sizeof(short));
  *data = ntohs(*data);
}

/* read_int:
     read a integer from socket sock to buffer data,
     byte order conversion is also done */

void read_int(int sock, int * data)
{
  read_bytes(sock, (char *)data, sizeof(int));
  *data = ntohl(*data);
}

int time_read_int(int sock, int * data)
{
  if (time_read_bytes(sock, (char *)data, sizeof(int)) == -1) {
    return -1;
  }
  *data = ntohl(*data);
  return 0;
}

/* read a string. Returns the length of the string.
   transfer through the socket.
   but the string in buffer might have been truncated.
   the returned string is '\0' terminated */

int read_string(int sock, char *data, int maxlen)
{
  int len;
  read_int(sock, &len);
  if (len == 0)
  {
    data[0] = 0;
    return 0;
  }
  if (len < maxlen)
  {
    read_bytes(sock, data, len);
    data[len] = 0;
  }
  else
  {
    int i;
    char tmp;
    read_bytes(sock, data, maxlen-1);
    data[maxlen-1] = 0;
    for (i = 0; i < len - maxlen + 1; i ++)  /* discard extra data */
      read_byte(sock, &tmp);
  }
  return len;
}


/* write_byte:
     write a byte to socket sock */

void write_byte(int sock, char data)
{
  write_bytes(sock, &data, 1);
}

/* write_short:
     byte order convert the short integer in data and write to socket sock */

void write_short(int sock, short data)
{
  short val = htons(data);
  write_bytes(sock, (char *)&val, sizeof(short));
}

/* write_int:
     byte order convert the integer in data and write to socket sock */
void write_int(int sock, int data)
{
  int val = htonl(data);
  write_bytes(sock, (char *)&val, sizeof(int));
}

int time_write_int(int sock, int data)
{
  int val = htonl(data);
  return time_write_bytes(sock, (char *)&val, sizeof(int));
}

/* write a string in data to socket sock, the string is led by a length */

void write_string(int sock, char *data)
{
  int len = strlen(data);
  
  write_int(sock, len);
  write_bytes(sock, data, len);
}

