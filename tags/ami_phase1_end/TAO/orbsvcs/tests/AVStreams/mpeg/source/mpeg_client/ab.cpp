
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

extern int asp[2];
extern void set_exit_routine_tag(int tag);

#include "ab.h"

ACE_RCSID(mpeg_client, ab, "$Id$")

AudioBuffer::AudioBuffer (void)
  :temp (0),
   bytes (-1),
   abuf (0),
   sid (-1),
   exit_tag (0),
   savedSocket (-1),
   packet (0),
   pkt_data (0),
   conn_tag (-1),
   fbstate (0),
   waketime (0),
   pcmdsn (-1),
   mode_ (INVALID)
{
}

//destructor.
AudioBuffer::~AudioBuffer (void)
{
  if (ACE_Reactor::instance ()->remove_handler (this->handler_,ACE_Event_Handler::READ_MASK) == -1)
    ACE_DEBUG ((LM_ERROR,"(%P)remove handler failed for Video_Notification_Handler\n"));

  delete this->handler_;
  if (ACE_Reactor::instance ()->remove_handler (this,ACE_Event_Handler::READ_MASK) == -1)
    ACE_DEBUG ((LM_ERROR,"(%P)remove handler failed for VideoBuffer\n"));

}

void 
AudioBuffer::set_silence (char *buf, int samples)
{
  memset(buf, 0xff, samples * shared->audioPara.bytesPerSample);
}

void 
AudioBuffer::ABinitBuf (int size) /* size in bytes */
{
  abuf = (ABBuffer *)creat_shared_mem(size);
  abuf->bufsize = size - sizeof(*abuf);
  abuf->buf = (char *)abuf + sizeof(*abuf);
  sid = creat_semaphore();
}

void
AudioBuffer::ABflushBuf (int nextSample)  /* flush the whole buffer */
{
  enter_cs(sid);
  abuf->bps = shared->audioPara.bytesPerSample;
  abuf->size = abuf->bufsize / abuf->bps;
  abuf->samples = 0;
  abuf->stuff = 0;
  abuf->ts = abuf->hs = nextSample;
  abuf->tind = 0;
  set_silence(abuf->buf, abuf->size);
  leave_cs(sid);
}

int 
AudioBuffer::ABcheckSamples (void) /* returns # of samples in ABbuf */
{
  return abuf->samples;
}

int
AudioBuffer::ABgetSamples (char * buf, int samples)
    /* read at most given number of samples from AB to buf, returns
       number of sample actually read */
{
  int as;
  int tsamples = samples;
  enter_cs(sid);
  if (abuf->stuff > 0) {
    as = min(abuf->stuff, samples);
    set_silence(buf, as);
    abuf->stuff -= as;
    samples -= as;
    buf += as * abuf->bps;
  }
  if (samples > 0) {
    as = min(samples, max(0, abuf->samples));
	 /* there may be fewer samples in abuf */
    if (as > 0) {
      int part1 = min(as, abuf->size - abuf->tind);
      ACE_OS::memcpy (buf, abuf->buf + (abuf->bps * abuf->tind), part1 * abuf->bps);
      set_silence(abuf->buf + (abuf->bps * abuf->tind), part1);
      if (part1 < as) {  /* This read cross the boundary of abuf */
	ACE_OS::memcpy (buf + (part1 * abuf->bps),
                        abuf->buf,
                        (as - part1) * abuf->bps);
	set_silence(abuf->buf, as - part1);
      }
    }
    abuf->samples -= samples;
    abuf->ts += samples;
    abuf->tind = (abuf->tind + samples) % abuf->size;
    if (as < samples) set_silence(buf + (as * abuf->bps), samples - as);
  }
  leave_cs(sid);
  return tsamples;
}

/* if samples < 0; then stuff |samples| silient samples to ABgetSamples(),
   otherwise wipe out this number of samples from AB */
int 
AudioBuffer::ABskipSamples (int samples)
{
  enter_cs(sid);
  if (samples <= 0) {
    abuf->stuff += (- samples);
  }
  else {
    abuf->samples -= samples;
    abuf->ts += samples;
    abuf->tind = (abuf->tind + samples) % abuf->size;
  }
  leave_cs(sid);
  return samples;
}

void 
AudioBuffer::ABdeleteBuf (void)
{
  remove_shared_mem((char *)abuf);
}

void 
AudioBuffer::ABdeleteSem (void)
{
  remove_semaphore(sid);
}

/* SIGUSR1 from CTR is for killing this process, without affecting any other ones. */

