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

extern int vsp[2];
extern void set_exit_routine_tag(int tag);

#include "vb.h"

ACE_RCSID(mpeg_client, vb, "$Id$")

block ** VideoBuffer::head = 0;
block ** VideoBuffer::tail = 0;
char * VideoBuffer::buf = 0;
int VideoBuffer::bufsize = -1;
int VideoBuffer::sid = -1;
int VideoBuffer::countid = -1;
int VideoBuffer::exit_tag = -1;
int VideoBuffer::conn_tag = -1;
int VideoBuffer::savedSocket = -1;

//constructor.
VideoBuffer::VideoBuffer (void)
  :msg (0),
   packet (0),
   msgsn (-1),
   ptr (0),
   ptr1 (0),
   tmp_buf (0),
   cmdsn (-1),
   fb_state (0),
   qosRecomputes (0),
   f (0),
   fa (0),
   reach_limit (0),
   not_action (1)
#ifdef STAT
  ,to_count (1),
  gap_msgsn (-1)
#endif
{
}

// Destructor.
VideoBuffer::~VideoBuffer (void)
{
  if (ACE_Reactor::instance ()->remove_handler (this->handler_,ACE_Event_Handler::READ_MASK) == -1)
    ACE_DEBUG ((LM_ERROR,"(%P)remove handler failed for Video_Notification_Handler\n"));

  delete this->handler_;
  if (ACE_Reactor::instance ()->remove_handler (this,ACE_Event_Handler::READ_MASK) == -1)
    ACE_DEBUG ((LM_ERROR,"(%P)remove handler failed for VideoBuffer\n"));
}

/* size in byte */
void 
VideoBuffer::VBinitBuf (int size)
{
  bufsize = size - sizeof(struct header);
  buf = creat_shared_mem(size);
  head = &((struct header *)buf)->h;
  tail = &((struct header *)buf)->t;
  buf += sizeof(struct header);
  sid = creat_semaphore();
  countid = creat_semaphore();
  enter_cs(countid);
  *head = *tail = (struct block *)buf;
  (*tail)->full = 0;
  (*tail)->next = NULL;
  (*tail)->shcode = SHCODE;
}

/* block version */
char* 
VideoBuffer::VBgetBuf (int size) 
{
  return 0;
}

/* non-block check, return True/False*/
int
VideoBuffer::VBcheckBuf (int size) 
{
  return 0;
}

void
VideoBuffer::VBputMsg (char * msgPtr)
{
}

/* block version */
char * 
VideoBuffer::VBgetMsg ()
{
  char *vb_ptr;

#ifdef STAT
  if (shared->collectStat && *head == *tail)
    shared->stat.VBemptyTimes ++;
#endif
  //  ACE_DEBUG ((LM_DEBUG,"(%P)waiting for countid\n"));
  enter_cs(countid);
  enter_cs(sid);
  while (*tail != *head && (*tail)->full == 0)
    *tail = (*tail)->next;
  leave_cs(sid);
  if (*head == *tail)
  {
    fprintf(stderr, "VB: getMsg run out of msg unexpectedly.\n");
    ACE_OS::exit (1);
  }
  vb_ptr = ((char*)*tail)+sizeof(**tail)+sizeof(VideoMessage);

  //  fprintf(stderr,"VBgetMsg: buf:%x, msg:%x\n", (int)buf, (int)vb_ptr);

  return vb_ptr;
}

/* non-block check, return Number of Msgs in buffer */
int
VideoBuffer::VBcheckMsg ()
{
  return get_semval(countid);
}

int
VideoBuffer::VBbufEmpty (void)
{
  /*
  Fprintf(stderr, "VB countid %d\n", get_semval(countid));
  */
  return get_semval(countid) <= 0;
}

void 
VideoBuffer::VBreclaimMsg (char * msgPtr)
{
  enter_cs(sid);
  *tail = (*tail)->next;
  leave_cs(sid);
}

void 
VideoBuffer::VBdeleteBuf (void)
{
  remove_shared_mem (buf - sizeof(struct header));
}

void 
VideoBuffer::VBdeleteSem (void)
{
  remove_semaphore(sid);
  remove_semaphore(countid);
}

