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

#include "include/common.h"
#include "mpeg_shared/routine.h"
#include "mpeg_shared/fileio.h"
#include "mpeg_shared/com.h"   
#include "mpeg_shared/sendpt.h"
#include "server_proto.h"

ACE_RCSID(mpeg_server, vs, "$Id$")

// %% Move all these functions to the VIDEO_SINGLETON class and remove
// zombie code like VideoServer ()...
static int CmdRead(char *buf, int psize)
{
  int res = wait_read_bytes (VIDEO_SINGLETON::instance ()->serviceSocket, 
                             buf, 
                             psize);
  if (res == 0) return(1);
  if (res == -1) {
    fprintf(stderr, "VS error on read VIDEO_SINGLETON::instance ()->cmdSocket, size %d", psize);
   ACE_OS::perror ("");
    return(-1);
  }
  return 0;
}

static void CmdWrite(char *buf, int size)
{
  int res = wait_write_bytes(VIDEO_SINGLETON::instance ()->serviceSocket, buf, size);
  if (res == -1) {
    if (errno != EPIPE)ACE_OS::perror ("VS writes to VIDEO_SINGLETON::instance ()->serviceSocket");
    ACE_OS::exit (errno != EPIPE);
  }
}

int FBread(char * buf, int size)
{ int res;
  while ((res = (VIDEO_SINGLETON::instance ()->conn_tag >= 0 ? wait_read_bytes(VIDEO_SINGLETON::instance ()->videoSocket, buf, size) :
		 ACE_OS::read (VIDEO_SINGLETON::instance ()->videoSocket, buf, size))) == -1)
  {
    if (errno == EINTR) {errno = 0; continue; }
    if (errno == EPIPE || errno == ECONNRESET) ACE_OS::exit (0);
   ACE_OS::perror ("VS reads Feedback VIDEO_SINGLETON::instance ()->packet");
    return -1;
  }
  if (res < size) {
    if (res) fprintf(stderr, "VS warn: FBread() res %dB < size %dB\n", res, size);
    return -1;
  }
  return 0;
}


/* send a given VIDEO_SINGLETON::instance ()->packet pointed by 'VIDEO_SINGLETON::instance ()->packet' to the network */

