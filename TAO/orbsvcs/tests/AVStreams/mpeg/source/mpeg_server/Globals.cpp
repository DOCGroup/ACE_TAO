// $Id$

/* Copyright (c) 1995 Oregon Graduate Institute of Science and Technology
 * P.O.Box 91000-1000, Portland, OR 97291, USA;
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its 
 * documentation for any purpose is hereby granted without fee, provided that 
 * the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of O.G.I. not be used in advertising or 
 * publicity pertaining to distribution of the software without specific, 
 * written prior permission. O.G.I. makes no representations about the 
 * suitability of this software for any purpose. It is provided "as is" 
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
 *     Department of Computer Science and Engineering
 *     email: scen@cse.ogi.edu
 */

#include "Globals.h"

ACE_RCSID(mpeg_server, Globals, "$Id$")

int Mpeg_Global::parentpid = -1;
int Mpeg_Global::listenSocketIn = -1;
int Mpeg_Global::listenSocketUn = -1;
struct linger Mpeg_Global::linger = {1,1};
int Mpeg_Global::live_audio = 0;
int Mpeg_Global::live_video = 0; /* 0 - no, 1 - to open, 2 - opened */
int Mpeg_Global::drift_ppm = 0; /* clock drift in ppm */
int Mpeg_Global::session_limit = SESSION_NUM;
int Mpeg_Global::session_num = 0;
int Mpeg_Global::rttag = 0;

int Video_Timer_Global::timerHeader = 0;
int Video_Timer_Global::timerGroup = 0;
int Video_Timer_Global::timerFrame = 0;
int Video_Timer_Global::timerOn = 0;
int Video_Timer_Global::timerAdjust = 0;
int Video_Timer_Global::preTimerVal = 0;

// Initialize the nasty int's, doubles and their friends.

Video_Global::Video_Global (void)
{
  data_host = 0;
  live_source = 0;
  video_format = 0;
 
  pkts_sent = 0;
  start_time = 0;
 
  conn_tag = -1;

  normalExit = 1;

  serviceSocket = 0;
  videoSocket = -1;

  ACE_OS::memset (videoFile, 
                  0,
                  PATH_SIZE);
  fp = 0;

  needHeader = 0;

  lastRef [0] = lastRef [1] = 0;

  lastRefPtr = 0;
  currentUPF = 0;
  addedUPF = 0;
  addedSignals = 0;
  VStimeAdvance = 0;
  fps = 0; /* current frames-per-second: playback speed */
  frameRateLimit = 0;

  packet = 0;
  packetBufSize = 0;
  msgsn = 0;
  packetsn = 0;
  msgsize = 0;

  precmd = 0;
  cmd = 0;
  cmdsn = 0;
  nextFrame = 0;
  nextGroup = 0;
  firstPatternSize = 0;
  firstSendPattern = 0;
  sendPatternGops = 0;
  ACE_OS::memset (sendPattern,
                  0,
                  PATTERN_SIZE);

#ifdef STAT
  framesSent = 0;
#endif /* STAT */

  fileSize = 0;
  maxS = 0;
  maxG = 0; 
  maxI = 0;
  maxP = 0;
  maxB = 0;
  minS = 0x7fffffff;
  minG = 0x7fffffff;
  minI = 0x7fffffff;
  minP = 0x7fffffff;
  minB = 0x7fffffff;

  numS = 0;
  numG = 0;
  numF = 0;
  numI = 0;
  numP = 0;
  numB = 0;

  averageFrameSize = 0;
  horizontalSize = 0;
  verticalSize = 0;
  pelAspectRatio = 0;
  pictureRate = 0;
  vbvBufferSize = 0;
  firstGopFrames = 0;
  patternSize = 0;

  ACE_OS::memset (pattern,
                  0,
                  PATTERN_SIZE);

  // struct pointers
  systemHeader = 0;

  gopTable = 0;

  frameTable = 0;

  // playvideo local vars

  preGroup = -1;
  preHeader = -1;
  preFrame = -1;

  fast_preGroup = -1;
  fast_preHeader= -1;
}

int
Video_Global::FBread (char *buf, int size)
{
  int res;

  while ((res = (this->conn_tag >= 0 ? wait_read_bytes (this->videoSocket, buf, size) :
                 read (this->videoSocket, buf, size))) == -1)
    {
      if (errno == EINTR) {errno = 0; continue; }
      if (errno == EPIPE || errno == ECONNRESET) exit (0);
      perror ("VS reads Feedback this->packet");
      return -1;
    }

  if (res < size) 
    {
      if (res)
        // @@ Can you please convert the printfs() and perrors to use
        // the appropriate ACE_DEBUG and ACE_ERROR macros? 
        fprintf (stderr, "VS warn: FBread () res %dB < size %dB\n", res, size);
      return -1;
    }
  return 0;
}

// send a given this->packet pointed by 'this->packet' to the network.

int
Video_Global::first_packet_send_to_network (int timeToUse)
{
  int count = 0;
  VideoMessage * msghd = (VideoMessage *) (((char *) this->packet) - sizeof (VideoMessage));
  int sent = 0;
  int packetSize = ntohl (this->packet->dataBytes);

  msghd->packetsn = htonl (this->packetsn ++);
  msghd->packetSize = htonl (packetSize + sizeof (* this->packet));
  
  fprintf (stderr, "VS to send FIRST pkt %d of size %d.\n",
           ntohl (msghd->packetsn), ntohl (msghd->packetSize));
  

  {
    VideoMessage * msg = NULL;
    int size = packetSize + sizeof (* this->packet); /* msghd->this->packetSize */
    int offset = 0;
    int targetTime;
  
    if (size > this->msgsize)
      {
        if (!timeToUse)
          {
            timeToUse = (this->msgsize + sizeof (*msg) + 28) * 2;
            /*
              set the max network as 500KB.
              28 - UDP header size
            */
            /*
              fprintf (stderr, "computed timeToUse %d. ", timeToUse);
            */
          }
        else
          {
            timeToUse = (timeToUse * 7) >> 3;
            /*
              fprintf (stderr, "preset timeToUse %d.", timeToUse);
            */
            timeToUse /= (size + this->msgsize - 1) / this->msgsize;
            timeToUse = min (timeToUse, (this->msgsize + sizeof (*msg) + 28) * 100);
            /* limit min network bandwidth = 10K */
          }

      }
    while (size > 0)
      {
        int segsize, sentsize;
        int resent = 0;
   
        if (msg == NULL) 
          { /* first message for current this->packet
             */
            count = 0;
            msg = msghd;
            targetTime = get_usec ();
          }
        else {
#if 0
          /* the select () is not precise enough for being used here*/
          int sleepTime;
          targetTime += timeToUse;
          sleepTime = get_duration (get_usec (), targetTime);
          if (sleepTime >= 5000) { /* resolution of timer is 10,000 usec */
            usleep (sleepTime); /* not first message, wait for a while */
          }
#endif
          /*
            count ++;
            if (! (count % 10)) usleep (10000);
          */
          msg = (VideoMessage *) ((char *)msg + this->msgsize);
          memcpy ((char *)msg, (char *)msghd, sizeof (* msg));
        }

        msg->msgsn = htonl (this->msgsn++);
        msg->msgOffset = htonl (offset);
        msg->msgSize = htonl (min (size, this->msgsize));
        // send the header seperately first
        segsize = sizeof (*msg);
        //        ACE_DEBUG ((LM_DEBUG,
        //          "(%P|%t) Sending the header, of size %d\n",
        //          segsize));
          
        while (write (this->videoSocket, 
                      (char *)msg,
                      segsize) == -1)
          {
            if (errno == EINTR)
              continue;
            if (errno == ENOBUFS) {
              if (resent) {
                perror ("Warning, pkt discarded because");
                sent = -1;
                break;
              }
              else {
                resent = 1;
                perror ("VS to sleep 5ms");
                usleep (5000);
                continue;
              }
            }
            if (errno != EPIPE) {
              fprintf (stderr, "VS error on send this->packet %d of size %d ",
                       this->msgsn-1, min (size, this->msgsize)+sizeof (*msg));
              perror ("");
            }
            exit (errno != EPIPE);
          }


        //        segsize = min (size, this->msgsize)+sizeof (*msg);
        segsize = min (size, this->msgsize);

        if (this->conn_tag != 0) { /* this->packet stream */
          //          cerr << "vs sending " << segsize  << " on fd = " << this->videoSocket << endl;
          //          ACE_DEBUG ((LM_DEBUG,"packetsn = %d,msgsn = %d\n",
          //          msg->packetsn,msg->msgsn));

          while ((sentsize = write (this->videoSocket, 
                                    (char *)msg + sizeof (*msg), 
                                    segsize)) == -1) {
            if (errno == EINTR)
              continue;
            if (errno == ENOBUFS) {
              if (resent) {
                perror ("Warning, pkt discarded because");
                sent = -1;
                break;
              }
              else {
                resent = 1;
                perror ("VS to sleep 5ms");
                usleep (5000);
                continue;
              }
            }
            if (errno != EPIPE) {
              fprintf (stderr, "VS error on send this->packet %d of size %d ",
                       this->msgsn-1, min (size, this->msgsize)+sizeof (*msg));
              perror ("");
            }
            exit (errno != EPIPE);
          }
        }
        else {
          sentsize = wait_write_bytes (this->videoSocket, (char *)msg, segsize);
          if (sentsize == -1) {
            if (errno != EPIPE) {
              fprintf (stderr, "VS error on send this->packet %d of size %d ",
                       this->msgsn-1, min (size, this->msgsize)+sizeof (*msg));
              perror ("");
            }
            exit (errno != EPIPE);
          }
        }
        if (sentsize < segsize) {
          SFprintf (stderr, "VS warning: message size %dB, sent only %dB\n",
                    segsize, sentsize);
        }
        if (sent == -1)
          break;
        /*
          fprintf (stderr, "VS: message %d of size %d sent.\n",
          this->msgsn-1, min (size, this->msgsize)+sizeof (*msg));
        */
        size -= this->msgsize;
        offset += this->msgsize;
      }
  }
  
  fprintf (stderr, "sent = %d\n", sent);
  
  if (!sent) this->pkts_sent ++;
  return sent;
}

/*
  * send a this->packet with given this->systemHeader (optional), gop (optional) and frame.
  *
  * sh - system header id, if -1, then no system header will be sent.
  *   otherwise, only when frame == 0, the given system header will be sent.
  * gop - group of pictures, gop header will be sent when frame == 0
  *    (first I frame );
  * frame - frame to be sent, offset internal to given gop.
  */

/* returns: 0 - this->packet sent, -1 - this->packet not sent (failed) */

int
Video_Global::SendPacket (int shtag, 
                          int gop, 
                          int frame, 
                          int timeToUse, 
                          int first_time)