int
VideoBuffer::VBprocess (int init_socket, int normal_socket)
{
  this->initSocket = init_socket;
  this->normalSocket = normal_socket;
  msgsn = -1;
  dataSocket = initSocket;
  exit_tag = 0;
  conn_tag = shared->videoMaxPktSize;
  savedSocket = normalSocket;

  //  ACE_DEBUG ((LM_DEBUG,"VideoBuffer::VBProcess ()\n"));
  /* buffer big enough for discard mode packet stream */
  if (conn_tag < 0)
    {  
      tmp_buf = (char *)ACE_OS::malloc(-conn_tag);
      if (tmp_buf == NULL) {
        fprintf(stderr, "AB failed to allocate %d bytes");
        ACE_OS::perror ("of tmp_buf");
        ACE_Reactor::instance ()->end_event_loop (); return -1;
      }
    }

  ACE_NEW_RETURN (this->handler_,
                  Video_Notification_Handler (),
                  -1);

  // Register the notification handler with the reactor.
  int result = ACE_Reactor::instance ()->register_handler (this->handler_,
                                                           ACE_Event_Handler::READ_MASK);
  if (result != 0)
    return result;

  result = ACE_Reactor::instance ()->register_handler (this,
                                                       ACE_Event_Handler::READ_MASK);

  if (result != 0)
    return result;

  this->state_ = READ_HEADER;
  temp = (char *)&msghd;
  bytes = sizeof (msghd);
  return 0;
}

ACE_HANDLE
VideoBuffer::get_handle (void) const
{
  if (this->socket_flag_)
    return this->normalSocket;
  else
    return this->initSocket;
}