int send_to_network(int timeToUse)
{
  int count = 0;
  VideoMessage * msghd = (VideoMessage *)(((char *)VIDEO_SINGLETON::instance ()->packet) - sizeof(VideoMessage));
  int sent = 0;
  int packetSize = ntohl(VIDEO_SINGLETON::instance ()->packet->dataBytes);

  msghd->packetsn = htonl(VIDEO_SINGLETON::instance ()->packetsn ++);
  msghd->packetSize = htonl(packetSize + sizeof(* VIDEO_SINGLETON::instance ()->packet));
  /*
  fprintf(stderr, "VS to send pkt %d of size %d.\n",
		  ntohl(msghd->VIDEO_SINGLETON::instance ()->packetsn), ntohl(msghd->VIDEO_SINGLETON::instance ()->packetSize));
  */
  {
    VideoMessage * msg = NULL;
    int size = packetSize + sizeof(* VIDEO_SINGLETON::instance ()->packet);  /* msghd->VIDEO_SINGLETON::instance ()->packetSize */
    int offset = 0;
    int targetTime;
    
    if (size > VIDEO_SINGLETON::instance ()->msgsize)
    {

      if (!timeToUse)
      {
	timeToUse = (VIDEO_SINGLETON::instance ()->msgsize + sizeof(*msg) + 28) * 2;
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
	timeToUse /= (size + VIDEO_SINGLETON::instance ()->msgsize - 1) / VIDEO_SINGLETON::instance ()->msgsize;
	timeToUse = min(timeToUse, (VIDEO_SINGLETON::instance ()->msgsize + sizeof(*msg) + 28) * 100);
	/* limit min network bandwidth = 10K */
      }

    }
    while (size > 0)
    {
      int segsize, sentsize;
      int resent = 0;
      
      if (msg == NULL) { /* first message for current VIDEO_SINGLETON::instance ()->packet */
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
	msg = (VideoMessage *)((char *)msg + VIDEO_SINGLETON::instance ()->msgsize);
	memcpy((char *)msg, (char *)msghd, sizeof(* msg));
      }
      msg->msgsn = htonl(VIDEO_SINGLETON::instance ()->msgsn++);
      msg->msgOffset = htonl(offset);
      msg->msgSize = htonl(min(size, VIDEO_SINGLETON::instance ()->msgsize));

      segsize = min(size, VIDEO_SINGLETON::instance ()->msgsize)+sizeof(*msg);
      if (VIDEO_SINGLETON::instance ()->conn_tag != 0) {  /* VIDEO_SINGLETON::instance ()->packet stream */
	while ((sentsize = ACE_OS::write (VIDEO_SINGLETON::instance ()->videoSocket, (char *)msg, segsize)) == -1) {
	  if (errno == EINTR)
	    continue;
	  if (errno == ENOBUFS) {
	    if (resent) {
	     ACE_OS::perror ("Warning, pkt discarded because");
	      sent = -1;
	      break;
	    }
	    else {
	      resent = 1;
	     ACE_OS::perror ("VS to sleep 5ms");
	      usleep(5000);
	      continue;
	    }
	  }
	  if (errno != EPIPE) {
	    fprintf(stderr, "VS error on send VIDEO_SINGLETON::instance ()->packet %d of size %d ",
		    VIDEO_SINGLETON::instance ()->msgsn-1, min(size, VIDEO_SINGLETON::instance ()->msgsize)+sizeof(*msg));
	   ACE_OS::perror ("");
	  }
	  ACE_OS::exit (errno != EPIPE);
	}
      }
      else {
	sentsize = wait_write_bytes(VIDEO_SINGLETON::instance ()->videoSocket, (char *)msg, segsize);
	if (sentsize == -1) {
	  if (errno != EPIPE) {
	    fprintf(stderr, "VS error on send VIDEO_SINGLETON::instance ()->packet %d of size %d ",
		    VIDEO_SINGLETON::instance ()->msgsn-1, min(size, VIDEO_SINGLETON::instance ()->msgsize)+sizeof(*msg));
	   ACE_OS::perror ("");
	  }
	  ACE_OS::exit (errno != EPIPE);
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
	      VIDEO_SINGLETON::instance ()->msgsn-1, min(size, VIDEO_SINGLETON::instance ()->msgsize)+sizeof(*msg));
      */
      size -= VIDEO_SINGLETON::instance ()->msgsize;
      offset += VIDEO_SINGLETON::instance ()->msgsize;
    }
  }
  /*
  fprintf(stderr, "sent = %d\n", sent);
  */
  if (!sent) VIDEO_SINGLETON::instance ()->pkts_sent ++;
  return sent;
}

/*
   * send a VIDEO_SINGLETON::instance ()->packet with given VIDEO_SINGLETON::instance ()->systemHeader (optional), gop (optional) and frame.
   *
   * sh - system header id, if -1, then no system header will be sent.
   *      otherwise, only when frame == 0, the given system header will be sent.
   * gop - group of pictures, gop header will be sent when frame == 0
   *       (first I frame );
   * frame - frame to be sent, offset internal to given gop.
   */

/* returns: 0 - VIDEO_SINGLETON::instance ()->packet sent, -1 - VIDEO_SINGLETON::instance ()->packet not sent (failed) */
static int SendPacket(int shtag, int gop, int frame, int timeToUse)
/* frame maybe out of range (PLAY, STEP), in this case, END_SEQ is sent
   to force display of last frame in VD */
{
  char * buf = ((char *) VIDEO_SINGLETON::instance ()->packet) + sizeof(VideoPacket);
  int f = VIDEO_SINGLETON::instance ()->gopTable[gop].previousFrames + frame;
  int sh = VIDEO_SINGLETON::instance ()->gopTable[gop].systemHeader;
  /*
  SFprintf(stderr, "VS to send VIDEO_SINGLETON::instance ()->packet gop-%d, frame-%d.\n", gop, frame);
  */

  VIDEO_SINGLETON::instance ()->packet->currentUPF = ntohl(VIDEO_SINGLETON::instance ()->currentUPF);

  if (frame >= VIDEO_SINGLETON::instance ()->gopTable[gop].totalFrames)
  {
    VIDEO_SINGLETON::instance ()->packet->cmd = htonl(VIDEO_SINGLETON::instance ()->cmd);
    VIDEO_SINGLETON::instance ()->packet->cmdsn = htonl(VIDEO_SINGLETON::instance ()->cmdsn);
    VIDEO_SINGLETON::instance ()->packet->sh = htonl(sh);
    VIDEO_SINGLETON::instance ()->packet->gop = htonl(gop);
    VIDEO_SINGLETON::instance ()->packet->frame = htonl(VIDEO_SINGLETON::instance ()->numF);
    VIDEO_SINGLETON::instance ()->packet->display = htonl(VIDEO_SINGLETON::instance ()->numF-1);
    VIDEO_SINGLETON::instance ()->packet->future = htonl((unsigned)-1);
    VIDEO_SINGLETON::instance ()->packet->past = htonl((unsigned)-1);
    VIDEO_SINGLETON::instance ()->packet->dataBytes = htonl(4);
    *(int*)((char*)VIDEO_SINGLETON::instance ()->packet + sizeof(*VIDEO_SINGLETON::instance ()->packet)) = htonl(SEQ_END_CODE);

    return send_to_network(timeToUse);
  }

  if (frame)
    shtag = 0;
  else if (VIDEO_SINGLETON::instance ()->needHeader)
  {
    shtag = 1;
    VIDEO_SINGLETON::instance ()->needHeader = 0;
  }
  
  VIDEO_SINGLETON::instance ()->packet->cmd = htonl(VIDEO_SINGLETON::instance ()->cmd);
  VIDEO_SINGLETON::instance ()->packet->cmdsn = htonl(VIDEO_SINGLETON::instance ()->cmdsn);
  VIDEO_SINGLETON::instance ()->packet->sh = htonl(sh);
  VIDEO_SINGLETON::instance ()->packet->gop = htonl(gop);
  VIDEO_SINGLETON::instance ()->packet->frame = htonl(f);
  if (VIDEO_SINGLETON::instance ()->frameTable[f].type == 'B')
  {
    int pre1 = -1, pre2 = -1, i = f;
    while (i>0)
      if (VIDEO_SINGLETON::instance ()->frameTable[--i].type != 'B')
      {
	pre1 = i;
	break;
      }
    while (i>0)
      if (VIDEO_SINGLETON::instance ()->frameTable[--i].type != 'B')
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
    if (pre1 != VIDEO_SINGLETON::instance ()->lastRef[VIDEO_SINGLETON::instance ()->lastRefPtr] ||
	pre2 != VIDEO_SINGLETON::instance ()->lastRef[1 - VIDEO_SINGLETON::instance ()->lastRefPtr])
    {
      /*
      fprintf(stderr,
	      "send of B frame %d gaveup for past %d/future %d ref not sent.\n",
	      f, pre2, pre1);
       */
      return -1;
    }
    VIDEO_SINGLETON::instance ()->packet->display = htonl(f);
    VIDEO_SINGLETON::instance ()->packet->future = htonl(pre1);
    VIDEO_SINGLETON::instance ()->packet->past = htonl(pre2);
  }
  else
  {
    int next = f;
    int pre = f;

    while (next < VIDEO_SINGLETON::instance ()->numF && VIDEO_SINGLETON::instance ()->frameTable[++next].type == 'B');
    while (pre > 0 && VIDEO_SINGLETON::instance ()->frameTable[--pre].type == 'B');
    if (VIDEO_SINGLETON::instance ()->frameTable[f].type == 'P' && pre != VIDEO_SINGLETON::instance ()->lastRef[VIDEO_SINGLETON::instance ()->lastRefPtr])
    {
      /*
      fprintf(stderr,
	      "send of P frame %d gaveup for past ref %d not sent.\n",
	      f, pre);
      fprintf(stderr, "ref0=%d, ref1=%d, ptr=%d.\n",
	      VIDEO_SINGLETON::instance ()->lastRef[0], VIDEO_SINGLETON::instance ()->lastRef[1], VIDEO_SINGLETON::instance ()->lastRefPtr);
       */
      return -1;
    }
    VIDEO_SINGLETON::instance ()->packet->display = htonl(next);
    VIDEO_SINGLETON::instance ()->packet->future = htonl((unsigned)-1);
    VIDEO_SINGLETON::instance ()->packet->past = htonl(VIDEO_SINGLETON::instance ()->frameTable[f].type == 'P' ? pre : (unsigned)-1);
  }
  {
    char * ptr = buf;
    int size = 0, offset = 0, i;
    if (shtag)   /* send system header */
    {
      size = VIDEO_SINGLETON::instance ()->systemHeader[sh].size;
      FileRead(VIDEO_SINGLETON::instance ()->systemHeader[sh].offset, ptr, size);
      ptr += size;
    }
    if (!frame)   /* send gop header */
    {
      size = VIDEO_SINGLETON::instance ()->gopTable[gop].headerSize;
      FileRead(VIDEO_SINGLETON::instance ()->gopTable[gop].offset, ptr, size);
      ptr += size;
    }
    size = VIDEO_SINGLETON::instance ()->frameTable[f].size;
    for (i=VIDEO_SINGLETON::instance ()->gopTable[gop].previousFrames; i<f; i++)
      offset += VIDEO_SINGLETON::instance ()->frameTable[i].size;
    FileRead((VIDEO_SINGLETON::instance ()->gopTable[gop].firstIoffset + offset), ptr, size);
    ptr += size;
    VIDEO_SINGLETON::instance ()->packet->dataBytes = htonl(ptr - buf);
  }

  {
    int sent = send_to_network(timeToUse);
    if (!sent)
    {
      /*
      fprintf(stderr, "%c%d\n", VIDEO_SINGLETON::instance ()->frameTable[f].type, f);
      fprintf(stderr, "%c frame %d sent.\n", VIDEO_SINGLETON::instance ()->frameTable[f].type, f);
      */
      if (VIDEO_SINGLETON::instance ()->frameTable[f].type != 'B')
      {
	VIDEO_SINGLETON::instance ()->lastRefPtr = 1 - VIDEO_SINGLETON::instance ()->lastRefPtr;
	VIDEO_SINGLETON::instance ()->lastRef[VIDEO_SINGLETON::instance ()->lastRefPtr] = f;
      }
    }
    return sent;
  }
}


int SendReferences(int group, int frame)
{
  unsigned char orgcmd;
  int i, base;
  int pregroup;
  int result;
  
  if (group < 0 || group >= VIDEO_SINGLETON::instance ()->numG) return 0;
  if (frame <= 0 || frame >= VIDEO_SINGLETON::instance ()->gopTable[group].totalFrames) return 0;
  
  orgcmd = VIDEO_SINGLETON::instance ()->cmd;
  VIDEO_SINGLETON::instance ()->cmd = CmdREF;
  
  if (group > 0) {
    pregroup = 1;
    base = VIDEO_SINGLETON::instance ()->gopTable[group].previousFrames;
    for (i = 0; i <= frame; i ++) {
      if (VIDEO_SINGLETON::instance ()->frameTable[i + base].type == 'P') {
	pregroup = 0;
	break;
      }
    }
  }
  else pregroup = 0;

  if (pregroup) { /* reference frame can be in previous group */
    pregroup = group -1;
    base = VIDEO_SINGLETON::instance ()->gopTable[pregroup].previousFrames;
    for (i = 0; i < VIDEO_SINGLETON::instance ()->gopTable[pregroup].totalFrames; i ++) {
      if (VIDEO_SINGLETON::instance ()->frameTable[i + base].type != 'B') {
	/*
	SFprintf(stderr, "REF group%d, frame%d\n", pregroup, i);
	*/
	result = SendPacket(i == 0, pregroup, i, 0);
        if (result != 0)
          return result;
      }
    }
  }

  base = VIDEO_SINGLETON::instance ()->gopTable[group].previousFrames;
  for (i = 0; i < frame; i ++) {
    if (VIDEO_SINGLETON::instance ()->frameTable[i + base].type != 'B') {
      /*
      SFprintf(stderr, "REF group%d, frame%d\n", group, i);
      */
      SendPacket(i == 0, group, i, 0);
    }
  }
  VIDEO_SINGLETON::instance ()->cmd = orgcmd;
}

int SendPicture(int * frame)
{
  int size;
  char * buf = ((char *) VIDEO_SINGLETON::instance ()->packet) + sizeof(VideoPacket);
  /*
  SFprintf(stderr, "VS to send picture %d.\n", *frame);
  */

  size = ReadLiveVideoPicture(frame, buf, VIDEO_SINGLETON::instance ()->packetBufSize);
  
  VIDEO_SINGLETON::instance ()->packet->currentUPF = ntohl(VIDEO_SINGLETON::instance ()->currentUPF);
  VIDEO_SINGLETON::instance ()->packet->cmd = htonl(VIDEO_SINGLETON::instance ()->cmd);
  VIDEO_SINGLETON::instance ()->packet->cmdsn = htonl(VIDEO_SINGLETON::instance ()->cmdsn);
  VIDEO_SINGLETON::instance ()->packet->sh = VIDEO_SINGLETON::instance ()->packet->gop = VIDEO_SINGLETON::instance ()->packet->frame = VIDEO_SINGLETON::instance ()->packet->display = htonl(*frame);
  VIDEO_SINGLETON::instance ()->packet->future = htonl((unsigned)-1);
  VIDEO_SINGLETON::instance ()->packet->past = htonl((unsigned)-1);

  VIDEO_SINGLETON::instance ()->packet->dataBytes = htonl(size);

  return send_to_network(VIDEO_SINGLETON::instance ()->currentUPF);
}

static int ReadInfoFromFile(void)
{
  int fd = -1, i;
  int fnlen =ACE_OS::strlen (VIDEO_SINGLETON::instance ()->videoFile);

  strcpy(&VIDEO_SINGLETON::instance ()->videoFile[fnlen], ".Info");
  fd = open(VIDEO_SINGLETON::instance ()->videoFile, O_RDONLY);
  if (fd == -1)
  {
    fprintf(stderr, "Reminder: VS fails to open %s for read, ", VIDEO_SINGLETON::instance ()->videoFile);
   ACE_OS::perror ("try create one");
    goto fail_ReadInfoFromFile;
  }
  read_int(fd, &i);
  if (i != VIDEO_SINGLETON::instance ()->fileSize)
  {
    fprintf(stderr, "Warning: VIDEO_SINGLETON::instance ()->fileSize in Info: %d not the same as actual %d.\n",
	    i, VIDEO_SINGLETON::instance ()->fileSize);
    goto fail_ReadInfoFromFile;
  }

  read_int(fd, &VIDEO_SINGLETON::instance ()->maxS);
  read_int(fd, &VIDEO_SINGLETON::instance ()->maxG);
  read_int(fd, &VIDEO_SINGLETON::instance ()->maxI);
  read_int(fd, &VIDEO_SINGLETON::instance ()->maxP);
  read_int(fd, &VIDEO_SINGLETON::instance ()->maxB);
  read_int(fd, &VIDEO_SINGLETON::instance ()->minS);
  read_int(fd, &VIDEO_SINGLETON::instance ()->minG);
  read_int(fd, &VIDEO_SINGLETON::instance ()->minI);
  read_int(fd, &VIDEO_SINGLETON::instance ()->minP);
  read_int(fd, &VIDEO_SINGLETON::instance ()->minB);
  read_int(fd, &VIDEO_SINGLETON::instance ()->numS);
  read_int(fd, &VIDEO_SINGLETON::instance ()->numG);
  read_int(fd, &VIDEO_SINGLETON::instance ()->numF);
  read_int(fd, &VIDEO_SINGLETON::instance ()->numI);
  read_int(fd, &VIDEO_SINGLETON::instance ()->numP);
  read_int(fd, &VIDEO_SINGLETON::instance ()->numB);
  read_int(fd, &VIDEO_SINGLETON::instance ()->averageFrameSize);
  read_int(fd, &VIDEO_SINGLETON::instance ()->horizontalSize);
  read_int(fd, &VIDEO_SINGLETON::instance ()->verticalSize);
  read_int(fd, &VIDEO_SINGLETON::instance ()->pelAspectRatio);
  read_int(fd, &VIDEO_SINGLETON::instance ()->pictureRate);
  read_int(fd, &VIDEO_SINGLETON::instance ()->vbvBufferSize);
  read_int(fd, &VIDEO_SINGLETON::instance ()->patternSize);

  memset(VIDEO_SINGLETON::instance ()->pattern, 0, PATTERN_SIZE);
  read_bytes(fd, VIDEO_SINGLETON::instance ()->pattern, VIDEO_SINGLETON::instance ()->patternSize);
#ifdef STAT
  VIDEO_SINGLETON::instance ()->framesSent = (char *)ACE_OS::malloc((VIDEO_SINGLETON::instance ()->numF + 7)>>3);
  if (VIDEO_SINGLETON::instance ()->framesSent == NULL)
  {
    fprintf(stderr, "Error: VS fails to alloc mem for VIDEO_SINGLETON::instance ()->framesSent for %d frames", VIDEO_SINGLETON::instance ()->numF);
   ACE_OS::perror ("");
    ACE_OS::exit (1);
  }
#endif
  VIDEO_SINGLETON::instance ()->systemHeader = (struct Video_Global::SystemHeader *)ACE_OS::malloc(sizeof(struct Video_Global::SystemHeader) * VIDEO_SINGLETON::instance ()->numS);
  if (VIDEO_SINGLETON::instance ()->systemHeader == NULL)
  {
   ACE_OS::perror ("Error: VS error on ACE_OS::malloc VIDEO_SINGLETON::instance ()->SystemHeader");
    ACE_OS::exit (1);
  }
  VIDEO_SINGLETON::instance ()->gopTable = (struct Video_Global::GopTable *)ACE_OS::malloc(sizeof(struct Video_Global::GopTable) * VIDEO_SINGLETON::instance ()->numG);
  if (VIDEO_SINGLETON::instance ()->gopTable == NULL)
  {
   ACE_OS::perror ("Error: VS error on ACE_OS::malloc GopHeader");
    ACE_OS::exit (1);
  }
  VIDEO_SINGLETON::instance ()->frameTable = (struct Video_Global::FrameTable *)ACE_OS::malloc(sizeof(Video_Global::FrameTable) * VIDEO_SINGLETON::instance ()->numF);
  if (VIDEO_SINGLETON::instance ()->frameTable == NULL)
  {
   ACE_OS::perror ("Error: VS error on ACE_OS::malloc VIDEO_SINGLETON::instance ()->frameTable");
    ACE_OS::exit (1);
  }
  VIDEO_SINGLETON::instance ()->packetBufSize = VIDEO_SINGLETON::instance ()->maxS + VIDEO_SINGLETON::instance ()->maxG + max(VIDEO_SINGLETON::instance ()->maxI, max(VIDEO_SINGLETON::instance ()->maxP, VIDEO_SINGLETON::instance ()->maxB));
  VIDEO_SINGLETON::instance ()->packet = (VideoPacket *)ACE_OS::malloc(sizeof(VideoMessage) + sizeof(VideoPacket) +
				 VIDEO_SINGLETON::instance ()->packetBufSize);
  if (VIDEO_SINGLETON::instance ()->packet == NULL)	
  {	
   ACE_OS::perror ("Error: VS error on ACE_OS::malloc VIDEO_SINGLETON::instance ()->packet buffer");
    ACE_OS::exit (1);
  }
  VIDEO_SINGLETON::instance ()->packet = (VideoPacket *)((char *)VIDEO_SINGLETON::instance ()->packet + sizeof(VideoMessage));

  for (i = 0; i < VIDEO_SINGLETON::instance ()->numS; i ++)
  {
    read_int(fd, (int *)&VIDEO_SINGLETON::instance ()->systemHeader[i].offset);
    read_int(fd, &VIDEO_SINGLETON::instance ()->systemHeader[i].size);
  }
  for (i = 0; i < VIDEO_SINGLETON::instance ()->numG; i ++)
  {
    read_int(fd, &VIDEO_SINGLETON::instance ()->gopTable[i].systemHeader);
    read_int(fd, (int *)&VIDEO_SINGLETON::instance ()->gopTable[i].offset);
    read_int(fd, &VIDEO_SINGLETON::instance ()->gopTable[i].headerSize);
    read_int(fd, &VIDEO_SINGLETON::instance ()->gopTable[i].size);
    read_int(fd, &VIDEO_SINGLETON::instance ()->gopTable[i].totalFrames);
    read_int(fd, &VIDEO_SINGLETON::instance ()->gopTable[i].previousFrames);
    read_int(fd, (int *)&VIDEO_SINGLETON::instance ()->gopTable[i].firstIoffset);
  }
  for (i = 0; i < VIDEO_SINGLETON::instance ()->numF; i ++)
  {
    read_byte(fd, &VIDEO_SINGLETON::instance ()->frameTable[i].type);
    read_short(fd, (short *)&VIDEO_SINGLETON::instance ()->frameTable[i].size);
  }

  ACE_OS::close (fd);
  /*
  fprintf(stderr, "Read Info from %s\n", VIDEO_SINGLETON::instance ()->videoFile);
  */
  VIDEO_SINGLETON::instance ()->videoFile[fnlen] = 0;
  return 0;
 fail_ReadInfoFromFile:
  if (fd >= 0)
    ACE_OS::close (fd);
  VIDEO_SINGLETON::instance ()->videoFile[fnlen] = 0;
  /*
  fprintf(stderr, "To scan Info from %s\n", VIDEO_SINGLETON::instance ()->videoFile);
  */
  return -1;
}

static void WriteInfoToFile(void)
{
  int fd = -1, i;
  int fnlen =ACE_OS::strlen (VIDEO_SINGLETON::instance ()->videoFile);

  strcpy(&VIDEO_SINGLETON::instance ()->videoFile[fnlen], ".Info");
  fd = open(VIDEO_SINGLETON::instance ()->videoFile, O_WRONLY | O_CREAT, 0444);
  if (fd == -1)
  {
    fprintf(stderr, "VS fails to open %s for write", VIDEO_SINGLETON::instance ()->videoFile);
   ACE_OS::perror ("");
    goto fail_WriteInfoToFile;
  }
  write_int(fd, VIDEO_SINGLETON::instance ()->fileSize);
  write_int(fd, VIDEO_SINGLETON::instance ()->maxS);
  write_int(fd, VIDEO_SINGLETON::instance ()->maxG);
  write_int(fd, VIDEO_SINGLETON::instance ()->maxI);
  write_int(fd, VIDEO_SINGLETON::instance ()->maxP);
  write_int(fd, VIDEO_SINGLETON::instance ()->maxB);
  write_int(fd, VIDEO_SINGLETON::instance ()->minS);
  write_int(fd, VIDEO_SINGLETON::instance ()->minG);
  write_int(fd, VIDEO_SINGLETON::instance ()->minI);
  write_int(fd, VIDEO_SINGLETON::instance ()->minP);
  write_int(fd, VIDEO_SINGLETON::instance ()->minB);
  write_int(fd, VIDEO_SINGLETON::instance ()->numS);
  write_int(fd, VIDEO_SINGLETON::instance ()->numG);
  write_int(fd, VIDEO_SINGLETON::instance ()->numF);
  write_int(fd, VIDEO_SINGLETON::instance ()->numI);
  write_int(fd, VIDEO_SINGLETON::instance ()->numP);
  write_int(fd, VIDEO_SINGLETON::instance ()->numB);
  write_int(fd, VIDEO_SINGLETON::instance ()->averageFrameSize);
  write_int(fd, VIDEO_SINGLETON::instance ()->horizontalSize);
  write_int(fd, VIDEO_SINGLETON::instance ()->verticalSize);
  write_int(fd, VIDEO_SINGLETON::instance ()->pelAspectRatio);
  write_int(fd, VIDEO_SINGLETON::instance ()->pictureRate);
  write_int(fd, VIDEO_SINGLETON::instance ()->vbvBufferSize);
  write_int(fd, VIDEO_SINGLETON::instance ()->patternSize);

  write_bytes(fd, VIDEO_SINGLETON::instance ()->pattern, VIDEO_SINGLETON::instance ()->patternSize);
  
  for (i = 0; i < VIDEO_SINGLETON::instance ()->numS; i ++)
  {
    write_int(fd, VIDEO_SINGLETON::instance ()->systemHeader[i].offset);
    write_int(fd, VIDEO_SINGLETON::instance ()->systemHeader[i].size);
  }
  for (i = 0; i < VIDEO_SINGLETON::instance ()->numG; i ++)
  {
    write_int(fd, VIDEO_SINGLETON::instance ()->gopTable[i].systemHeader);
    write_int(fd, VIDEO_SINGLETON::instance ()->gopTable[i].offset);
    write_int(fd, VIDEO_SINGLETON::instance ()->gopTable[i].headerSize);
    write_int(fd, VIDEO_SINGLETON::instance ()->gopTable[i].size);
    write_int(fd, VIDEO_SINGLETON::instance ()->gopTable[i].totalFrames);
    write_int(fd, VIDEO_SINGLETON::instance ()->gopTable[i].previousFrames);
    write_int(fd, VIDEO_SINGLETON::instance ()->gopTable[i].firstIoffset);
  }
  for (i = 0; i < VIDEO_SINGLETON::instance ()->numF; i ++)
  {
    write_byte(fd, VIDEO_SINGLETON::instance ()->frameTable[i].type);
    write_short(fd, VIDEO_SINGLETON::instance ()->frameTable[i].size);
  }

  ACE_OS::close (fd);
  VIDEO_SINGLETON::instance ()->videoFile[fnlen] = 0;
  return;
 fail_WriteInfoToFile:
  if (fd >= 0)
    ACE_OS::close (fd);
  VIDEO_SINGLETON::instance ()->videoFile[fnlen] = 0;
  return;
}

/*
#define nextByte  {fileptr ++; \
		   if (fread(&nb, 1, 1, fp) == 0) \
		   { \
		    ACE_OS::perror ("VS Crossed EOF or error while scanning"); \
		     return 1; \
		   } }

*/

#define nextByte  {int val; fileptr ++; \
		   if ((val = getc(VIDEO_SINGLETON::instance ()->fp)) == EOF) \
		   {\
		    ACE_OS::perror ("Crossed EOF or error while scanning"); \
		     return 1; \
		   } nb = val;}

