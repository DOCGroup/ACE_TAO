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
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>

#include "../include/common.h"
#include "../mpeg_shared/routine.h"
#include "../mpeg_shared/fileio.h"
#include "../mpeg_shared/com.h"   
#include "../mpeg_shared/sendpt.h"
#include "proto.h"

#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) > (b) ? (a) : (b))

/* Start codes. */

#define READ_BLOCK_SIZE 512
#define MAX_TIMER_ADJUST (100 * SPEEDUP_INV_SCALE)

#define SEQ_END_CODE 0x000001b7
#define SEQ_START_CODE 0x000001b3
#define GOP_START_CODE 0x000001b8
#define PICTURE_START_CODE 0x00000100
#define SLICE_MIN_START_CODE 0x00000101
#define SLICE_MAX_START_CODE 0x000001af
#define EXT_START_CODE 0x000001b5
#define USER_START_CODE 0x000001b2

//extern int Mpeg_Global::drift_ppm;
//extern int Mpeg_Global::session_limit, Mpeg_Global::session_num;

static int live_source = 0;
static int video_format;

static int pkts_sent = 0;
static time_t start_time;

static int conn_tag;

static int normalExit = 1;

static int serviceSocket;
static int videoSocket = -1;

static char videoFile[PATH_SIZE];
static FILE *fp;
static int needHeader = 0;
static int lastRef[2];
static int lastRefPtr = 0;
static int currentUPF = 0;
static int addedUPF = 0;
static int addedSignals = 0;
static int VStimeAdvance;
static double fps;  /* current frames-per-second: playback speed */
static double frameRateLimit;

static VideoPacket * packet = NULL;
static int packetBufSize;
static int msgsn = 0;
static int packetsn = 0;
static int msgsize;

static unsigned char precmd, cmd;
static int cmdsn;
static int nextFrame;
static int nextGroup;
static int firstPatternSize;
static char * firstSendPattern = NULL;
static int sendPatternGops;
static char sendPattern[PATTERN_SIZE];

#ifdef STAT
static char * framesSent = NULL;
#endif

static int fileSize = 0;
static int maxS = 0, maxG = 0,  maxI = 0, maxP = 0, maxB = 0;
static int minS=0x7fffffff, minG = 0x7fffffff;
static int minI = 0x7fffffff, minP = 0x7fffffff, minB = 0x7fffffff;
static int numS = 0, numG = 0, numF, numI = 0, numP = 0, numB = 0;
static int averageFrameSize;
static int horizontalSize;
static int verticalSize;
static int pelAspectRatio;
static int pictureRate;
static int vbvBufferSize;
static int firstGopFrames = 0;
static int patternSize = 0;
static char pattern[PATTERN_SIZE];

static struct SystemHeader
{
  long offset;
  int size;
} * systemHeader;
static struct GopTable
{
  int systemHeader;
  long offset;
  int headerSize;
  int size;
  int totalFrames;
  int previousFrames;
  long firstIoffset;
} * gopTable;
static struct FrameTable
{
  char type;
  unsigned short size;
} * frameTable = NULL;

#define FileRead(position, buf, size) \
	{ \
	  if (fseek(fp, (position), 0) == -1) \
	  { \
	    perror("VS error on fseek VideoFile"); \
	    exit(1); \
	  } \
	  while (fread((buf), (size), 1, fp) == 0) \
	  { if (errno == EINTR) { errno = 0; continue;}\
	    perror("VS error on fread VideoFile"); \
	    exit(1); \
	  } \
	}

/*
static void FileRead(long position, char * pbuf, int psize)
{
  int size = psize;
  char * buf = pbuf;
  if (psize == 0)
    return;
  
  if (fseek(fp, position, 0) == -1)
  {
    perror("VS error on fseek VideoFile");
    exit(1);
  }

  while (size > 0)
  {
    int sz = min(size, READ_BLOCK_SIZE);
    if (size < psize)
      usleep(10);
    if (fread(buf, sz, 1, fp) == 0)
    {
      if (errno == EINTR)
	continue;
      perror("VS error on fread videoFile");
      exit(1);
    }
    buf += sz;
    size -= sz;
  }
}
*/

static void CmdRead(char *buf, int psize)
{
  int res = wait_read_bytes(serviceSocket, buf, psize);
  if (res == 0) exit(0);
  if (res == -1) {
    fprintf(stderr, "VS error on read cmdSocket, size %d", psize);
    perror("");
    exit(1);
  }
}

static void CmdWrite(char *buf, int size)
{
  int res = wait_write_bytes(serviceSocket, buf, size);
  if (res == -1) {
    if (errno != EPIPE) perror("VS writes to serviceSocket");
    exit(errno != EPIPE);
  }
}

static int FBread(char * buf, int size)
{ int res;
  while ((res = (conn_tag >= 0 ? wait_read_bytes(videoSocket, buf, size) :
		 read(videoSocket, buf, size))) == -1)
  {
    if (errno == EINTR) {errno = 0; continue; }
    if (errno == EPIPE || errno == ECONNRESET) exit(0);
    perror("VS reads Feedback packet");
    return -1;
  }
  if (res < size) {
    if (res) fprintf(stderr, "VS warn: FBread() res %dB < size %dB\n", res, size);
    return -1;
  }
  return 0;
}


/* send a given packet pointed by 'packet' to the network */

static int send_to_network(int timeToUse)
{
  int count = 0;
  VideoMessage * msghd = (VideoMessage *)(((char *)packet) - sizeof(VideoMessage));
  int sent = 0;
  int packetSize = ntohl(packet->dataBytes);

  msghd->packetsn = htonl(packetsn ++);
  msghd->packetSize = htonl(packetSize + sizeof(* packet));
  /*
  fprintf(stderr, "VS to send pkt %d of size %d.\n",
		  ntohl(msghd->packetsn), ntohl(msghd->packetSize));
  */
  {
    VideoMessage * msg = NULL;
    int size = packetSize + sizeof(* packet);  /* msghd->packetSize */
    int offset = 0;
    int targetTime;
    
    if (size > msgsize)
    {

      if (!timeToUse)
      {
	timeToUse = (msgsize + sizeof(*msg) + 28) * 2;
	    /*
	       set the max network as 500KB.
	       28 - UDP header size
	    */
	/*
	fprintf(stderr, "computed timeToUse %d. ", timeToUse);
	*/
      }
      else
      {
	timeToUse = (timeToUse  * 7) >> 3;
	/*
	fprintf(stderr, "preset timeToUse %d.", timeToUse);
	*/
	timeToUse /= (size + msgsize - 1) / msgsize;
	timeToUse = min(timeToUse, (msgsize + sizeof(*msg) + 28) * 100);
	/* limit min network bandwidth = 10K */
      }

    }
    while (size > 0)
    {
      int segsize, sentsize;
      int resent = 0;
      
      if (msg == NULL) { /* first message for current packet */
	count = 0;
	msg = msghd;
	targetTime =  get_usec();
      }
      else {
#if 0
	/* the select() is not precise enough for being used here*/
	int sleepTime;
	targetTime += timeToUse;
	sleepTime = get_duration(get_usec(), targetTime);
	if (sleepTime >= 5000) {  /* resolution of timer is 10,000 usec */
	  usleep(sleepTime); /* not first message, wait for a while */
	}
#endif
	/*
	count ++;
	if (!(count % 10)) usleep(10000);
	*/
	msg = (VideoMessage *)((char *)msg + msgsize);
	memcpy((char *)msg, (char *)msghd, sizeof(* msg));
      }
      msg->msgsn = htonl(msgsn++);
      msg->msgOffset = htonl(offset);
      msg->msgSize = htonl(min(size, msgsize));

      segsize = min(size, msgsize)+sizeof(*msg);
      if (conn_tag != 0) {  /* packet stream */
	while ((sentsize = write(videoSocket, (char *)msg, segsize)) == -1) {
	  if (errno == EINTR)
	    continue;
	  if (errno == ENOBUFS) {
	    if (resent) {
	      perror("Warning, pkt discarded because");
	      sent = -1;
	      break;
	    }
	    else {
	      resent = 1;
	      perror("VS to sleep 5ms");
	      usleep(5000);
	      continue;
	    }
	  }
	  if (errno != EPIPE) {
	    fprintf(stderr, "VS error on send packet %d of size %d ",
		    msgsn-1, min(size, msgsize)+sizeof(*msg));
	    perror("");
	  }
	  exit(errno != EPIPE);
	}
      }
      else {
	sentsize = wait_write_bytes(videoSocket, (char *)msg, segsize);
	if (sentsize == -1) {
	  if (errno != EPIPE) {
	    fprintf(stderr, "VS error on send packet %d of size %d ",
		    msgsn-1, min(size, msgsize)+sizeof(*msg));
	    perror("");
	  }
	  exit(errno != EPIPE);
	}
      }
      if (sentsize < segsize) {
	SFprintf(stderr, "VS warning: message size %dB, sent only %dB\n",
		segsize, sentsize);
      }
      if (sent == -1)
	break;
      /*
      fprintf(stderr, "VS: message %d of size %d sent.\n",
	      msgsn-1, min(size, msgsize)+sizeof(*msg));
      */
      size -= msgsize;
      offset += msgsize;
    }
  }
  /*
  fprintf(stderr, "sent = %d\n", sent);
  */
  if (!sent) pkts_sent ++;
  return sent;
}

