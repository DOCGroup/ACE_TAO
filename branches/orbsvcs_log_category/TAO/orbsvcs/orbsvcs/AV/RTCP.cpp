/**
 * Copyright (c) 1994-1995 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by the University of
 *      California, Berkeley and the Network Research Group at
 *      Lawrence Berkeley Laboratory.
 * 4. Neither the name of the University nor of the Laboratory may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

// $Id$
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/AV/ntp-time.h"
#include "orbsvcs/AV/RTCP.h"
#include "orbsvcs/AV/media_timer.h"
#include "tao/debug.h"
#include "orbsvcs/AV/global.h"
#include "orbsvcs/AV/md5.h"

#include "orbsvcs/AV/RTCP_Packet.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_strings.h"
#include "ace/Truncate.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_AV_RTCP_Callback::receive_control_frame (ACE_Message_Block *data,
                                             const ACE_Addr &peer_address)
{
  int length = static_cast<int> (data->length ());
  int more = length;
  char *buf_ptr = data->rd_ptr ();
  char first_rtcp_packet = 1;
  RTCP_Channel_In *c;

  // This code performs the RTCP Header validity checks detailed in RFC 1889
  // Appendix A.2

  while (more > 0)
    {
      // the second byte of the control packet is the type
      switch ((unsigned char)buf_ptr[length - more + 1])
      {
        case RTCP_PT_SR:
          {
            RTCP_SR_Packet sr(&buf_ptr[length-more],
                              &more);

            if (!sr.is_valid(first_rtcp_packet))
              ORBSVCS_DEBUG ((LM_DEBUG,
                          "TAO_AV_RTCP_Callback::receive_control_frame - "
                          "warning invalid rtcp packet\n"));

            if (this->inputs_.find (sr.ssrc (), c) == -1)
              {
                ACE_NEW_RETURN (c,
                                RTCP_Channel_In (sr.ssrc (),
                                                 &peer_address),
                                -1);
                this->inputs_.bind (sr.ssrc (), c);
              }
            c->updateStatistics (&sr);

            if (TAO_debug_level > 0)
              sr.dump ();
            break;
          }
        case RTCP_PT_RR:
          {
            RTCP_RR_Packet rr(&buf_ptr[length-more],
                              &more);

            if (!rr.is_valid(first_rtcp_packet))
              ORBSVCS_DEBUG ((LM_DEBUG,
                          "TAO_AV_RTCP_Callback::receive_control_frame - "
                          "warning invalid rtcp packet\n"));

            if (this->inputs_.find (rr.ssrc (), c) == -1)
              {
                ACE_NEW_RETURN (c,
                                RTCP_Channel_In (rr.ssrc (),
                                                 &peer_address),
                                -1);
                this->inputs_.bind (rr.ssrc (), c);
              }

            c->updateStatistics (&rr);

            if (TAO_debug_level > 0)
              rr.dump ();
            break;
          }
        case RTCP_PT_SDES:
          {
            RTCP_SDES_Packet sdes (&buf_ptr[length-more],
                                   &more);

            if (!sdes.is_valid(first_rtcp_packet))
              ORBSVCS_DEBUG ((LM_DEBUG,
                          "TAO_AV_RTCP_Callback::receive_control_frame - "
                          "warning invalid rtcp packet\n"));

            if (TAO_debug_level > 0)
              sdes.dump ();
            break;
          }
        case RTCP_PT_BYE:
          {
            RTCP_BYE_Packet bye (&buf_ptr[length-more],
                                 &more);

            if (!bye.is_valid(first_rtcp_packet))
              ORBSVCS_DEBUG ((LM_DEBUG,
                          "TAO_AV_RTCP_Callback::receive_control_frame - "
                          "warning invalid rtcp packet\n"));

            // Inform the listener that a source(s) has left the session
            ACE_UINT32 *ssrc_list;
            unsigned char length;

            bye.ssrc_list(&ssrc_list, length);

            for (int i=0; i<length; i++)
              {
                RTCP_Channel_In *c = 0;

                // remove the channel from the list
                this->inputs_.unbind(ssrc_list[i], c);

                if (c != 0)
                  delete c;
              }

            if (TAO_debug_level > 0)
              bye.dump ();

            break;
          }
        case RTCP_PT_APP:
          // If we receive one of these, ignore it.
          ORBSVCS_DEBUG ((LM_DEBUG,
                      "TAO_AV_RTCP_Callback::receive_control_frame - "
                      "APP packet - ignore\n"));
          more -= (4 + (ACE_UINT16)buf_ptr[length - more + 2]);
          break;
        default:
          ORBSVCS_DEBUG ((LM_DEBUG,
                      "TAO_AV_RTCP_Callback::receive_control_frame - "
                      "UNKNOWN packet type %u; ignore the rest\n",
                      (int)buf_ptr[length - more + 1]));
          more = 0;
      }

      first_rtcp_packet = 0;

    }

    if (more != 0)
      ORBSVCS_DEBUG ((LM_DEBUG,
                  "TAO_AV_RTCP_Callback::receive_control_frame - "
                  "Error in overall packet length\n"));
    return 0;
}

ACE_INT32 random32 (int);

ACE_UINT32
TAO_AV_RTCP::alloc_srcid (ACE_UINT32 addr)
{
  md5_string s;

  s.type = addr;
  s.tv = ACE_OS::gettimeofday ();
  s.pid = ACE_OS::getpid();
  s.pgid = ACE_OS::getpgid(s.pid);
  s.ppid = ACE_OS::getppid();
  s.uid = ACE_OS::getuid();
  s.gid = ACE_OS::getgid();

  unsigned char *string_val = (unsigned char *) &s;
  int length = sizeof(s);

  MD5_CTX context;
  union
    {
      char c[16];
      u_long x[4];
    } digest;
  ACE_UINT32 r;
  int i;

  MD5Init (&context);
  MD5Update (&context, string_val, length);
  MD5Final ((unsigned char*)&digest, &context);
  r=0;
  for (i=0; i<3; i++)
    r ^= digest.x[i];

  return r;

/* used to be this
  ACE_Time_Value tv = ACE_OS::gettimeofday ();
  ACE_UINT32 srcid = ACE_UINT32 (tv.sec () + tv.usec ());
  srcid += (ACE_UINT32)ACE_OS::getuid();
  srcid += (ACE_UINT32)ACE_OS::getpid();
  srcid += addr;
  return (srcid);
*/
}