/* frame maybe out of range (PLAY, STEP), in this case, END_SEQ is sent
  to force display of last frame in VD */
{
  char * buf = ((char *) this->packet) + sizeof (VideoPacket);
  int f = this->gopTable[gop].previousFrames + frame;
  int sh = this->gopTable[gop].systemHeader;
  /*
    SFprintf (stderr, "VS to send this->packet gop-%d, frame-%d.\n", gop, frame);
    */

  this->packet->currentUPF = ntohl (this->currentUPF);

  if (frame >= this->gopTable[gop].totalFrames)
    {
      this->packet->cmd = htonl (this->cmd);
      this->packet->cmdsn = htonl (this->cmdsn);
      this->packet->sh = htonl (sh);
      this->packet->gop = htonl (gop);
      this->packet->frame = htonl (this->numF);
      this->packet->display = htonl (this->numF-1);
      this->packet->future = htonl ((unsigned)-1);
      this->packet->past = htonl ((unsigned)-1);
      this->packet->dataBytes = htonl (4);
      * (int*) ((char*)this->packet + sizeof (*this->packet)) = htonl (SEQ_END_CODE);

      return send_to_network (timeToUse);
    }

  if (frame)
    shtag = 0;
  else if (this->needHeader)
    {
      shtag = 1;
      this->needHeader = 0;
    }
 
  this->packet->cmd = htonl (this->cmd);
  this->packet->cmdsn = htonl (this->cmdsn);
  this->packet->sh = htonl (sh);
  this->packet->gop = htonl (gop);
  this->packet->frame = htonl (f);
  if (this->frameTable[f].type == 'B')
    {
      int pre1 = -1, pre2 = -1, i = f;
      while (i>0)
        if (this->frameTable[--i].type != 'B')
          {
            pre1 = i;
            break;
          }
      while (i>0)
        if (this->frameTable[--i].type != 'B')
          {
            pre2 = i;
            break;
          }
      if (pre2 == -1)
        {
          /*
            fprintf (stderr,
            "frame %d-%d (%d) is a B without past ref, no to be sent.\n",
            gop, frame, f);
            */
          return -1;
        }
      if (pre1 != this->lastRef[this->lastRefPtr] ||
          pre2 != this->lastRef[1 - this->lastRefPtr])
        {
          /*
            fprintf (stderr,
            "send of B frame %d gaveup for past %d/future %d ref not sent.\n",
            f, pre2, pre1);
            */
          return -1;
        }
      this->packet->display = htonl (f);
      this->packet->future = htonl (pre1);
      this->packet->past = htonl (pre2);
    }
  else
    {
      int next = f;
      int pre = f;

      while (next < this->numF && this->frameTable[++next].type == 'B');
      while (pre > 0 && this->frameTable[--pre].type == 'B');
      if (this->frameTable[f].type == 'P' && pre != this->lastRef[this->lastRefPtr])
        {
          /*
            fprintf (stderr,
            "send of P frame %d gaveup for past ref %d not sent.\n",
            f, pre);
            fprintf (stderr, "ref0=%d, ref1=%d, ptr=%d.\n",
            this->lastRef[0], this->lastRef[1], this->lastRefPtr);
            */
          return -1;
        }
      this->packet->display = htonl (next);
      this->packet->future = htonl ((unsigned)-1);
      this->packet->past = htonl (this->frameTable[f].type == 'P' ? pre : (unsigned)-1);
    }
  {
    char * ptr = buf;
    int size = 0, offset = 0, i;
    if (shtag)  /* send system header */
      {
        size = this->systemHeader[sh].size;
        FileRead (this->systemHeader[sh].offset, ptr, size);
        ptr += size;
      }
    if (!frame)  /* send gop header */
      {
        size = this->gopTable[gop].headerSize;
        FileRead (this->gopTable[gop].offset, ptr, size);
        ptr += size;
      }
    size = this->frameTable[f].size;
    for (i=this->gopTable[gop].previousFrames; i<f; i++)
      offset += this->frameTable[i].size;
    FileRead ((this->gopTable[gop].firstIoffset + offset), ptr, size);
    ptr += size;
    this->packet->dataBytes = htonl (ptr - buf);
  }

  {
    int sent;
    if (first_time == 1)
      {
        //        ACE_DEBUG ((LM_DEBUG, 
        //          "(%P|%t) Sending first frame to client\n"));
        sent = first_packet_send_to_network (timeToUse);
      }
    else
      sent = send_to_network (timeToUse);
    if (!sent)
      {
        /*
          fprintf (stderr, "%c%d\n", this->frameTable[f].type, f);
          fprintf (stderr, "%c frame %d sent.\n", this->frameTable[f].type, f);
          */
        if (this->frameTable[f].type != 'B')
          {
            this->lastRefPtr = 1 - this->lastRefPtr;
            this->lastRef[this->lastRefPtr] = f;
          }
      }
    return sent;
  }
}

int
Video_Global::CmdRead (char *buf, int psize)
{
  int res = wait_read_bytes (this->serviceSocket, 
                             buf, 
                             psize);
  if (res == 0) return (1);
  if (res == -1) {
    fprintf (stderr, "VS error on read this->cmdSocket, size %d", psize);
    perror ("");
    return (-1);
  }
  return 0;
}

int
Video_Global::CmdWrite (char *buf, int size)
{
  int res = wait_write_bytes (this->serviceSocket, buf, size);
  if (res == -1) {
    if (errno != EPIPE) perror ("VS writes to this->serviceSocket");
    return (-1);
  }
  return 0;
}

int
Video_Global::PLAYliveVideo (PLAYpara * para)
{
  int doscale;
  int count;
  int first_frame;
  int frame = para->nextFrame;
  int nfds = (this->serviceSocket > this->videoSocket ? this->serviceSocket : this->videoSocket) + 1;
  struct fd_set read_mask;
  struct timeval tval = {0, 0};
  double ratio;
  int result;
 
  this->currentUPF = (int) (1000000.0 / this->fps); /* ignore para.usecPerFrame */
  if (this->frameRateLimit < this->fps) {
    doscale = 1;
    ratio = min (this->frameRateLimit, this->fps) / this->fps;
    first_frame = frame;
    count = 0;
    /*
      fprintf (stderr, "doscale %d, this->frameRateLimit %5.2f, this->fps %5.2f, ratio %5.2f\n",
      doscale, this->frameRateLimit, this->fps, ratio);
      */
  }
  else doscale = 0;
  StartPlayLiveVideo ();

  for (;;) {

    if (doscale) {
      for (;;) {
	if ((int) ((frame - first_frame) * ratio + 0.5) < count) frame ++;
	else break;
      }
      count ++;
    }
    SendPicture (&frame);
    frame ++;
  
    FD_ZERO (&read_mask);
    FD_SET (this->serviceSocket, &read_mask);
    FD_SET (this->videoSocket, &read_mask);

    // @@ Is this code actually used anymore, i.e., do we need to
    // ACE-ify it?

#ifdef _HPUX_SOURCE
    if (select (nfds, (int *)&read_mask, NULL, NULL, &tval) == -1)
#else
      if (select (nfds, &read_mask, NULL, NULL, &tval) == -1)
#endif
        {
          if (errno == EINTR)
            continue;
          perror ("Error - VS select between service and video sockets");
          StopPlayLiveVideo ();
          exit (1);
   
        }
    if (FD_ISSET (this->serviceSocket, &read_mask))  /* stop */
      {
        u_char tmp;
        result = CmdRead ((char *)&tmp, 1);
        if (result != 0)
          return result;
        if (tmp == CmdCLOSE) {
          StopPlayLiveVideo ();
          exit (0);
        }
        else if (tmp == CmdSTOP) {
          this->cmd = tmp;
          /*
            fprintf (stderr, "VS: this->CmdSTOP. . .\n");
            */
          result = CmdRead ((char *)&this->cmdsn, sizeof (int));
          if (result != 0)
            return result;
#ifdef NeedByteOrderConversion
          this->cmdsn = ntohl (this->cmdsn);
#endif
          StopPlayLiveVideo ();
          break;
        }
        else if (tmp == CmdSPEED)
          {
            SPEEDpara speed_para;
            /*
              fprintf (stderr, "VS: this->CmdSPEED. . .\n");
              */
            result = CmdRead ((char *)&speed_para, sizeof (speed_para));
            if (result != 0)
              return result;
            /* ignore this thing for live video */
          }
        else
          {
            fprintf (stderr, "VS error (live): this->cmd=%d while expect STOP/SPEED.\n", tmp);
            this->normalExit = 0;
            StopPlayLiveVideo ();
            exit (1);
          }
      }
    if (FD_ISSET (this->videoSocket, &read_mask)) /* feedback, only for frame rate
                                                     adjustment */
      {
        VideoFeedBackPara fb_para;
        if (FBread ((char *)&fb_para, sizeof (fb_para)) == -1 ||
            ntohl (fb_para.cmdsn) != this->cmdsn) {
          /*
            SFprintf (stderr, "VS warning: a FB this->packet discarded.\n");
            */
          return 0;
        }
#ifdef NeedByteOrderConversion
        fb_para.this->frameRateLimit1000 = ntohl (fb_para.this->frameRateLimit1000);
#endif
        this->frameRateLimit = fb_para.frameRateLimit1000 / 1000.0;
        if (this->frameRateLimit < this->fps) {
          doscale = 1;
          ratio = min (this->frameRateLimit, this->fps) / this->fps;
          first_frame = frame;
          count = 0;
          /*
            fprintf (stderr, "doscale %d, this->frameRateLimit %5.2f, this->fps %5.2f, ratio %5.2f\n",
            doscale, this->frameRateLimit, this->fps, ratio);
            */
        }
        else doscale = 0;
      }
  }
  return 0;
}

void
Video_Global::ComputeFirstSendPattern (float limit)
{
  char * buf = this->firstSendPattern;
  int len = this->firstPatternSize;
  char * pat = (char *)malloc (len);
  int f;

  if (pat == NULL) {
    fprintf (stderr, "VS error on allocating %d bytes for computing first SP", len);
    perror ("");
    exit (1);
  }
  for (f = 0; f < len; f ++) {
    pat[f] = this->frameTable[f].type;
  }
  memset (buf, 0, len);
 
  if (limit <= 0)
    limit = 1.0;
 
  f = (int) ((double)len *
             ((double)limit / (1000000.0 / (double)this->currentUPF)) + 0.5);
  /* rounded to integer, instead of truncated */
  if (f >= len)
    f = len;
  else if (f <= 1)
    f = 1;

  ComputeSendPattern (pat, buf, len, f);

  /*
    f = len - f;
    fprintf (stderr, "this->Firstthis->Sendthis->Pattern (%d frames dropped): ", f);
    {
    int i;
    for (i = 0; i < len; i ++)
    fputc (buf[i] ? pat[i] : '-', stderr);
    }
    fputc ('\n', stderr);
    */
  free (pat);
}

int
Video_Global::FrameToGroup (int * frame)
{
  int f = * frame;
  int i = 0;
  while (i < this->numG && this->gopTable[i].previousFrames <= f) i++;
  i --;
  * frame = f - this->gopTable[i].previousFrames;
  return i;
}

int 
Video_Global::SendReferences (int group, int frame)
{
  u_char orgcmd;
  int i, base;
  int pregroup;
  int result;
 
  if (group < 0 || group >= this->numG) return 0;
  if (frame <= 0 || frame >= this->gopTable[group].totalFrames) return 0;
 
  orgcmd = this->cmd;
  this->cmd = CmdREF;
 
  if (group > 0) {
    pregroup = 1;
    base = this->gopTable[group].previousFrames;
    for (i = 0; i <= frame; i ++) {
      if (this->frameTable[i + base].type == 'P') {
	pregroup = 0;
	break;
      }
    }
  }
  else pregroup = 0;

  if (pregroup) { /* reference frame can be in previous group */
    pregroup = group -1;
    base = this->gopTable[pregroup].previousFrames;
    for (i = 0; i < this->gopTable[pregroup].totalFrames; i ++) {
      if (this->frameTable[i + base].type != 'B') {
	/*
          SFprintf (stderr, "REF group%d, frame%d\n", pregroup, i);
          */
	result = SendPacket (i == 0, pregroup, i, 0);
        if (result != 0)
          return result;
      }
    }
  }

  base = this->gopTable[group].previousFrames;
  for (i = 0; i < frame; i ++) {
    if (this->frameTable[i + base].type != 'B') {
      /*
        SFprintf (stderr, "REF group%d, frame%d\n", group, i);
        */
      SendPacket (i == 0, group, i, 0);
    }
  }
  this->cmd = orgcmd;
}