void 
AudioBuffer::exit_on_kill (void)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) ABprocess killed \n"));
  extern void set_exit_routine_tag(int tag);
  set_exit_routine_tag(0);
  //  ComCloseConn(savedSocket);
  vbuffer->VBdeleteBuf();
  ACE_OS::exit (0);
}

ACE_HANDLE
AudioBuffer::get_handle (void) const
{
  return this->dataSocket;
}

int
AudioBuffer::handle_input (ACE_HANDLE fd)
{
  //  ACE_DEBUG ((LM_DEBUG,"handle_input:mode = %d\n",this->mode_));
  int len;
  switch (this->mode_)
    {
    case READ_HEADER:
      {
        int len;
        if (conn_tag >= 0) 
          {
            //            ACE_DEBUG ((LM_DEBUG,"non discard mode: "));
            if (bytes < 0)
              bytes = sizeof(*packet);
            len = ACE_OS::read (dataSocket, (char *)temp, bytes);
          }
        else
          {  /* discard mode packet stream, read all bytes */
            //            ACE_DEBUG ((LM_DEBUG,"discard mode: "));
            if (bytes <  0)
              bytes = PACKET_SIZE;
            len = ACE_OS::read (dataSocket, (char *)packet, bytes);
            //      ACE_DEBUG ((LM_DEBUG,"(%P|%t) ABprocess: got a %d sized packet\n",len));
          }
        if (len == -1) 
          {
            if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)
              {
                return 0;
              }
            perror("AB ACE_OS::read () audio packet from discard-mode packet stream");
            ACE_Reactor::instance ()->end_event_loop ();
            return -1;
          }

        if (len == 0)
          {
            fprintf(stderr, "Error: AB found dataSocket broken\n");
            ACE_Reactor::instance ()->end_event_loop (); return -1;
          }
        //        ACE_DEBUG ((LM_DEBUG,"packet: bytes = %d,len = %d\n",bytes,len));
        if (conn_tag >= 0)
          {
            temp += len;
            bytes -= len;
            if (bytes == 0)
              {
                // header reading is done.
                this->mode_ = READ_DATA;
                bytes = -1;
                len = sizeof (*packet);
              }
            else
              return 0;
          }
        if (len < sizeof (*packet))
          {
            fprintf(stderr, "Warn: AB discard len = %d bytes of supposed header.\n", len);
            return 0;
          }
        // process the header.
#ifdef NeedByteOrderConversion
        packet->dataBytes = ntohl(packet->dataBytes);
#endif

        //    ACE_DEBUG ((LM_DEBUG,"(%P|%t) ABprocess: Received  %d sized packet\n",len));
        if (packet->dataBytes <= 0)
          {
            fprintf(stderr, "AB Error: pkt->dataBytes %d, len %d\n",
                    packet->dataBytes,
                    len);
            ACE_Reactor::instance ()->end_event_loop (); return -1;
          }
        bytes = packet->dataBytes + sizeof(*packet);
    
        if (bytes > PACKET_SIZE)
          {
            Fprintf(stderr, "Fatal error: AB packet buf (%dB) too small (%d)\n",
                    PACKET_SIZE, bytes);
            ACE_Reactor::instance ()->end_event_loop (); return -1;
          }
        if (conn_tag > 0)
          {
            temp = (char *)packet + sizeof(*packet);
            bytes = bytes - sizeof(*packet);
            ACE_DEBUG ((LM_DEBUG,"(%P) Ready to read the data part of the packet\n"));
            break;
          }
      }
      // fall through and process the header.
    case READ_DATA:
      {
        // code to read the audio packet and buffer it.
        if (conn_tag >= 0)
          {
            len = ACE_OS::read (dataSocket,temp,bytes);

            if (len <= 0) 
              {
                if (len == -1)ACE_OS::perror ("AB encounter error on wait_read_bytes()");
                else fprintf(stderr, "AB encounter EOF on wait_read_bytes()\n");
              }
            temp +=len;
            bytes -= len;
            if (bytes != 0)
              return 0;
          }
        // set the parameters for the header reading.
        this->mode_ = READ_HEADER;
        bytes = -1;
        temp = (char *)packet;
#ifdef NeedByteOrderConversion
        packet->cmdsn = ntohl(packet->cmdsn);
        packet->samplesPerSecond = ntohl(packet->samplesPerSecond);
        packet->resend = ntohl(packet->resend);
        packet->firstSample = ntohl(packet->firstSample);
        packet->samples = ntohl(packet->samples);
        packet->actualSamples = ntohl(packet->actualSamples);
        /* dataBytes already byte-reordered */
#endif
        /*
          Fprintf(stderr, "AB got a packet: %d(%d)\n",
          packet->firstSample, packet->samples);
        */
        /*
          if (packet->firstSample % 10240 && !packet->resend) continue;
        */
        if (packet->samples * abuf->bps > PACKET_SIZE - sizeof(*packet)) {
          fprintf(stderr, "Fatal error: AB has too small packet buffer, %d out of %d\n",
                  PACKET_SIZE, packet->samples * abuf->bps + sizeof(*packet));
          ACE_Reactor::instance ()->end_event_loop (); return -1;
        }

        if (packet->cmdsn != shared->cmdsn) { /* outdated packet */
          /*
            Fprintf(stderr, "AB discarded an outdated packet\n");
          */
          return 0;
        }
        enter_cs(sid);
        if (packet->firstSample + packet->samples <= abuf->ts)
          {
            /* all samples too late, discard it */
            abuf->hs = max(abuf->hs, packet->firstSample + packet->samples);
            abuf->samples = abuf->hs - abuf->ts;
            leave_cs(sid);
            /*
              Fprintf(stderr, "AB all sample in packet %d(%d) too late\n",
              packet->firstSample, packet->samples);
            */
            feedback ();
          }
        else if (packet->firstSample >= abuf->ts + abuf->size) {
          /* all samples too early, discard the packet */
          abuf->hs = max(abuf->hs, packet->firstSample + packet->samples);
          abuf->samples = abuf->hs - abuf->ts;
          leave_cs(sid);
          /*
            Fprintf(stderr, "AB all sample in packet %d(%d) too early\n",
            packet->firstSample, packet->samples);
          */
          feedback ();
        }
        else if (packet->samples > packet->actualSamples) {
          leave_cs(sid);
          fprintf(stderr, "Error: AB interpolation not available yet.\n");
          ACE_Reactor::instance ()->end_event_loop (); return -1;
        }
        else
          {
            int oldhs = abuf->hs;
            int firstSample = max(packet->firstSample, abuf->ts);
            int samples = min(packet->samples -
                              (firstSample - packet->firstSample),
                              (abuf->ts + abuf->size) - packet->firstSample);
            char * data = pkt_data +
              (firstSample - packet->firstSample) * abuf->bps;
            int dstart = (abuf->tind + (firstSample - abuf->ts)) % abuf->size;
            int part1 = min(samples, abuf->size - dstart);
            ACE_OS::memcpy (abuf->buf + (dstart * abuf->bps), data, part1 * abuf->bps);
            if (part1 < samples) {
              memcpy(abuf->buf, data + part1 * abuf->bps,
                     (samples - part1) * abuf->bps);
            }
            abuf->hs = max(abuf->hs, packet->firstSample + packet->samples);
            abuf->samples = abuf->hs - abuf->ts;
            dstart =max(oldhs, abuf->ts);
      
            leave_cs(sid);
      
            part1 = firstSample - dstart;
            if (packet->resend) {
              Fprintf(stderr, "AB got resent %d(%d)\n",
                      packet->firstSample, packet->samples);
            }
            else if (part1 > 0) {
              int res;
              AudioFeedBackPara para;
              Fprintf(stderr, "AB found gap %d(%d)\n", dstart, part1);
              para.cmdsn = htonl(shared->cmdsn);
              para.type = htonl(1);
              para.data.ap.firstSample = htonl(dstart);
              para.data.ap.samples = htonl(part1);
              // register ourself for the write handler.
              int result;
              result = ACE_Reactor::instance ()->register_handler (this,ACE_Event_Handler::WRITE_MASK);
              if (result != 0)
                return result;
              this->mode_ = WRITE_FEEDBACK2;
              temp = (char *)&para;
              bytes = sizeof (para);
            }
          }
      }
      break;
    default:
      break;
    }
  return 0;
}