int
VideoBuffer::handle_input (ACE_HANDLE fd)
{
  //  ACE_DEBUG ((LM_DEBUG,"VideoBuffer::handle_input:state = %d\n",this->state_));
  switch (this->state_)
    {
    case READ_NEXT_HEADER:
    case READ_HEADER:
      {
        if (conn_tag >= 0)
          len = ACE_OS::read (dataSocket,temp,bytes);
        else
          {
            len = ACE_OS::read (dataSocket,tmp_buf,-conn_tag);
            //            fprintf (stderr,"VB read packet len = %d\n",len);
            ACE_OS::memcpy ((char *)&msghd, tmp_buf, sizeof(msghd));
          }
        if (len == -1) {
          if (errno == EWOULDBLOCK || errno == EAGAIN) {
            perror("VB sleep for 10ms");
            usleep(10000);
            // set the pointers before going into the next loop.
            temp = (char *)&msghd;
            bytes = sizeof (msghd);
            return 0;
          }
          ACE_OS::perror ("VB ACE_OS::read () data");
          ACE_Reactor::instance ()->end_event_loop (); return -1;
        }
        if (len == 0) {  /* EOF, connection closed by peer */
          fprintf(stderr, "Error: VB found dataSocket broken\n");
          for (;;) {
            usleep(1000000);
          }
        }
        if (conn_tag >= 0)
          {
            temp += len;
            bytes -= len;
            if (bytes != 0)
              return 0;
            else
              len = sizeof (msghd);
          }
        if (len < sizeof(msghd))
          {
            // go back to reading the next header.
            temp = (char *)&msghd;
            bytes = sizeof (msghd);
            fprintf(stderr, "VD warn: PEEK1ed %dB < expected %dB\n",len, sizeof(msghd));
            //            continue;
          }
#ifdef NeedByteOrderConversion
        msghd.packetsn = ntohl(msghd.packetsn);
        msghd.packetSize = ntohl(msghd.packetSize);
        msghd.msgsn = ntohl(msghd.msgsn);
        msghd.msgOffset = ntohl(msghd.msgOffset);
        msghd.msgSize = ntohl(msghd.msgSize);
#endif

        if (this->state_ == READ_NEXT_HEADER)
          {
#ifdef STAT
            {
              int gap = msghd.msgsn - gap_msgsn;
              gap = (gap >MSGGAP_MAX) ? MSGGAP_MAX : gap < MSGGAP_MIN ? MSGGAP_MIN : gap;
              shared->stat.VBmsgGaps[gap - MSGGAP_MIN] ++;
              if (gap >0) gap_msgsn = msghd.msgsn;
            }
#endif
            if (msghd.msgsn <= msgsn) 
              { /* outdated message, wait for next one */
            
                fprintf(stderr, "VB discard outdated or dup msgsn %d, pktsn %d\n",
                        msghd.msgsn, msghd.packetsn);
            
                this->state_ = SKIP_NEXT_MESSAGE;
                bytes = msghd.msgSize;
                //	  skip_message(dataSocket, &msghd);
                //	  continue;
                return 0;
              }
      
            if ((msghd.msgsn > msgsn + 1) || (msghd.msgOffset == 0))
              {
                /* message out of order, abandon current packet */
                /*
	fprintf(stderr, "VB msg out of order for current packet, discard it.\n");
	*/
#ifdef STAT
                to_count = 0;
#endif
              }
            else
              {
                //                ACE_DEBUG ((LM_DEBUG,"assigning next msgsn %d\n",msghd.msgsn));
                msgsn = msghd.msgsn;
                this->state_ = READ_MESSAGE;
                temp = ptr +sizeof (msghd);
                bytes = msghd.msgSize;
                // make a recursive call as we just have to do a memcpy from the buffer.
                this->handle_input (dataSocket);
                return 0;
              }
          } 

        //    fprintf(stderr, "VB PEEK1 a msg sn-%d, size-%d, pkt-%d, pktsize-%d\n",msghd.msgsn, msghd.msgSize, msghd.packetsn, msghd.packetSize);

#ifdef STAT
        if (to_count) {
          int gap = msghd.msgsn - gap_msgsn;
          gap = (gap >MSGGAP_MAX) ? MSGGAP_MAX : gap < MSGGAP_MIN ? MSGGAP_MIN : gap;
          shared->stat.VBmsgGaps[gap - MSGGAP_MIN] ++;
          if (gap >0) gap_msgsn = msghd.msgsn;
        }
        to_count = 1;
#endif
        if (msghd.msgsn <= msgsn)  /* outdated msg */
          {
            fprintf(stderr, "VB discard outdated msgsn %d, pktsn %d when expecting first %d\n",
                    msghd.msgsn, msghd.packetsn,msgsn);
            this->state_ = SKIP_MESSAGE;
            bytes = msghd.msgSize;
            //            skip_message(dataSocket, &msghd);
            //          continue;
            return 0;
          }
        else if (msghd.msgOffset != 0)  /* not first msg of a packet */
          {
    
            /*
              Fprintf(stderr, "VB discard non-first msg msgsn %d, pktsn %d\n",
              msghd.msgsn, msghd.packetsn);
            */
            this->state_ = SKIP_MESSAGE;
            bytes = msghd.msgSize;
            //            skip_message(dataSocket, &msghd);
            //          continue;
            return 0;
          }
        else
          {
            //            ACE_DEBUG ((LM_DEBUG,"assigning msgsn %d\n",msghd.msgsn));
            msgsn = msghd.msgsn;
          }
    
        /* allocate packet for the incoming msg */
        bsize =  msghd.packetSize + sizeof(**head)*2 + sizeof(msghd);
        bsize = ((bsize+3)>>2)<<2;
        enter_cs(sid);
        if (*head >= *tail)
          {
            if (bufsize - (int)((char*)*head - buf) >= bsize )
              msg =(VideoMessage *)((char*)*head + sizeof(**head));
            else if ((int)((char*)*tail - buf) >= bsize)
              {
                (*head)->next = (struct block *)buf;
                (*head)->full = 0;
                *head = (struct block *)buf;
                msg = (VideoMessage *)(buf + sizeof(**head));
                *head = (struct block *)buf;
                (*head)->shcode = SHCODE;
              }
            else /* not enough buffer, discard current message */
              {
                leave_cs(sid);
#ifdef STAT
                if (shared->collectStat)
                  shared->stat.VBdroppedFrames ++;
#endif
                /*
                  Fprintf(stderr, "VB not enough space 1, drop msg.sn %d pktsn %d\n",
                  msghd.msgsn, msghd.packetsn);
                */
                this->state_ = SKIP_MESSAGE;
                bytes = msghd.msgSize;
                //                skip_message(dataSocket, &msghd);
                //      continue;
                return 0;
              }
          }
        else /* *head < *tail */
          if ((char*)*tail - (char*)*head >= bsize)
            msg = (VideoMessage *)((char*)*head + sizeof(**head));
          else   /* not enough buffer, abandon current message */
            {
              leave_cs(sid);
#ifdef STAT
              if (shared->collectStat)
                shared->stat.VBdroppedFrames ++;
#endif
              /*
                Fprintf(stderr, "VB not enough space 1, drop msg.sn %d pktsn %d\n",
                msghd.msgsn, msghd.packetsn);

              */
              this->state_ = SKIP_MESSAGE;
              bytes = msghd.msgSize;
              //              skip_message(dataSocket, &msghd);
              //            continue;
              return 0;
            }
        leave_cs(sid);

        //fprintf(stderr, "VB allocated a buffer for comming packet.\n");

        psize = msghd.packetSize;
        poffset = 0;
        packet = (VideoPacket *)((char*)msg + sizeof(msghd));
        *(((int*)packet)+(msghd.packetSize>>2)) = 0;
        /* clear the last no more than three bytes, for
           proper detecting the end of packet by VD */
        ptr = (char*)msg;
        this->state_ = READ_MESSAGE;
        temp = ptr +sizeof (msghd);
        bytes = msghd.msgSize;
      }
      break;
    case SKIP_NEXT_MESSAGE:
    case SKIP_MESSAGE:
      {
        char buffer[BUFSIZ];
          if (conn_tag >= 0) 
            {
              int size = bytes > BUFSIZ ? BUFSIZ : bytes;
              int res =ACE_OS::read (dataSocket, buffer, size);
              bytes -= res;

              if (bytes != 0)
                return 0;
            }
          if (this->state_ == SKIP_MESSAGE)
            this->state_ = READ_HEADER;
          else if (this->state_ == SKIP_NEXT_MESSAGE)
            this->state_ = READ_NEXT_HEADER;
          temp = (char *)&msghd;
          bytes = sizeof (msghd);
          break;
      }
    case READ_MESSAGE:
      {
        if (conn_tag >= 0)
          {
            int val;
            val = ACE_OS::read (dataSocket,temp,bytes);

            if (val == -1 && (errno == EINTR || errno == EAGAIN | errno == EWOULDBLOCK))
              {   /* interrupted or need to wait, try again */
                if (errno == EAGAIN | errno == EWOULDBLOCK) usleep(10000);
                errno = 0;
                return 0;
              }
            if (val == -1)
              {
                ACE_OS::perror ("Error -- Read from socket");
                ACE_Reactor::instance ()->end_event_loop (); return -1;
              }
            if (val == 0) /* EOF encountered */
              {
                ACE_DEBUG ((LM_DEBUG, "Error -- EOF reached while trying to read %d bytes.\n"));
                ACE_Reactor::instance ()->end_event_loop (); return -1;
              }
            temp += val;
            bytes -= val;
            if (bytes < 0)  /* weird thing is happening */
              {
                ACE_DEBUG ((LM_DEBUG, "Error: read too much from socket, %d out of %d bytes.\n"));
                ACE_Reactor::instance ()->end_event_loop (); return -1;
              }
            if (bytes != 0)
              return 0;
          }
        else
          memcpy(temp, tmp_buf + sizeof(msghd), bytes);
        poffset += msghd.msgSize;
        psize -= msghd.msgSize;
        ptr += msghd.msgSize;

        //        fprintf(stderr, "VB packet remain size %d\n", psize);

        if (psize == 0)
          {
            //            ACE_DEBUG ((LM_DEBUG,"finished receiving current packet\n"));
            /* finished receiving the current packet */
#ifdef NeedByteOrderConversion
            packet->cmd = ntohl(packet->cmd);
            packet->cmdsn = ntohl(packet->cmdsn);
            packet->sh = ntohl(packet->sh);
            packet->gop = ntohl(packet->gop);
            packet->frame = ntohl(packet->frame);
            packet->display = ntohl(packet->display);
            packet->future = ntohl(packet->future);
            packet->past = ntohl(packet->past);
            packet->currentUPF = ntohl(packet->currentUPF);
            packet->dataBytes = ntohl(packet->dataBytes);
#endif
            pcmdsn = packet->cmdsn;
            pcmd = packet->cmd;
            pfid = packet->frame;
            pgop = packet->gop;
            shared->VBheadFrame = (pcmd == CmdPLAY) ? pfid : pgop;
	
            shared->currentUPF = packet->currentUPF;
            enter_cs(sid);
            (*head)->full = 1;
            psize = sizeof(**head) + sizeof(*msg) + msghd.packetSize;
            psize = ((psize+3)>>2)<<2;
            ptr = (char*)*head + psize;
            (*head)->next = (struct block *) ptr;
            (*head) = (struct block *)ptr;
            (*head)->shcode = SHCODE;
            leave_cs(countid);
            leave_cs(sid);

            /* VB receives all frame except for the INIT one through normalSocket */
            if (dataSocket != normalSocket) 
              {
                this->socket_flag_ = 1;
                fprintf(stderr, "VB got INIT frame.\n");
                ACE_OS::write (initSocket, (char *)&initSocket, 1);  /* write a garbage byte */
                //                ACE_OS::close (initSocket);
                //                dataSocket = normalSocket;
                //                int result = ACE_Reactor::instance ()->remove_handler (this,ACE_Event_Handler::READ_MASK);
                //                if (result != 0)
                //                  ACE_DEBUG ((LM_DEBUG,"remove handler failed for read_mask\n"));
                
              }
	
            /* following is synchronization feedback algorithm */
            this->sync_feedback ();
            if (dataSocket != normalSocket)
              {
                dataSocket = normalSocket;
                int result = ACE_Reactor::instance ()->register_handler (this,ACE_Event_Handler::READ_MASK);
                if (result != 0)
                  ACE_DEBUG ((LM_DEBUG,"register handler failed for read_mask after datasocket change\n"));
                return -1;
              }
            //            return 0;
            break;  /* got the whole packet, break to the out-most loop for next packet */
          }  /* end if (psize == 0) */
        else if (psize < 0)
          {
            fprintf(stderr, "VB error: received too many msgs for a packet.\n");
            ACE_Reactor::instance ()->end_event_loop ();
            return -1;
          }
        this->state_ = READ_NEXT_HEADER;
        temp = (char *)&msghd;
        bytes = sizeof (msghd);
        break;
      }
    }
  return 0;
}