int
Video_Global::GetFeedBack ()
{
  VideoFeedBackPara para;
  struct itimerval val;
  int timerUsec;
 
  if (FBread ((char *)&para, sizeof (para)) == -1 ||
      ntohl (para.cmdsn) != this->cmdsn) {
    /*
      SFprintf (stderr, "VS warning: a FB this->packet discarded.\n");
      */
    return -1;
  }
#ifdef NeedByteOrderConversion
  para.this->needHeader = ntohl (para.this->needHeader);
  para.addUsecPerFrame = ntohl (para.addUsecPerFrame);
  para.addFrames = ntohl (para.addFrames);
  para.this->sendthis->PatternGops = ntohl (para.this->sendthis->PatternGops);
  para.this->frameRateLimit1000 = ntohl (para.this->frameRateLimit1000);
#endif
  this->frameRateLimit = para.frameRateLimit1000 / 1000.0;
  this->sendPatternGops = para.sendPatternGops;
 
  if (!Video_Timer_Global::timerOn) return 0;

  this->needHeader = para.needHeader;
  memcpy (this->sendPattern, para.sendPattern, PATTERN_SIZE);
  if (para.addFrames <= 0 || Video_Timer_Global::timerAdjust < MAX_TIMER_ADJUST)
    {
      Video_Timer_Global::timerAdjust += para.addFrames * SPEEDUP_INV_SCALE;
      Video_Timer_Global::TimerSpeed ();
    }
  else /* drastic compensation for big gap */
    this->addedSignals += para.addFrames;
  if (para.addUsecPerFrame) {
    this->addedUPF += para.addUsecPerFrame;
    Video_Timer_Global::TimerSpeed ();
  }
  /*
    SFprintf (stderr, "VS fb: addf %d, addupf %d\n",
    para.addFrames, para.addUsecPerFrame);
    */

  return 0;
}

int
Video_Global::SendPicture (int * frame)
{
  int size;
  char * buf = ((char *) this->packet) + sizeof (VideoPacket);
  /*
    SFprintf (stderr, "VS to send picture %d.\n", *frame);
    */

  size = ReadLiveVideoPicture (frame, buf, this->packetBufSize);
 
  this->packet->currentUPF = ntohl (this->currentUPF);
  this->packet->cmd = htonl (this->cmd);
  this->packet->cmdsn = htonl (this->cmdsn);
  this->packet->sh = this->packet->gop = this->packet->frame = this->packet->display = htonl (*frame);
  this->packet->future = htonl ((unsigned)-1);
  this->packet->past = htonl ((unsigned)-1);

  this->packet->dataBytes = htonl (size);

  return send_to_network (this->currentUPF);
}

int
Video_Global::ReadInfoFromFile (void)
{
  int fd = -1, i;
  int fnlen = strlen (this->videoFile);

  strcpy (&this->videoFile[fnlen], ".Info");
  fd = open (this->videoFile, O_RDONLY);
  if (fd == -1)
    {
      fprintf (stderr, "Reminder: VS fails to open %s for read, ", this->videoFile);
      perror ("try create one");
      goto fail_ReadInfoFromFile;
    }
  read_int (fd, &i);
  if (i != this->fileSize)
    {
      fprintf (stderr, "Warning: this->fileSize in Info: %d not the same as actual %d.\n",
               i, this->fileSize);
      goto fail_ReadInfoFromFile;
    }

  read_int (fd, &this->maxS);
  read_int (fd, &this->maxG);
  read_int (fd, &this->maxI);
  read_int (fd, &this->maxP);
  read_int (fd, &this->maxB);
  read_int (fd, &this->minS);
  read_int (fd, &this->minG);
  read_int (fd, &this->minI);
  read_int (fd, &this->minP);
  read_int (fd, &this->minB);
  read_int (fd, &this->numS);
  read_int (fd, &this->numG);
  read_int (fd, &this->numF);
  read_int (fd, &this->numI);
  read_int (fd, &this->numP);
  read_int (fd, &this->numB);
  read_int (fd, &this->averageFrameSize);
  read_int (fd, &this->horizontalSize);
  read_int (fd, &this->verticalSize);
  read_int (fd, &this->pelAspectRatio);
  read_int (fd, &this->pictureRate);
  read_int (fd, &this->vbvBufferSize);
  read_int (fd, &this->patternSize);

  memset (this->pattern, 0, PATTERN_SIZE);
  read_bytes (fd, this->pattern, this->patternSize);
#ifdef STAT
  this->framesSent = (char *)malloc ((this->numF + 7)>>3);
  if (this->framesSent == NULL)
    {
      fprintf (stderr, "Error: VS fails to alloc mem for this->framesSent for %d frames", this->numF);
      perror ("");
      exit (1);
    }
#endif
  this->systemHeader = (struct Video_Global::SystemHeader *)malloc (sizeof (struct Video_Global::SystemHeader) * this->numS);
  if (this->systemHeader == NULL)
    {
      perror ("Error: VS error on malloc this->SystemHeader");
      exit (1);
    }
  this->gopTable = (struct Video_Global::GopTable *)malloc (sizeof (struct Video_Global::GopTable) * this->numG);
  if (this->gopTable == NULL)
    {
      perror ("Error: VS error on malloc GopHeader");
      exit (1);
    }
  this->frameTable = (struct Video_Global::FrameTable *)malloc (sizeof (Video_Global::FrameTable) * this->numF);
  if (this->frameTable == NULL)
    {
      perror ("Error: VS error on malloc this->frameTable");
      exit (1);
    }
  this->packetBufSize = this->maxS + this->maxG + max (this->maxI, max (this->maxP, this->maxB));
  this->packet = (VideoPacket *)malloc (sizeof (VideoMessage) + sizeof (VideoPacket) +
                                        this->packetBufSize);
  if (this->packet == NULL)	
    {	
      perror ("Error: VS error on malloc this->packet buffer");
      exit (1);
    }
  this->packet = (VideoPacket *) ((char *)this->packet + sizeof (VideoMessage));

  for (i = 0; i < this->numS; i ++)
    {
      read_int (fd, (int *)&this->systemHeader[i].offset);
      read_int (fd, &this->systemHeader[i].size);
    }
  for (i = 0; i < this->numG; i ++)
    {
      read_int (fd, &this->gopTable[i].systemHeader);
      read_int (fd, (int *)&this->gopTable[i].offset);
      read_int (fd, &this->gopTable[i].headerSize);
      read_int (fd, &this->gopTable[i].size);
      read_int (fd, &this->gopTable[i].totalFrames);
      read_int (fd, &this->gopTable[i].previousFrames);
      read_int (fd, (int *)&this->gopTable[i].firstIoffset);
    }
  for (i = 0; i < this->numF; i ++)
    {
      read_byte (fd, &this->frameTable[i].type);
      read_short (fd, (short *)&this->frameTable[i].size);
    }

  close (fd);
  /*
    fprintf (stderr, "Read Info from %s\n", this->videoFile);
    */
  this->videoFile[fnlen] = 0;
  return 0;
fail_ReadInfoFromFile:
  if (fd >= 0)
    close (fd);
  this->videoFile[fnlen] = 0;
  /*
    fprintf (stderr, "To scan Info from %s\n", this->videoFile);
    */
  return -1;
}

void
Video_Global::WriteInfoToFile (void)
{
  int fd = -1, i;
  int fnlen = strlen (this->videoFile);

  strcpy (&this->videoFile[fnlen], ".Info");
  fd = open (this->videoFile, O_WRONLY | O_CREAT, 0444);
  if (fd == -1)
    {
      fprintf (stderr, "VS fails to open %s for write", this->videoFile);
      perror ("");
      goto fail_WriteInfoToFile;
    }
  write_int (fd, this->fileSize);
  write_int (fd, this->maxS);
  write_int (fd, this->maxG);
  write_int (fd, this->maxI);
  write_int (fd, this->maxP);
  write_int (fd, this->maxB);
  write_int (fd, this->minS);
  write_int (fd, this->minG);
  write_int (fd, this->minI);
  write_int (fd, this->minP);
  write_int (fd, this->minB);
  write_int (fd, this->numS);
  write_int (fd, this->numG);
  write_int (fd, this->numF);
  write_int (fd, this->numI);
  write_int (fd, this->numP);
  write_int (fd, this->numB);
  write_int (fd, this->averageFrameSize);
  write_int (fd, this->horizontalSize);
  write_int (fd, this->verticalSize);
  write_int (fd, this->pelAspectRatio);
  write_int (fd, this->pictureRate);
  write_int (fd, this->vbvBufferSize);
  write_int (fd, this->patternSize);

  write_bytes (fd, this->pattern, this->patternSize);
 
  for (i = 0; i < this->numS; i ++)
    {
      write_int (fd, this->systemHeader[i].offset);
      write_int (fd, this->systemHeader[i].size);
    }
  for (i = 0; i < this->numG; i ++)
    {
      write_int (fd, this->gopTable[i].systemHeader);
      write_int (fd, this->gopTable[i].offset);
      write_int (fd, this->gopTable[i].headerSize);
      write_int (fd, this->gopTable[i].size);
      write_int (fd, this->gopTable[i].totalFrames);
      write_int (fd, this->gopTable[i].previousFrames);
      write_int (fd, this->gopTable[i].firstIoffset);
    }
  for (i = 0; i < this->numF; i ++)
    {
      write_byte (fd, this->frameTable[i].type);
      write_short (fd, this->frameTable[i].size);
    }

  close (fd);
  this->videoFile[fnlen] = 0;
  return;
fail_WriteInfoToFile:
  if (fd >= 0)
    close (fd);
  this->videoFile[fnlen] = 0;
  return;
}

int
Video_Global::init_MPEG1_video_file (void)
{
  u_char nb;
  int state = 0;
  u_long fileptr = 0;
  u_long i, j, k;
  int shptr, gopptr, ftptr;
  int inpic = 0;
  u_long picptr = 0;
  int pictype = 0;
  int first = 0;
  int failureType = 0;
 
  this->fp = fopen (this->videoFile, "r");
  if (this->fp == NULL)
    {
      fprintf (stderr, "error on opening video file %s", this->videoFile);
      perror ("");
      return 2;
    }
  if (fseek (this->fp, 0, 2) == -1)
    {
      fprintf (stderr, "File %s not seekable", this->videoFile);
      perror ("");
      return 3;
    }
  this->fileSize = ftell (this->fp);
 
  fseek (this->fp, 0, 0);

  if (ReadInfoFromFile ())
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
                      fprintf (stderr, "VS error: given file is not in MPEG format.\n");
                      return 4;
                    }
                  this->numS ++;
                  break;
                case 0xb8:	/* gop_start_code */
                  this->numG ++;
                  break;
                case 0x00:	/* picture_start_code */
                  nextByte;
                  nextByte;
                  nb &= 0x38;
                  if (nb == 0x08)
                    {
                      this->numI ++;
                      if (this->numG == 2)
                        this->pattern[this->patternSize++] = 'I';
                    }
                  else if (nb == 0x10)
                    {
                      this->numP ++;
                      if (this->numG == 2)
                        this->pattern[this->patternSize++] = 'P';
                    }
                  else if (nb == 0x18)
                    {
                      this->numB ++;
                      if (this->numG == 2)
                        this->pattern[this->patternSize++] = 'B';
                    }
                  /*
                    else
                    fprintf (stderr, "VS error: unkonw picture type %d\n", nb);
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
          fprintf (stderr, "VS error: given file \"%s\" is not of MPEG format.\n", this->videoFile);
          return 4;
        }

      this->pattern[this->patternSize] = 0;
      memset (this->sendPattern, 1, PATTERN_SIZE);

      this->numF = this->numI + this->numP + this->numB;
      this->averageFrameSize = fileptr / (unsigned)this->numF;
      /*
        fprintf (stderr, "Pass one finished, total bytes read: %u, average frame size %d\n",
        fileptr, this->averageFrameSize);
        fprintf (stderr, "this->numS-%d, this->numG-%d, this->numF-%d, this->numI-%d, this->numP-%d, this->numB-%d\n",
        this->numS, this->numG, this->numI, this->numI, this->numP, this->numB);
        fprintf (stderr, "this->Pattern detected: %s\n", this->pattern);
        */
      if (this->numF > MAX_FRAMES)
        {
          fprintf (stderr, "VS error: this->Number of frames (%d) is bigger than MAX_FRAMES (%d).\n\
you need to update the constant definition in common.h and recompile.\n",
                   this->numF, MAX_FRAMES);
          return 5;
        }