#define computePicSize \
	if (inpic) \
	{ \
	  if (pictype == 'I') \
	  { \
	    VIDEO_SINGLETON::instance ()->maxI = max(VIDEO_SINGLETON::instance ()->maxI, (int)(fileptr - picptr - 4)); \
	    VIDEO_SINGLETON::instance ()->minI = min(VIDEO_SINGLETON::instance ()->minI, (int)(fileptr - picptr - 4)); \
	  } \
	  else if (pictype == 'P') \
	  { \
	    VIDEO_SINGLETON::instance ()->maxP = max(VIDEO_SINGLETON::instance ()->maxP, (int)(fileptr - picptr - 4)); \
	    VIDEO_SINGLETON::instance ()->minP = min(VIDEO_SINGLETON::instance ()->minP, (int)(fileptr - picptr - 4)); \
	  } \
	  else \
	  { \
	    VIDEO_SINGLETON::instance ()->maxB = max(VIDEO_SINGLETON::instance ()->maxB, (int)(fileptr - picptr - 4)); \
	    VIDEO_SINGLETON::instance ()->minB = min(VIDEO_SINGLETON::instance ()->minB, (int)(fileptr - picptr - 4)); \
	  } \
	  VIDEO_SINGLETON::instance ()->frameTable[ftptr].type = pictype; \
	  VIDEO_SINGLETON::instance ()->frameTable[ftptr++].size = (int)(fileptr - picptr - 4); \
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
  
  VIDEO_SINGLETON::instance ()->fp = fopen(VIDEO_SINGLETON::instance ()->videoFile, "r");
  if (VIDEO_SINGLETON::instance ()->fp == NULL)
  {
    fprintf(stderr, "error on opening video file %s", VIDEO_SINGLETON::instance ()->videoFile);
   ACE_OS::perror ("");
    return 2;
  }
  if (fseek(VIDEO_SINGLETON::instance ()->fp, 0, 2) == -1)
  {
    fprintf(stderr, "File %s not seekable", VIDEO_SINGLETON::instance ()->videoFile);
   ACE_OS::perror ("");
    return 3;
  }
  VIDEO_SINGLETON::instance ()->fileSize = ftell(VIDEO_SINGLETON::instance ()->fp);
  
  fseek(VIDEO_SINGLETON::instance ()->fp, 0, 0);

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
	  VIDEO_SINGLETON::instance ()->numS ++;
	  break;
	case 0xb8:	/* gop_start_code */
	  VIDEO_SINGLETON::instance ()->numG ++;
	  break;
	case 0x00:	/* picture_start_code */
	  nextByte;
	  nextByte;
	  nb &= 0x38;
	  if (nb == 0x08)
	  {
	    VIDEO_SINGLETON::instance ()->numI ++;
	    if (VIDEO_SINGLETON::instance ()->numG == 2)
	      VIDEO_SINGLETON::instance ()->pattern[VIDEO_SINGLETON::instance ()->patternSize++] = 'I';
	  }
	  else if (nb == 0x10)
	  {
	    VIDEO_SINGLETON::instance ()->numP ++;
	    if (VIDEO_SINGLETON::instance ()->numG == 2)
	      VIDEO_SINGLETON::instance ()->pattern[VIDEO_SINGLETON::instance ()->patternSize++] = 'P';
	  }
	  else if (nb == 0x18)
	  {
	    VIDEO_SINGLETON::instance ()->numB ++;
	    if (VIDEO_SINGLETON::instance ()->numG == 2)
	      VIDEO_SINGLETON::instance ()->pattern[VIDEO_SINGLETON::instance ()->patternSize++] = 'B';
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
      fprintf(stderr, "VS error: given file \"%s\" is not of MPEG format.\n", VIDEO_SINGLETON::instance ()->videoFile);
      return 4;
    }

    VIDEO_SINGLETON::instance ()->pattern[VIDEO_SINGLETON::instance ()->patternSize] = 0;
    memset(VIDEO_SINGLETON::instance ()->sendPattern, 1, PATTERN_SIZE);

    VIDEO_SINGLETON::instance ()->numF = VIDEO_SINGLETON::instance ()->numI + VIDEO_SINGLETON::instance ()->numP + VIDEO_SINGLETON::instance ()->numB;
    VIDEO_SINGLETON::instance ()->averageFrameSize = fileptr / (unsigned)VIDEO_SINGLETON::instance ()->numF;
    /*
    fprintf(stderr, "Pass one finished, total bytes read: %u, average frame size %d\n",
	    fileptr, VIDEO_SINGLETON::instance ()->averageFrameSize);
    fprintf(stderr, "VIDEO_SINGLETON::instance ()->numS-%d, VIDEO_SINGLETON::instance ()->numG-%d, VIDEO_SINGLETON::instance ()->numF-%d, VIDEO_SINGLETON::instance ()->numI-%d, VIDEO_SINGLETON::instance ()->numP-%d, VIDEO_SINGLETON::instance ()->numB-%d\n",
	    VIDEO_SINGLETON::instance ()->numS, VIDEO_SINGLETON::instance ()->numG, VIDEO_SINGLETON::instance ()->numI, VIDEO_SINGLETON::instance ()->numI, VIDEO_SINGLETON::instance ()->numP, VIDEO_SINGLETON::instance ()->numB);
    fprintf(stderr, "VIDEO_SINGLETON::Instance ()->Pattern detected: %s\n", VIDEO_SINGLETON::instance ()->pattern);
     */
    if (VIDEO_SINGLETON::instance ()->numF > MAX_FRAMES)
    {
      fprintf(stderr, "VS error: VIDEO_SINGLETON::instance ()->Number of frames (%d) is bigger than MAX_FRAMES (%d).\n\
  you need to update the constant definition in common.h and recompile.\n",
	      VIDEO_SINGLETON::instance ()->numF, MAX_FRAMES);
      return 5;
    }

#ifdef STAT
    VIDEO_SINGLETON::instance ()->framesSent = (char *)ACE_OS::malloc((VIDEO_SINGLETON::instance ()->numF + 7)>>3);
    if (VIDEO_SINGLETON::instance ()->framesSent == NULL)
    {
      fprintf(stderr, "VS fails to alloc mem for VIDEO_SINGLETON::instance ()->framesSent for %d frames", VIDEO_SINGLETON::instance ()->numF);
     ACE_OS::perror ("");
      return 6;
    }
#endif

    VIDEO_SINGLETON::instance ()->systemHeader = (struct Video_Global::SystemHeader *)ACE_OS::malloc(sizeof(struct Video_Global::SystemHeader) * VIDEO_SINGLETON::instance ()->numS);
    if (VIDEO_SINGLETON::instance ()->systemHeader == NULL)
    {
     ACE_OS::perror ("VS error on ACE_OS::malloc VIDEO_SINGLETON::instance ()->SystemHeader");
      return 7;
    }
    VIDEO_SINGLETON::instance ()->gopTable = (struct Video_Global::GopTable *)ACE_OS::malloc(sizeof(struct Video_Global::GopTable) * VIDEO_SINGLETON::instance ()->numG);
    if (VIDEO_SINGLETON::instance ()->gopTable == NULL)
    {
     ACE_OS::perror ("VS error on ACE_OS::malloc GopHeader");
      return 8;
    }
    VIDEO_SINGLETON::instance ()->frameTable = (struct Video_Global::FrameTable *)ACE_OS::malloc(sizeof(Video_Global::FrameTable) * VIDEO_SINGLETON::instance ()->numF);
    if (VIDEO_SINGLETON::instance ()->frameTable == NULL)
    {
     ACE_OS::perror ("VS error on ACE_OS::malloc VIDEO_SINGLETON::instance ()->frameTable");
      return 9;
    }

    rewind(VIDEO_SINGLETON::instance ()->fp);
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
	  if (gopptr >= 0 && VIDEO_SINGLETON::instance ()->gopTable[gopptr].size == 0)
	    VIDEO_SINGLETON::instance ()->gopTable[gopptr].size = fileptr - VIDEO_SINGLETON::instance ()->gopTable[gopptr].offset - 4;
	  computePicSize;
	  goto exit_phase2;
	  break;
	case 0xb3:	/* seq_start_code */
	  if (gopptr >= 0 && VIDEO_SINGLETON::instance ()->gopTable[gopptr].size == 0)
	    VIDEO_SINGLETON::instance ()->gopTable[gopptr].size = fileptr - VIDEO_SINGLETON::instance ()->gopTable[gopptr].offset - 4;
	  computePicSize;
	  shptr ++;
	  VIDEO_SINGLETON::instance ()->systemHeader[shptr].offset = fileptr - 4;
	  VIDEO_SINGLETON::instance ()->systemHeader[shptr].size = 0;
	  break;
	case 0xb8:	/* gop_start_code */
	  if (VIDEO_SINGLETON::instance ()->systemHeader[shptr].size == 0)
	     VIDEO_SINGLETON::instance ()->systemHeader[shptr].size =fileptr -  VIDEO_SINGLETON::instance ()->systemHeader[shptr].offset - 4;
	  if (gopptr >= 0 && VIDEO_SINGLETON::instance ()->gopTable[gopptr].size == 0)
	    VIDEO_SINGLETON::instance ()->gopTable[gopptr].size = fileptr - VIDEO_SINGLETON::instance ()->gopTable[gopptr].offset - 4;
	  computePicSize;
	  gopptr ++;
	  VIDEO_SINGLETON::instance ()->gopTable[gopptr].systemHeader = shptr;
	  VIDEO_SINGLETON::instance ()->gopTable[gopptr].offset = fileptr - 4;
	  VIDEO_SINGLETON::instance ()->gopTable[gopptr].headerSize = 0;
	  VIDEO_SINGLETON::instance ()->gopTable[gopptr].size = 0;
	  VIDEO_SINGLETON::instance ()->gopTable[gopptr].totalFrames = 0;
	  VIDEO_SINGLETON::instance ()->gopTable[gopptr].previousFrames = gopptr ?
	      (VIDEO_SINGLETON::instance ()->gopTable[gopptr - 1].totalFrames + VIDEO_SINGLETON::instance ()->gopTable[gopptr - 1].previousFrames) : 0;

	  break;
	case 0x00:	/* picture_start_code */
	  if (VIDEO_SINGLETON::instance ()->gopTable[gopptr].headerSize == 0)
	  {
	    VIDEO_SINGLETON::instance ()->gopTable[gopptr].headerSize = fileptr - VIDEO_SINGLETON::instance ()->gopTable[gopptr].offset - 4;
	    VIDEO_SINGLETON::instance ()->gopTable[gopptr].firstIoffset = fileptr - 4;
	  }
	  VIDEO_SINGLETON::instance ()->gopTable[gopptr].totalFrames ++;
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
    for (shptr = 0; shptr<VIDEO_SINGLETON::instance ()->numS; shptr++)
    {
      VIDEO_SINGLETON::instance ()->maxS = max(VIDEO_SINGLETON::instance ()->maxS, VIDEO_SINGLETON::instance ()->systemHeader[shptr].size);
      VIDEO_SINGLETON::instance ()->minS = min(VIDEO_SINGLETON::instance ()->minS, VIDEO_SINGLETON::instance ()->systemHeader[shptr].size);
    }
    for (gopptr = 0; gopptr<VIDEO_SINGLETON::instance ()->numG; gopptr++)
    {
      VIDEO_SINGLETON::instance ()->maxG = max(VIDEO_SINGLETON::instance ()->maxG, VIDEO_SINGLETON::instance ()->gopTable[gopptr].headerSize);
      VIDEO_SINGLETON::instance ()->minG = min(VIDEO_SINGLETON::instance ()->minG, VIDEO_SINGLETON::instance ()->gopTable[gopptr].headerSize);
    }
    VIDEO_SINGLETON::instance ()->packetBufSize = VIDEO_SINGLETON::instance ()->maxS + VIDEO_SINGLETON::instance ()->maxG + max(VIDEO_SINGLETON::instance ()->maxI, max(VIDEO_SINGLETON::instance ()->maxP, VIDEO_SINGLETON::instance ()->maxB));
    VIDEO_SINGLETON::instance ()->packet = (VideoPacket *)ACE_OS::malloc(sizeof(VideoMessage) + sizeof(VideoPacket) +
				   VIDEO_SINGLETON::instance ()->packetBufSize);
    if (VIDEO_SINGLETON::instance ()->packet == NULL)
    {
     ACE_OS::perror ("VS error on ACE_OS::malloc VIDEO_SINGLETON::instance ()->packet buffer");
      return 10;
    }
    VIDEO_SINGLETON::instance ()->packet = (VideoPacket *)((char *)VIDEO_SINGLETON::instance ()->packet + sizeof(VideoMessage));
    /*
    fprintf(stderr, "Pass 2 finished.\n");
    fprintf(stderr, "VIDEO_SINGLETON::instance ()->maxS-%d, VIDEO_SINGLETON::instance ()->maxG-%d, VIDEO_SINGLETON::instance ()->maxI-%d, VIDEO_SINGLETON::instance ()->maxP-%d, VIDEO_SINGLETON::instance ()->maxB-%d.\n", VIDEO_SINGLETON::instance ()->maxS, VIDEO_SINGLETON::instance ()->maxG, VIDEO_SINGLETON::instance ()->maxI, VIDEO_SINGLETON::instance ()->maxP, VIDEO_SINGLETON::instance ()->maxB);
    fprintf(stderr, "VIDEO_SINGLETON::instance ()->minS-%d, VIDEO_SINGLETON::instance ()->minG-%d, VIDEO_SINGLETON::instance ()->minI-%d, VIDEO_SINGLETON::instance ()->minP-%d, VIDEO_SINGLETON::instance ()->minB-%d.\n", VIDEO_SINGLETON::instance ()->minS, VIDEO_SINGLETON::instance ()->minG, VIDEO_SINGLETON::instance ()->minI, VIDEO_SINGLETON::instance ()->minP, VIDEO_SINGLETON::instance ()->minB);
    */
    /*
    {
      int i;

      fprintf(stderr, "id: offset    size  -- system header table:\n");
      for (i=0; i<VIDEO_SINGLETON::instance ()->numS; i++)
	fprintf(stderr, "%-3d %-9u %d\n", i, VIDEO_SINGLETON::instance ()->systemHeader[i].offset, VIDEO_SINGLETON::instance ()->systemHeader[i].size);
      fprintf(stderr,
	      "id:  header   offset   hdsize   totSize  frames   preframs Ioffset  Isize -- GOP\n");
      for (i=0; i<VIDEO_SINGLETON::instance ()->numG; i++)
      {
	fprintf(stderr, "%-4d %-8d %-8u %-8d %-8d %-8d %-8d %-8u %d\n",
		i,
		VIDEO_SINGLETON::instance ()->gopTable[i].VIDEO_SINGLETON::instance ()->systemHeader,
		VIDEO_SINGLETON::instance ()->gopTable[i].offset,
		VIDEO_SINGLETON::instance ()->gopTable[i].headerSize,
		VIDEO_SINGLETON::instance ()->gopTable[i].size,
		VIDEO_SINGLETON::instance ()->gopTable[i].totalFrames,
		VIDEO_SINGLETON::instance ()->gopTable[i].previousFrames,
		VIDEO_SINGLETON::instance ()->gopTable[i].firstIoffset,
		VIDEO_SINGLETON::instance ()->frameTable[VIDEO_SINGLETON::instance ()->gopTable[i].previousFrames].size
		);
      }

     fprintf(stderr, "\nframe information:");
      for (i=0; i<VIDEO_SINGLETON::instance ()->numF; i++)
	fprintf(stderr, "%c%c%-8d", (i%10 ? '\0' : '\n'), VIDEO_SINGLETON::instance ()->frameTable[i].type, VIDEO_SINGLETON::instance ()->frameTable[i].size);
      fprintf(stderr, "\n");

    }
  */
    fseek(VIDEO_SINGLETON::instance ()->fp, VIDEO_SINGLETON::instance ()->systemHeader[0].offset+4, 0);
    nextByte;
    VIDEO_SINGLETON::instance ()->horizontalSize = ((int)nb <<4) & 0xff0;
    nextByte;
    VIDEO_SINGLETON::instance ()->horizontalSize |= (nb >>4) & 0x0f;
    VIDEO_SINGLETON::instance ()->verticalSize = ((int)nb <<8) & 0xf00;
    nextByte;
    VIDEO_SINGLETON::instance ()->verticalSize |= (int)nb & 0xff;
    nextByte;
    VIDEO_SINGLETON::instance ()->pelAspectRatio = ((int)nb >> 4) & 0x0f;
    VIDEO_SINGLETON::instance ()->pictureRate = (int)nb & 0x0f;
    nextByte;
    nextByte;
    nextByte;
    VIDEO_SINGLETON::instance ()->vbvBufferSize = ((int)nb << 5) & 0x3e0;
    nextByte;
    VIDEO_SINGLETON::instance ()->vbvBufferSize |= ((int)nb >>3) & 0x1f;
    /*
    fprintf(stderr, "SysHeader info: hsize-%d, vsize-%d, pelAspect-%d, rate-%d, vbv-%d.\n",
	    VIDEO_SINGLETON::instance ()->horizontalSize, VIDEO_SINGLETON::instance ()->verticalSize, VIDEO_SINGLETON::instance ()->pelAspectRatio, VIDEO_SINGLETON::instance ()->pictureRate, VIDEO_SINGLETON::instance ()->vbvBufferSize);
     */
    WriteInfoToFile();
  }