/*
   * send a packet with given systemHeader (optional), gop (optional) and frame.
   *
   * sh - system header id, if -1, then no system header will be sent.
   *      otherwise, only when frame == 0, the given system header will be sent.
   * gop - group of pictures, gop header will be sent when frame == 0
   *       (first I frame );
   * frame - frame to be sent, offset internal to given gop.
   */

/* returns: 0 - packet sent, -1 - packet not sent (failed) */
static int SendPacket(int shtag, int gop, int frame, int timeToUse)
/* frame maybe out of range (PLAY, STEP), in this case, END_SEQ is sent
   to force display of last frame in VD */
{
  char * buf = ((char *) packet) + sizeof(VideoPacket);
  int f = gopTable[gop].previousFrames + frame;
  int sh = gopTable[gop].systemHeader;
  /*
  SFprintf(stderr, "VS to send packet gop-%d, frame-%d.\n", gop, frame);
  */

  packet->currentUPF = ntohl(currentUPF);

  if (frame >= gopTable[gop].totalFrames)
  {
    packet->cmd = htonl(cmd);
    packet->cmdsn = htonl(cmdsn);
    packet->sh = htonl(sh);
    packet->gop = htonl(gop);
    packet->frame = htonl(numF);
    packet->display = htonl(numF-1);
    packet->future = htonl((unsigned)-1);
    packet->past = htonl((unsigned)-1);
    packet->dataBytes = htonl(4);
    *(int*)((char*)packet + sizeof(*packet)) = htonl(SEQ_END_CODE);

    return send_to_network(timeToUse);
  }

  if (frame)
    shtag = 0;
  else if (needHeader)
  {
    shtag = 1;
    needHeader = 0;
  }
  
  packet->cmd = htonl(cmd);
  packet->cmdsn = htonl(cmdsn);
  packet->sh = htonl(sh);
  packet->gop = htonl(gop);
  packet->frame = htonl(f);
  if (frameTable[f].type == 'B')
  {
    int pre1 = -1, pre2 = -1, i = f;
    while (i>0)
      if (frameTable[--i].type != 'B')
      {
	pre1 = i;
	break;
      }
    while (i>0)
      if (frameTable[--i].type != 'B')
      {
	pre2 = i;
	break;
      }
    if (pre2 ==  -1)
    {
      /*
      fprintf(stderr,
	      "frame %d-%d (%d) is a B without past ref, no to be sent.\n",
	      gop, frame, f);
       */
      return -1;
    }
    if (pre1 != lastRef[lastRefPtr] ||
	pre2 != lastRef[1 - lastRefPtr])
    {
      /*
      fprintf(stderr,
	      "send of B frame %d gaveup for past %d/future %d ref not sent.\n",
	      f, pre2, pre1);
       */
      return -1;
    }
    packet->display = htonl(f);
    packet->future = htonl(pre1);
    packet->past = htonl(pre2);
  }
  else
  {
    int next = f;
    int pre = f;

    while (next < numF && frameTable[++next].type == 'B');
    while (pre > 0 && frameTable[--pre].type == 'B');
    if (frameTable[f].type == 'P' && pre != lastRef[lastRefPtr])
    {
      /*
      fprintf(stderr,
	      "send of P frame %d gaveup for past ref %d not sent.\n",
	      f, pre);
      fprintf(stderr, "ref0=%d, ref1=%d, ptr=%d.\n",
	      lastRef[0], lastRef[1], lastRefPtr);
       */
      return -1;
    }
    packet->display = htonl(next);
    packet->future = htonl((unsigned)-1);
    packet->past = htonl(frameTable[f].type == 'P' ? pre : (unsigned)-1);
  }
  {
    char * ptr = buf;
    int size = 0, offset = 0, i;
    if (shtag)   /* send system header */
    {
      size = systemHeader[sh].size;
      FileRead(systemHeader[sh].offset, ptr, size);
      ptr += size;
    }
    if (!frame)   /* send gop header */
    {
      size = gopTable[gop].headerSize;
      FileRead(gopTable[gop].offset, ptr, size);
      ptr += size;
    }
    size = frameTable[f].size;
    for (i=gopTable[gop].previousFrames; i<f; i++)
      offset += frameTable[i].size;
    FileRead((gopTable[gop].firstIoffset + offset), ptr, size);
    ptr += size;
    packet->dataBytes = htonl(ptr - buf);
  }

  {
    int sent = send_to_network(timeToUse);
    if (!sent)
    {
      /*
      fprintf(stderr, "%c%d\n", frameTable[f].type, f);
      fprintf(stderr, "%c frame %d sent.\n", frameTable[f].type, f);
      */
      if (frameTable[f].type != 'B')
      {
	lastRefPtr = 1 - lastRefPtr;
	lastRef[lastRefPtr] = f;
      }
    }
    return sent;
  }
}


static void SendReferences(int group, int frame)
{
  unsigned char orgcmd;
  int i, base;
  int pregroup;
  
  if (group < 0 || group >= numG) return;
  if (frame <= 0 || frame >= gopTable[group].totalFrames) return;
  
  orgcmd = cmd;
  cmd = CmdREF;
  
  if (group > 0) {
    pregroup = 1;
    base = gopTable[group].previousFrames;
    for (i = 0; i <= frame; i ++) {
      if (frameTable[i + base].type == 'P') {
	pregroup = 0;
	break;
      }
    }
  }
  else pregroup = 0;

  if (pregroup) { /* reference frame can be in previous group */
    pregroup = group -1;
    base = gopTable[pregroup].previousFrames;
    for (i = 0; i < gopTable[pregroup].totalFrames; i ++) {
      if (frameTable[i + base].type != 'B') {
	/*
	SFprintf(stderr, "REF group%d, frame%d\n", pregroup, i);
	*/
	SendPacket(i == 0, pregroup, i, 0);
      }
    }
  }

  base = gopTable[group].previousFrames;
  for (i = 0; i < frame; i ++) {
    if (frameTable[i + base].type != 'B') {
      /*
      SFprintf(stderr, "REF group%d, frame%d\n", group, i);
      */
      SendPacket(i == 0, group, i, 0);
    }
  }
  cmd = orgcmd;
}

static int SendPicture(int * frame)
{
  int size;
  char * buf = ((char *) packet) + sizeof(VideoPacket);
  /*
  SFprintf(stderr, "VS to send picture %d.\n", *frame);
  */

  size = ReadLiveVideoPicture(frame, buf, packetBufSize);
  
  packet->currentUPF = ntohl(currentUPF);
  packet->cmd = htonl(cmd);
  packet->cmdsn = htonl(cmdsn);
  packet->sh = packet->gop = packet->frame = packet->display = htonl(*frame);
  packet->future = htonl((unsigned)-1);
  packet->past = htonl((unsigned)-1);

  packet->dataBytes = htonl(size);

  return send_to_network(currentUPF);
}