int
VideoBuffer::handle_output (ACE_HANDLE fd)
{
  if ((this->state_ == WRITE_FEEDBACK1) || (this->state_ == WRITE_FEEDBACK2))
    {
      // send the feedback to the server.
      VideoFeedBackPara para;
      para.cmdsn = htonl(shared->cmdsn);
      para.addUsecPerFrame = htonl(fb_addupf);
      para.addFrames = htonl(fb_addf);
      para.needHeader = htonl(shared->needHeader);
      shared->needHeader = 0;
      para.frameRateLimit1000 =
        htonl((long)(shared->frameRateLimit * 1000.0));
      para.sendPatternGops = htonl(shared->sendPatternGops);
      ACE_OS::memcpy (para.sendPattern, shared->sendPattern, PATTERN_SIZE);
      
      // fprintf(stderr, "VB to send a fb packet...");

      int res;
      if (conn_tag != 0) 
        { /* packet stream */
          if (temp == 0)
            {
              temp = (char *)&para;
              bytes = sizeof (para);
            }
          res = ACE_OS::write (dataSocket, temp, bytes);
          if (res == -1)
            {
              if (errno == EINTR)
                return 0;
              if (errno == ENOBUFS) {
                perror("VB Warning, fb packet discarded for");
                // Here we should handle the return -1 case!
                fb_state = 4;
              }
              else
                {
                  ACE_OS::perror ("VB error, fb packet sending failed");
                  ACE_Reactor::instance ()->end_event_loop (); return -1;
                }
            }
          else if (res == 0)
            {
              ACE_OS::perror ("VB error, sending fb,socket closed");
              ACE_Reactor::instance ()->end_event_loop (); return -1;
            }
          else
            {
              temp += res;
              bytes -= res;
              if (bytes != 0)
                return 0;
            }
        }
      else
        {
          res = ACE_OS::write (dataSocket, (char *)&para, sizeof(para));
          if (res == -1)
            {
              ACE_OS::perror ("VB error, fb packet sending failed");
              ACE_Reactor::instance ()->end_event_loop (); return -1;
            }
          if (res < sizeof(para))
            {
              fprintf(stderr, "VB send_feedback() warn: res %dB < sizeof(para) %dB\n",
                      res, sizeof(para));
            }
        }
      if (errno != ENOBUFS) // fb_state == 4;
        qosRecomputes = len;
      ACE_Reactor::instance ()->remove_handler (this,
                                                ACE_Event_Handler::WRITE_MASK);
      if (this->state_ == WRITE_FEEDBACK2)
        {
          if (fb_state == 6) 
            { /* record the time if an action packet is
                 successfully send, and indicate that an
                 feedback action leads to state 6, which after
                 delay sometime leads to state 3.
                 The action_delay should have been related
                 to round-trip time. */
              action_time = get_usec();
              action_delay = shared->usecPerFrame * 100;
              not_action = 0;
            }
        }
      else if (this->state_ == WRITE_FEEDBACK2)
        {
          this->feedback_action ();
        }
#ifdef STAT
      {
        int i;
        if ((i = shared->stat.fbPacketNumber) < MAX_FB_PACKETS) {
          shared->stat.fbPackets[i].frameId = shared->nextFrame;
          shared->stat.fbPackets[i].addUsecPerFrame = addupf;
          shared->stat.fbPackets[i].addFrames = addf;
          shared->stat.fbPackets[i].frames = shared->sendPatternGops *
            shared->patternSize;
          shared->stat.fbPackets[i].framesDropped = shared->framesDropped;
          shared->stat.fbPackets[i].frameRateLimit = shared->frameRateLimit;
          shared->stat.fbPackets[i].advance = advance;
        }
        shared->stat.fbPacketNumber ++;
      }
#endif
      //  Now return to the reading header position.
      this->state_ = READ_HEADER;
      temp = (char *)&msghd;
      bytes = sizeof (msghd);
      int result = ACE_Reactor::instance ()->remove_handler (this,
                                                             ACE_Event_Handler::WRITE_MASK);
      if (result != 0)
        ACE_DEBUG ((LM_DEBUG,"remove_handler failed for write"));
    }
  return 0;
}