#if 0
  {
    int i, j = 20;
    
    for (i = VIDEO_SINGLETON::instance ()->numG - 1;; i --) {
      if (VIDEO_SINGLETON::instance ()->gopTable[i].offset < 4235260) {
	fprintf(stderr, "group %d: offset %ld\n", i, VIDEO_SINGLETON::instance ()->gopTable[i].offset);
	if (j -- == 0) break;
      }
    }
    /*
    for (i = 0; i  < VIDEO_SINGLETON::instance ()->numG; i ++) {
      if (VIDEO_SINGLETON::instance ()->gopTable[i].previousFrames > 1800) {
	fprintf(stderr, "group %d: offset %ld pre-frames %d\n",
		i, VIDEO_SINGLETON::instance ()->gopTable[i].offset, VIDEO_SINGLETON::instance ()->gopTable[i].previousFrames);
	break;
      }
    }
    */
  }
#endif
  {
    VIDEO_SINGLETON::instance ()->firstPatternSize = VIDEO_SINGLETON::instance ()->gopTable[0].totalFrames;
    VIDEO_SINGLETON::instance ()->firstSendPattern = (char *)ACE_OS::malloc(VIDEO_SINGLETON::instance ()->firstPatternSize);
    if (VIDEO_SINGLETON::instance ()->firstSendPattern == NULL)
    {
      fprintf(stderr, "VS failed to allocate VIDEO_SINGLETON::instance ()->firstVIDEO_SINGLETON::instance ()->SendVIDEO_SINGLETON::Instance ()->Pattern for %d frames",
	      VIDEO_SINGLETON::instance ()->firstPatternSize);
     ACE_OS::perror ("");
      return 11;
    }
  }
  VIDEO_SINGLETON::instance ()->firstGopFrames = VIDEO_SINGLETON::instance ()->gopTable[0].totalFrames;
  return 0;
}

 int INITvideo(void)
{
  INITvideoPara para;
  int failureType = 0;
  int result;
  /*
  fprintf(stderr, "VS about to read Para.\n");
  */
  result = CmdRead((char *)&para, sizeof(para));
  if (result != 0)
    return result;
#ifdef NeedByteOrderConversion
  para.sn = ntohl(para.sn);
  para.version = ntohl(para.version);
  para.nameLength = ntohl(para.nameLength);
#endif
  if (para.nameLength>0)
    {
    result = CmdRead(VIDEO_SINGLETON::instance ()->videoFile, para.nameLength);
    if (result != 0)
      return result;
    }
  if (Mpeg_Global::session_num > Mpeg_Global::session_limit || para.version != VERSION) {
    char errmsg[128];
    VIDEO_SINGLETON::instance ()->cmd = CmdFAIL;
    CmdWrite((char *)&VIDEO_SINGLETON::instance ()->cmd, 1);
    if (Mpeg_Global::session_num > Mpeg_Global::session_limit) {
      sprintf(errmsg,
	      "Too many sessions being serviced, please try again later.\n");
    }
    else {
      sprintf(errmsg, "Version # not match, VS %d.%02d, Client %d.%02d",
	      VERSION / 100, VERSION % 100,
	      para.version / 100, para.version % 100);
    }
    write_string(VIDEO_SINGLETON::instance ()->serviceSocket, errmsg);
    ACE_OS::exit (0);
  }
  VIDEO_SINGLETON::instance ()->cmdsn = para.sn;
  /*
  fprintf(stderr, "MPEG file %s got.\n", VIDEO_SINGLETON::instance ()->videoFile);
  */
  VIDEO_SINGLETON::instance ()->videoFile[para.nameLength] = 0;

  if (!strncasecmp("LiveVideo", VIDEO_SINGLETON::instance ()->videoFile, 9)) {
    if (OpenLiveVideo(&VIDEO_SINGLETON::instance ()->video_format, &VIDEO_SINGLETON::instance ()->horizontalSize,
		      &VIDEO_SINGLETON::instance ()->verticalSize, &VIDEO_SINGLETON::instance ()->averageFrameSize,
		      &VIDEO_SINGLETON::instance ()->fps, &VIDEO_SINGLETON::instance ()->pelAspectRatio) == -1) {
      failureType = 100;
      goto failure;
    }
    if (VIDEO_SINGLETON::instance ()->video_format == VIDEO_MPEG2) {
      failureType = 101;
      goto failure;
    }
    VIDEO_SINGLETON::instance ()->live_source = 1;

    VIDEO_SINGLETON::instance ()->fileSize =0x7fffffff;
    VIDEO_SINGLETON::instance ()->maxS = VIDEO_SINGLETON::instance ()->maxG = VIDEO_SINGLETON::instance ()->maxI = VIDEO_SINGLETON::instance ()->maxP = VIDEO_SINGLETON::instance ()->maxB = VIDEO_SINGLETON::instance ()->minS = VIDEO_SINGLETON::instance ()->minG = VIDEO_SINGLETON::instance ()->minI = VIDEO_SINGLETON::instance ()->minP = VIDEO_SINGLETON::instance ()->minB = 1;
    VIDEO_SINGLETON::instance ()->numS = VIDEO_SINGLETON::instance ()->numG = VIDEO_SINGLETON::instance ()->numF = VIDEO_SINGLETON::instance ()->numI = 0x7fffffff;
    VIDEO_SINGLETON::instance ()->numP = VIDEO_SINGLETON::instance ()->numB = 0;
    VIDEO_SINGLETON::instance ()->vbvBufferSize = 1;
    VIDEO_SINGLETON::instance ()->firstGopFrames = 1;
    VIDEO_SINGLETON::instance ()->patternSize = 1;
    VIDEO_SINGLETON::instance ()->pattern[0] = 'I';
    VIDEO_SINGLETON::instance ()->pattern[1] = 0;
    VIDEO_SINGLETON::instance ()->packetBufSize = VIDEO_SINGLETON::instance ()->verticalSize * VIDEO_SINGLETON::instance ()->horizontalSize * 3;
    VIDEO_SINGLETON::instance ()->packet = (VideoPacket *)ACE_OS::malloc(sizeof(VideoMessage) + sizeof(VideoPacket) +
				   VIDEO_SINGLETON::instance ()->packetBufSize);
    if (VIDEO_SINGLETON::instance ()->packet == NULL)	
    {
     ACE_OS::perror ("Error: VS error on ACE_OS::malloc VIDEO_SINGLETON::instance ()->packet buffer");
      ACE_OS::exit (1);
    }
    VIDEO_SINGLETON::instance ()->packet = (VideoPacket *)((char *)VIDEO_SINGLETON::instance ()->packet + sizeof(VideoMessage));

  }
  else {
    static double pictureRateTable[] = {23.976, 24, 25, 29.97, 30, 50, 59.94, 60};

    VIDEO_SINGLETON::instance ()->video_format = VIDEO_MPEG1;
    failureType = init_MPEG1_video_file();
    if (failureType) goto failure;
    VIDEO_SINGLETON::instance ()->fps = pictureRateTable[VIDEO_SINGLETON::instance ()->pictureRate - 1];
  }
  
  {
    INITvideoReply reply;

    reply.totalHeaders = htonl(VIDEO_SINGLETON::instance ()->numS);
    reply.totalGroups = htonl(VIDEO_SINGLETON::instance ()->numG);
    reply.totalFrames = htonl(VIDEO_SINGLETON::instance ()->numF);
    reply.sizeIFrame = htonl(VIDEO_SINGLETON::instance ()->maxI);
    reply.sizePFrame = htonl(VIDEO_SINGLETON::instance ()->maxP);
    reply.sizeBFrame = htonl(VIDEO_SINGLETON::instance ()->maxB);
    reply.sizeSystemHeader = htonl(VIDEO_SINGLETON::instance ()->maxS);
    reply.sizeGop = htonl(VIDEO_SINGLETON::instance ()->maxG);
    reply.averageFrameSize = htonl(VIDEO_SINGLETON::instance ()->averageFrameSize);
    reply.verticalSize = htonl(VIDEO_SINGLETON::instance ()->verticalSize);
    reply.horizontalSize = htonl(VIDEO_SINGLETON::instance ()->horizontalSize);
    reply.pelAspectRatio = htonl(VIDEO_SINGLETON::instance ()->pelAspectRatio);
    reply.pictureRate1000 = htonl((int)(VIDEO_SINGLETON::instance ()->fps * 1000));
    reply.vbvBufferSize = htonl(VIDEO_SINGLETON::instance ()->vbvBufferSize);
    reply.firstGopFrames = htonl(VIDEO_SINGLETON::instance ()->firstGopFrames);
    reply.patternSize = htonl(VIDEO_SINGLETON::instance ()->patternSize);
    strncpy(reply.pattern, VIDEO_SINGLETON::instance ()->pattern, PATTERN_SIZE);
    
    reply.live = htonl(VIDEO_SINGLETON::instance ()->live_source);
    reply.format = htonl(VIDEO_SINGLETON::instance ()->video_format);
    
    CmdWrite((char *)&VIDEO_SINGLETON::instance ()->cmd, 1);
    
    CmdWrite((char *)&reply, sizeof(reply));

    /* write the first SH, GOP and IFrame to VIDEO_SINGLETON::instance ()->serviceSocket (TCP),
       using code for SendPacket() */
    {
      int tmpSocket = VIDEO_SINGLETON::instance ()->videoSocket;
      
      if (VIDEO_SINGLETON::instance ()->live_source) StartPlayLiveVideo();
      
      VIDEO_SINGLETON::instance ()->videoSocket = VIDEO_SINGLETON::instance ()->serviceSocket;
      
      if (VIDEO_SINGLETON::instance ()->live_source) {
	int frame = 0;
	SendPicture(&frame);
      }
      else if (VIDEO_SINGLETON::instance ()->video_format == VIDEO_MPEG1) {
	SendPacket(1, 0, 0, 0);
      }
      else {
	fprintf(stderr, "VS: VIDEO_SINGLETON::instance ()->video_format %d not supported.\n",
		VIDEO_SINGLETON::instance ()->video_format);
      }
      VIDEO_SINGLETON::instance ()->videoSocket = tmpSocket;
      
      if (VIDEO_SINGLETON::instance ()->live_source) StopPlayLiveVideo();
    }

    return 0;
    
  }
 failure:
  {
    char * msg;
    char errmsg[64];
    VIDEO_SINGLETON::instance ()->cmd = CmdFAIL;
    sprintf(errmsg, "VS failed to alloc internal buf (type %d)", failureType);
    CmdWrite((char *)&VIDEO_SINGLETON::instance ()->cmd, 1);
    msg = failureType == 1 ? "not a complete MPEG stream" :
          failureType == 2 ? "can't open MPEG file" :
          failureType == 3 ? "MPEG file is not seekable" :
          failureType == 4 ? "not an MPEG stream" :
          failureType == 5 ?
	     "too many frames in MPEG file, need change MAX_FRAMES and recompile VS" :
	  failureType == 100 ? "failed to connect to live video source" :
	  failureType == 101 ? "live MPEG2 not supported" :
          errmsg;
    write_string(VIDEO_SINGLETON::instance ()->serviceSocket, msg);
    ACE_OS::exit (0);
  }
}