#ifdef STAT
      this->framesSent = (char *)malloc ((this->numF + 7)>>3);
      if (this->framesSent == NULL)
        {
          fprintf (stderr, "VS fails to alloc mem for this->framesSent for %d frames", this->numF);
          perror ("");
          return 6;
        }
#endif

      this->systemHeader = (struct Video_Global::SystemHeader *)malloc (sizeof (struct Video_Global::SystemHeader) * this->numS);
      if (this->systemHeader == NULL)
        {
          perror ("VS error on malloc this->SystemHeader");
          return 7;
        }
      this->gopTable = (struct Video_Global::GopTable *)malloc (sizeof (struct Video_Global::GopTable) * this->numG);
      if (this->gopTable == NULL)
        {
          perror ("VS error on malloc GopHeader");
          return 8;
        }
      this->frameTable = (struct Video_Global::FrameTable *)malloc (sizeof (Video_Global::FrameTable) * this->numF);
      if (this->frameTable == NULL)
        {
          perror ("VS error on malloc this->frameTable");
          return 9;
        }

      rewind (this->fp);
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
                  if (gopptr >= 0 && this->gopTable[gopptr].size == 0)
                    this->gopTable[gopptr].size = fileptr - this->gopTable[gopptr].offset - 4;
                  computePicSize;
                  goto exit_phase2;
                  break;
                case 0xb3:	/* seq_start_code */
                  if (gopptr >= 0 && this->gopTable[gopptr].size == 0)
                    this->gopTable[gopptr].size = fileptr - this->gopTable[gopptr].offset - 4;
                  computePicSize;
                  shptr ++;
                  this->systemHeader[shptr].offset = fileptr - 4;
                  this->systemHeader[shptr].size = 0;
                  break;
                case 0xb8:	/* gop_start_code */
                  if (this->systemHeader[shptr].size == 0)
                    this->systemHeader[shptr].size =fileptr - this->systemHeader[shptr].offset - 4;
                  if (gopptr >= 0 && this->gopTable[gopptr].size == 0)
                    this->gopTable[gopptr].size = fileptr - this->gopTable[gopptr].offset - 4;
                  computePicSize;
                  gopptr ++;
                  this->gopTable[gopptr].systemHeader = shptr;
                  this->gopTable[gopptr].offset = fileptr - 4;
                  this->gopTable[gopptr].headerSize = 0;
                  this->gopTable[gopptr].size = 0;
                  this->gopTable[gopptr].totalFrames = 0;
                  this->gopTable[gopptr].previousFrames = gopptr ?
                    (this->gopTable[gopptr - 1].totalFrames + this->gopTable[gopptr - 1].previousFrames) : 0;

                  break;
                case 0x00:	/* picture_start_code */
                  if (this->gopTable[gopptr].headerSize == 0)
                    {
                      this->gopTable[gopptr].headerSize = fileptr - this->gopTable[gopptr].offset - 4;
                      this->gopTable[gopptr].firstIoffset = fileptr - 4;
                    }
                  this->gopTable[gopptr].totalFrames ++;
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
      for (shptr = 0; shptr<this->numS; shptr++)
        {
          this->maxS = max (this->maxS, this->systemHeader[shptr].size);
          this->minS = min (this->minS, this->systemHeader[shptr].size);
        }
      for (gopptr = 0; gopptr<this->numG; gopptr++)
        {
          this->maxG = max (this->maxG, this->gopTable[gopptr].headerSize);
          this->minG = min (this->minG, this->gopTable[gopptr].headerSize);
        }
      this->packetBufSize = this->maxS + this->maxG + max (this->maxI, max (this->maxP, this->maxB));
      this->packet = (VideoPacket *)malloc (sizeof (VideoMessage) + sizeof (VideoPacket) +
                                            this->packetBufSize);
      if (this->packet == NULL)
        {
          perror ("VS error on malloc this->packet buffer");
          return 10;
        }
      this->packet = (VideoPacket *) ((char *)this->packet + sizeof (VideoMessage));
      /*
        fprintf (stderr, "Pass 2 finished.\n");
        fprintf (stderr, "this->maxS-%d, this->maxG-%d, this->maxI-%d, this->maxP-%d, this->maxB-%d.\n", this->maxS, this->maxG, this->maxI, this->maxP, this->maxB);
        fprintf (stderr, "this->minS-%d, this->minG-%d, this->minI-%d, this->minP-%d, this->minB-%d.\n", this->minS, this->minG, this->minI, this->minP, this->minB);
        */
      /*
        {
        int i;

        fprintf (stderr, "id: offset  size -- system header table:\n");
        for (i=0; i<this->numS; i++)
	fprintf (stderr, "%-3d %-9u %d\n", i, this->systemHeader[i].offset, this->systemHeader[i].size);
        fprintf (stderr,
        "id: header  offset  hdsize  totSize frames  preframs Ioffset Isize -- GOP\n");
        for (i=0; i<this->numG; i++)
        {
	fprintf (stderr, "%-4d %-8d %-8u %-8d %-8d %-8d %-8d %-8u %d\n",
        i,
        this->gopTable[i].this->systemHeader,
        this->gopTable[i].offset,
        this->gopTable[i].headerSize,
        this->gopTable[i].size,
        this->gopTable[i].totalFrames,
        this->gopTable[i].previousFrames,
        this->gopTable[i].firstIoffset,
        this->frameTable[this->gopTable[i].previousFrames].size
        );
        }

        fprintf (stderr, "\nframe information:");
        for (i=0; i<this->numF; i++)
	fprintf (stderr, "%c%c%-8d", (i%10 ? '\0' : '\n'), this->frameTable[i].type, this->frameTable[i].size);
        fprintf (stderr, "\n");

        }
        */
      fseek (this->fp, this->systemHeader[0].offset+4, 0);
      nextByte;
      this->horizontalSize = ((int)nb <<4) & 0xff0;
      nextByte;
      this->horizontalSize |= (nb >>4) & 0x0f;
      this->verticalSize = ((int)nb <<8) & 0xf00;
      nextByte;
      this->verticalSize |= (int)nb & 0xff;
      nextByte;
      this->pelAspectRatio = ((int)nb >> 4) & 0x0f;
      this->pictureRate = (int)nb & 0x0f;
      nextByte;
      nextByte;
      nextByte;
      this->vbvBufferSize = ((int)nb << 5) & 0x3e0;
      nextByte;
      this->vbvBufferSize |= ((int)nb >>3) & 0x1f;
      /*
        fprintf (stderr, "SysHeader info: hsize-%d, vsize-%d, pelAspect-%d, rate-%d, vbv-%d.\n",
        this->horizontalSize, this->verticalSize, this->pelAspectRatio, this->pictureRate, this->vbvBufferSize);
        */
      WriteInfoToFile ();
    }
#if 0
  {
    int i, j = 20;
  
    for (i = this->numG - 1;; i --) {
      if (this->gopTable[i].offset < 4235260) {
	fprintf (stderr, "group %d: offset %ld\n", i, this->gopTable[i].offset);
	if (j -- == 0) break;
      }
    }
    /*
      for (i = 0; i < this->numG; i ++) {
      if (this->gopTable[i].previousFrames > 1800) {
      fprintf (stderr, "group %d: offset %ld pre-frames %d\n",
      i, this->gopTable[i].offset, this->gopTable[i].previousFrames);
      break;
      }
      }
      */
  }
#endif
  {
    this->firstPatternSize = this->gopTable[0].totalFrames;
    this->firstSendPattern = (char *)malloc (this->firstPatternSize);
    if (this->firstSendPattern == NULL)
      {
        fprintf (stderr, "VS failed to allocate this->firstthis->Sendthis->Pattern for %d frames",
                 this->firstPatternSize);
        perror ("");
        return 11;
      }
  }
  this->firstGopFrames = this->gopTable[0].totalFrames;
  return 0;
}
int
Video_Global::play_send (int debug)
{
  // ACE_DEBUG ((LM_DEBUG,"play_send: sending the frame \n"));
  int curGroup = Video_Timer_Global::timerGroup;
  int curFrame = Video_Timer_Global::timerFrame;
  int curHeader = Video_Timer_Global::timerHeader;
  char * sp;
  
  if (this->preGroup != curGroup || 
      curFrame != this->preFrame)
    {
      int sendStatus = -1;
      int frameStep = 1;
      if (debug)
        cerr << " curgroup = " << curGroup << endl ;
      if (curGroup == 0)
        {
   
          int i = curFrame + 1;
          while (i < this->firstPatternSize && 
                 !this->firstSendPattern[i])
            {
              frameStep ++;
              i++;
            }
        }
      else /* (curGroup > 0) */
        {
          int i = curFrame + 1;
          sp = this->sendPattern + ((curGroup - 1) % this->sendPatternGops) * this->patternSize;
          while (i < this->patternSize && !sp[i])
            {
              frameStep ++;
              i++;
            }
        }
      if (curGroup == 0)
        {
          if (debug)
            cerr << "first : " <<
              this->firstSendPattern[curFrame] << endl;
          if (this->firstSendPattern[curFrame])
            sendStatus = 0;
          else /*  (!this->firstthis->Sendthis->Pattern[curFrame]) */
            {
              int i = curFrame - 1;
              while (i > 0 && !this->firstSendPattern[i])
                i--;
              if (i > this->preFrame)
                /* the frame (curGroup, i) hasn't been sent yet */
                {
                  sendStatus = 0;
                  curFrame = i;
                }
              else
                sendStatus = -1;
              if (debug)
                cerr << "SendStatus = " << sendStatus << endl;
            }
        }
      else if (sp[curFrame]) /* curGroup > 0 */
	sendStatus = 0;
      else /*  (!sp[curFrame]) */
        {
          int i = curFrame - 1;
          while (i > 0 && !sp[i])
            i--;
          if (curGroup == this->preGroup && i > this->preFrame)
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
          sendStatus = this->SendPacket (this->preHeader != curHeader,
                                         curGroup, curFrame,
                                         (this->currentUPF + this->addedUPF) * frameStep);
          if (sendStatus == -1)
            return -1;
          if (!sendStatus)
            {
              this->preHeader = curHeader;
              this->preGroup = curGroup;
              this->preFrame = curFrame;
#ifdef STAT
              if (this->play_para.collectStat)
                {
                  int f = this->gopTable[curGroup].previousFrames + curFrame;
                  this->framesSent[f>>3] |= (1 << (f % 8));
                }
#endif
            }
        }
    }
  return 0;
}