int
AudioBuffer::handle_output (ACE_HANDLE fd)
{
  ACE_DEBUG ((LM_DEBUG,"handle_output:mode = %d\n",this->mode_));
  int res;
  if ((this->mode_ == WRITE_FEEDBACK1) || (this->mode_ == WRITE_FEEDBACK2))
    {
      // send feedback.

      if (conn_tag != 0)
        { /* packet stream */
          res = ACE_OS::write (dataSocket, temp, bytes);
          if (res == -1)
            {
              if (errno == ENOBUFS) {
                ACE_OS::perror ("AB Warning, resend-req packet discarded for");
                return 0;
              }
              ACE_OS::perror ("AB error, resend-req  packet sending failed");
              ACE_Reactor::instance ()->end_event_loop (); return -1;
            }
        }
      else 
        {
          res = ACE_OS::write (dataSocket, temp, bytes);
          if (res == -1) {
            ACE_OS::perror ("AB error, resend-req  packet sending failed");
            ACE_Reactor::instance ()->end_event_loop (); return -1;
          }
        }
      if (res == 0)
        {
          ACE_DEBUG ((LM_DEBUG,"(%P|%t)AudioBuffer::handle_output:write failed\n"));
          ACE_Reactor::instance ()->end_event_loop (); return -1;
        }
      temp += res;
      bytes -= res;
      if (bytes != 0)
          return 0;
      else
        {
          // remove the write handler.
          int result;
          result = ACE_Reactor::instance ()->remove_handler (this,
                                                             ACE_Event_Handler::WRITE_MASK);
          if (result != 0)
            return result;
          
          this->mode_ = READ_HEADER;
          temp = (char *)packet;
          bytes == -1;
        }
    }
  if (this->mode_ == WRITE_FEEDBACK1)
    {
      // adjust the wakeup time and feedback state.
        waketime = get_usec() + STARTUP_WAIT;
        fbstate = 1;
    }
  return 0;
}