#define CheckGroupRange(pnextGroup) \
{ if ((pnextGroup) < 0 || (pnextGroup) >= VIDEO_SINGLETON::instance ()->numG) \
  { fprintf(stderr, "VS: %d.VIDEO_SINGLETON::instance ()->nextGroup(%d) out of range (%d).\n", VIDEO_SINGLETON::instance ()->cmd, (pnextGroup), VIDEO_SINGLETON::instance ()->numG); \
    return 0; } }

#define CheckFrameRange(pnextFrame) \
{ if ((pnextFrame) < 0 || (pnextFrame) >= VIDEO_SINGLETON::instance ()->numF) \
  { fprintf(stderr, "VS: %d.VIDEO_SINGLETON::instance ()->nextFrame(%d) out of range (%d).\n", VIDEO_SINGLETON::instance ()->cmd, (pnextFrame), VIDEO_SINGLETON::instance ()->numF); \
    return 0; } }

int FrameToGroup(int * frame)
{
  int f = * frame;
  int i = 0;
  while (i < VIDEO_SINGLETON::instance ()->numG && VIDEO_SINGLETON::instance ()->gopTable[i].previousFrames <= f) i++;
  i --;
  * frame = f - VIDEO_SINGLETON::instance ()->gopTable[i].previousFrames;
  return i;
}