double
TAO_AV_RTCP::rtcp_interval (int members,
                            int senders,
                            double rtcp_bw,
                            int we_sent,
                            int packet_size,
                            int *avg_rtcp_size,
                            int initial)
{
  // Minimum time between RTCP packets from this site (in sec.).
  // This time prevents the reports from 'clumping' when sessions
  // are small and the law of large numbers isn't helping to smooth
  // out the traffic.  It also keeps the report interval from
  // becoming ridiculously small during transient outages like a
  // network partition.
//  double const RTCP_MIN_TIME = 5.0;   (from RTP.h)

  // Fraction of the RTCP bandwidth to be shared among active
  // senders.  (This fraction was chosen so that in a typical
  // session with one or two active senders, the computed report
  // time would be roughly equal to the minimum report time so that
  // we don't unnecessarily slow down receiver reports.) The
  // receiver fraction must be 1 - the sender fraction.
//  double const RTCP_SENDER_BW_FRACTION = 0.25;   (from RTP.h)
//  double const RTCP_RCVR_BW_FRACTION = (1-RTCP_SENDER_BW_FRACTION); (from RTP.h)

  // Gain (smoothing constant) for the low-pass filter that
  // estimates the average RTCP packet size
//  double const RTCP_SIZE_GAIN = (1.0/16.0);   (from RTP.h)

  double t;
  double rtcp_min_time = RTCP_MIN_RPT_TIME;
  int n;   // number of members for computation

  // Very first call at application start-up uses half the min
  // delay for quicker notification while still allowing some time
  // before reporting for randomization and to learn about other
  // sources so the report interval will converge to the correct
  // interval more quickly.  The average RTCP size is initialized
  // to 128 octets which is conservative (it assumes everyone else
  // is generating SRs instead of RRs: 20 IP + 8 UDP + 52 SR + 48
  // SDES CNAME).
  if (initial)
    {
      // initialize the random number generator
      ACE_OS::srand(ACE_Utils::truncate_cast<u_int> (ACE_OS::time(0L)));

      rtcp_min_time /= 2;
      *avg_rtcp_size = 128;
    }

  // If there were active senders, give them at least a minimum
  // share of the RTCP bandwidth.  Otherwise all participants share
  // the RTCP bandwidth equally.
  n = members;
  if ((senders > 0) && (senders < members*RTCP_SENDER_BW_FRACTION))
    {
      if (we_sent)
        {
          rtcp_bw *= RTCP_SENDER_BW_FRACTION;
          n = senders;
        }
      else
        {
          rtcp_bw *= RTCP_RECEIVER_BW_FRACTION;
          n -= senders;
        }
    }

  // Update the average size estimate by the size of the report
  // packet we just sent.
  *avg_rtcp_size += (int)((packet_size - *avg_rtcp_size)*RTCP_SIZE_GAIN);

  // The effective number of sites times the average packet size is
  // the total number of octets sent when each site sends a report.
  // Dividing this by the effective bandwidth gives the time
  // interval over which those packets must be sent in order to
  // meet the bandwidth target, with a minimum enforced.  In that
  // time interval we send one report so this time is also our
  // average time between reports.
  t = (*avg_rtcp_size) * n / rtcp_bw;
  if (t < rtcp_min_time)
    t = rtcp_min_time;

  // To avoid traffic bursts from unintended synchronization with
  // other sites, we then pick our actual next report interval as a
  // random number uniformly distributed between 0.5*t and 1.5*t.

  // TODO: this may not be right.  need a random number between 0 and 1
  int max_rand = 32768;

  return t * ((double)ACE_OS::rand()/max_rand + 0.5);
//  return t * (drand48() + 0.5);
}