int
Video_Global::fast_play_send (void)
{
 if (this->fast_preGroup != Video_Timer_Global::timerGroup)
  {
    int result;
    result = this->SendPacket (this->fast_preHeader != Video_Timer_Global::timerHeader, Video_Timer_Global::timerGroup, 0,
                               this->fast_para.usecPerFrame * this->patternSize >> 2);
    if (result == -1)
      return -1;
   this->fast_preHeader = Video_Timer_Global::timerHeader;
   this->fast_preGroup = Video_Timer_Global::timerGroup;
  }
 return 0;
}

int
Video_Global::position (void)
{
  int result;
  POSITIONpara pos_para;
  /*
    fprintf (stderr, "POSITION . . .\n");
    */
  result = CmdRead ((char *)&pos_para, sizeof (pos_para));
  if (result != 0)
    return result;

  if (this->live_source) return 0;
 
#ifdef NeedByteOrderConversion
  pos_para.nextGroup = ntohl (pos_para.nextGroup);
  pos_para.sn = ntohl (pos_para.sn);
#endif
 
  CheckGroupRange (pos_para.nextGroup);
  this->cmdsn = pos_para.sn;
  result = SendPacket (this->numS>1 || pos_para.nextGroup == 0, pos_para.nextGroup, 0, 0);
  return result;
}

int
Video_Global::step_video ()
{
  int group;
  STEPpara step_para;
  int tag = 0;
  int result;

  result = CmdRead ((char *)&step_para, sizeof (step_para));
  if (result != 0)
    return result;
#ifdef NeedByteOrderConversion
  step_para.sn = ntohl (step_para.sn);
  step_para.this->nextFrame = ntohl (step_para.this->nextFrame);
#endif

  this->cmdsn = step_para.sn;

  if (!this->live_source) {
    if (step_para.nextFrame >= this->numF) /* send SEQ_END */
      {
        tag = 1;
        step_para.nextFrame --;
      }
    /*
      fprintf (stderr, "STEP . . .frame-%d\n", step_para.this->nextFrame);
      */
    CheckFrameRange (step_para.nextFrame);
    group = FrameToGroup (&step_para.nextFrame);
    if (this->precmd != CmdSTEP && !tag ) {
      result = SendReferences (group, step_para.nextFrame);
      if (result < 0 )
        return result;
    }
  }
  if (this->live_source) StartPlayLiveVideo ();
 
  if (this->live_source) {
    SendPicture (&step_para.nextFrame);
  }
  else if (this->video_format == VIDEO_MPEG1) {
    SendPacket (this->numS>1, group, tag ? this->numF : step_para.nextFrame, 0);
  }
  else {
    fprintf (stderr, "VS: wierd1\n");
  }
 
  if (this->live_source) StopPlayLiveVideo ();
  return 0;
}

int
Video_Global::fast_forward (void)
{
  //  return this->init_fast_play ()
  return 0;
}

int
Video_Global::fast_backward (void)
{
//  return this->init_fast_play ();
  return 0;
}

int
Video_Global::stat_stream (void)
{
  int i, j = 0;
  for (i = 0; i < this->numF; i++)
    {
      short size = htons (this->frameTable[i].size);
      char type = this->frameTable[i].type;
      if (i == this->gopTable[j].previousFrames)
        {
          type = tolower (type);
          j ++;
        }
      CmdWrite ((char *)&type, 1);
      CmdWrite ((char *)&size, 2);
    }
  return 0;
}

int
Video_Global::stat_sent (void)
{
#ifdef STAT
  CmdWrite ((char *)this->framesSent, (this->numF + 7) / 8);
#else
  int i;
  char zeroByte = 0;
  for (i = 0; i < (this->numF + 7) / 8; i++)
    CmdWrite ((char *)&zeroByte, 1);
#endif
  return 0;
}

int
Video_Global::init_play (Video_Control::PLAYpara para,
                         CORBA::Long_out vts)
{
  // ~~ why do we need the play_para in Video_Global , why can't just use
  // the para that's passed.
  int result;

  ACE_DEBUG ((LM_DEBUG,
              " (%P|%t) Video_Global::init_play ()"));

  // this gets the parameters for the play command
  //  result = this->CmdRead ((char *)&this->play_para, sizeof (this->play_para));
  //  if (result != 0)
  //    return result;

  // Assign the passed play 
  this->play_para = para ;
#ifdef NeedByteOrderConversion
  this->play_para.sn = ntohl (this->play_para.sn);
  this->play_para.nextFrame = ntohl (this->play_para.nextFrame);
  this->play_para.usecPerFrame = ntohl (this->play_para.usecPerFrame);
  this->play_para.framesPerSecond = ntohl (this->play_para.framesPerSecond);
  this->play_para.frameRateLimit1000 = ntohl (this->play_para.frameRateLimit1000);
  this->play_para.collectStat = ntohl (this->play_para.collectStat);
  this->play_para.sendPatternGops = ntohl (this->play_para.sendPatternGops);
  this->play_para.VStimeAdvance = ntohl (this->play_para.VStimeAdvance);
#endif

  this->frameRateLimit = this->play_para.frameRateLimit1000 / 1000.0;
  this->cmdsn = this->play_para.sn;
  this->currentUPF = this->play_para.usecPerFrame;
  this->VStimeAdvance = this->play_para.VStimeAdvance;

  vts = get_usec ();
  //  cerr << "vts is " << vts << endl;
  // begin evil code
  //  {
    //    int vts = get_usec ();
    //    this->CmdWrite ((char *)&ts, sizeof (int));
  //  }
  // end evil code
 
  if (this->live_source || this->video_format != VIDEO_MPEG1) {
    PLAYpara live_play_para; // xxx hack to compile the code
    if (this->live_source) 
      this->PLAYliveVideo (&live_play_para);
    return 0;
  }
 
  fprintf (stderr, "this->VStimeAdvance from client: %d\n", this->VStimeAdvance);
 
  this->sendPatternGops = this->play_para.sendPatternGops;
  ComputeFirstSendPattern (this->frameRateLimit);
#ifdef STAT
  if (this->play_para.collectStat)
    memset (this->framesSent, 0, (this->numF + 7)>>3);
#endif
  CheckFrameRange (this->play_para.nextFrame);
  Video_Timer_Global::timerFrame = this->play_para.nextFrame;
  Video_Timer_Global::timerGroup = FrameToGroup (&Video_Timer_Global::timerFrame);
  Video_Timer_Global::timerHeader = this->gopTable[Video_Timer_Global::timerGroup].systemHeader;
  //  memcpy (this->sendPattern, this->play_para.sendPattern, PATTERN_SIZE);
  // Do a sequence copy..

  for (int i=0; i<PATTERN_SIZE ; i++)
    this->sendPattern[i] = this->play_para.sendPattern[i];
  result = SendReferences (Video_Timer_Global::timerGroup, Video_Timer_Global::timerFrame);
  if (result < 0)
    return result;
  Video_Timer_Global::StartTimer ();

  // Sends the first frame of the video... not true anymore since the
  // user can position the stream anywhere and then call play.
  result = play_send (0);
  return 0;
}

CORBA::Boolean
Video_Global::init_fast_play (const Video_Control::FFpara &ff_para )
{
  // save the parameters for future reference
  this->fast_para = ff_para;
  int result;
 
  //  result = CmdRead ((char *)&this->ff_para, sizeof (this->ff_para));
  // if (result != 0)
  //  return result;

  if (this->live_source) return 0;
 
  this->VStimeAdvance = ff_para.VStimeAdvance;
  /*
    fprintf (stderr, "this->VStimeAdvance from client: %d\n", this->VStimeAdvance);
    */
  CheckGroupRange (ff_para.nextGroup);
  this->cmdsn = ff_para.sn;
  Video_Timer_Global::timerGroup = ff_para.nextGroup;
  Video_Timer_Global::timerFrame = 0;
  Video_Timer_Global::timerHeader = this->gopTable[Video_Timer_Global::timerGroup].systemHeader;
  this->currentUPF = ff_para.usecPerFrame;
  Video_Timer_Global::StartTimer ();

  fast_play_send ();
  return 0;
}

int
Video_Global::init_video (void)
{
  INITvideoPara para;
  int failureType = 0;
  int result;
  /*
    fprintf (stderr, "VS about to read Para.\n");
    */
  result = CmdRead ((char *)&para, sizeof (para));
  if (result != 0)
    return result;
#ifdef NeedByteOrderConversion
  para.sn = ntohl (para.sn);
  para.version = ntohl (para.version);
  para.nameLength = ntohl (para.nameLength);
#endif
  if (para.nameLength>0)
    {
      result = CmdRead (this->videoFile, para.nameLength);
      if (result != 0)
        return result;
    }
  if (Mpeg_Global::session_num > Mpeg_Global::session_limit || para.version != VERSION) {
    char errmsg[128];
    this->cmd = CmdFAIL;
    CmdWrite ((char *)&this->cmd, 1);
    if (Mpeg_Global::session_num > Mpeg_Global::session_limit) {
      sprintf (errmsg,
               "Too many sessions being serviced, please try again later.\n");
    }
    else {
      sprintf (errmsg, "Version # not match, VS %d.%02d, Client %d.%02d",
               VERSION / 100, VERSION % 100,
               para.version / 100, para.version % 100);
    }
    write_string (this->serviceSocket, errmsg);
    exit (0);
  }
  this->cmdsn = para.sn;
  /*
    fprintf (stderr, "MPEG file %s got.\n", this->videoFile);
    */
  this->videoFile[para.nameLength] = 0;

  if (!strncasecmp ("LiveVideo", this->videoFile, 9)) {
    if (OpenLiveVideo (&this->video_format, &this->horizontalSize,
                       &this->verticalSize, &this->averageFrameSize,
                       &this->fps, &this->pelAspectRatio) == -1) {
      failureType = 100;
      goto failure;
    }
    if (this->video_format == VIDEO_MPEG2) {
      failureType = 101;
      goto failure;
    }
    this->live_source = 1;

    this->fileSize =0x7fffffff;
    this->maxS = this->maxG = this->maxI = this->maxP = this->maxB = this->minS = this->minG = this->minI = this->minP = this->minB = 1;
    this->numS = this->numG = this->numF = this->numI = 0x7fffffff;
    this->numP = this->numB = 0;
    this->vbvBufferSize = 1;
    this->firstGopFrames = 1;
    this->patternSize = 1;
    this->pattern[0] = 'I';
    this->pattern[1] = 0;
    this->packetBufSize = this->verticalSize * this->horizontalSize * 3;
    this->packet = (VideoPacket *)malloc (sizeof (VideoMessage) + sizeof (VideoPacket) +
                                          this->packetBufSize);
    if (this->packet == NULL)	
      {
        perror ("Error: VS error on malloc this->packet buffer");
        exit (1);
      }
    this->packet = (VideoPacket *) ((char *)this->packet + sizeof (VideoMessage));

  }
  else {
    static double pictureRateTable[] = {23.976, 24, 25, 29.97, 30, 50, 59.94, 60};

    this->video_format = VIDEO_MPEG1;
    failureType = init_MPEG1_video_file ();
    if (failureType) goto failure;
    this->fps = pictureRateTable[this->pictureRate - 1];
  }
 
  {
    INITvideoReply reply;

    reply.totalHeaders = htonl (this->numS);
    reply.totalGroups = htonl (this->numG);
    reply.totalFrames = htonl (this->numF);
    reply.sizeIFrame = htonl (this->maxI);
    reply.sizePFrame = htonl (this->maxP);
    reply.sizeBFrame = htonl (this->maxB);
    reply.sizeSystemHeader = htonl (this->maxS);
    reply.sizeGop = htonl (this->maxG);
    reply.averageFrameSize = htonl (this->averageFrameSize);
    reply.verticalSize = htonl (this->verticalSize);
    reply.horizontalSize = htonl (this->horizontalSize);
    reply.pelAspectRatio = htonl (this->pelAspectRatio);
    reply.pictureRate1000 = htonl ((int) (this->fps * 1000));
    reply.vbvBufferSize = htonl (this->vbvBufferSize);
    reply.firstGopFrames = htonl (this->firstGopFrames);
    reply.patternSize = htonl (this->patternSize);
    strncpy (reply.pattern, this->pattern, PATTERN_SIZE);
  
    reply.live = htonl (this->live_source);
    reply.format = htonl (this->video_format);
  
    CmdWrite ((char *)&this->cmd, 1);
  
    CmdWrite ((char *)&reply, sizeof (reply));

    /* write the first SH, GOP and IFrame to this->serviceSocket (TCP),
       using code for SendPacket () */
    {
      int tmpSocket = this->videoSocket;
   
      if (this->live_source) StartPlayLiveVideo ();
   
      this->videoSocket = this->serviceSocket;
   
      if (this->live_source) {
	int frame = 0;
	SendPicture (&frame);
      }
      else if (this->video_format == VIDEO_MPEG1) {
	SendPacket (1, 0, 0, 0);
      }
      else {
	fprintf (stderr, "VS: this->video_format %d not supported.\n",
                 this->video_format);
      }
      this->videoSocket = tmpSocket;
   
      if (this->live_source) StopPlayLiveVideo ();
    }

    return 0;
  
  }
failure:
  {
    char * msg;
    char errmsg[64];
    this->cmd = CmdFAIL;
    sprintf (errmsg, "VS failed to alloc internal buf (type %d)", failureType);
    CmdWrite ((char *)&this->cmd, 1);
    msg = failureType == 1 ? "not a complete MPEG stream" :
      failureType == 2 ? "can't open MPEG file" :
      failureType == 3 ? "MPEG file is not seekable" :
      failureType == 4 ? "not an MPEG stream" :
      failureType == 5 ?
      "too many frames in MPEG file, need change MAX_FRAMES and recompile VS" :
      failureType == 100 ? "failed to connect to live video source" :
      failureType == 101 ? "live MPEG2 not supported" :
      errmsg;
    write_string (this->serviceSocket, msg);
    exit (0);
  }
}