static int POSITIONvideo()
{
  int result;
  POSITIONpara para;
  /*
  fprintf(stderr, "POSITION . . .\n");
  */
  result = CmdRead((char *)&para, sizeof(para));
  if (result != 0)
    return result;

  if (VIDEO_SINGLETON::instance ()->live_source) return 0;
  
#ifdef NeedByteOrderConversion
  para.VIDEO_SINGLETON::instance ()->nextGroup = ntohl(para.VIDEO_SINGLETON::instance ()->nextGroup);
  para.sn = ntohl(para.sn);
#endif
  
  CheckGroupRange(para.nextGroup);
  VIDEO_SINGLETON::instance ()->cmdsn = para.sn;
  result = SendPacket(VIDEO_SINGLETON::instance ()->numS>1 || para.nextGroup == 0, para.nextGroup, 0, 0);
  return result;
}

static int STEPvideo()
{
  int group;
  STEPpara para;
  int tag = 0;
  int result;

  result = CmdRead((char *)&para, sizeof(para));
  if (result != 0)
    return result;
#ifdef NeedByteOrderConversion
  para.sn = ntohl(para.sn);
  para.VIDEO_SINGLETON::instance ()->nextFrame = ntohl(para.VIDEO_SINGLETON::instance ()->nextFrame);
#endif

  VIDEO_SINGLETON::instance ()->cmdsn = para.sn;

  if (!VIDEO_SINGLETON::instance ()->live_source) {
    if (para.nextFrame >= VIDEO_SINGLETON::instance ()->numF)  /* send SEQ_END */
    {
      tag = 1;
      para.nextFrame --;
    }
    /*
    fprintf(stderr, "STEP . . .frame-%d\n", para.VIDEO_SINGLETON::instance ()->nextFrame);
    */
    CheckFrameRange(para.nextFrame);
    group = FrameToGroup(&para.nextFrame);
    if (VIDEO_SINGLETON::instance ()->precmd != CmdSTEP && !tag ) {
      result = SendReferences(group, para.nextFrame);
      if (result < 0 )
        return result;
    }
  }
  if (VIDEO_SINGLETON::instance ()->live_source) StartPlayLiveVideo();
  
  if (VIDEO_SINGLETON::instance ()->live_source) {
    SendPicture(&para.nextFrame);
  }
  else if (VIDEO_SINGLETON::instance ()->video_format == VIDEO_MPEG1) {
    SendPacket(VIDEO_SINGLETON::instance ()->numS>1, group, tag ? VIDEO_SINGLETON::instance ()->numF : para.nextFrame, 0);
  }
  else {
    fprintf(stderr, "VS: wierd1\n");
  }
  
  if (VIDEO_SINGLETON::instance ()->live_source) StopPlayLiveVideo();
  return 0;
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
  int usec = VIDEO_SINGLETON::instance ()->currentUPF + VIDEO_SINGLETON::instance ()->addedUPF;
  
  if (Mpeg_Global::drift_ppm) {
    usec -= (int)((double)usec * (double)Mpeg_Global::drift_ppm / 1000000.0);
  }
  
  if (timerAdjust > 1)
    usec = (int)(((double)usec * (double)(SPEEDUP_INV_SCALE - 1)) /
		 (double)SPEEDUP_INV_SCALE);
  val3 = get_duration(preTimerVal, (val2 = get_usec()));
  /*
  if (val3 >= usec<< 1))
    fprintf(stderr, "Slower: %d out of VIDEO_SINGLETON::instance ()->currentUPF %d.\n",
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
  VIDEO_SINGLETON::instance ()->addedSignals += val2;
  
  if (VIDEO_SINGLETON::instance ()->addedSignals) {
    val2 = timerAdjust;
    if (timerAdjust < MAX_TIMER_ADJUST) {
      timerAdjust += VIDEO_SINGLETON::instance ()->addedSignals * SPEEDUP_INV_SCALE;
      if (val2 < SPEEDUP_INV_SCALE) {
	TimerSpeed();
      }
    }
    else {
      /*
      fprintf(stderr, "VS timerAdjust %d, VIDEO_SINGLETON::instance ()->addedSignals %d, timerFrame %d\n",
	      timerAdjust, VIDEO_SINGLETON::instance ()->addedSignals, timerFrame);
      */
      for (val3 = 0; val3 < VIDEO_SINGLETON::instance ()->addedSignals; val3 ++)
        TimerProcessing();
    }
    VIDEO_SINGLETON::instance ()->addedSignals = 0;
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
  if (VIDEO_SINGLETON::instance ()->cmd == CmdPLAY)
  {
    if (timerGroup == VIDEO_SINGLETON::instance ()->numG - 1 && timerFrame >= VIDEO_SINGLETON::instance ()->gopTable[timerGroup].totalFrames - 1)
    {
      timerFrame ++;  /* force sending of END_SEQ when PLAY VIDEO_SINGLETON::instance ()->cmd */
      StopTimer();
      return;
    }
    else
    {
      timerFrame ++;
      if (timerFrame >= VIDEO_SINGLETON::instance ()->gopTable[timerGroup].totalFrames)
      {
        timerGroup ++;
        timerFrame = 0;
        timerHeader = VIDEO_SINGLETON::instance ()->gopTable[timerGroup].systemHeader;
      }
    }
  }
  else {
    if (VIDEO_SINGLETON::instance ()->cmd == CmdFF) {
      if (timerGroup == VIDEO_SINGLETON::instance ()->numG - 1) {
	StopTimer();
	return;
      }
      timerGroup ++;
      timerHeader = VIDEO_SINGLETON::instance ()->gopTable[timerGroup].systemHeader;
    }
    else {
      if (timerGroup == 0) {
	StopTimer();
	return;
      }
      timerGroup --;
      timerHeader = VIDEO_SINGLETON::instance ()->gopTable[timerGroup].systemHeader;
    }
  }
}

static void StartTimer(void)
{
  VIDEO_SINGLETON::instance ()->addedUPF = 0;
  VIDEO_SINGLETON::instance ()->addedSignals = 0;
  timerAdjust = (VIDEO_SINGLETON::instance ()->VStimeAdvance * SPEEDUP_INV_SCALE) / VIDEO_SINGLETON::instance ()->currentUPF;
  /*
  SFprintf(stderr, "VS StartTimer(): fast-start frames %d\n",
	  timerAdjust / SPEEDUP_INV_SCALE);
  */
  TimerSpeed();
  setsignal(SIGALRM, timerHandler);
  timerOn = 1;
  preTimerVal = get_usec();
  /*
  fprintf(stderr, "VS: timer started at %d upf.\n", VIDEO_SINGLETON::instance ()->currentUPF + VIDEO_SINGLETON::instance ()->addedUPF);
  */
}

static void TimerSpeed(void)
{
  struct itimerval val;
  int usec = VIDEO_SINGLETON::instance ()->currentUPF + VIDEO_SINGLETON::instance ()->addedUPF;
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
	  "VS TimerSpeed() at %s speed, timerAdjust %d VIDEO_SINGLETON::instance ()->addedSignals %d.\n",
	  (timerAdjust > 1) ? "higher" : "normal", timerAdjust, VIDEO_SINGLETON::instance ()->addedSignals);
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

void GetFeedBack()
{
  VideoFeedBackPara para;
  struct itimerval val;
  int timerUsec;
  
  if (FBread((char *)&para, sizeof(para)) == -1 ||
      ntohl(para.cmdsn) != VIDEO_SINGLETON::instance ()->cmdsn) {
    /*
    SFprintf(stderr, "VS warning: a FB VIDEO_SINGLETON::instance ()->packet discarded.\n");
    */
    return;
  }
#ifdef NeedByteOrderConversion
  para.VIDEO_SINGLETON::instance ()->needHeader = ntohl(para.VIDEO_SINGLETON::instance ()->needHeader);
  para.addUsecPerFrame = ntohl(para.addUsecPerFrame);
  para.addFrames = ntohl(para.addFrames);
  para.VIDEO_SINGLETON::instance ()->sendVIDEO_SINGLETON::Instance ()->PatternGops = ntohl(para.VIDEO_SINGLETON::instance ()->sendVIDEO_SINGLETON::Instance ()->PatternGops);
  para.VIDEO_SINGLETON::instance ()->frameRateLimit1000 = ntohl(para.VIDEO_SINGLETON::instance ()->frameRateLimit1000);
#endif
  VIDEO_SINGLETON::instance ()->frameRateLimit = para.frameRateLimit1000 / 1000.0;
  VIDEO_SINGLETON::instance ()->sendPatternGops = para.sendPatternGops;
  
  if (!timerOn) return;

  VIDEO_SINGLETON::instance ()->needHeader = para.needHeader;
  ACE_OS::memcpy (VIDEO_SINGLETON::instance ()->sendPattern, para.sendPattern, PATTERN_SIZE);
  if (para.addFrames <= 0 || timerAdjust < MAX_TIMER_ADJUST)
  {
    timerAdjust += para.addFrames * SPEEDUP_INV_SCALE;
    TimerSpeed();
  }
  else /* drastic compensation for big gap */
    VIDEO_SINGLETON::instance ()->addedSignals += para.addFrames;
  if (para.addUsecPerFrame) {
    VIDEO_SINGLETON::instance ()->addedUPF += para.addUsecPerFrame;
    TimerSpeed();
  }
  /*
  SFprintf(stderr, "VS fb: addf %d, addupf %d\n",
	  para.addFrames, para.addUsecPerFrame);
  */
}

static int FastVideoPlay(void)
{
  int result;
  FFpara para;
  int preGroup = -1;
  int preHeader = -1;
  int nfds = (VIDEO_SINGLETON::instance ()->serviceSocket > VIDEO_SINGLETON::instance ()->videoSocket ? VIDEO_SINGLETON::instance ()->serviceSocket : VIDEO_SINGLETON::instance ()->videoSocket) + 1;
  
  result = CmdRead((char *)&para, sizeof(para));
  if (result != 0)
    return result;
#ifdef NeedByteOrderConversion
  para.sn = ntohl(para.sn);
  para.VIDEO_SINGLETON::instance ()->nextGroup = ntohl(para.VIDEO_SINGLETON::instance ()->nextGroup);
  para.usecPerFrame = ntohl(para.usecPerFrame);
  para.framesPerSecond = ntohl(para.framesPerSecond);
  para.VIDEO_SINGLETON::instance ()->VStimeAdvance = ntohl(para.VIDEO_SINGLETON::instance ()->VStimeAdvance);
#endif

  if (VIDEO_SINGLETON::instance ()->live_source) return 0;
  
  VIDEO_SINGLETON::instance ()->VStimeAdvance = para.VStimeAdvance;
  /*
  fprintf(stderr, "VIDEO_SINGLETON::instance ()->VStimeAdvance from client: %d\n", VIDEO_SINGLETON::instance ()->VStimeAdvance);
  */
  CheckGroupRange(para.nextGroup);
  VIDEO_SINGLETON::instance ()->cmdsn = para.sn;
  timerGroup = para.nextGroup;
  timerFrame = 0;
  timerHeader = VIDEO_SINGLETON::instance ()->gopTable[timerGroup].systemHeader;
  VIDEO_SINGLETON::instance ()->currentUPF = para.usecPerFrame;
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
		 para.usecPerFrame * VIDEO_SINGLETON::instance ()->patternSize >> 2);
      preHeader = timerHeader;
      preGroup = timerGroup;
    }

    FD_ZERO(&read_mask);
    FD_SET(VIDEO_SINGLETON::instance ()->serviceSocket, &read_mask);
    FD_SET(VIDEO_SINGLETON::instance ()->videoSocket, &read_mask);