int
VideoBuffer::sync_feedback (void)
{
  int result;
  if (shared->config.syncEffective) 
    {
      if (fb_state > 1 && fb_state != 4 && (len = shared->qosRecomputes) != qosRecomputes)
        {
          /* QoS feedback packet is sent if at any time send pattern is
           recomputed, and sync feedback is not in active fb_state*/
          this->state_ = WRITE_FEEDBACK1;
          result = ACE_Reactor::instance ()->register_handler (this,ACE_Event_Handler::WRITE_MASK);
          if (result != 0)
            return result;
          fb_addupf = 0;
          fb_addf = 0;
          fb_advance = advance;
          temp = 0;
          return 0;
        }
      this->feedback_action ();
    }  /* end if (shared->config.syncEffective) */
  else 
    fb_state = 0;
  return 0;
}

int
VideoBuffer::feedback_action (void)
{
  switch (fb_state)
    {
    case 4:  /* active */
      if (pcmdsn != cmdsn)
        {
          cmdsn = pcmdsn;
          if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) 
            fb_state = 0;
          else
            fb_state = 1;
          break;
        }
      if (pcmd == CmdPLAY && shared->usecPerFrame != upf)
        {
          /* jump to fb_state 5 if speed changes */
          fb_state = 5;
          break;
        }
      {
        int interval = shared->usecPerFrame;
        double val = (double)(pcmd == CmdPLAY ?
                              pfid - shared->nextFrame :
                              (pcmd == CmdFF ?
                               pgop - shared->nextGroup :
                               shared->nextGroup - pgop));
        fv = DoFilter(f, val); /* get average #frames in the whole client
                                  pipeline, including all stages */
        val = val - fv;
        fav = DoFilter(fa, val >= 0.0 ? val : -val);
        /* get average #frames jitter in the whole client pipeline */
							      
        val = fav * interval * 6;
        /* convert deviation in frame into microseconds, 6 is a magic number */

        /* tries to recompute advance (in microseconds), and med/high/low
           in adaptation to current jitter level */
        if ((val > advance && !reach_limit) ||
            (advance > min_advance && val < advance >> 3)) {
          advance = (int) max(2 * val, min_advance);
          med = advance / interval;
          /*
            if (pcmd == CmdPLAY) {
            if (med < shared->VDframeNumber) {
            med = shared->VDframeNumber;
            }
            }
            else
          */
          if (med < 2 ) { /* but keep minimum buffer fill level */
            med = 2;
          }
          if (med > (VB_BUF_SIZE / shared->averageFrameSize) / 2) {
            reach_limit = 1;
            med = (VB_BUF_SIZE / shared->averageFrameSize) / 2;
            Fprintf(stderr,
                    "VB VSadvance control: VBbuf limit reached, med %d.\n", med);
          }
          else reach_limit = 0;
          high = med + med / 2;
          low = med - med / 2;
          period = med * MAX_CLOCK_DRIFT;
          Fprintf(stderr,
                  "VB: VS advance control: fav %5.2f, med %d, advance %d at nextFrame %d\n",
                  fav, med, advance, shared->nextFrame);
        }
      }
      /* record the current time (fid or gopid) if buffer fill level drift
         only very little */
      if ((int)fv == med)
        {
          init_fv = fv;
          init_pos = pcmd == CmdPLAY ? pfid : pgop;
          break;
        }
      /*
        fprintf(stderr, "VB fb: fv %lf\n", fv);
      */
      /* try send action if low/high water mark is passed, or qos recomputed */
      /* There is problem here, the deltas of upf are sent, instead of upf
         and frame themself. This scheme is not robust in case case feedback
         packets are lost, and get resent */
      len = shared->qosRecomputes;
      if (fv >= high || fv <= low || len != qosRecomputes)
        {
          int addupf, addf;
          int pos = pcmd == CmdPLAY ? pfid : pgop;
          int dist = (int)(pcmd == CmdFB ? init_pos - pos : pos - init_pos);
          if (fv >= high || fv <= low)
            {
              if (dist < period)
                { /* try skip or stall */
                  addf =  (int)(med - fv);
                  addupf = 0;
                }
              else
                { /* try adjust VS clock rate */
                  int added = (int)((double)cupf * (fv - (double)med) / (double) dist);
                  addf = (int)(med - fv);
                  addupf = added;
                  cupf += added;
                }
              fb_state = 6;
            }
          else
            {  /* fb only recomputed sendpattern, no fb_state change */
              addupf = 0;
              addf = 0;
            }

          /* tries to send a feedback packet. */
          if (shared->live)
            /* no sync feedback with live video */
            qosRecomputes = len;
          else 
            {
              this->state_ = WRITE_FEEDBACK2;
              int result = ACE_Reactor::instance ()->register_handler (this,ACE_Event_Handler::WRITE_MASK);
              if (result != 0)
                return result;
              fb_addupf = addupf;
              fb_addf = addf;
              fb_advance = advance;
              return 0;
            }
          if (fb_state == 6)
            { /* record the time if an action packet is
                 successfully send, and indicate that an
                 feedback action leads to fb_state 6, which after
                 delay sometime leads to fb_state 3.
                 The action_delay should have been related
                 to round-trip time. */
              action_time = get_usec();
              action_delay = shared->usecPerFrame * 100;
              not_action = 0;
            }
        }
      break;
    case 6:  /* reset after action */
      if (pcmdsn != cmdsn)
        {
          cmdsn = pcmdsn;
          if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) 
            fb_state = 0;
          else
            fb_state = 1;
          break;
        }
      if (pcmd == CmdPLAY && shared->usecPerFrame != upf)
        {
          fb_state = 5;
          break;
        }
      /* Jump to transition fb_state 3 only after delay for some
         time, when feedback action has been taken, and the
         effect has been propogated back to the client */
      if (get_duration(action_time, get_usec()) >= action_delay) 
        fb_state = 3;
      break;
    case 5:  /* reset after speed change, feedback stays in this
                fb_state as long as play speed is changing */
      if (pcmdsn != cmdsn)
        {
          cmdsn = pcmdsn;
          if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) 
            fb_state = 0;
          else 
            fb_state = 1;
          break;
        }
      /* Jump to transition fb_state 3, indicating that the transition
         is not caused by feedback action */
      if (shared->currentUPF == shared->usecPerFrame)
        {
          not_action = 1;
          fb_state = 3;
        }
      break;
    case 2:  /* delay after start, this delay is for avoiding feedback
                action during server fast start-up. */
      if (pcmdsn != cmdsn) {
        cmdsn = pcmdsn;
        if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
          fb_state = 0;
        }
        else {
          fb_state = 1;
        }
        break;
      }
      if (pcmd == CmdPLAY) {
        if (pfid - startpos >= delay) {
          advance = shared->VStimeAdvance;
          fb_state = 3;
        }
      }
      else if (pcmd == CmdFF) {
        if (pgop - startpos >= delay) {
          advance = shared->VStimeAdvance;
          fb_state = 3;
        }
      }
      else {  /* CmdFB */
        if (startpos - pgop >= delay) {
          advance = shared->VStimeAdvance;
          fb_state = 3;
        }
      }
      break;
    case 3:  /* transient fb_state, entered after start-up delay,
                action-delay, or play-speed change */
      if (pcmdsn != cmdsn) {
        cmdsn = pcmdsn;
        if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
          fb_state = 0;
        }
        else {
          fb_state = 1;
        }
        break;
      }
      /* Initialize both buffer-fill-level and jitter filters */
      if (f == NULL) {
        f = NewFilter(FILTER_LOWPASS, shared->config.filterPara >= 1 ?
                      shared->config.filterPara : 100);
      }
      else {
        f = ResetFilter(f, shared->config.filterPara >= 1 ?
                        shared->config.filterPara : 100);
      }
      if (fa == NULL) {
        fa = NewFilter(FILTER_LOWPASS, shared->config.filterPara >= 1 ?
                       shared->config.filterPara : 100);
        DoFilter(fa, 0.0);
      }
      else if (not_action) { /* reset jitter level filter only
                                if entering this fb_state is not
                                cause by feedback action */
        fa = ResetFilter(fa, shared->config.filterPara >= 1 ?
                         shared->config.filterPara : 100);
      }
      if (f == NULL || fa == NULL) {
        ACE_OS::perror ("VB failed to allocate space for filters");
        fb_state = 0;
      }
      else {
        init_fv =
          DoFilter(f, (double)(pcmd == CmdPLAY ?
                               pfid - shared->nextFrame :
                               (pcmd == CmdFF ?
                                pgop - shared->nextGroup :
                                shared->nextGroup - pgop)));
        init_pos = pcmd == CmdPLAY ? pfid : pgop;
        upf = shared->currentUPF;
        cupf = upf;
        {
          int interval = shared->usecPerFrame;

          /* upon speed change, 'advance', in microseconds, will
             not change, but med/high/low will be updated. This
             may suggest that in the new toolkit version of the
             same feedback systems, the buffer-fill level and
             jitter is measured directly in microseconds, not in
             #frames then converting to microseconds. */
          med = advance / interval;
          /*
            if (pcmd == CmdPLAY) {
            if (med < shared->VDframeNumber) {
            med = shared->VDframeNumber;
            min_advance = advance = med * interval;
            }
            }
            else
          */
          if (med < 2 ) {
            med = 2;
            advance = med * interval;
            min_advance = max(advance, shared->VStimeAdvance);
          }
          else min_advance = shared->VStimeAdvance;
          if (med > (VB_BUF_SIZE / shared->averageFrameSize) / 2) {
            reach_limit = 1;
            med = (VB_BUF_SIZE / shared->averageFrameSize) / 2;
            if (not_action) {
              Fprintf(stderr,
                      "VB start/speed-change: VBbuf limit reached, med %d.\n", med);
            }
          }
          else reach_limit = 0;
        }
        high = med + med / 2;
        low = med - med / 2;
        delay = med * (SPEEDUP_INV_SCALE - 1); /* this delay is to avoid
                                                  feedback when VS is in
                                                  fast-start period */
        period = med * MAX_CLOCK_DRIFT;
        if (not_action) {
          Fprintf(stderr,
                  "VB start/speed-change: med %d, advance %d at nextFrame %d\n",
                  med, advance, shared->nextFrame);
        }
        fb_state = 4;
      }
      break;
    case 0:  /* idle */
      if (pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB) {
        cmdsn = pcmdsn;
        fb_state = 1;
      }
      break;
    case 1:  /* start */
      if (pcmdsn != cmdsn) {
        cmdsn = pcmdsn;
        if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
          fb_state = 0;
        }
        break;
      }
      startpos = pcmd == CmdPLAY ? pfid : pgop;
      advance = shared->VStimeAdvance;

      /*  following from vs.c:
          timerAdjust = (VStimeAdvance * SPEEDUP_INV_SCALE) / currentUPF;
      */
      med = advance / shared->usecPerFrame;
      /*
        if (pcmd != CmdPLAY) med /= shared->patternSize;
      */
      delay = med * (SPEEDUP_INV_SCALE - 1); /* this delay is to avoid
                                                feedback when VS is in
                                                fast-start period */
      not_action = 1;
      fb_state = 2;
      break;
    default:
      fprintf(stderr, "VB: unknown Feedback fb_state %d reached.\n", fb_state);
      fb_state = 0;
      break;
    }
}