// TAO_AV_RTCP_Flow_Factory
TAO_AV_RTCP_Flow_Factory::TAO_AV_RTCP_Flow_Factory (void)
{
}

TAO_AV_RTCP_Flow_Factory::~TAO_AV_RTCP_Flow_Factory (void)
{
}

int
TAO_AV_RTCP_Flow_Factory::match_protocol (const char *flow_string)
{
  if (ACE_OS::strncasecmp (flow_string,"RTCP",4) == 0)
      return 1;

  return 0;
}

int
TAO_AV_RTCP_Flow_Factory::init (int /* argc */,
                                ACE_TCHAR * /* argv */ [])
{
  return 0;
}

TAO_AV_Protocol_Object*
TAO_AV_RTCP_Flow_Factory::make_protocol_object (TAO_FlowSpec_Entry * /*entry*/,
                                                TAO_Base_StreamEndPoint * /*endpoint*/,
                                                TAO_AV_Flow_Handler *handler,
                                                TAO_AV_Transport *transport)
{
  TAO_AV_Callback *client_cb = 0;
  TAO_AV_RTCP_Callback *rtcp_cb = 0;

  // TODO: need to handle a client callback at some point
//  endpoint->get_control_callback (entry->flowname (),
//                                  client_cb);

  TAO_AV_Protocol_Object *object = 0;
  ACE_NEW_RETURN (object,
                  TAO_AV_RTCP_Object (client_cb,
                                      rtcp_cb,
                                      transport),
                  0);

  rtcp_cb->open (object, handler);

  return object;
}

// TAO_AV_RTCP_Object
int
TAO_AV_RTCP_Object::handle_input (void)
{
  size_t bufsiz = 2*this->transport_->mtu ();
  ACE_Message_Block data (bufsiz);

  int n = this->transport_->recv (data.rd_ptr (),bufsiz);
  if (n == 0)
    {
      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_ERROR, "TAO_AV_RTCP::handle_input:connection closed\n"));
      return -1;
    }
  if (n < 0)
    {
      if (TAO_debug_level > 0)
        ORBSVCS_DEBUG ((LM_ERROR,"TAO_AV_RTCP::handle_input:recv error\n"));
      return -1;
    }
  data.wr_ptr (n);
  ACE_Addr *peer_addr = this->transport_->get_peer_addr ();
  this->callback_->receive_control_frame (&data,*peer_addr);
  return 0;
}