#ifdef _HPUX_SOURCE
    if (select(nfds, (int *)&read_mask, NULL, NULL, NULL) == -1)
#else
    if (select(nfds, &read_mask, NULL, NULL, NULL) == -1)
#endif
    {
      if (errno == EINTR)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "PLAYVIDEO:select EINTR signal \n"));
          continue;
        }
     ACE_OS::perror ("Error - VS select between service and video sockets");
      ACE_OS::exit (1);
      
    }
    if (FD_ISSET(VIDEO_SINGLETON::instance ()->serviceSocket, &read_mask))   /* stop */
    {
      result = CmdRead((char *)&VIDEO_SINGLETON::instance ()->cmd, 1);
      if (result != 0)
        return result;
      if (VIDEO_SINGLETON::instance ()->cmd == CmdCLOSE) {
	exit(0);
      }
      else if (VIDEO_SINGLETON::instance ()->cmd != CmdSTOP) {
	fprintf(stderr, "VS error: VIDEO_SINGLETON::instance ()->cmd=%d while STOP is expected.\n", VIDEO_SINGLETON::instance ()->cmd);
	VIDEO_SINGLETON::instance ()->normalExit = 0;
	exit(1);
      }
      result = CmdRead((char *)&VIDEO_SINGLETON::instance ()->cmdsn, sizeof(int));
      if (result != 0 )
        return result;
#ifdef NeedByteOrderConversion
      VIDEO_SINGLETON::instance ()->cmdsn = ntohl(VIDEO_SINGLETON::instance ()->cmdsn);
#endif
      StopTimer();
      break;
    }
    if (FD_ISSET(VIDEO_SINGLETON::instance ()->videoSocket, &read_mask))  /* feedback, speed adjustment */
    {
      ACE_DEBUG((LM_DEBUG,
                 "FastVideo Play:Data socket selected"));
      GetFeedBack();
    }
    ACE_DEBUG((LM_DEBUG,
               "none of the sockets selected \n"));
  }
  return 0;
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

void ComputeFirstSendPattern(float limit)
{
  char * buf = VIDEO_SINGLETON::instance ()->firstSendPattern;
  int len = VIDEO_SINGLETON::instance ()->firstPatternSize;
  char * pat = (char *)ACE_OS::malloc(len);
  int f;

  if (pat == NULL) {
    fprintf(stderr, "VS error on allocating %d bytes for computing first SP", len);
   ACE_OS::perror ("");
    ACE_OS::exit (1);
  }
  for (f = 0; f < len; f ++) {
    pat[f] = VIDEO_SINGLETON::instance ()->frameTable[f].type;
  }
  memset(buf, 0, len);
  
  if (limit <= 0)
    limit = 1.0;
  
  f = (int)((double)len *
	    ((double)limit / (1000000.0 / (double)VIDEO_SINGLETON::instance ()->currentUPF)) + 0.5);
  /* rounded to integer, instead of truncated */
  if (f >= len)
    f = len;
  else if (f <= 1)
    f = 1;

  ComputeSendPattern(pat, buf, len, f);

  /*
  f = len - f;
  fprintf(stderr, "VIDEO_SINGLETON::instance ()->FirstVIDEO_SINGLETON::instance ()->SendVIDEO_SINGLETON::Instance ()->Pattern (%d frames dropped): ", f);
  {
    int i;
    for (i = 0; i < len; i ++)
      fputc(buf[i] ? pat[i] : '-', stderr);
  }
  fputc('\n', stderr);
  */
  ACE_OS::free (pat);
}

 int PLAYliveVideo(PLAYpara * para)
{
  int doscale;
  int count;
  int first_frame;
  int frame = para->nextFrame;
  int nfds = (VIDEO_SINGLETON::instance ()->serviceSocket > VIDEO_SINGLETON::instance ()->videoSocket ? VIDEO_SINGLETON::instance ()->serviceSocket : VIDEO_SINGLETON::instance ()->videoSocket) + 1;
  struct fd_set read_mask;
  struct timeval tval = {0, 0};
  double ratio;
  int result;
  
  VIDEO_SINGLETON::instance ()->currentUPF = (int)(1000000.0 / VIDEO_SINGLETON::instance ()->fps);  /* ignore para.usecPerFrame */
  if (VIDEO_SINGLETON::instance ()->frameRateLimit < VIDEO_SINGLETON::instance ()->fps) {
    doscale = 1;
    ratio = min(VIDEO_SINGLETON::instance ()->frameRateLimit, VIDEO_SINGLETON::instance ()->fps) / VIDEO_SINGLETON::instance ()->fps;
    first_frame = frame;
    count = 0;
    /*
    fprintf(stderr, "doscale %d, VIDEO_SINGLETON::instance ()->frameRateLimit %5.2f, VIDEO_SINGLETON::instance ()->fps %5.2f, ratio %5.2f\n",
	    doscale, VIDEO_SINGLETON::instance ()->frameRateLimit, VIDEO_SINGLETON::instance ()->fps, ratio);
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
    FD_SET(VIDEO_SINGLETON::instance ()->serviceSocket, &read_mask);
    FD_SET(VIDEO_SINGLETON::instance ()->videoSocket, &read_mask);
#ifdef _HPUX_SOURCE
    if (select(nfds, (int *)&read_mask, NULL, NULL, &tval) == -1)
#else
    if (select(nfds, &read_mask, NULL, NULL, &tval) == -1)
#endif
    {
      if (errno == EINTR)
        continue;
     ACE_OS::perror ("Error - VS select between service and video sockets");
      StopPlayLiveVideo();
      ACE_OS::exit (1);
      
    }
    if (FD_ISSET(VIDEO_SINGLETON::instance ()->serviceSocket, &read_mask))   /* stop */
    {
      unsigned char tmp;
      result = CmdRead((char *)&tmp, 1);
      if (result != 0)
        return result;
      if (tmp == CmdCLOSE) {
	StopPlayLiveVideo();
	exit(0);
      }
      else if (tmp == CmdSTOP) {
	VIDEO_SINGLETON::instance ()->cmd = tmp;
	/*
	fprintf(stderr, "VS: VIDEO_SINGLETON::Instance ()->CmdSTOP. . .\n");
	*/
        result = CmdRead((char *)&VIDEO_SINGLETON::instance ()->cmdsn, sizeof(int));
        if (result != 0)
          return result;
#ifdef NeedByteOrderConversion
	VIDEO_SINGLETON::instance ()->cmdsn = ntohl(VIDEO_SINGLETON::instance ()->cmdsn);
#endif
	StopPlayLiveVideo();
        break;
      }
      else if (tmp == CmdSPEED)
      {
	SPEEDpara para;
	/*
	fprintf(stderr, "VS: VIDEO_SINGLETON::Instance ()->CmdSPEED. . .\n");
	*/
	result = CmdRead((char *)&para, sizeof(para));
        if (result != 0)
          return result;
	/* ignore this thing for live video */
      }
      else
      {
	fprintf(stderr, "VS error(live): VIDEO_SINGLETON::instance ()->cmd=%d while expect STOP/SPEED.\n", tmp);
	VIDEO_SINGLETON::instance ()->normalExit = 0;
	StopPlayLiveVideo();
	exit(1);
      }
    }
    if (FD_ISSET(VIDEO_SINGLETON::instance ()->videoSocket, &read_mask))  /* feedback, only for frame rate
					       adjustment */
    {
      VideoFeedBackPara para;
      if (FBread((char *)&para, sizeof(para)) == -1 ||
	  ntohl(para.cmdsn) != VIDEO_SINGLETON::instance ()->cmdsn) {
	/*
	   SFprintf(stderr, "VS warning: a FB VIDEO_SINGLETON::instance ()->packet discarded.\n");
	   */
	return 0;
      }
#ifdef NeedByteOrderConversion
      para.VIDEO_SINGLETON::instance ()->frameRateLimit1000 = ntohl(para.VIDEO_SINGLETON::instance ()->frameRateLimit1000);
#endif
      VIDEO_SINGLETON::instance ()->frameRateLimit = para.frameRateLimit1000 / 1000.0;
      if (VIDEO_SINGLETON::instance ()->frameRateLimit < VIDEO_SINGLETON::instance ()->fps) {
	doscale = 1;
	ratio = min(VIDEO_SINGLETON::instance ()->frameRateLimit, VIDEO_SINGLETON::instance ()->fps) / VIDEO_SINGLETON::instance ()->fps;
	first_frame = frame;
	count = 0;
	/*
	fprintf(stderr, "doscale %d, VIDEO_SINGLETON::instance ()->frameRateLimit %5.2f, VIDEO_SINGLETON::instance ()->fps %5.2f, ratio %5.2f\n",
		doscale, VIDEO_SINGLETON::instance ()->frameRateLimit, VIDEO_SINGLETON::instance ()->fps, ratio);
	*/
      }
      else doscale = 0;
    }
  }
  return 0;
}