//--------------------------------------------------------
// Video_Timer_Global methods
void
Video_Timer_Global::StartTimer (void)
{
  VIDEO_SINGLETON::instance ()->addedUPF = 0;
  VIDEO_SINGLETON::instance ()->addedSignals = 0;
  timerAdjust = (VIDEO_SINGLETON::instance ()->VStimeAdvance * SPEEDUP_INV_SCALE) / VIDEO_SINGLETON::instance ()->currentUPF;
  /*
    SFprintf (stderr, "VS StartTimer (): fast-start frames %d\n",
    timerAdjust / SPEEDUP_INV_SCALE);
    */
  TimerSpeed ();
  // setsignal (SIGALRM, timerHandler);
  timerOn = 1;
  preTimerVal = get_usec ();
  /*
    fprintf (stderr, "VS: timer started at %d upf.\n", VIDEO_SINGLETON::instance ()->currentUPF + VIDEO_SINGLETON::instance ()->addedUPF);
    */
}

void
Video_Timer_Global::StopTimer (void)
{
  struct itimerval val;
  // ## I have to incorporate this logic into the changed code 
  // setsignal (SIGALRM, SIG_IGN);
  val.it_interval.tv_sec = val.it_value.tv_sec = 0;
  val.it_interval.tv_usec = val.it_value.tv_usec = 0;
  setitimer (ITIMER_REAL, &val, NULL);
  timerOn = 0;
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Video_Timer_Global::StopTimer: timer stopped\n"));
}

void
Video_Timer_Global::TimerSpeed (void)
{
  struct itimerval val;
  int usec = VIDEO_SINGLETON::instance ()->currentUPF + VIDEO_SINGLETON::instance ()->addedUPF;
  if (Mpeg_Global::drift_ppm) {
    /*
      int drift = (double)usec * (double)Mpeg_Global::drift_ppm / 1000000.0;
      SFprintf (stderr, "Mpeg_Global::drift_ppm %d, usec %d, drift %d, new usec %d\n",
      Mpeg_Global::drift_ppm, usec, drift, usec - drift);
      */
    usec -= (int) ((double)usec * (double)Mpeg_Global::drift_ppm / 1000000.0);
  }
  if (timerAdjust > 1)
    usec = (int) (((double)usec * (double) (SPEEDUP_INV_SCALE - 1)) /
                  (double)SPEEDUP_INV_SCALE);
  val.it_interval.tv_sec = val.it_value.tv_sec = usec / 1000000;
  val.it_interval.tv_usec = val.it_value.tv_usec = usec % 1000000;
  setitimer (ITIMER_REAL, &val, NULL);
  /*
    SFprintf (stderr,
    "VS TimerSpeed () at %s speed, timerAdjust %d VIDEO_SINGLETON::instance ()->addedSignals %d.\n",
    (timerAdjust > 1) ? "higher" : "normal", timerAdjust, VIDEO_SINGLETON::instance ()->addedSignals);
    */

}

void
Video_Timer_Global::TimerProcessing (void)
{
  /* 
     fprintf (stderr, "VS: timerHandler...\n");
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
        TimerSpeed ();
    }
  if (VIDEO_SINGLETON::instance ()->cmd == CmdPLAY)
    {
      if (timerGroup == VIDEO_SINGLETON::instance ()->numG - 1 && timerFrame >= VIDEO_SINGLETON::instance ()->gopTable[timerGroup].totalFrames - 1)
        {
          timerFrame ++; /* force sending of END_SEQ when PLAY VIDEO_SINGLETON::instance ()->cmd */
          StopTimer ();
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
	StopTimer ();
	return;
      }
      timerGroup ++;
      timerHeader = VIDEO_SINGLETON::instance ()->gopTable[timerGroup].systemHeader;
    }
    else {
      if (timerGroup == 0) {
	StopTimer ();
	return;
      }
      timerGroup --;
      timerHeader = VIDEO_SINGLETON::instance ()->gopTable[timerGroup].systemHeader;
    }
  }

}

void
Video_Timer_Global::timerHandler (int sig)
{
  // ACE_DEBUG ((LM_DEBUG,
  //       "Video_Timer_Global::timerHandler\n"));

  int val2, val3;
  int usec = VIDEO_SINGLETON::instance ()->currentUPF + VIDEO_SINGLETON::instance ()->addedUPF;
 
  if (Mpeg_Global::drift_ppm) {
    usec -= (int) ((double)usec * (double)Mpeg_Global::drift_ppm / 1000000.0);
  }
 
  if (timerAdjust > 1)
    usec = (int) (((double)usec * (double) (SPEEDUP_INV_SCALE - 1)) /
                  (double)SPEEDUP_INV_SCALE);
  val3 = get_duration (preTimerVal, (val2 = get_usec ()));
  /*
    if (val3 >= usec<< 1))
    fprintf (stderr, "Slower: %d out of VIDEO_SINGLETON::instance ()->currentUPF %d.\n",
    val3, usec);
    else
    fprintf (stderr, "+\n");
    */
  preTimerVal = val2;
  if (val3 < 0 || val3 > 100000000)
    val3 = usec;
  val2 = (val3 + (usec>>1)) / usec;
  if (val2 < 0) val2 = 0;
  if (val2) {
    TimerProcessing ();
    val2 --;
  }
  VIDEO_SINGLETON::instance ()->addedSignals += val2;
 
  if (VIDEO_SINGLETON::instance ()->addedSignals) {
    val2 = timerAdjust;
    if (timerAdjust < MAX_TIMER_ADJUST) {
      timerAdjust += VIDEO_SINGLETON::instance ()->addedSignals * SPEEDUP_INV_SCALE;
      if (val2 < SPEEDUP_INV_SCALE) {
	TimerSpeed ();
      }
    }
    else {
      /*
        fprintf (stderr, "VS timerAdjust %d, VIDEO_SINGLETON::instance ()->addedSignals %d, timerFrame %d\n",
        timerAdjust, VIDEO_SINGLETON::instance ()->addedSignals, timerFrame);
        */
      for (val3 = 0; val3 < VIDEO_SINGLETON::instance ()->addedSignals; val3 ++)
        TimerProcessing ();
    }
    VIDEO_SINGLETON::instance ()->addedSignals = 0;
  }
}

// send the first packet, given by packet pointed by
// 'this->packet' to the network.
int
Video_Global::send_to_network (int timeToUse)
{
  int count = 0;
  VideoMessage * msghd = (VideoMessage *) (((char *) this->packet) - sizeof (VideoMessage));
  int sent = 0;
  int packetSize = ntohl (this->packet->dataBytes);

  msghd->packetsn = htonl (this->packetsn ++);
  msghd->packetSize = htonl (packetSize + sizeof (* this->packet));
  
  //  fprintf (stderr, "VS to send pkt %d of size %d.\n",
  //           ntohl (msghd->packetsn), ntohl (msghd->packetSize));
  

  {
    VideoMessage * msg = NULL;
    int size = packetSize + sizeof (* this->packet); /* msghd->this->packetSize */
    int offset = 0;
    int targetTime;
  
    if (size > this->msgsize)
      {
        if (!timeToUse)
          {
            timeToUse = (this->msgsize + sizeof (*msg) + 28) * 2;
            /*
              set the max network as 500KB.
              28 - UDP header size
              */
            /*
              fprintf (stderr, "computed timeToUse %d. ", timeToUse);
              */
          }
        else
          {
            timeToUse = (timeToUse * 7) >> 3;
            /*
              fprintf (stderr, "preset timeToUse %d.", timeToUse);
              */
            timeToUse /= (size + this->msgsize - 1) / this->msgsize;
            timeToUse = min (timeToUse, (this->msgsize + sizeof (*msg) + 28) * 100);
            /* limit min network bandwidth = 10K */
          }

      }
    while (size > 0)
      {
        int segsize, sentsize;
        int resent = 0;
   
        if (msg == NULL) { /* first message for current this->packet */
          count = 0;
          msg = msghd;
          targetTime = get_usec ();
        }
        else {
#if 0
          /* the select () is not precise enough for being used here*/
          int sleepTime;
          targetTime += timeToUse;
          sleepTime = get_duration (get_usec (), targetTime);
          if (sleepTime >= 5000) { /* resolution of timer is 10,000 usec */
            usleep (sleepTime); /* not first message, wait for a while */
          }
#endif
          /*
            count ++;
            if (! (count % 10)) usleep (10000);
            */
          msg = (VideoMessage *) ((char *)msg + this->msgsize);
          memcpy ((char *)msg, (char *)msghd, sizeof (* msg));
        }
        msg->msgsn = htonl (this->msgsn++);
        msg->msgOffset = htonl (offset);
        msg->msgSize = htonl (min (size, this->msgsize));

        segsize = min (size, this->msgsize)+sizeof (*msg);
        if (this->conn_tag != 0) { /* this->packet stream */
          //          cerr << "sending " << segsize  << " on fd = " << this->videoSocket << endl;
          while ((sentsize = write (this->videoSocket, (char *)msg, segsize)) == -1) {
            if (errno == EINTR)
              continue;
            if (errno == ENOBUFS) {
              if (resent) {
                perror ("Warning, pkt discarded because");
                sent = -1;
                break;
              }
              else {
                resent = 1;
                perror ("VS to sleep 5ms");
                usleep (5000);
                continue;
              }
            }
            if (errno != EPIPE) {
              fprintf (stderr, "VS error on send this->packet %d of size %d ",
                       this->msgsn-1, min (size, this->msgsize)+sizeof (*msg));
              perror ("");
            }
            exit (errno != EPIPE);
          }
        }
        else {
          sentsize = wait_write_bytes (this->videoSocket, (char *)msg, segsize);
          if (sentsize == -1) {
            if (errno != EPIPE) {
              fprintf (stderr, "VS error on send this->packet %d of size %d ",
                       this->msgsn-1, min (size, this->msgsize)+sizeof (*msg));
              perror ("");
            }
            exit (errno != EPIPE);
          }
        }
        if (sentsize < segsize) {
          SFprintf (stderr, "VS warning: message size %dB, sent only %dB\n",
                    segsize, sentsize);
        }
        if (sent == -1)
          break;
        /*
          fprintf (stderr, "VS: message %d of size %d sent.\n",
          this->msgsn-1, min (size, this->msgsize)+sizeof (*msg));
          */
        size -= this->msgsize;
        offset += this->msgsize;
      }
  }
  /*
    fprintf (stderr, "sent = %d\n", sent);
    */
  if (!sent) this->pkts_sent ++;
  return sent;
}