static int ReadInfoFromFile(void)
{
  int fd = -1, i;
  int fnlen = strlen(videoFile);

  strcpy(&videoFile[fnlen], ".Info");
  fd = open(videoFile, O_RDONLY);
  if (fd == -1)
  {
    fprintf(stderr, "Reminder: VS fails to open %s for read, ", videoFile);
    perror("try create one");
    goto fail_ReadInfoFromFile;
  }
  read_int(fd, &i);
  if (i != fileSize)
  {
    fprintf(stderr, "Warning: fileSize in Info: %d not the same as actual %d.\n",
	    i, fileSize);
    goto fail_ReadInfoFromFile;
  }

  read_int(fd, &maxS);
  read_int(fd, &maxG);
  read_int(fd, &maxI);
  read_int(fd, &maxP);
  read_int(fd, &maxB);
  read_int(fd, &minS);
  read_int(fd, &minG);
  read_int(fd, &minI);
  read_int(fd, &minP);
  read_int(fd, &minB);
  read_int(fd, &numS);
  read_int(fd, &numG);
  read_int(fd, &numF);
  read_int(fd, &numI);
  read_int(fd, &numP);
  read_int(fd, &numB);
  read_int(fd, &averageFrameSize);
  read_int(fd, &horizontalSize);
  read_int(fd, &verticalSize);
  read_int(fd, &pelAspectRatio);
  read_int(fd, &pictureRate);
  read_int(fd, &vbvBufferSize);
  read_int(fd, &patternSize);

  memset(pattern, 0, PATTERN_SIZE);
  read_bytes(fd, pattern, patternSize);
#ifdef STAT
  framesSent = (char *)malloc((numF + 7)>>3);
  if (framesSent == NULL)
  {
    fprintf(stderr, "Error: VS fails to alloc mem for framesSent for %d frames", numF);
    perror("");
    exit(1);
  }
#endif
  systemHeader = (struct SystemHeader *)malloc(sizeof(struct SystemHeader) * numS);
  if (systemHeader == NULL)
  {
    perror("Error: VS error on malloc SystemHeader");
    exit(1);
  }
  gopTable = (struct GopTable *)malloc(sizeof(struct GopTable) * numG);
  if (gopTable == NULL)
  {
    perror("Error: VS error on malloc GopHeader");
    exit(1);
  }
  frameTable = (struct FrameTable *)malloc(sizeof(* frameTable) * numF);
  if (frameTable == NULL)
  {
    perror("Error: VS error on malloc frameTable");
    exit(1);
  }
  packetBufSize = maxS + maxG + max(maxI, max(maxP, maxB));
  packet = (VideoPacket *)malloc(sizeof(VideoMessage) + sizeof(VideoPacket) +
				 packetBufSize);
  if (packet == NULL)	
  {	
    perror("Error: VS error on malloc packet buffer");
    exit(1);
  }
  packet = (VideoPacket *)((char *)packet + sizeof(VideoMessage));

  for (i = 0; i < numS; i ++)
  {
    read_int(fd, (int *)&systemHeader[i].offset);
    read_int(fd, &systemHeader[i].size);
  }
  for (i = 0; i < numG; i ++)
  {
    read_int(fd, &gopTable[i].systemHeader);
    read_int(fd, (int *)&gopTable[i].offset);
    read_int(fd, &gopTable[i].headerSize);
    read_int(fd, &gopTable[i].size);
    read_int(fd, &gopTable[i].totalFrames);
    read_int(fd, &gopTable[i].previousFrames);
    read_int(fd, (int *)&gopTable[i].firstIoffset);
  }
  for (i = 0; i < numF; i ++)
  {
    read_byte(fd, &frameTable[i].type);
    read_short(fd, (short *)&frameTable[i].size);
  }

  close(fd);
  /*
  fprintf(stderr, "Read Info from %s\n", videoFile);
  */
  videoFile[fnlen] = 0;
  return 0;
 fail_ReadInfoFromFile:
  if (fd >= 0)
    close(fd);
  videoFile[fnlen] = 0;
  /*
  fprintf(stderr, "To scan Info from %s\n", videoFile);
  */
  return -1;
}

static void WriteInfoToFile(void)
{
  int fd = -1, i;
  int fnlen = strlen(videoFile);

  strcpy(&videoFile[fnlen], ".Info");
  fd = open(videoFile, O_WRONLY | O_CREAT, 0444);
  if (fd == -1)
  {
    fprintf(stderr, "VS fails to open %s for write", videoFile);
    perror("");
    goto fail_WriteInfoToFile;
  }
  write_int(fd, fileSize);
  write_int(fd, maxS);
  write_int(fd, maxG);
  write_int(fd, maxI);
  write_int(fd, maxP);
  write_int(fd, maxB);
  write_int(fd, minS);
  write_int(fd, minG);
  write_int(fd, minI);
  write_int(fd, minP);
  write_int(fd, minB);
  write_int(fd, numS);
  write_int(fd, numG);
  write_int(fd, numF);
  write_int(fd, numI);
  write_int(fd, numP);
  write_int(fd, numB);
  write_int(fd, averageFrameSize);
  write_int(fd, horizontalSize);
  write_int(fd, verticalSize);
  write_int(fd, pelAspectRatio);
  write_int(fd, pictureRate);
  write_int(fd, vbvBufferSize);
  write_int(fd, patternSize);

  write_bytes(fd, pattern, patternSize);
  
  for (i = 0; i < numS; i ++)
  {
    write_int(fd, systemHeader[i].offset);
    write_int(fd, systemHeader[i].size);
  }
  for (i = 0; i < numG; i ++)
  {
    write_int(fd, gopTable[i].systemHeader);
    write_int(fd, gopTable[i].offset);
    write_int(fd, gopTable[i].headerSize);
    write_int(fd, gopTable[i].size);
    write_int(fd, gopTable[i].totalFrames);
    write_int(fd, gopTable[i].previousFrames);
    write_int(fd, gopTable[i].firstIoffset);
  }
  for (i = 0; i < numF; i ++)
  {
    write_byte(fd, frameTable[i].type);
    write_short(fd, frameTable[i].size);
  }

  close(fd);
  videoFile[fnlen] = 0;
  return;
 fail_WriteInfoToFile:
  if (fd >= 0)
    close(fd);
  videoFile[fnlen] = 0;
  return;
}

/*
#define nextByte  {fileptr ++; \
		   if (fread(&nb, 1, 1, fp) == 0) \
		   { \
		     perror("VS Crossed EOF or error while scanning"); \
		     return 1; \
		   } }

*/

#define nextByte  {int val; fileptr ++; \
		   if ((val = getc(fp)) == EOF) \
		   {\
		     perror("Crossed EOF or error while scanning"); \
		     return 1; \
		   } nb = val;}

#define computePicSize \
	if (inpic) \
	{ \
	  if (pictype == 'I') \
	  { \
	    maxI = max(maxI, (int)(fileptr - picptr - 4)); \
	    minI = min(minI, (int)(fileptr - picptr - 4)); \
	  } \
	  else if (pictype == 'P') \
	  { \
	    maxP = max(maxP, (int)(fileptr - picptr - 4)); \
	    minP = min(minP, (int)(fileptr - picptr - 4)); \
	  } \
	  else \
	  { \
	    maxB = max(maxB, (int)(fileptr - picptr - 4)); \
	    minB = min(minB, (int)(fileptr - picptr - 4)); \
	  } \
	  frameTable[ftptr].type = pictype; \
	  frameTable[ftptr++].size = (int)(fileptr - picptr - 4); \
	  inpic = 0; \
	}