int
TAO_AV_RTCP_Object::send_frame (ACE_Message_Block *frame,
                                TAO_AV_frame_info * /*frame_info*/)
{
  return this->transport_->send (frame);
}

int
TAO_AV_RTCP_Object::send_frame (const iovec *iov,
                               int iovcnt,
                                TAO_AV_frame_info * /*frame_info*/)
{
  return this->transport_->send (iov,
                                 iovcnt);
}

int
TAO_AV_RTCP_Object::send_frame (const char*,
                                size_t)
{
  return 0;
}

TAO_AV_RTCP_Object::TAO_AV_RTCP_Object (TAO_AV_Callback *client_cb,
                                        TAO_AV_RTCP_Callback *&rtcp_cb,
                                        TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (&rtcp_cb_, transport)
{
  rtcp_cb = &this->rtcp_cb_;
  this->client_cb_ = client_cb;

}

TAO_AV_RTCP_Object::~TAO_AV_RTCP_Object (void)
{
}

int
TAO_AV_RTCP_Object::destroy (void)
{
  this->callback_->handle_destroy ();
  delete this;

  return 0;
}

int
TAO_AV_RTCP_Object::set_policies (const TAO_AV_PolicyList &/*policy_list*/)
{
  return -1;
}

int
TAO_AV_RTCP_Object::start (void)
{
  return this->callback_->handle_start ();
}

int
TAO_AV_RTCP_Object::stop (void)
{
  return this->callback_->handle_stop ();
}

int
TAO_AV_RTCP_Object::handle_control_input (ACE_Message_Block *frame,
                                          const ACE_Addr &peer_address)
{
  return this->callback_->receive_frame (frame,
                                         0,
                                         peer_address);
}

int
TAO_AV_RTCP_Object::handle_control_output (ACE_Message_Block *frame)
{
  TAO_AV_RTCP_Callback *cb = dynamic_cast<TAO_AV_RTCP_Callback*> (this->callback_);

  return cb->send_frame (frame);
}

void
TAO_AV_RTCP_Object::ts_offset (ACE_UINT32 ts_offset)
{
  TAO_AV_RTCP_Callback *cb = dynamic_cast<TAO_AV_RTCP_Callback*> (this->callback_);
  cb->ts_offset (ts_offset);
}

// TAO_AV_RTCP_Callback
TAO_AV_RTCP_Callback::TAO_AV_RTCP_Callback (void)
  :is_initial_timeout_(1),
   packet_size_(0)
{
  char cname[256];
  char host[256];
  ACE_OS::hostname(host, sizeof(host));

  // TODO: determine username auto-magically?
  ACE_OS::sprintf(cname, "username@%s", host);

  this->output_.cname(cname);
}

TAO_AV_RTCP_Callback::~TAO_AV_RTCP_Callback (void)
{
}

void
TAO_AV_RTCP_Callback::schedule (int ms)
{
  this->timeout_ = ms;
}

int
TAO_AV_RTCP_Callback::handle_start (void)
{
  return 0;
}

int
TAO_AV_RTCP_Callback::handle_stop (void)
{
  return this->send_report(1);
}

int
TAO_AV_RTCP_Callback::handle_timeout (void * /*arg*/)
{
  return this->send_report(0);
}

int
TAO_AV_RTCP_Callback::send_report (int bye)
{
  // get the RTCP control object in order to get the ssrc
  TAO_AV_RTCP_Object *rtcp_prot_obj = dynamic_cast<TAO_AV_RTCP_Object*> (this->protocol_object_);
  ACE_UINT32 my_ssrc = rtcp_prot_obj->ssrc ();

  RTCP_Packet *cp;
  RTCP_SDES_Packet sdes;
  ACE_CString value = "";
  ACE_CString note = "";
  unsigned char sdes_type = 0;
  RTCP_BYE_Packet *bye_packet = 0;  // only used for bye
  ACE_UINT32 ssrc_list[1];          // only used for bye

  // get an iterator for the incoming channels.
  ACE_Hash_Map_Iterator<ACE_UINT32, RTCP_Channel_In*, ACE_Null_Mutex> iter (this->inputs_);
  iter = this->inputs_.begin();

  // first send an SR/RR
  RR_Block *blocks = 0;
  RR_Block *b_iter = 0;
  RR_Block *b_ptr = 0;

  while (iter != this->inputs_.end() )
    {
      if (!b_iter)
      {
        b_ptr = (*iter).int_id_->getRRBlock ();
        if (b_ptr)
          {
            blocks = b_ptr;
            b_iter = b_ptr;
          }
      }
      else
      {
        b_ptr = (*iter).int_id_->getRRBlock ();
        if (b_ptr)
          {
            b_iter->next_ = b_ptr;
          }
      }

      iter++;
    }

  if (b_iter)
    b_iter->next_ = 0;

  if (this->output_.active ())
    {
      // get the NTP timestamp
      ACE_Time_Value unix_now = ACE_OS::gettimeofday ();
      TAO_AV_RTCP::ntp64 ntp_now = ntp64time (unix_now);
      ACE_UINT32 rtp_ts = ACE_Utils::truncate_cast<ACE_UINT32> (
                            unix_now.sec () * 8000 + unix_now.usec () / 125 +
                            this->timestamp_offset_);
      ACE_NEW_RETURN(cp,
                     RTCP_SR_Packet (my_ssrc,
                                     ntp_now.upper,
                                     ntp_now.lower,
                                     rtp_ts,
                                     this->output_.packets_sent (),
                                     this->output_.octets_sent (),
                                     blocks),
                     -1);
    }
  else
    {
      ACE_NEW_RETURN(cp,
                     RTCP_RR_Packet (my_ssrc,
                                     blocks),
                     -1);
    }

  /*
   * We always send a cname plus one other sdes
   * There's a schedule for what we send sequenced by sdes_seq_:
   *   - send 'email' every 0th & 4th packet
   *   - send 'note' every 2nd packet
   *   - send 'tool' every 6th packet
   *   - send 'name' in all the odd slots
   * (if 'note' is not the empty string, we switch the roles
   *  of name & note)
   */

  // TODO: need capability to change these settings
  switch (this->sdes_count_%8)
  {
    case 0:
    case 4:
      value = "tao-users@wustl.edu";
      sdes_type = RTCP_SDES_EMAIL;
      break;
    case 2:
      if (note.length () > 0)
        {
          value = "Joe User";
          sdes_type = RTCP_SDES_NAME;
        }
      else
        {
          value = "An important note...";
          sdes_type = RTCP_SDES_NOTE;
        }
      break;
    case 6:
      value = "TAO A/V Service";
      sdes_type = RTCP_SDES_TOOL;
      break;
    case 1:
    case 3:
    case 5:
    case 7:
      if (note.length () == 0)
        {
          value = "Joe User";
          sdes_type = RTCP_SDES_NAME;
        }
      else
        {
          value = "An important note...";
          sdes_type = RTCP_SDES_NOTE;
        }
      break;
  }

  ++this->sdes_count_;

  sdes.add_item (my_ssrc,
                 RTCP_SDES_CNAME,
                 static_cast<unsigned char> (ACE_OS::strlen(this->output_.cname())),
                 this->output_.cname());
  if (bye)
    {
      ssrc_list[0] = rtcp_prot_obj->ssrc ();

      ACE_NEW_RETURN (bye_packet,
                      RTCP_BYE_Packet(ssrc_list,
                                      sizeof(ssrc_list)/sizeof(ssrc_list[0]),
                                      "Got bored."),
                      -1);
    }
  else
    {
      unsigned char length = (unsigned char)(value.length() & 0xFF);
      sdes.add_item (my_ssrc, sdes_type, length, value.c_str ());
    }

  // create the message block
  char *cp_ptr;
  char *sdes_ptr;
  char *bye_ptr = 0;
  ACE_UINT16 cp_length;
  ACE_UINT16 sdes_length;
  ACE_UINT16 bye_length = 0;
  cp->get_packet_data (&cp_ptr, cp_length);
  sdes.get_packet_data (&sdes_ptr, sdes_length);
  if (bye_packet)
    bye_packet->get_packet_data(&bye_ptr, bye_length);

  ACE_Message_Block mb (cp_length + sdes_length + bye_length);

  ACE_OS::memcpy (mb.wr_ptr (), cp_ptr, cp_length);
  mb.wr_ptr (cp_length);
  ACE_OS::memcpy (mb.wr_ptr (), sdes_ptr, sdes_length);
  mb.wr_ptr (sdes_length);
  if (bye_length)
    {
      ACE_OS::memcpy (mb.wr_ptr (), bye_ptr, bye_length);
      mb.wr_ptr (bye_length);
    }

  // send the report
  this->protocol_object_->send_frame (&mb);

  this->packet_size_ = cp_length + sdes_length + bye_length;

  delete cp;
  if (bye_packet)
    delete bye_packet;

  return 0;
}

void
//TAO_AV_RTCP_Callback::get_timeout (ACE_Time_Value *tv,
TAO_AV_RTCP_Callback::get_timeout (ACE_Time_Value *&tv,
                                   void *& /*arg*/)
{
  int senders = 0;
  int members = 1;  // count self as member

  // TODO: this should be 5% of the session bw
  double rtcp_bw = 1000;
  double interval;

  ACE_Hash_Map_Iterator<ACE_UINT32, RTCP_Channel_In*, ACE_Null_Mutex> iter (this->inputs_);
  iter = this->inputs_.begin();

  if (this->output_.active ())
    senders++;

  // determine the number of senders and members of this session
  while (iter != this->inputs_.end ())
    {
      if ((*iter).int_id_->active ())
        {
          if ((*iter).int_id_->sender ())
            senders++;
          members++;
        }
      iter++;
    }

  // Here we do the RTCP timeout calculation.
  interval = TAO_AV_RTCP::rtcp_interval (members,                  // members
                                         senders,                  // senders
                                         rtcp_bw,                  // rtcp_bw
                                         this->output_.active (),  // we_sent
                                         this->packet_size_,       // packet_size
                                         &this->avg_rtcp_size_,    // avg_rtcp_size
                                         this->is_initial_timeout_);  // initial)

  this->is_initial_timeout_ = 0;

  ACE_NEW (tv,
           ACE_Time_Value);

  tv->sec ((int)interval);
  tv->usec ((int)((interval - (int)interval) * 1000000));
}

int
TAO_AV_RTCP_Callback::handle_destroy (void)
{
  return 0;
}

int
TAO_AV_RTCP_Callback::receive_frame (ACE_Message_Block *frame,
                                     TAO_AV_frame_info *,
                                     const ACE_Addr &peer_address)
{
  RTCP_Channel_In *c;

  RTP_Packet packet (frame->rd_ptr(), static_cast<int> (frame->length()));

  if (this->inputs_.find (packet.ssrc(), c) < 0)
    {
      ACE_NEW_RETURN (c,
                      RTCP_Channel_In (packet.ssrc(),
                                       &peer_address),
                      -1);

      this->inputs_.bind (packet.ssrc(), c);
    }

  c->recv_rtp_packet (frame, &peer_address);
  return 0;
}

int
TAO_AV_RTCP_Callback::send_frame (ACE_Message_Block *frame)
{
  RTP_Packet packet (frame->rd_ptr(), static_cast<int> (frame->length()));
  this->output_.updateStatistics (&packet);

  return 0;
}

void
TAO_AV_RTCP_Callback::ts_offset (ACE_UINT32 offset)
{
  this->timestamp_offset_ = offset;
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_AV, TAO_AV_RTCP_Flow_Factory)
ACE_STATIC_SVC_DEFINE (TAO_AV_RTCP_Flow_Factory,
                       ACE_TEXT ("RTCP_Flow_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_AV_RTCP_Flow_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)