Audio_Global::Audio_Global (void)
  :state (AUDIO_WAITING),
   addSamples (0),
   nextTime (0),
   upp (0),
   delta_sps (0),
   bytes_sent (0),
   start_time (0),
   conn_tag (0),
   serviceSocket (-1),
   audioSocket (-1),
   fd (0),
   totalSamples (0),
   fileSize (0),
   cmd (0),
   live_source (0),
   databuf_size (0),
   cmdsn (0),
   nextsample (0),
   sps (0),
   spslimit (0),
   spp (0),
   pktbuf (0),
   fbpara (0)
{
}

int 
Audio_Global::CmdRead(char *buf, int psize)
{
  int res = wait_read_bytes(serviceSocket, buf, psize);
  if (res == 0) return (1);
  if (res == -1) {
    fprintf(stderr, "AS error on read cmdSocket, size %d", psize);
    perror("");
    return (-1);
  }
  return 0;
}

void
Audio_Global::CmdWrite(char *buf, int size)
{
  int res = wait_write_bytes(serviceSocket, buf, size);
  if (res == -1) {
    if (errno != EPIPE) perror("AS writes to serviceSocket");
    exit(errno != EPIPE);
  }
}

int 
Audio_Global::INITaudio(void)
{
  int result;
  int failureType; /* 0 - can't open file, 1 - can't open live source */
  INITaudioPara para;

  result = CmdRead((char *)&para, sizeof(para));
  if (result != 0)
    return result;
#ifdef NeedByteOrderConversion
  para.sn = ntohl(para.sn);
  para.version = ntohl(para.version);
  para.nameLength = ntohl(para.nameLength);
  para.para.encodeType = ntohl(para.para.encodeType);
  para.para.channels = ntohl(para.para.channels);
  para.para.samplesPerSecond = ntohl(para.para.samplesPerSecond);
  para.para.bytesPerSample = ntohl(para.para.bytesPerSample);
#endif
  if (para.nameLength>0)
    result = CmdRead(audioFile, para.nameLength);
  if (result != 0)
    return result;
  if (Mpeg_Global::session_num > Mpeg_Global::session_limit || para.version != VERSION) {
    char errmsg[128];
    cmd = CmdFAIL;
    CmdWrite((char *)&cmd, 1);
    if (Mpeg_Global::session_num > Mpeg_Global::session_limit) {
      sprintf(errmsg,
	      "Too many sessions being serviced, please try again later.\n");
    }
    else {
      sprintf(errmsg, "Version # not match, AS %d.%02d, Client %d.%02d",
	      VERSION / 100, VERSION % 100,
	      para.version / 100, para.version % 100);
    }
    write_string(serviceSocket, errmsg);
    return(1);
  }
  memcpy(&audioPara, &para.para, sizeof(audioPara));
  /*
  fprintf(stderr, "Client Audio para: encode %d, ch %d, sps %d, bps %d.\n",
	  para.para.encodeType, para.para.channels,
	  para.para.samplesPerSecond, para.para.bytesPerSample);
   */
  audioFile[para.nameLength] = 0;
  {
    int len = strlen(audioFile);
    if (strncasecmp("LiveAudio", audioFile, 9) &&
	strcasecmp(".au", audioFile+len-3)) {
      char errmsg[128];
      cmd = CmdFAIL;
      CmdWrite((char *)&cmd, 1);
      sprintf(errmsg, "%s without suffix .au", audioFile);
      write_string(serviceSocket, errmsg);
      return(1);
    }
  }
  /*
  fprintf(stderr, "Audio file %s got.\n", audioFile);
  */

  if (!strncasecmp("LiveAudio", audioFile, 9)) {
    fd = OpenLiveAudio(&(para.para));
    if (fd == -1) {
      failureType = 1;
      goto failure;
    }
    fileSize =0x7fffffff;
    totalSamples = fileSize / audioPara.bytesPerSample;
    live_source = 1;
  }
  else {
    LeaveLiveAudio();
    fd = open(audioFile, O_RDONLY);
    if (fd == -1)
    {
      fprintf(stderr, "AS error on opening audio file %s", audioFile);
      perror("");
      failureType = 0;
      goto failure;
    }

    /* Try to get audioFile format audioPara here */

    /* figure out totalsamples */
    fileSize = lseek(fd, 0L, SEEK_END);
    lseek(fd, 0L, SEEK_SET);
    totalSamples = fileSize / audioPara.bytesPerSample;
    /*
    fprintf(stderr, "Total Samples=%d in audio file %ss.\n", totalSamples, audioFile);
    */
  }
  {
    INITaudioReply reply;
    
    reply.para.encodeType = htonl(audioPara.encodeType);
    reply.para.channels = htonl(audioPara.channels);
    reply.para.samplesPerSecond = htonl(audioPara.samplesPerSecond);
    reply.para.bytesPerSample = htonl(audioPara.bytesPerSample);
    reply.totalSamples = htonl(totalSamples);

    reply.live = htonl(live_source);
    reply.format = htonl(AUDIO_RAW);
    
    CmdWrite((char *)&cmd, 1);
    CmdWrite((char *)&reply, sizeof(reply));
  }
  return 0;
  
 failure:
  {
    /*
    fprintf(stderr, "AS error: failed initializing audio file.\n");
    */
    cmd = CmdFAIL;
    CmdWrite((char *)&cmd, 1);
    write_string(serviceSocket,
		 failureType == 0 ? "Failed to open audio file for read." :
		 "Failed to connect to live audio source.");
    return(1);
  }
}

/* send a packet of audio samples to audioSocket
   returns: 0 - no more data from audio file: EOF reached;
            1 - More data is available from the audio file */
int
Audio_Global::send_packet (int firstSample, int samples)
{
  //  ACE_DEBUG ((LM_DEBUG,"(%P|%t) send_packet called\n"));
  long offset = firstSample * audioPara.bytesPerSample;
  int size = samples * audioPara.bytesPerSample;
  char * buf = (char *)pktbuf + sizeof(*pktbuf);
  int len;
  int resent = 0;
  int segsize, sentsize;

  if (live_source) {
    len = ReadLiveAudioSamples(buf, samples);
    len *= audioPara.bytesPerSample;
  }
  else {
    lseek(fd, offset, SEEK_SET);
    while ((len = read(fd, buf, size)) == -1) {
      if (errno == EINTR)
	continue;   /* interrupted */
      perror("AS error on read audio file");
      return(-1);
    }
    if (len < audioPara.bytesPerSample) {
      return 0;
    }
  }
  
  samples = len / audioPara.bytesPerSample;
  len = samples * audioPara.bytesPerSample;
  bytes_sent += len;
  pktbuf->firstSample = htonl(firstSample);
  pktbuf->samples = htonl(samples);
  pktbuf->actualSamples = htonl(samples);
  pktbuf->dataBytes = htonl(len);
  if (spslimit < sps) { /* interpolation needed */
    SFprintf(stderr, "AS audio sample interpolation not available yet.\n");
  }
  segsize = sizeof(*pktbuf) + len;
  if (conn_tag != 0) {
    while ((sentsize = write(audioSocket, (char *)pktbuf, segsize)) == -1) {
      if (errno == EINTR) /* interrupted */
	continue;
      if (errno == ENOBUFS) {
	if (resent) {
	  perror("AS Warning, pkt discarded because");
	  break;
	}
	else {
	  resent = 1;
	  usleep(5000);
	  continue;
	}
      }
      if (errno != EPIPE) {
	fprintf(stderr, "AS error on send audio packet %d(%d):",
		firstSample, samples);
	perror("");
      }
      exit((errno != EPIPE));
    }
  }
  else {
    sentsize = wait_write_bytes(audioSocket, (char *)pktbuf, segsize);
    if (sentsize == -1) {
      if (errno != EPIPE) {
	fprintf(stderr, "AS error on send audio packet %d(%d):",
		firstSample, samples);
	perror("");
      }
      exit((errno != EPIPE));
     }
  }
  if (sentsize < segsize) {
    SFprintf(stderr, "AS warning: message size %dB, sent only %dB\n",
	    segsize, sentsize);
  }
  /*
  SFprintf(stderr, "AS sent audio packet %d(%d).\n",
	  firstSample, samples);
  */
  return (len < size ? 0 : 1);
}

/* send a packet of audio samples to audioSocket
   returns: 0 - no more data from audio file: EOF reached;
            1 - More data is available from the audio file */
int
Audio_Global::SendPacket (void)
{
  int moredata;
  pktbuf->cmdsn = htonl(cmdsn);
  pktbuf->resend = htonl(0);
  pktbuf->samplesPerSecond = htonl(sps);
  moredata = send_packet(nextsample, spp);
  if (moredata) 
    {
      nextsample += spp;
    }
  return moredata;
}

void
Audio_Global::ResendPacket (int firstsample, int samples)
{
  pktbuf->cmdsn = htonl(cmdsn);
  pktbuf->resend = htonl(1);
  pktbuf->samplesPerSecond = htonl(sps);
  while (samples > 0) {
    int size = samples < spp ? samples : spp;
    send_packet(firstsample, size);
    firstsample += size;
    samples -= size;
    if (samples > 0) {
      usleep(10000);
    }
  }
}