static int init_MPEG1_video_file(void)
{
  unsigned char nb;
  int state = 0;
  unsigned long fileptr = 0;
  unsigned long i, j, k;
  int shptr, gopptr, ftptr;
  int inpic = 0;
  unsigned long picptr = 0;
  int pictype = 0;
  int first = 0;
  int failureType = 0;
  
  fp = fopen(videoFile, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "error on opening video file %s", videoFile);
    perror("");
    return 2;
  }
  if (fseek(fp, 0, 2) == -1)
  {
    fprintf(stderr, "File %s not seekable", videoFile);
    perror("");
    return 3;
  }
  fileSize = ftell(fp);
  
  fseek(fp, 0, 0);

  if (ReadInfoFromFile())
  {
    for (;;)
    {
      nextByte;
      if (state >= 0 && nb == 0x00)
	state ++;
      else if (state >= 2 && nb == 0x01)
	state = -1;
      else if (state == -1)
      {
	if (!first) first ++;
	else if (first == 1) first ++;

	switch (nb)
	{
	case 0xb7:	/* seq_end_code */
	  goto exit_phase1;
	  break;
	case 0xb3:	/* seq_start_code */
	  if (first == 1) first = 3;
	  if (first != 3)
	  {
	    fprintf(stderr, "VS error: given file is not in MPEG format.\n");
	    return 4;
	  }
	  numS ++;
	  break;
	case 0xb8:	/* gop_start_code */
	  numG ++;
	  break;
	case 0x00:	/* picture_start_code */
	  nextByte;
	  nextByte;
	  nb &= 0x38;
	  if (nb == 0x08)
	  {
	    numI ++;
	    if (numG == 2)
	      pattern[patternSize++] = 'I';
	  }
	  else if (nb == 0x10)
	  {
	    numP ++;
	    if (numG == 2)
	      pattern[patternSize++] = 'P';
	  }
	  else if (nb == 0x18)
	  {
	    numB ++;
	    if (numG == 2)
	      pattern[patternSize++] = 'B';
	  }
	  /*
	  else
	    fprintf(stderr, "VS error: unkonw picture type %d\n", nb);
	  */
	  break;
	default:
	  break;
	}
	state = 0;
      }
      else
	state = 0;
    }
   exit_phase1:

    if (first != 3)
    {
      fprintf(stderr, "VS error: given file \"%s\" is not of MPEG format.\n", videoFile);
      return 4;
    }

    pattern[patternSize] = 0;
    memset(sendPattern, 1, PATTERN_SIZE);

    numF = numI + numP + numB;
    averageFrameSize = fileptr / (unsigned)numF;
    /*
    fprintf(stderr, "Pass one finished, total bytes read: %u, average frame size %d\n",
	    fileptr, averageFrameSize);
    fprintf(stderr, "numS-%d, numG-%d, numF-%d, numI-%d, numP-%d, numB-%d\n",
	    numS, numG, numI, numI, numP, numB);
    fprintf(stderr, "Pattern detected: %s\n", pattern);
     */
    if (numF > MAX_FRAMES)
    {
      fprintf(stderr, "VS error: Number of frames (%d) is bigger than MAX_FRAMES (%d).\n\
  you need to update the constant definition in common.h and recompile.\n",
	      numF, MAX_FRAMES);
      return 5;
    }

#ifdef STAT
    framesSent = (char *)malloc((numF + 7)>>3);
    if (framesSent == NULL)
    {
      fprintf(stderr, "VS fails to alloc mem for framesSent for %d frames", numF);
      perror("");
      return 6;
    }
#endif

    systemHeader = (struct SystemHeader *)malloc(sizeof(struct SystemHeader) * numS);
    if (systemHeader == NULL)
    {
      perror("VS error on malloc SystemHeader");
      return 7;
    }
    gopTable = (struct GopTable *)malloc(sizeof(struct GopTable) * numG);
    if (gopTable == NULL)
    {
      perror("VS error on malloc GopHeader");
      return 8;
    }
    frameTable = (struct FrameTable *)malloc(sizeof(* frameTable) * numF);
    if (frameTable == NULL)
    {
      perror("VS error on malloc frameTable");
      return 9;
    }

    rewind(fp);
    fileptr = 0;
    state = 0;
    inpic = 0;
    shptr = -1;
    gopptr = -1;
    ftptr = 0;

    for (;;)
    {
      nextByte;
      if (state >= 0 && nb == 0x00)
	state ++;
      else if (state >= 2 && nb == 0x01)
	state = -1;
      else if (state == -1)
      {
	switch (nb)
	{
	case 0xb7:	/* seq_end_code */
	  if (gopptr >= 0 && gopTable[gopptr].size == 0)
	    gopTable[gopptr].size = fileptr - gopTable[gopptr].offset - 4;
	  computePicSize;
	  goto exit_phase2;
	  break;
	case 0xb3:	/* seq_start_code */
	  if (gopptr >= 0 && gopTable[gopptr].size == 0)
	    gopTable[gopptr].size = fileptr - gopTable[gopptr].offset - 4;
	  computePicSize;
	  shptr ++;
	  systemHeader[shptr].offset = fileptr - 4;
	  systemHeader[shptr].size = 0;
	  break;
	case 0xb8:	/* gop_start_code */
	  if (systemHeader[shptr].size == 0)
	     systemHeader[shptr].size =fileptr -  systemHeader[shptr].offset - 4;
	  if (gopptr >= 0 && gopTable[gopptr].size == 0)
	    gopTable[gopptr].size = fileptr - gopTable[gopptr].offset - 4;
	  computePicSize;
	  gopptr ++;
	  gopTable[gopptr].systemHeader = shptr;
	  gopTable[gopptr].offset = fileptr - 4;
	  gopTable[gopptr].headerSize = 0;
	  gopTable[gopptr].size = 0;
	  gopTable[gopptr].totalFrames = 0;
	  gopTable[gopptr].previousFrames = gopptr ?
	      (gopTable[gopptr - 1].totalFrames + gopTable[gopptr - 1].previousFrames) : 0;

	  break;
	case 0x00:	/* picture_start_code */
	  if (gopTable[gopptr].headerSize == 0)
	  {
	    gopTable[gopptr].headerSize = fileptr - gopTable[gopptr].offset - 4;
	    gopTable[gopptr].firstIoffset = fileptr - 4;
	  }
	  gopTable[gopptr].totalFrames ++;
	  computePicSize;
	  picptr = fileptr - 4;
	  nextByte;
	  nextByte;
	  nb &= 0x38;
	  if (nb == 0x08)
	  {
	    pictype = 'I';
	    inpic = 1;
	  }
	  else if (nb == 0x10)
	  {
	    pictype = 'P';
	    inpic = 1;
	  }
	  else if (nb == 0x18)
	  {
	    pictype = 'B';
	    inpic = 1;
	  }
	  break;
	default:

	  break;
	}
	state = 0;
      }
      else
	state = 0;
    }

  exit_phase2:
    for (shptr = 0; shptr<numS; shptr++)
    {
      maxS = max(maxS, systemHeader[shptr].size);
      minS = min(minS, systemHeader[shptr].size);
    }
    for (gopptr = 0; gopptr<numG; gopptr++)
    {
      maxG = max(maxG, gopTable[gopptr].headerSize);
      minG = min(minG, gopTable[gopptr].headerSize);
    }
    packetBufSize = maxS + maxG + max(maxI, max(maxP, maxB));
    packet = (VideoPacket *)malloc(sizeof(VideoMessage) + sizeof(VideoPacket) +
				   packetBufSize);
    if (packet == NULL)
    {
      perror("VS error on malloc packet buffer");
      return 10;
    }
    packet = (VideoPacket *)((char *)packet + sizeof(VideoMessage));
    /*
    fprintf(stderr, "Pass 2 finished.\n");
    fprintf(stderr, "maxS-%d, maxG-%d, maxI-%d, maxP-%d, maxB-%d.\n", maxS, maxG, maxI, maxP, maxB);
    fprintf(stderr, "minS-%d, minG-%d, minI-%d, minP-%d, minB-%d.\n", minS, minG, minI, minP, minB);
    */
    /*
    {
      int i;

      fprintf(stderr, "id: offset    size  -- system header table:\n");
      for (i=0; i<numS; i++)
	fprintf(stderr, "%-3d %-9u %d\n", i, systemHeader[i].offset, systemHeader[i].size);
      fprintf(stderr,
	      "id:  header   offset   hdsize   totSize  frames   preframs Ioffset  Isize -- GOP\n");
      for (i=0; i<numG; i++)
      {
	fprintf(stderr, "%-4d %-8d %-8u %-8d %-8d %-8d %-8d %-8u %d\n",
		i,
		gopTable[i].systemHeader,
		gopTable[i].offset,
		gopTable[i].headerSize,
		gopTable[i].size,
		gopTable[i].totalFrames,
		gopTable[i].previousFrames,
		gopTable[i].firstIoffset,
		frameTable[gopTable[i].previousFrames].size
		);
      }

     fprintf(stderr, "\nframe information:");
      for (i=0; i<numF; i++)
	fprintf(stderr, "%c%c%-8d", (i%10 ? '\0' : '\n'), frameTable[i].type, frameTable[i].size);
      fprintf(stderr, "\n");

    }
  */
    fseek(fp, systemHeader[0].offset+4, 0);
    nextByte;
    horizontalSize = ((int)nb <<4) & 0xff0;
    nextByte;
    horizontalSize |= (nb >>4) & 0x0f;
    verticalSize = ((int)nb <<8) & 0xf00;
    nextByte;
    verticalSize |= (int)nb & 0xff;
    nextByte;
    pelAspectRatio = ((int)nb >> 4) & 0x0f;
    pictureRate = (int)nb & 0x0f;
    nextByte;
    nextByte;
    nextByte;
    vbvBufferSize = ((int)nb << 5) & 0x3e0;
    nextByte;
    vbvBufferSize |= ((int)nb >>3) & 0x1f;
    /*
    fprintf(stderr, "SysHeader info: hsize-%d, vsize-%d, pelAspect-%d, rate-%d, vbv-%d.\n",
	    horizontalSize, verticalSize, pelAspectRatio, pictureRate, vbvBufferSize);
     */
    WriteInfoToFile();
  }