ACE_HANDLE
Notification_Handler::get_handle (void) const
{
  return -1;
}

int
Notification_Handler::handle_input (ACE_HANDLE fd)
{
  ACE_DEBUG ((LM_DEBUG,"Notification_Handler::handle_input"));
  char message[BUFSIZ];
  message [0] = 0;
  // used to indicate that we should exit.
  int result =
    ACE_OS::read (fd,message,BUFSIZ);

  ACE_DEBUG ((LM_DEBUG,"result:%d,message[0]:%d\n",result,message[0]));
  if (result == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"AB process exiting, notification socket eof while reading\n"));
      ACE_Reactor::instance ()->end_event_loop (); 
      return -1;
    }
  if (result == -1)
    {
      ACE_DEBUG ((LM_DEBUG,"AB process exiting, notification socket error while reading\n"));
      ACE_Reactor::instance ()->end_event_loop (); 
      return -1;
    }
  ACE_DEBUG ((LM_DEBUG," %d %d\n",result,message[0]));

  switch (message[0])
    {
    case EXIT:
      ACE_DEBUG ((LM_DEBUG,"AB process exiting because of exit signal\n"));
      set_exit_routine_tag(0);
      VideoBuffer::VBdeleteBuf();
      ACE_Reactor::instance ()->end_event_loop (); 
      return -1;
    default:
      break;
    }
  return 0;
}

ACE_HANDLE
Video_Notification_Handler::get_handle (void) const
{
  return vsp[1];
}