int
AudioBuffer::ABprocess (int socket)
{
  int result;
  ACE_DEBUG ((LM_DEBUG,"(%P|%t)AudioBuffer::ABprocess ()\n"));
  packet = (AudioPacket *)ACE_OS::malloc(PACKET_SIZE);
  if (packet == NULL) {
   ACE_OS::perror ("AB failed to allocate mem for packet buffer");
    ACE_Reactor::instance ()->end_event_loop (); return -1;
  }

  this->dataSocket = socket;
  ACE_NEW_RETURN (this->handler_,
                  Audio_Notification_Handler,
                  -1);
  // Register the notification handler with the reactor.
  result = ACE_Reactor::instance ()->register_handler (this->handler_,
                                                       ACE_Event_Handler::READ_MASK);
  if (result != 0)
    return result;

  result = ACE_Reactor::instance ()->register_handler (this,
                                                       ACE_Event_Handler::READ_MASK);

  if (result != 0)
    return result;

  conn_tag = shared->audioMaxPktSize;
  exit_tag = 0;

  this->mode_ = READ_HEADER; 
  pkt_data = (char *)packet + sizeof(*packet);
  temp = (char *)packet;
  bytes = -1;
  return 0;
}  

// following is feedback algorithm.
void 
AudioBuffer::feedback (void)
{
  if (shared->live || (!shared->config.syncEffective)) return;
  /*
    fprintf(stderr, "AB fbstate = %d\n", fbstate);
    */
  switch (fbstate) {
  case 0: /* startup init */
    if (pcmdsn != packet->cmdsn) {
      fbstate = 0;
      pcmdsn = packet->cmdsn;
    }
    else {
      fbstate = 1;
      waketime = get_usec() + STARTUP_WAIT;
    }
    break;
  case 1: /* startup wait */
    if (pcmdsn != packet->cmdsn) {
      fbstate = 0;
      break;
    }
    if (get_usec() >= waketime) {
      fbstate = 2;
    }
    break;
  case 2: /* monitoring */
    if (pcmdsn != packet->cmdsn) {
      fbstate = 0;
      break;
    }
    if (abuf->samples < abuf->size >>2 ||
        abuf->samples > (abuf->size * 3) >> 2) 
      {
        /* feedback action needed */
        AudioFeedBackPara para;
        int res;
        para.data.fb.addsps = 0;
        para.data.fb.addSamples = (abuf->size >> 2) - abuf->samples;
	
        Fprintf(stderr, "AB sends fb: %dsps, %dsamples\n",
                para.data.fb.addsps,
                para.data.fb.addSamples);
        para.cmdsn = htonl(shared->cmdsn);
        para.type = htonl(0);
        para.data.fb.addsps = htonl(para.data.fb.addsps);
        para.data.fb.addSamples = htonl(para.data.fb.addSamples);
        int result = ACE_Reactor::instance ()->register_handler (this,ACE_Event_Handler::WRITE_MASK);
        if (result != 0)
          ACE_DEBUG ((LM_DEBUG,"register_hanlder for write failed\n"));
        this->mode_ = WRITE_FEEDBACK1;
        temp = (char *)&para;
        bytes = sizeof (para);
        return;
      }
    break;
  default:
    break;
  }
}

ACE_HANDLE
Audio_Notification_Handler::get_handle (void) const
{
  return asp[1];
}