#if 0
  {
    int i, j = 20;
    
    for (i = numG - 1;; i --) {
      if (gopTable[i].offset < 4235260) {
	fprintf(stderr, "group %d: offset %ld\n", i, gopTable[i].offset);
	if (j -- == 0) break;
      }
    }
    /*
    for (i = 0; i  < numG; i ++) {
      if (gopTable[i].previousFrames > 1800) {
	fprintf(stderr, "group %d: offset %ld pre-frames %d\n",
		i, gopTable[i].offset, gopTable[i].previousFrames);
	break;
      }
    }
    */
  }
#endif
  {
    firstPatternSize = gopTable[0].totalFrames;
    firstSendPattern = (char *)malloc(firstPatternSize);
    if (firstSendPattern == NULL)
    {
      fprintf(stderr, "VS failed to allocate firstSendPattern for %d frames",
	      firstPatternSize);
      perror("");
      return 11;
    }
  }
  firstGopFrames = gopTable[0].totalFrames;
  return 0;
}

static void INITvideo(void)
{
  INITvideoPara para;
  int failureType = 0;
  /*
  fprintf(stderr, "VS about to read Para.\n");
  */
  CmdRead((char *)&para, sizeof(para));
#ifdef NeedByteOrderConversion
  para.sn = ntohl(para.sn);
  para.version = ntohl(para.version);
  para.nameLength = ntohl(para.nameLength);
#endif
  if (para.nameLength>0)
    CmdRead(videoFile, para.nameLength);
  if (Mpeg_Global::session_num > Mpeg_Global::session_limit || para.version != VERSION) {
    char errmsg[128];
    cmd = CmdFAIL;
    CmdWrite((char *)&cmd, 1);
    if (Mpeg_Global::session_num > Mpeg_Global::session_limit) {
      sprintf(errmsg,
	      "Too many sessions being serviced, please try again later.\n");
    }
    else {
      sprintf(errmsg, "Version # not match, VS %d.%02d, Client %d.%02d",
	      VERSION / 100, VERSION % 100,
	      para.version / 100, para.version % 100);
    }
    write_string(serviceSocket, errmsg);
    exit(0);
  }
  cmdsn = para.sn;
  /*
  fprintf(stderr, "MPEG file %s got.\n", videoFile);
  */
  videoFile[para.nameLength] = 0;

  if (!strncasecmp("LiveVideo", videoFile, 9)) {
    if (OpenLiveVideo(&video_format, &horizontalSize,
		      &verticalSize, &averageFrameSize,
		      &fps, &pelAspectRatio) == -1) {
      failureType = 100;
      goto failure;
    }
    if (video_format == VIDEO_MPEG2) {
      failureType = 101;
      goto failure;
    }
    live_source = 1;

    fileSize =0x7fffffff;
    maxS = maxG = maxI = maxP = maxB = minS = minG = minI = minP = minB = 1;
    numS = numG = numF = numI = 0x7fffffff;
    numP = numB = 0;
    vbvBufferSize = 1;
    firstGopFrames = 1;
    patternSize = 1;
    pattern[0] = 'I';
    pattern[1] = 0;
    packetBufSize = verticalSize * horizontalSize * 3;
    packet = (VideoPacket *)malloc(sizeof(VideoMessage) + sizeof(VideoPacket) +
				   packetBufSize);
    if (packet == NULL)	
    {
      perror("Error: VS error on malloc packet buffer");
      exit(1);
    }
    packet = (VideoPacket *)((char *)packet + sizeof(VideoMessage));

  }
  else {
    static double pictureRateTable[] = {23.976, 24, 25, 29.97, 30, 50, 59.94, 60};

    video_format = VIDEO_MPEG1;
    failureType = init_MPEG1_video_file();
    if (failureType) goto failure;
    fps = pictureRateTable[pictureRate - 1];
  }
  
  {
    INITvideoReply reply;

    reply.totalHeaders = htonl(numS);
    reply.totalGroups = htonl(numG);
    reply.totalFrames = htonl(numF);
    reply.sizeIFrame = htonl(maxI);
    reply.sizePFrame = htonl(maxP);
    reply.sizeBFrame = htonl(maxB);
    reply.sizeSystemHeader = htonl(maxS);
    reply.sizeGop = htonl(maxG);
    reply.averageFrameSize = htonl(averageFrameSize);
    reply.verticalSize = htonl(verticalSize);
    reply.horizontalSize = htonl(horizontalSize);
    reply.pelAspectRatio = htonl(pelAspectRatio);
    reply.pictureRate1000 = htonl((int)(fps * 1000));
    reply.vbvBufferSize = htonl(vbvBufferSize);
    reply.firstGopFrames = htonl(firstGopFrames);
    reply.patternSize = htonl(patternSize);
    strncpy(reply.pattern, pattern, PATTERN_SIZE);
    
    reply.live = htonl(live_source);
    reply.format = htonl(video_format);
    
    CmdWrite((char *)&cmd, 1);
    
    CmdWrite((char *)&reply, sizeof(reply));

    /* write the first SH, GOP and IFrame to serviceSocket (TCP),
       using code for SendPacket() */
    {
      int tmpSocket = videoSocket;
      
      if (live_source) StartPlayLiveVideo();
      
      videoSocket = serviceSocket;
      
      if (live_source) {
	int frame = 0;
	SendPicture(&frame);
      }
      else if (video_format == VIDEO_MPEG1) {
	SendPacket(1, 0, 0, 0);
      }
      else {
	fprintf(stderr, "VS: video_format %d not supported.\n",
		video_format);
      }
      videoSocket = tmpSocket;
      
      if (live_source) StopPlayLiveVideo();
    }

    return;
    
  }
 failure:
  {
    char * msg;
    char errmsg[64];
    cmd = CmdFAIL;
    sprintf(errmsg, "VS failed to alloc internal buf (type %d)", failureType);
    CmdWrite((char *)&cmd, 1);
    msg = failureType == 1 ? "not a complete MPEG stream" :
          failureType == 2 ? "can't open MPEG file" :
          failureType == 3 ? "MPEG file is not seekable" :
          failureType == 4 ? "not an MPEG stream" :
          failureType == 5 ?
	     "too many frames in MPEG file, need change MAX_FRAMES and recompile VS" :
	  failureType == 100 ? "failed to connect to live video source" :
	  failureType == 101 ? "live MPEG2 not supported" :
          errmsg;
    write_string(serviceSocket, msg);
    exit(0);
  }
}

#define CheckGroupRange(pnextGroup) \
{ if ((pnextGroup) < 0 || (pnextGroup) >= numG) \
  { fprintf(stderr, "VS: %d.nextGroup(%d) out of range (%d).\n", cmd, (pnextGroup), numG); \
    return; } }

#define CheckFrameRange(pnextFrame) \
{ if ((pnextFrame) < 0 || (pnextFrame) >= numF) \
  { fprintf(stderr, "VS: %d.nextFrame(%d) out of range (%d).\n", cmd, (pnextFrame), numF); \
    return; } }

static int FrameToGroup(int * frame)
{
  int f = * frame;
  int i = 0;
  while (i < numG && gopTable[i].previousFrames <= f) i++;
  i --;
  * frame = f - gopTable[i].previousFrames;
  return i;
}

static void POSITIONvideo()
{
  POSITIONpara para;
  /*
  fprintf(stderr, "POSITION . . .\n");
  */
  CmdRead((char *)&para, sizeof(para));

  if (live_source) return;
  
#ifdef NeedByteOrderConversion
  para.nextGroup = ntohl(para.nextGroup);
  para.sn = ntohl(para.sn);
#endif
  
  CheckGroupRange(para.nextGroup);
  cmdsn = para.sn;
  SendPacket(numS>1 || para.nextGroup == 0, para.nextGroup, 0, 0);
}

static void STEPvideo()
{
  int group;
  STEPpara para;
  int tag = 0;

  CmdRead((char *)&para, sizeof(para));
#ifdef NeedByteOrderConversion
  para.sn = ntohl(para.sn);
  para.nextFrame = ntohl(para.nextFrame);
#endif

  cmdsn = para.sn;

  if (!live_source) {
    if (para.nextFrame >= numF)  /* send SEQ_END */
    {
      tag = 1;
      para.nextFrame --;
    }
    /*
    fprintf(stderr, "STEP . . .frame-%d\n", para.nextFrame);
    */
    CheckFrameRange(para.nextFrame);
    group = FrameToGroup(&para.nextFrame);
    if (precmd != CmdSTEP && !tag ) {
      SendReferences(group, para.nextFrame);
    }
  }
  if (live_source) StartPlayLiveVideo();
  
  if (live_source) {
    SendPicture(&para.nextFrame);
  }
  else if (video_format == VIDEO_MPEG1) {
    SendPacket(numS>1, group, tag ? numF : para.nextFrame, 0);
  }
  else {
    fprintf(stderr, "VS: wierd1\n");
  }
  
  if (live_source) StopPlayLiveVideo();
}