#if 0
int
Audio_Global::PLAYaudio(void)
{
  int hasdata = 1;
  int addSamples;
  int packets = 0;
  unsigned nextTime;
  int upp;  /* micro-seconds per packet */
  int delta_sps = 0;  /* compensation for sps from feedback msgs */
  int nfds = (serviceSocket > audioSocket ? serviceSocket : audioSocket) + 1;
  int result;
  /*
  fprintf(stderr, "PLAY . . .\n");
  */
  {
    PLAYaudioPara para;
    result = CmdRead((char *)&para, sizeof(para));
    if (result != 0)
      return result;
#ifdef NeedByteOrderConversion
    para.sn = ntohl(para.sn);
    para.nextSample = ntohl(para.nextSample);
    para.samplesPerSecond = ntohl(para.samplesPerSecond);
    para.samplesPerPacket = ntohl(para.samplesPerPacket);
    para.ABsamples = ntohl(para.ABsamples);
    para.spslimit = ntohl(para.spslimit);
#endif
    nextsample = para.nextSample;
    cmdsn = para.sn;
    sps = para.samplesPerSecond;
    spslimit = para.spslimit;
    spp = para.samplesPerPacket;
    addSamples = para.ABsamples / 2;
    if (spp * audioPara.bytesPerSample > databuf_size) {
      spp = databuf_size / audioPara.bytesPerSample;
    }
    /*
    SFprintf(stderr, "AS got CmdPLAY: sps %d\n", sps);
    */
  }
  /*
  fprintf(stderr, "AS: nextSampe=%d for PLAY.\n", para.nextSample);
  */

  upp = (int)(1000000.0 / ((double)sps / (double)spp));
  nextTime = get_usec();
  
  CmdWrite((char *)&nextTime, sizeof(int));

  if (live_source) {
    StartPlayLiveAudio();
  }
  
  for (;;)
  {
    struct fd_set read_mask, write_mask;
    struct timeval tval;
    unsigned curTime = get_usec();
    
    if (hasdata) {
      if (addSamples < - spp) {  /* slow down by not sending packets */
	nextTime += upp;
	addSamples += spp;
      }
      else {
	int need_sleep = 0;
	while (nextTime <= curTime && hasdata) {
	  if (need_sleep) usleep(5000);
	  hasdata = SendPacket();
	  need_sleep = 1;
	  packets ++;
	  nextTime += upp;
	  if (addSamples > 0 && packets % SPEEDUP_SCALE == 0) {
	    addSamples -= spp;
	    usleep(5000);
	    hasdata = SendPacket();
	    packets ++;
	  }
	}
      }
    }
    curTime = nextTime - curTime;
    if (curTime > 5000000) curTime = 5000000; /* limit on 5 second weit time
						 in case error happens */
    tval.tv_sec = curTime / 1000000;
    tval.tv_usec = curTime % 1000000;
    FD_ZERO(&read_mask);
    FD_SET(serviceSocket, &read_mask);
    FD_SET(audioSocket, &read_mask);
#ifdef _HPUX_SOURCE
    if (select(nfds, (int *)&read_mask, NULL, NULL, hasdata ? &tval : NULL) == -1)
#else
    if (select(nfds, &read_mask, NULL, NULL, hasdata ? &tval : NULL) == -1)
#endif
    {
      if (errno == EINTR)
        continue;
      perror("AS error on select reading or writing");
      return(-1);
    }
    if (FD_ISSET(serviceSocket, &read_mask)){  /* STOP, SPEED, or CLOSE*/
      unsigned char tmp;
      result = CmdRead((char *)&tmp, 1);
      if (result != 0)
        return result;
      switch (tmp)
      {
      case CmdSPEED:
	{
	  SPEEDaudioPara para;
	  result = CmdRead((char *)&para, sizeof(para));
          if (result != 0)
            return result;
#ifdef NeedByteOrderConversion
	  para.sn = ntohl(para.sn);
	  para.samplesPerSecond = ntohl(para.samplesPerSecond);
	  para.samplesPerPacket = ntohl(para.samplesPerPacket);
	  para.spslimit = ntohl(para.spslimit);
#endif
	  sps  = para.samplesPerSecond;
	  spslimit = para.spslimit;
	  spp = para.samplesPerPacket;
	  if (spp * audioPara.bytesPerSample > databuf_size) {
	    spp = databuf_size / audioPara.bytesPerSample;
	  }
	  delta_sps = 0;  /* reset compensation value */
	  upp = (int)(1000000.0 / ((double)sps / (double)spp));
	  /*
	  SFprintf(stderr, "AS got CmdSPEED: sps %d\n", sps);
	  */
	}
	break;
      case CmdSTOP:
	{
	  int val;
	  cmd = tmp;
	  /*
	     fprintf(stderr, "AS: CmdSTOP. . .\n");
	     */
	  result = CmdRead((char *)&val, sizeof(int));
          if (result != 0)
            return result;
	  /*
	  CmdWrite(AUDIO_STOP_PATTERN, strlen(AUDIO_STOP_PATTERN));
	  */
	  if (live_source) {
	    StopPlayLiveAudio();
	  }
	  return 0;  /* return from PLAYaudio() */
	}
      case CmdCLOSE:
	if (live_source) {
	  StopPlayLiveAudio();
	}
	return(1);  /* The whole AS session terminates */
      default:
	if (live_source) {
	  StopPlayLiveAudio();
	}
	fprintf(stderr, "AS error: cmd=%d while expects STOP/SPEED/CLOSE.\n", tmp);
	return(-1);
      }
    }
    
    if (FD_ISSET(audioSocket, &read_mask)){  /* Feedback packet */
      int bytes, len;
      for (;;) {
	if (conn_tag >= 0) {
	  len = wait_read_bytes(audioSocket, (char *)fbpara, sizeof(*fbpara));
	  if (len == 0) return(1); /* connection broken */
	  else if (len < 0) { /* unexpected error */
	    perror("AS read1 FB");
	    return(-1);
	  }
	}
	else { /* discard mode packet stream, read the whole packet */
	  len = read(audioSocket, (char *)fbpara,  FBBUF_SIZE);
	}
	if (len == -1) {
	  if (errno == EINTR) continue; /* interrupt */
	  else {
	    if (errno != EPIPE && errno != ECONNRESET) perror("AS failed to read() fbmsg header");
	    break;
	  }
	}
	break;
      }
      if (len < sizeof(*fbpara)) {
	if (len > 0) fprintf(stderr,
			 "AS warn read() len %dB < sizeof(*fbpara) %dB\n",
			 len, sizeof(*fbpara));
	continue;
      }
#ifdef NeedByteOrderConversion
      fbpara->type = ntohl(fbpara->type);
#endif
      bytes = (fbpara->type > 0) ?
              sizeof(APdescriptor) * (fbpara->type - 1) :
              0;
      if (bytes > 0) {
	if (conn_tag >= 0) { /* not discard mode packet stream,
				read the rest of packet */
	  len = wait_read_bytes(audioSocket,
				((char *)fbpara) + sizeof(*fbpara),
				bytes);
	  if (len == 0) return(1); /* connection broken */
	  else if (len < 0) { /* unexpected error */
	    perror("AS read2 FB");
	    return(-1);
	  }
	  len += sizeof(*fbpara);
	}
      }
      bytes += sizeof(*fbpara);
      if (len < bytes) {
	if (len > 0) fprintf(stderr,
			     "AS only read partial FBpacket, %dB out of %dB.\n",
			     len, bytes);
	continue;
      }
      if (live_source) {  /* ignore all feedback messags for live source */
	continue;
      }
      
#ifdef NeedByteOrderConversion
      fbpara->cmdsn = ntohl(fbpara->cmdsn);
#endif
      if (len != sizeof(*fbpara) +
	  (fbpara->type ? (fbpara->type -1) * sizeof(APdescriptor) : 0)) {
	/* unknown message, discard */
	SFprintf(stderr, "AS Unkown fb msg: len = %d, type = %d\n",
		len, fbpara->type);
	continue;
      }
      if (fbpara->cmdsn != cmdsn) {  /* discard the outdated message */
	continue;
      }
#ifdef NeedByteOrderConversion
      {
	int i, * ptr = (int *)fbpara + 2;
	for (i = 0; i < (len >> 2) - 2; i++) *ptr = ntohl(*ptr);
      }
#endif
      if (fbpara->type == 0) { /* feedback message */
	/*
	SFprintf(stderr, "AS got fbmsg: addsamples %d, addsps %d\n",
		fbpara->data.fb.addSamples, fbpara->data.fb.addsps);
	*/
	addSamples += fbpara->data.fb.addSamples;
	if (fbpara->data.fb.addsps) {
	  delta_sps += fbpara->data.fb.addsps;
	  upp = (int)(1000000.0 / ((double)(sps + delta_sps) / (double)spp));
	}
      }
      else { /* resend requests */
	APdescriptor * req = &(fbpara->data.ap);
	int i;
	/*
	SFprintf(stderr, "AS got %d resend reqs\n", fbpara->type);
	*/
	for (i = 0; i < fbpara->type; i ++) {
	  ResendPacket(req->firstSample, req->samples);
	  req ++;
	}
      }
    }
  }
}
#endif

// our version of play audio.
int
Audio_Global::play_audio(void)
{
  int result;
 
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) play_audio () called \n"));
 
  {
    PLAYaudioPara para;
    result = CmdRead((char *)&para, sizeof(para));
    if (result != 0)
      return result;
#ifdef NeedByteOrderConversion
    para.sn = ntohl(para.sn);
    para.nextSample = ntohl(para.nextSample);
    para.samplesPerSecond = ntohl(para.samplesPerSecond);
    para.samplesPerPacket = ntohl(para.samplesPerPacket);
    para.ABsamples = ntohl(para.ABsamples);
    para.spslimit = ntohl(para.spslimit);
#endif
    nextsample = para.nextSample;
    cmdsn = para.sn;
    sps = para.samplesPerSecond;
    spslimit = para.spslimit;
    spp = para.samplesPerPacket;
    addSamples = para.ABsamples / 2;
    if (spp * audioPara.bytesPerSample > databuf_size) {
      spp = databuf_size / audioPara.bytesPerSample;
    }
    /*
      SFprintf(stderr, "AS got CmdPLAY: sps %d\n", sps);
    */
  }
  /*
  fprintf(stderr, "AS: nextSampe=%d for PLAY.\n", para.nextSample);
  */

  upp = (int)(1000000.0 / ((double)sps / (double)spp));
  nextTime = get_usec();
  
  CmdWrite((char *)&nextTime, sizeof(int));
  if (live_source) {
    StartPlayLiveAudio();
  }
  this->send_audio ();
}

int
Audio_Global::send_audio (void)
{
  unsigned curTime = get_usec();
    
  if (hasdata) {
    if (addSamples < - spp) {  /* slow down by not sending packets */
      /*      ACE_DEBUG ((LM_DEBUG,"(%P|%t) slow down by not sending\n")); */
      nextTime += upp;
      addSamples += spp;
    }
    else {
      /*      ACE_DEBUG ((LM_DEBUG,"(%P|%t) sending."
                  "nexttime = %d, curTime = %d, hasdata = %d\n",
                  nextTime, curTime, hasdata)); */
      int need_sleep = 0;
      while ( (nextTime <= curTime) && (hasdata)) {
        if (need_sleep) usleep(5000);
        hasdata = SendPacket();
        need_sleep = 1;
        packets ++;
        nextTime += upp;
        if (addSamples > 0 && packets % SPEEDUP_SCALE == 0) {
          addSamples -= spp;
          usleep(5000);
          hasdata = SendPacket();
          packets ++;
        }
      }
    }
  }
  curTime = nextTime - curTime;
  if (curTime > 5000000) curTime = 5000000; /* limit on 5 second weit time
                                               in case error happens */
  tval.tv_sec = curTime / 1000000;
  tval.tv_usec = curTime % 1000000;
  
  if (hasdata)
    {
      // schedule a sigalrm to simulate select timeout.
      ACE_Time_Value tv (tval);
      ACE_OS::ualarm (tv,0);
    }
  return 0;
}


void 
Audio_Global::on_exit_routine(void)
{
  struct sockaddr_in peeraddr_in;
  int size = sizeof(peeraddr_in);
  
  /*
  fprintf(stderr, "An AS session terminated\n");
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
      printf("%s: %s %3dm%02ds %dB %s\n",
	     buf,
	     hp == NULL ? inet_ntoa(peeraddr_in.sin_addr) : hp->h_name,
	     (val - start_time) / 60, (val - start_time) % 60,
	     bytes_sent, audioFile);
    }
  }
  ComCloseConn(serviceSocket);
  ComCloseConn(audioSocket);
}