static int PLAYvideo()
{
  PLAYpara para;
  int preGroup = -1;
  int preHeader = -1;
  int preFrame = -1;
  int result;
  
  fprintf(stderr, "PLAY . . .\n");
  
  result = CmdRead((char *)&para, sizeof(para));
  if (result != 0)
    return result;
#ifdef NeedByteOrderConversion
  para.sn = ntohl(para.sn);
  para.VIDEO_SINGLETON::instance ()->nextFrame = ntohl(para.VIDEO_SINGLETON::instance ()->nextFrame);
  para.usecPerFrame = ntohl(para.usecPerFrame);
  para.framesPerSecond = ntohl(para.framesPerSecond);
  para.VIDEO_SINGLETON::instance ()->frameRateLimit1000 = ntohl(para.VIDEO_SINGLETON::instance ()->frameRateLimit1000);
  para.collectStat = ntohl(para.collectStat);
  para.VIDEO_SINGLETON::instance ()->sendVIDEO_SINGLETON::Instance ()->PatternGops = ntohl(para.VIDEO_SINGLETON::instance ()->sendVIDEO_SINGLETON::Instance ()->PatternGops);
  para.VIDEO_SINGLETON::instance ()->VStimeAdvance = ntohl(para.VIDEO_SINGLETON::instance ()->VStimeAdvance);
#endif

  VIDEO_SINGLETON::instance ()->frameRateLimit = para.frameRateLimit1000 / 1000.0;
  VIDEO_SINGLETON::instance ()->cmdsn = para.sn;
  VIDEO_SINGLETON::instance ()->currentUPF = para.usecPerFrame;
  VIDEO_SINGLETON::instance ()->VStimeAdvance = para.VStimeAdvance;

  {
    int ts = htonl(get_usec());
    CmdWrite((char *)&ts, sizeof(int));
  }
  
  if (VIDEO_SINGLETON::instance ()->live_source || VIDEO_SINGLETON::instance ()->video_format != VIDEO_MPEG1) {
    if (VIDEO_SINGLETON::instance ()->live_source) 
      PLAYliveVideo (&para);
    return 0;
  }
  
  
  fprintf(stderr, "VIDEO_SINGLETON::instance ()->VStimeAdvance from client: %d\n", VIDEO_SINGLETON::instance ()->VStimeAdvance);
  
  VIDEO_SINGLETON::instance ()->sendPatternGops = para.sendPatternGops;
  ComputeFirstSendPattern(VIDEO_SINGLETON::instance ()->frameRateLimit);
#ifdef STAT
  if (para.collectStat)
    memset(VIDEO_SINGLETON::instance ()->framesSent, 0, (VIDEO_SINGLETON::instance ()->numF + 7)>>3);
#endif
  CheckFrameRange(para.nextFrame);
  timerFrame = para.nextFrame;
  timerGroup = FrameToGroup(&timerFrame);
  timerHeader = VIDEO_SINGLETON::instance ()->gopTable[timerGroup].systemHeader;
  ACE_OS::memcpy (VIDEO_SINGLETON::instance ()->sendPattern, para.sendPattern, PATTERN_SIZE);
  result = SendReferences(timerGroup, timerFrame);
  if (result < 0)
    return result;
  StartTimer();
  
  fprintf (stderr, "VS Going into the for loop\n");
  for (;;)
  {
    int curGroup = timerGroup;
    int curFrame = timerFrame;
    int curHeader = timerHeader;
    char * sp;
    struct fd_set read_mask;
    int nfds = (VIDEO_SINGLETON::instance ()->serviceSocket > VIDEO_SINGLETON::instance ()->videoSocket ? VIDEO_SINGLETON::instance ()->serviceSocket : VIDEO_SINGLETON::instance ()->videoSocket) + 1;
    
    if (preGroup != curGroup || curFrame != preFrame)
    {
      int sendStatus = -1;
      int frameStep = 1;
      if (curGroup == 0)
      {
	int i = curFrame + 1;
	while (i < VIDEO_SINGLETON::instance ()->firstPatternSize && !VIDEO_SINGLETON::instance ()->firstSendPattern[i])
	{
	  frameStep ++;
	  i++;
	}
      }
      else  /* (curGroup > 0) */
      {
	int i = curFrame + 1;
	sp = VIDEO_SINGLETON::instance ()->sendPattern + ((curGroup - 1) % VIDEO_SINGLETON::instance ()->sendPatternGops) * VIDEO_SINGLETON::instance ()->patternSize;
	while (i < VIDEO_SINGLETON::instance ()->patternSize && !sp[i])
	{
	  frameStep ++;
	  i++;
	}
      }
      if (curGroup == 0)
      {
	if (VIDEO_SINGLETON::instance ()->firstSendPattern[curFrame])
	  sendStatus = 0;
	else /*  (!VIDEO_SINGLETON::instance ()->firstVIDEO_SINGLETON::instance ()->SendVIDEO_SINGLETON::Instance ()->Pattern[curFrame]) */
	{
	  int i = curFrame - 1;
	  while (i > 0 && !VIDEO_SINGLETON::instance ()->firstSendPattern[i])
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
        // Send the current video frame, calls send_to_network which
        // fragments and sends via blocking write .
	sendStatus = SendPacket(preHeader != curHeader,
				curGroup, curFrame,
				(VIDEO_SINGLETON::instance ()->currentUPF + VIDEO_SINGLETON::instance ()->addedUPF) * frameStep);
	if (!sendStatus)
	{
	  preHeader = curHeader;
	  preGroup = curGroup;
	  preFrame = curFrame;
#ifdef STAT
	  if (para.collectStat)
	  {
	    int f = VIDEO_SINGLETON::instance ()->gopTable[curGroup].previousFrames + curFrame;
	    VIDEO_SINGLETON::instance ()->framesSent[f>>3] |= (1 << (f % 8));
	  }
#endif
	}
      }
    }

    // Wait for a command on serviceSocket or Feedback on videoSocket (UDP)
    FD_ZERO(&read_mask);
    FD_SET(VIDEO_SINGLETON::instance ()->serviceSocket, &read_mask);
    FD_SET(VIDEO_SINGLETON::instance ()->videoSocket, &read_mask);
#ifdef _HPUX_SOURCE
    if (select(nfds, (int *)&read_mask, NULL, NULL, NULL) == -1)
#else
    if (select(nfds, &read_mask, NULL, NULL, NULL) == -1)
#endif
    {
      if (errno == EINTR)  /* select() interrupted */
        {
          ACE_DEBUG ((LM_DEBUG,
                      "PLAYVideo:Select interrupted coninuing..\n"));
          continue;
        }
     ACE_OS::perror ("Error - VS select between service and video sockets");
      ACE_OS::exit (1);
      
    }
    if (FD_ISSET(VIDEO_SINGLETON::instance ()->serviceSocket, &read_mask))   /* stop, speed change, loop swap */
    {
      ACE_DEBUG ((LM_DEBUG,
                  "PLAYvideo:serviceSocket selected \n"));
      unsigned char tmp;
      result = CmdRead((char *)&tmp, 1);
      if (result != 0)
        return result;
        
      if (tmp == CmdCLOSE) {
	exit(0);
      }
      else if (tmp == CmdSTOP) {
	VIDEO_SINGLETON::instance ()->cmd = tmp;
	/*
	fprintf(stderr, "VS: VIDEO_SINGLETON::Instance ()->CmdSTOP. . .\n");
	*/
        result = CmdRead((char *)&VIDEO_SINGLETON::instance ()->cmdsn, sizeof(int));
        if (result != 0)
          return result;
#ifdef NeedByteOrderConversion
	VIDEO_SINGLETON::instance ()->cmdsn = ntohl(VIDEO_SINGLETON::instance ()->cmdsn);
#endif
        StopTimer();
        break;
      }
      else if (tmp == CmdSPEED)
      {
	SPEEDpara para;
	/*
	fprintf(stderr, "VS: VIDEO_SINGLETON::Instance ()->CmdSPEED. . .\n");
	*/
	result = CmdRead((char *)&para, sizeof(para));
        if (result != 0)
          return result;
#ifdef NeedByteOrderConversion
	para.sn = ntohl(para.sn);
	para.usecPerFrame = ntohl(para.usecPerFrame);
	para.framesPerSecond = ntohl(para.framesPerSecond);
	para.sendPatternGops = ntohl(para.sendPatternGops);
	para.frameRateLimit1000 = ntohl(para.frameRateLimit1000);
#endif
	VIDEO_SINGLETON::instance ()->frameRateLimit = para.frameRateLimit1000 / 1000.0;
	VIDEO_SINGLETON::instance ()->sendPatternGops = para.sendPatternGops;
	VIDEO_SINGLETON::instance ()->currentUPF = para.usecPerFrame;
	VIDEO_SINGLETON::instance ()->addedUPF = 0;
	memcpy(VIDEO_SINGLETON::instance ()->sendPattern, para.sendPattern, PATTERN_SIZE);
	TimerSpeed();
      }
      else
      {
	fprintf(stderr, "VS error: VIDEO_SINGLETON::instance ()->cmd=%d while expect STOP/SPEED.\n", tmp);
	VIDEO_SINGLETON::instance ()->normalExit = 0;
	exit(1);
      }
    }
    if (FD_ISSET(VIDEO_SINGLETON::instance ()->videoSocket, &read_mask))  /* feedBack, speed adjustment */
    {
      ACE_DEBUG ((LM_DEBUG,
                  "PLAYVIDEO:Data SOCKET selected\n"));
      GetFeedBack();
    }
    ACE_DEBUG ((LM_DEBUG,
                "PLAYVIDEO:none selected\n"));
  }
  return 0;
}

#include <ctype.h>

static void STATstream(void)
{
  int i, j = 0;
  for (i = 0; i < VIDEO_SINGLETON::instance ()->numF; i++)
  {
    short size = htons(VIDEO_SINGLETON::instance ()->frameTable[i].size);
    char type = VIDEO_SINGLETON::instance ()->frameTable[i].type;
    if (i == VIDEO_SINGLETON::instance ()->gopTable[j].previousFrames)
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
  CmdWrite((char *)VIDEO_SINGLETON::instance ()->framesSent, (VIDEO_SINGLETON::instance ()->numF + 7) / 8);
#else
  int i;
  char zeroByte = 0;
  for (i = 0; i < (VIDEO_SINGLETON::instance ()->numF + 7) / 8; i++)
    CmdWrite((char *)&zeroByte, 1);
#endif
}

static void on_exit_routine(void)
{
  struct sockaddr_in peeraddr_in;
  int size = sizeof(peeraddr_in);
  /*
  if (!VIDEO_SINGLETON::instance ()->normalExit) {
    fprintf(stderr, "VS exitting abnormally, dump core...\n");
    ACE_OS::kill (getpid(), SIGSEGV);
    usleep(2000000);
  }
  */
  /*
  fprintf(stderr, "A VS session terminated.\n");
  */
  if (getpeername(VIDEO_SINGLETON::instance ()->serviceSocket,
		  (struct sockaddr *)&peeraddr_in, &size) == 0 &&
      peeraddr_in.sin_family == AF_INET) {
    if (strncmp(inet_ntoa(peeraddr_in.sin_addr), "129.95.50", 9)) {
      struct hostent *hp;
      time_t val =ACE_OS::time (NULL);
      char * buf = ACE_OS::ctime (&VIDEO_SINGLETON::instance ()->start_time);
      
      hp = gethostbyaddr((char *)&(peeraddr_in.sin_addr), 4, AF_INET);
      buf[strlen(buf)-1] = 0;
      printf("%s: %s %3dm%02ds %dP %s\n",
	     buf,
	     hp == NULL ? inet_ntoa(peeraddr_in.sin_addr) : hp->h_name,
	     (val - VIDEO_SINGLETON::instance ()->start_time) / 60, (val - VIDEO_SINGLETON::instance ()->start_time) % 60,
	     VIDEO_SINGLETON::instance ()->pkts_sent, VIDEO_SINGLETON::instance ()->videoFile);
    }
  }
  ComCloseConn(VIDEO_SINGLETON::instance ()->serviceSocket);
  ComCloseConn(VIDEO_SINGLETON::instance ()->videoSocket);
}

int VideoServer(int ctr_fd, int data_fd, int rttag, int max_pkt_size)
{
  int result;

  VIDEO_SINGLETON::instance ()->serviceSocket = ctr_fd;
  VIDEO_SINGLETON::instance ()->videoSocket = data_fd;
  VIDEO_SINGLETON::instance ()->conn_tag = max_pkt_size;
  
  if (max_pkt_size > 0) VIDEO_SINGLETON::instance ()->msgsize = max_pkt_size;
  else if (max_pkt_size < 0) VIDEO_SINGLETON::instance ()->msgsize = - max_pkt_size;
  else VIDEO_SINGLETON::instance ()->msgsize = 1024 * 1024;
  /*
  SFprintf(stderr, "VS VIDEO_SINGLETON::instance ()->msgsize = %d\n", VIDEO_SINGLETON::instance ()->msgsize);
  */
  VIDEO_SINGLETON::instance ()->msgsize -= sizeof(VideoMessage);
  
  VIDEO_SINGLETON::instance ()->start_time =ACE_OS::time (NULL);

  atexit(on_exit_routine);

  VIDEO_SINGLETON::instance ()->lastRef[0] = VIDEO_SINGLETON::instance ()->lastRef[1] = -1;
  VIDEO_SINGLETON::instance ()->lastRefPtr = 0;

  result = INITvideo();

  if (result != 0)
    return result;

  if (rttag) {
    if (SetRTpriority("VS", 0) == -1) rttag = 0;
  }
  
  for (;;)
  {
    
    fprintf(stderr, "VS: waiting for a new command...\n");
    
    VIDEO_SINGLETON::instance ()->precmd = VIDEO_SINGLETON::instance ()->cmd;
    result = CmdRead((char *)&VIDEO_SINGLETON::instance ()->cmd, 1);
    if (result != 0)
      {
        //        cerr << result;
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) VideoServer "),
                          result);
      }
    fprintf(stderr, "VS got VIDEO_SINGLETON::instance ()->cmd %d\n", VIDEO_SINGLETON::instance ()->cmd);
    
    switch (VIDEO_SINGLETON::instance ()->cmd)
    {
    case CmdPOSITION:
    case CmdPOSITIONrelease:
      result = POSITIONvideo();
      if (result != 0)
        return result;
      break;
    case CmdSTEP:
      result = STEPvideo();
      if (result != 0)
        return result;
      break;
    case CmdFF:
      FFvideo();
      break;
    case CmdFB:
      FBvideo();
      break;
    case CmdPLAY:
      result = PLAYvideo();
      if (result != 0)
        return result;
      break;
    case CmdCLOSE:
      /*
      fprintf(stderr, "a session closed.\n");
      VIDEO_SINGLETON::instance ()->normalExit =1;
      */
      //      ACE_OS::exit (0);
      return 0;
      break;
    case CmdSTATstream:
      STATstream();
      break;
    case CmdSTATsent:
      STATsent();
      break;
    default:
      fprintf(stderr,
	      "VS error: video channel command %d not known.\n", VIDEO_SINGLETON::instance ()->cmd);
      VIDEO_SINGLETON::instance ()->normalExit = 0;
      return -1;
      break;
    }
  }
}