static int timerHeader, timerGroup, timerFrame;
static int timerOn = 0;
static int timerAdjust = 0;
static int preTimerVal = 0;
static void StopTimer(void);
static void TimerSpeed(void);
static void TimerProcessing(void);

static void timerHandler(int sig)
{
  int val2, val3;
  int usec = currentUPF + addedUPF;
  
  if (Mpeg_Global::drift_ppm) {
    usec -= (int)((double)usec * (double)Mpeg_Global::drift_ppm / 1000000.0);
  }
  
  if (timerAdjust > 1)
    usec = (int)(((double)usec * (double)(SPEEDUP_INV_SCALE - 1)) /
		 (double)SPEEDUP_INV_SCALE);
  val3 = get_duration(preTimerVal, (val2 = get_usec()));
  /*
  if (val3 >= usec<< 1))
    fprintf(stderr, "Slower: %d out of currentUPF %d.\n",
            val3, usec);
  else
    fprintf(stderr, "+\n");
  */
  preTimerVal = val2;
  if (val3 < 0 || val3 > 100000000)
    val3 = usec;
  val2 = (val3 + (usec>>1)) / usec;
  if (val2 < 0) val2 = 0;
  if (val2) {
    TimerProcessing();
    val2 --;
  }
  addedSignals += val2;
  
  if (addedSignals) {
    val2 = timerAdjust;
    if (timerAdjust < MAX_TIMER_ADJUST) {
      timerAdjust += addedSignals * SPEEDUP_INV_SCALE;
      if (val2 < SPEEDUP_INV_SCALE) {
	TimerSpeed();
      }
    }
    else {
      /*
      fprintf(stderr, "VS timerAdjust %d, addedSignals %d, timerFrame %d\n",
	      timerAdjust, addedSignals, timerFrame);
      */
      for (val3 = 0; val3 < addedSignals; val3 ++)
        TimerProcessing();
    }
    addedSignals = 0;
  }
}

static void TimerProcessing(void)
{
/*  
  fprintf(stderr, "VS: timerHandler...\n");
*/
  if (!timerOn) {
    return;
  }
  if (timerAdjust < 0)
  {
    timerAdjust += SPEEDUP_INV_SCALE;
    return;
  }
  if (timerAdjust >0)
  {
    if ((--timerAdjust) == 0)
      TimerSpeed();
  }
  if (cmd == CmdPLAY)
  {
    if (timerGroup == numG - 1 && timerFrame >= gopTable[timerGroup].totalFrames - 1)
    {
      timerFrame ++;  /* force sending of END_SEQ when PLAY cmd */
      StopTimer();
      return;
    }
    else
    {
      timerFrame ++;
      if (timerFrame >= gopTable[timerGroup].totalFrames)
      {
        timerGroup ++;
        timerFrame = 0;
        timerHeader = gopTable[timerGroup].systemHeader;
      }
    }
  }
  else {
    if (cmd == CmdFF) {
      if (timerGroup == numG - 1) {
	StopTimer();
	return;
      }
      timerGroup ++;
      timerHeader = gopTable[timerGroup].systemHeader;
    }
    else {
      if (timerGroup == 0) {
	StopTimer();
	return;
      }
      timerGroup --;
      timerHeader = gopTable[timerGroup].systemHeader;
    }
  }
}

static void StartTimer(void)
{
  addedUPF = 0;
  addedSignals = 0;
  timerAdjust = (VStimeAdvance * SPEEDUP_INV_SCALE) / currentUPF;
  /*
  SFprintf(stderr, "VS StartTimer(): fast-start frames %d\n",
	  timerAdjust / SPEEDUP_INV_SCALE);
  */
  TimerSpeed();
  setsignal(SIGALRM, timerHandler);
  timerOn = 1;
  preTimerVal = get_usec();
  /*
  fprintf(stderr, "VS: timer started at %d upf.\n", currentUPF + addedUPF);
  */
}

static void TimerSpeed(void)
{
  struct itimerval val;
  int usec = currentUPF + addedUPF;
  if (Mpeg_Global::drift_ppm) {
    /*
    int drift = (double)usec * (double)Mpeg_Global::drift_ppm / 1000000.0;
    SFprintf(stderr, "Mpeg_Global::drift_ppm %d, usec %d, drift %d, new usec %d\n",
	    Mpeg_Global::drift_ppm, usec, drift, usec - drift);
     */
    usec -= (int)((double)usec * (double)Mpeg_Global::drift_ppm / 1000000.0);
  }
  if (timerAdjust > 1)
    usec = (int)(((double)usec * (double)(SPEEDUP_INV_SCALE - 1)) /
		 (double)SPEEDUP_INV_SCALE);
  val.it_interval.tv_sec =  val.it_value.tv_sec = usec / 1000000;
  val.it_interval.tv_usec = val.it_value.tv_usec = usec % 1000000;
  setitimer(ITIMER_REAL, &val, NULL);
  /*
  SFprintf(stderr,
	  "VS TimerSpeed() at %s speed, timerAdjust %d addedSignals %d.\n",
	  (timerAdjust > 1) ? "higher" : "normal", timerAdjust, addedSignals);
  */
}

static void StopTimer()
{
  struct itimerval val;
  setsignal(SIGALRM, SIG_IGN);
  val.it_interval.tv_sec =  val.it_value.tv_sec = 0;
  val.it_interval.tv_usec = val.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &val, NULL);
  timerOn = 0;
  /*
  fprintf(stderr, "VS: timer stopped.\n");
  */
}

static void GetFeedBack()
{
  VideoFeedBackPara para;
  struct itimerval val;
  int timerUsec;
  
  if (FBread((char *)&para, sizeof(para)) == -1 ||
      ntohl(para.cmdsn) != cmdsn) {
    /*
    SFprintf(stderr, "VS warning: a FB packet discarded.\n");
    */
    return;
  }
#ifdef NeedByteOrderConversion
  para.needHeader = ntohl(para.needHeader);
  para.addUsecPerFrame = ntohl(para.addUsecPerFrame);
  para.addFrames = ntohl(para.addFrames);
  para.sendPatternGops = ntohl(para.sendPatternGops);
  para.frameRateLimit1000 = ntohl(para.frameRateLimit1000);
#endif
  frameRateLimit = para.frameRateLimit1000 / 1000.0;
  sendPatternGops = para.sendPatternGops;
  
  if (!timerOn) return;

  needHeader = para.needHeader;
  memcpy(sendPattern, para.sendPattern, PATTERN_SIZE);
  if (para.addFrames <= 0 || timerAdjust < MAX_TIMER_ADJUST)
  {
    timerAdjust += para.addFrames * SPEEDUP_INV_SCALE;
    TimerSpeed();
  }
  else /* drastic compensation for big gap */
    addedSignals += para.addFrames;
  if (para.addUsecPerFrame) {
    addedUPF += para.addUsecPerFrame;
    TimerSpeed();
  }
  /*
  SFprintf(stderr, "VS fb: addf %d, addupf %d\n",
	  para.addFrames, para.addUsecPerFrame);
  */
}

static void FastVideoPlay(void)
{
  FFpara para;
  int preGroup = -1;
  int preHeader = -1;
  int nfds = (serviceSocket > videoSocket ? serviceSocket : videoSocket) + 1;
  
  CmdRead((char *)&para, sizeof(para));
#ifdef NeedByteOrderConversion
  para.sn = ntohl(para.sn);
  para.nextGroup = ntohl(para.nextGroup);
  para.usecPerFrame = ntohl(para.usecPerFrame);
  para.framesPerSecond = ntohl(para.framesPerSecond);
  para.VStimeAdvance = ntohl(para.VStimeAdvance);
#endif

  if (live_source) return;
  
  VStimeAdvance = para.VStimeAdvance;
  /*
  fprintf(stderr, "VStimeAdvance from client: %d\n", VStimeAdvance);
  */
  CheckGroupRange(para.nextGroup);
  cmdsn = para.sn;
  timerGroup = para.nextGroup;
  timerFrame = 0;
  timerHeader = gopTable[timerGroup].systemHeader;
  currentUPF = para.usecPerFrame;
  StartTimer();
  
  for (;;)
  {
    struct fd_set read_mask;
/*
    fprintf(stderr, "VS: FF/FB - a loop begines. . .\n");
*/
    if (preGroup != timerGroup)
    {
      SendPacket(preHeader != timerHeader, timerGroup, 0,
		 para.usecPerFrame * patternSize >> 2);
      preHeader = timerHeader;
      preGroup = timerGroup;
    }

    FD_ZERO(&read_mask);
    FD_SET(serviceSocket, &read_mask);
    FD_SET(videoSocket, &read_mask);
#ifdef _HPUX_SOURCE
    if (select(nfds, (int *)&read_mask, NULL, NULL, NULL) == -1)
#else
    if (select(nfds, &read_mask, NULL, NULL, NULL) == -1)
#endif
    {
      if (errno == EINTR)
        continue;
      perror("Error - VS select between service and video sockets");
      exit(1);
      
    }
    if (FD_ISSET(serviceSocket, &read_mask))   /* stop */
    {
      CmdRead((char *)&cmd, 1);
      if (cmd == CmdCLOSE) {
	exit(0);
      }
      else if (cmd != CmdSTOP) {
	fprintf(stderr, "VS error: cmd=%d while STOP is expected.\n", cmd);
	normalExit = 0;
	exit(1);
      }
      CmdRead((char *)&cmdsn, sizeof(int));
#ifdef NeedByteOrderConversion
      cmdsn = ntohl(cmdsn);
#endif
      StopTimer();
      break;
    }
    if (FD_ISSET(videoSocket, &read_mask))  /* feedback, speed adjustment */
    {
      GetFeedBack();
    }
  }
}

static void FFvideo()
{
  /*
  fprintf(stderr, "FF . . .\n");
  */
  FastVideoPlay();
}

static void FBvideo()
{
  /*
  fprintf(stderr, "FB . . . \n");
  */
  FastVideoPlay();
}

static void ComputeFirstSendPattern(float limit)
{
  char * buf = firstSendPattern;
  int len = firstPatternSize;
  char * pat = (char *)malloc(len);
  int f;

  if (pat == NULL) {
    fprintf(stderr, "VS error on allocating %d bytes for computing first SP", len);
    perror("");
    exit(1);
  }
  for (f = 0; f < len; f ++) {
    pat[f] = frameTable[f].type;
  }
  memset(buf, 0, len);
  
  if (limit <= 0)
    limit = 1.0;
  
  f = (int)((double)len *
	    ((double)limit / (1000000.0 / (double)currentUPF)) + 0.5);
  /* rounded to integer, instead of truncated */
  if (f >= len)
    f = len;
  else if (f <= 1)
    f = 1;

  ComputeSendPattern(pat, buf, len, f);

  /*
  f = len - f;
  fprintf(stderr, "FirstSendPattern (%d frames dropped): ", f);
  {
    int i;
    for (i = 0; i < len; i ++)
      fputc(buf[i] ? pat[i] : '-', stderr);
  }
  fputc('\n', stderr);
  */
  free(pat);
}

static void PLAYliveVideo(PLAYpara * para)
{
  int doscale;
  int count;
  int first_frame;
  int frame = para->nextFrame;
  int nfds = (serviceSocket > videoSocket ? serviceSocket : videoSocket) + 1;
  struct fd_set read_mask;
  struct timeval tval = {0, 0};
  double ratio;
  
  currentUPF = (int)(1000000.0 / fps);  /* ignore para.usecPerFrame */
  if (frameRateLimit < fps) {
    doscale = 1;
    ratio = min(frameRateLimit, fps) / fps;
    first_frame = frame;
    count = 0;
    /*
    fprintf(stderr, "doscale %d, frameRateLimit %5.2f, fps %5.2f, ratio %5.2f\n",
	    doscale, frameRateLimit, fps, ratio);
    */
  }
  else doscale = 0;
  StartPlayLiveVideo();

  for (;;) {

    if (doscale) {
      for (;;) {
	if ((int)((frame - first_frame) * ratio + 0.5) < count) frame ++;
	else break;
      }
      count ++;
    }
    SendPicture(&frame);
    frame ++;
    
    FD_ZERO(&read_mask);
    FD_SET(serviceSocket, &read_mask);
    FD_SET(videoSocket, &read_mask);
#ifdef _HPUX_SOURCE
    if (select(nfds, (int *)&read_mask, NULL, NULL, &tval) == -1)
#else
    if (select(nfds, &read_mask, NULL, NULL, &tval) == -1)
#endif
    {
      if (errno == EINTR)
        continue;
      perror("Error - VS select between service and video sockets");
      StopPlayLiveVideo();
      exit(1);
      
    }
    if (FD_ISSET(serviceSocket, &read_mask))   /* stop */
    {
      unsigned char tmp;
      CmdRead((char *)&tmp, 1);
      if (tmp == CmdCLOSE) {
	StopPlayLiveVideo();
	exit(0);
      }
      else if (tmp == CmdSTOP) {
	cmd = tmp;
	/*
	fprintf(stderr, "VS: CmdSTOP. . .\n");
	*/
        CmdRead((char *)&cmdsn, sizeof(int));
#ifdef NeedByteOrderConversion
	cmdsn = ntohl(cmdsn);
#endif
	StopPlayLiveVideo();
        break;
      }
      else if (tmp == CmdSPEED)
      {
	SPEEDpara para;
	/*
	fprintf(stderr, "VS: CmdSPEED. . .\n");
	*/
	CmdRead((char *)&para, sizeof(para));
	/* ignore this thing for live video */
      }
      else
      {
	fprintf(stderr, "VS error(live): cmd=%d while expect STOP/SPEED.\n", tmp);
	normalExit = 0;
	StopPlayLiveVideo();
	exit(1);
      }
    }
    if (FD_ISSET(videoSocket, &read_mask))  /* feedback, only for frame rate
					       adjustment */
    {
      VideoFeedBackPara para;
      if (FBread((char *)&para, sizeof(para)) == -1 ||
	  ntohl(para.cmdsn) != cmdsn) {
	/*
	   SFprintf(stderr, "VS warning: a FB packet discarded.\n");
	   */
	return;
      }
#ifdef NeedByteOrderConversion
      para.frameRateLimit1000 = ntohl(para.frameRateLimit1000);
#endif
      frameRateLimit = para.frameRateLimit1000 / 1000.0;
      if (frameRateLimit < fps) {
	doscale = 1;
	ratio = min(frameRateLimit, fps) / fps;
	first_frame = frame;
	count = 0;
	/*
	fprintf(stderr, "doscale %d, frameRateLimit %5.2f, fps %5.2f, ratio %5.2f\n",
		doscale, frameRateLimit, fps, ratio);
	*/
      }
      else doscale = 0;
    }
  }
}

static void PLAYvideo()
{
  PLAYpara para;
  int preGroup = -1;
  int preHeader = -1;
  int preFrame = -1;
  /*
  fprintf(stderr, "PLAY . . .\n");
  */
  CmdRead((char *)&para, sizeof(para));
#ifdef NeedByteOrderConversion
  para.sn = ntohl(para.sn);
  para.nextFrame = ntohl(para.nextFrame);
  para.usecPerFrame = ntohl(para.usecPerFrame);
  para.framesPerSecond = ntohl(para.framesPerSecond);
  para.frameRateLimit1000 = ntohl(para.frameRateLimit1000);
  para.collectStat = ntohl(para.collectStat);
  para.sendPatternGops = ntohl(para.sendPatternGops);
  para.VStimeAdvance = ntohl(para.VStimeAdvance);
#endif

  frameRateLimit = para.frameRateLimit1000 / 1000.0;
  cmdsn = para.sn;
  currentUPF = para.usecPerFrame;
  VStimeAdvance = para.VStimeAdvance;

  {
    int ts = htonl(get_usec());
    CmdWrite((char *)&ts, sizeof(int));
  }
  
  if (live_source || video_format != VIDEO_MPEG1) {
    if (live_source) PLAYliveVideo(&para);
    return;
  }
  
  /*
  fprintf(stderr, "VStimeAdvance from client: %d\n", VStimeAdvance);
  */
  sendPatternGops = para.sendPatternGops;
  ComputeFirstSendPattern(frameRateLimit);
#ifdef STAT
  if (para.collectStat)
    memset(framesSent, 0, (numF + 7)>>3);
#endif
  CheckFrameRange(para.nextFrame);
  timerFrame = para.nextFrame;
  timerGroup = FrameToGroup(&timerFrame);
  timerHeader = gopTable[timerGroup].systemHeader;
  memcpy(sendPattern, para.sendPattern, PATTERN_SIZE);
  SendReferences(timerGroup, timerFrame);
  StartTimer();
  
  for (;;)
  {
    int curGroup = timerGroup;
    int curFrame = timerFrame;
    int curHeader = timerHeader;
    char * sp;
    struct fd_set read_mask;
    int nfds = (serviceSocket > videoSocket ? serviceSocket : videoSocket) + 1;
    
    if (preGroup != curGroup || curFrame != preFrame)
    {
      int sendStatus = -1;
      int frameStep = 1;
      if (curGroup == 0)
      {
	int i = curFrame + 1;
	while (i < firstPatternSize && !firstSendPattern[i])
	{
	  frameStep ++;
	  i++;
	}
      }
      else  /* (curGroup > 0) */
      {
	int i = curFrame + 1;
	sp = sendPattern + ((curGroup - 1) % sendPatternGops) * patternSize;
	while (i < patternSize && !sp[i])
	{
	  frameStep ++;
	  i++;
	}
      }
      if (curGroup == 0)
      {
	if (firstSendPattern[curFrame])
	  sendStatus = 0;
	else /*  (!firstSendPattern[curFrame]) */
	{
	  int i = curFrame - 1;
	  while (i > 0 && !firstSendPattern[i])
	    i--;
	  if (i > preFrame)
	     /* the frame (curGroup, i) hasn't been sent yet */
	  {
	    sendStatus = 0;
	    curFrame = i;
	  }
	  else
	    sendStatus = -1;
	}
      }
      else if (sp[curFrame]) /* curGroup > 0 */
	sendStatus = 0;
      else /*  (!sp[curFrame]) */
      {
	int i = curFrame - 1;
	while (i > 0 && !sp[i])
	  i--;
	if (curGroup == preGroup && i > preFrame)
	   /* the frame (curGroup, i) hasn't been sent yet */
	{
	  sendStatus = 0;
	  curFrame = i;
	}
	else
	  sendStatus = -1;
      }
      if (!sendStatus)
      {
	sendStatus = SendPacket(preHeader != curHeader,
				curGroup, curFrame,
				(currentUPF + addedUPF) * frameStep);
	if (!sendStatus)
	{
	  preHeader = curHeader;
	  preGroup = curGroup;
	  preFrame = curFrame;
#ifdef STAT
	  if (para.collectStat)
	  {
	    int f = gopTable[curGroup].previousFrames + curFrame;
	    framesSent[f>>3] |= (1 << (f % 8));
	  }
#endif
	}
      }
    }

    FD_ZERO(&read_mask);
    FD_SET(serviceSocket, &read_mask);
    FD_SET(videoSocket, &read_mask);
#ifdef _HPUX_SOURCE
    if (select(nfds, (int *)&read_mask, NULL, NULL, NULL) == -1)
#else
    if (select(nfds, &read_mask, NULL, NULL, NULL) == -1)
#endif
    {
      if (errno == EINTR)  /* select() interrupted */
        continue;
      perror("Error - VS select between service and video sockets");
      exit(1);
      
    }
    if (FD_ISSET(serviceSocket, &read_mask))   /* stop, speed change, loop swap */
    {
      unsigned char tmp;
      CmdRead((char *)&tmp, 1);
      if (tmp == CmdCLOSE) {
	exit(0);
      }
      else if (tmp == CmdSTOP) {
	cmd = tmp;
	/*
	fprintf(stderr, "VS: CmdSTOP. . .\n");
	*/
        CmdRead((char *)&cmdsn, sizeof(int));
#ifdef NeedByteOrderConversion
	cmdsn = ntohl(cmdsn);
#endif
        StopTimer();
        break;
      }
      else if (tmp == CmdSPEED)
      {
	SPEEDpara para;
	/*
	fprintf(stderr, "VS: CmdSPEED. . .\n");
	*/
	CmdRead((char *)&para, sizeof(para));
#ifdef NeedByteOrderConversion
	para.sn = ntohl(para.sn);
	para.usecPerFrame = ntohl(para.usecPerFrame);
	para.framesPerSecond = ntohl(para.framesPerSecond);
	para.sendPatternGops = ntohl(para.sendPatternGops);
	para.frameRateLimit1000 = ntohl(para.frameRateLimit1000);
#endif
	frameRateLimit = para.frameRateLimit1000 / 1000.0;
	sendPatternGops = para.sendPatternGops;
	currentUPF = para.usecPerFrame;
	addedUPF = 0;
	memcpy(sendPattern, para.sendPattern, PATTERN_SIZE);
	TimerSpeed();
      }
      else
      {
	fprintf(stderr, "VS error: cmd=%d while expect STOP/SPEED.\n", tmp);
	normalExit = 0;
	exit(1);
      }
    }
    if (FD_ISSET(videoSocket, &read_mask))  /* feedBack, speed adjustment */
    {
      GetFeedBack();
    }
  }
}

#include <ctype.h>

static void STATstream(void)
{
  int i, j = 0;
  for (i = 0; i < numF; i++)
  {
    short size = htons(frameTable[i].size);
    char type = frameTable[i].type;
    if (i == gopTable[j].previousFrames)
    {
      type = tolower(type);
      j ++;
    }
    CmdWrite((char *)&type, 1);
    CmdWrite((char *)&size, 2);
  }
}

static void STATsent(void)
{
#ifdef STAT
  CmdWrite((char *)framesSent, (numF + 7) / 8);
#else
  int i;
  char zeroByte = 0;
  for (i = 0; i < (numF + 7) / 8; i++)
    CmdWrite((char *)&zeroByte, 1);
#endif
}

static void on_exit_routine(void)
{
  struct sockaddr_in peeraddr_in;
  int size = sizeof(peeraddr_in);
  /*
  if (!normalExit) {
    fprintf(stderr, "VS exitting abnormally, dump core...\n");
    kill(getpid(), SIGSEGV);
    usleep(2000000);
  }
  */
  /*
  fprintf(stderr, "A VS session terminated.\n");
  */
  if (getpeername(serviceSocket,
		  (struct sockaddr *)&peeraddr_in, &size) == 0 &&
      peeraddr_in.sin_family == AF_INET) {
    if (strncmp(inet_ntoa(peeraddr_in.sin_addr), "129.95.50", 9)) {
      struct hostent *hp;
      time_t val = time(NULL);
      char * buf = ctime(&start_time);
      
      hp = gethostbyaddr((char *)&(peeraddr_in.sin_addr), 4, AF_INET);
      buf[strlen(buf)-1] = 0;
      printf("%s: %s %3dm%02ds %dP %s\n",
	     buf,
	     hp == NULL ? inet_ntoa(peeraddr_in.sin_addr) : hp->h_name,
	     (val - start_time) / 60, (val - start_time) % 60,
	     pkts_sent, videoFile);
    }
  }
  ComCloseConn(serviceSocket);
  ComCloseConn(videoSocket);
}

void VideoServer(int ctr_fd, int data_fd, int rttag, int max_pkt_size)
{
  serviceSocket = ctr_fd;
  videoSocket = data_fd;
  conn_tag = max_pkt_size;
  
  if (max_pkt_size > 0) msgsize = max_pkt_size;
  else if (max_pkt_size < 0) msgsize = - max_pkt_size;
  else msgsize = 1024 * 1024;
  /*
  SFprintf(stderr, "VS msgsize = %d\n", msgsize);
  */
  msgsize -= sizeof(VideoMessage);
  
  start_time = time(NULL);

  atexit(on_exit_routine);

  lastRef[0] = lastRef[1] = -1;
  lastRefPtr = 0;

  INITvideo();

  if (rttag) {
    if (SetRTpriority("VS", 0) == -1) rttag = 0;
  }
  
  for (;;)
  {
    /*
    fprintf(stderr, "VS: waiting for a new command...\n");
    */
    precmd = cmd;
    CmdRead((char *)&cmd, 1);
    /*
    SFprintf(stderr, "VS got cmd %d\n", cmd);
    */
    switch (cmd)
    {
    case CmdPOSITION:
    case CmdPOSITIONrelease:
      POSITIONvideo();
      break;
    case CmdSTEP:
      STEPvideo();
      break;
    case CmdFF:
      FFvideo();
      break;
    case CmdFB:
      FBvideo();
      break;
    case CmdPLAY:
      PLAYvideo();
      break;
    case CmdCLOSE:
      /*
      fprintf(stderr, "a session closed.\n");
      normalExit =1;
      */
      exit(0);
      break;
    case CmdSTATstream:
      STATstream();
      break;
    case CmdSTATsent:
      STATsent();
      break;
    default:
      fprintf(stderr,
	      "VS error: video channel command %d not known.\n", cmd);
      normalExit = 0;
      exit(1);
      break;
    }
  }
}
