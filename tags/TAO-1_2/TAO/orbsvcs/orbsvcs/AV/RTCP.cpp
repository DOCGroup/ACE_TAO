/*
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
#include "ntp-time.h"
#include "RTCP.h"
#include "media-timer.h"
#include "tao/debug.h"


// TAO_AV_RTP_State
TAO_AV_RTP_State::TAO_AV_RTP_State (void)
  :bad_version_(0),
   badoptions_(0),
   badfmt_(0),
   badext_(0),
   nrunt_(0),
   last_np_(0),
   sdes_seq_(0),
   rtcp_inv_bw_(0.),
   rtcp_avg_size_(128.),
   confid_(-1)
{
  ACE_NEW (pktbuf_,
           u_char [2 * RTP_MTU]);
}

//------------------------------------------------------------
// TAO_AV_RTCP
//------------------------------------------------------------

u_char*
TAO_AV_RTCP::build_sdes_item (u_char* p,
                              int code,
                              TAO_AV_Source& s)
{
  const char* value = s.sdes (code);
  if (value != 0)
    {
      int len = strlen (value);
      *p++ = code;
      *p++ = len;
      memcpy (p, value, len);
      p += len;
    }
  return (p);
}

int
TAO_AV_RTCP::build_sdes (rtcphdr* rh,
                         TAO_AV_Source& ls,
                         TAO_AV_RTP_State *state)
{
  int flags = RTP_VERSION << 14 | 1 << 8 | RTCP_PT_SDES;
  rh->rh_flags = htons (flags);
  rh->rh_ssrc = ls.srcid ();
  u_char* p = (u_char*) (rh + 1);
  p = build_sdes_item (p, RTCP_SDES_CNAME, ls);

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
  int nameslot, noteslot;
  const char* note = ls.sdes (RTCP_SDES_NOTE);
  if (note)
    {
      if (*note)
        {
          nameslot = RTCP_SDES_NOTE;
          noteslot = RTCP_SDES_NAME;
        } else
          {
            nameslot = RTCP_SDES_NAME;
            noteslot = RTCP_SDES_NOTE;
          }
    }
  else
    {
      nameslot = RTCP_SDES_NAME;
      noteslot = RTCP_SDES_NAME;
    }
  u_int seq = (++state->sdes_seq_) & 0x7;
  switch (seq)
    {
    case 0:
    case 4:
      p = build_sdes_item (p, RTCP_SDES_EMAIL, ls);
      break;
    case 2:
      p = build_sdes_item (p, noteslot, ls);
      break;
    case 6:
      p = build_sdes_item (p, RTCP_SDES_TOOL, ls);
      break;
    default:
      p = build_sdes_item (p, nameslot, ls);
    }

  int len = p - (u_char*)rh;
  int pad = 4 - (len & 3);
  len += pad;
  rh->rh_len = htons ( (len >> 2) - 1);
  while (--pad >= 0)
    *p++ = 0;

  return (len);
}

int
TAO_AV_RTCP::build_bye (rtcphdr* rh,
                        TAO_AV_Source& ls)
{
  int flags =
    RTP_VERSION << 14 | 1 << 8 | RTCP_PT_BYE;
  rh->rh_flags = ntohs (flags);
  rh->rh_len = htons (1);
  rh->rh_ssrc = ls.srcid ();
  return (8);
}

void
TAO_AV_RTCP::parse_rr_records (ACE_UINT32,
                              rtcp_rr*,
                              int,
                              const u_char*,
                              ACE_UINT32)
{
  if (TAO_debug_level > 0) ACE_DEBUG ( (LM_DEBUG,"TAO_AV_RTCP::parse_rr_records\n"));
}

void
TAO_AV_RTCP::parse_sr (rtcphdr* rh,
                       int flags,
                       u_char*ep,
                       TAO_AV_Source* ps,
                       ACE_UINT32 addr,
                       TAO_AV_SourceManager *source_manager)
{
  rtcp_sr* sr = ACE_reinterpret_cast (rtcp_sr*, (rh + 1));
  TAO_AV_Source* s;
  ACE_UINT32 ssrc = rh->rh_ssrc;
  if (ps->srcid () != ssrc)
    s = source_manager->lookup (ssrc, ssrc, addr);
  else
    s = ps;

  s->lts_ctrl (ACE_OS::gettimeofday ());
  s->sts_ctrl (ntohl (sr->sr_ntp.upper) << 16 |
              ntohl (sr->sr_ntp.lower) >> 16);

  int cnt = flags >> 8 & 0x1f;
  parse_rr_records (ssrc, ACE_reinterpret_cast (rtcp_rr*, (sr + 1)), cnt, ep, addr);
}

void
TAO_AV_RTCP::parse_rr (rtcphdr* rh,
                       int flags,
                       u_char* ep,
                       TAO_AV_Source* ps,
                       ACE_UINT32 addr,
                       TAO_AV_SourceManager *source_manager)
{
  TAO_AV_Source* s;
  ACE_UINT32 ssrc = rh->rh_ssrc;
  if (ps->srcid () != ssrc)
    s = source_manager->lookup (ssrc, ssrc, addr);
  else
    s = ps;

  s->lts_ctrl (ACE_OS::gettimeofday ());
  int cnt = flags >> 8 & 0x1f;
  parse_rr_records (ssrc, ACE_reinterpret_cast (rtcp_rr*, (rh + 1)), cnt, ep, addr);
}

int
TAO_AV_RTCP::sdesbody (ACE_UINT32* p,
                       u_char* ep,
                       TAO_AV_Source* ps,
                       ACE_UINT32 addr,
                       ACE_UINT32 ssrc,
                       TAO_AV_SourceManager *source_manager)
{
  TAO_AV_Source* s;
  ACE_UINT32 srcid = *p;
  if (ps->srcid () != srcid)
    s = source_manager->lookup (srcid, ssrc, addr);
  else
    s = ps;
  if (s == 0)
    return (0);
  /*
   * Note ctrl packet since we will never see any direct ctrl packets
   * from a TAO_AV_Source through a mixer (and we don't want the TAO_AV_Source to
   * time out).
   */
  s->lts_ctrl (ACE_OS::gettimeofday ());

  u_char* cp = (u_char*) (p + 1);
  while (cp < ep)
    {
      char buf[256];

      u_int type = cp[0];
      if (type == 0)
        {
          /* end of chunk */
          return ( ( (cp - (u_char*)p) >> 2) + 1);
        }
      u_int len = cp[1];
      u_char* eopt = cp + len + 2;
      if (eopt > ep)
        return (0);

      if (type >= RTCP_SDES_MIN && type <= RTCP_SDES_MAX)
        {
          memcpy (buf, (char*)&cp[2], len);
          buf[len] = 0;
          s->sdes (type, buf);
        }
      else
        /*XXX*/;

      cp = eopt;
    }
  return (0);
}

void
TAO_AV_RTCP::parse_sdes (rtcphdr* rh,
                         int flags,
                         u_char* ep,
                         TAO_AV_Source* ps,
                         ACE_UINT32 addr,
                         ACE_UINT32 ssrc,
                         TAO_AV_SourceManager *source_manager)
{
  int cnt = flags >> 8 & 0x1f;
  ACE_UINT32* p = (ACE_UINT32*)&rh->rh_ssrc;
  while (--cnt >= 0)
    {
      int n = TAO_AV_RTCP::sdesbody (p, ep, ps, addr, ssrc, source_manager);
      if (n == 0)
        break;
      p += n;
    }
  if (cnt >= 0)
    ps->badsdes (1);
}

void
TAO_AV_RTCP::parse_bye (rtcphdr* rh,
                        int flags,
                        u_char* ep,
                        TAO_AV_Source* ps,
                        TAO_AV_SourceManager *source_manager)
{
  int cnt = flags >> 8 & 0x1f;
  ACE_UINT32* p = (ACE_UINT32*)&rh->rh_ssrc;

  while (--cnt >= 0)
    {
      if (p >= (ACE_UINT32*)ep)
        {
          ps->badbye (1);
          return;
        }
      TAO_AV_Source* s;
      if (ps->srcid () != rh->rh_ssrc)
        s = source_manager->consult (*p);
      else
        s = ps;
      if (s != 0)
        s->lts_done (ACE_OS::gettimeofday ());
      ++p;
    }
}

/*XXX check for buffer overflow*/
/*
 * Send an RTPv2 report packet.
 */
void
TAO_AV_RTCP::send_report (int bye,
                          TAO_AV_Protocol_Object *protocol_object,
                          TAO_AV_SourceManager *source_manager,
                          TAO_AV_RTP_State *state,
                          TAO_AV_RTCP_Callback *callback)
{
  if (source_manager->localsrc () == 0)
    return;
  TAO_AV_Source& s = *source_manager->localsrc ();
  rtcphdr* rh = (rtcphdr*)state->pktbuf_;
  rh->rh_ssrc = s.srcid ();
  int flags = RTP_VERSION << 14;
  timeval now = ACE_OS::gettimeofday ();
  s.lts_ctrl (now);
  int we_sent = 0;
  rtcp_rr* rr;
  /*
   * If we've sent data since our last sender report send a
   * new report.  The MediaTimer check is to make sure we still
   * have a grabber -- if not, we won't be able to interpret the
   * media timestamps so there's no point in sending an SR.
   */
  MediaTimer* mt = MediaTimer::instance ();
  if (s.np () != state->last_np_ && mt) {
    state->last_np_ = s.np ();
    we_sent = 1;
    flags |= RTCP_PT_SR;
    rtcp_sr* sr = ACE_reinterpret_cast (rtcp_sr*, (rh + 1));
    sr->sr_ntp = ntp64time (now);
    sr->sr_ntp.upper = ACE_HTONL (sr->sr_ntp.upper);
    sr->sr_ntp.lower = ACE_HTONL (sr->sr_ntp.lower);
    sr->sr_ts = htonl (mt->ref_ts ());
    sr->sr_np = htonl (s.np ());
    sr->sr_nb = htonl (s.nb ());
    rr = ACE_reinterpret_cast (rtcp_rr*, (sr + 1));
  } else {
    flags |= RTCP_PT_RR;
    rr = ACE_reinterpret_cast (rtcp_rr*, (rh + 1));
  }
  int nrr = 0;
  int nsrc = 0;
  /*
         * we don't want to inflate report interval if user has set
         * the flag that causes all TAO_AV_Sources to be 'kept' so we
         * consider TAO_AV_Sources 'inactive' if we haven't heard a control
         * msg from them for ~32 reporting intervals.
         */
  u_int inactive = u_int (state->rint_ * (32./1000.));
  if (inactive < 2)
    inactive = 2;
  for (TAO_AV_Source* sp = source_manager->sources (); sp != 0; sp = sp->next_) {
    ++nsrc;
    int received = sp->np () - sp->snp ();
    if (received == 0) {
      if (u_int (now.tv_sec - sp->lts_ctrl ().tv_sec) > inactive)
        --nsrc;
      continue;
    }
    sp->snp (sp->np ());
    rr->rr_srcid = sp->srcid ();
    int expected = sp->ns () - sp->sns ();
    sp->sns (sp->ns ());
    ACE_UINT32 v;
    int lost = expected - received;
    if (lost <= 0)
      v = 0;
    else
      /* expected != 0 if lost > 0 */
      v = ( (lost << 8) / expected) << 24;
    /* XXX should saturate on over/underflow */
    v |= (sp->ns () - sp->np ()) & 0xffffff;
    rr->rr_loss = htonl (v);
    rr->rr_ehsr = htonl (sp->ehs ());
    rr->rr_dv = sp->delvar ();
    rr->rr_lsr = htonl (sp->sts_ctrl ());
    if (sp->lts_ctrl ().tv_sec == 0)
      rr->rr_dlsr = 0;
    else {
      ACE_UINT32 ntp_now = ntptime (now);
      ACE_UINT32 ntp_then = ntptime (sp->lts_ctrl ());
      rr->rr_dlsr = htonl (ntp_now - ntp_then);
    }
    ++rr;
    if (++nrr >= 31)
      break;
  }
  flags |= nrr << 8;
  rh->rh_flags = htons (flags);
  int len = (u_char*)rr - state->pktbuf_;
  rh->rh_len = htons ( (len >> 2) - 1);

  if (bye)
    len += build_bye ( ACE_reinterpret_cast (rtcphdr*,rr), s);
  else
    len += build_sdes ( ACE_reinterpret_cast (rtcphdr*, rr), s,state);

  ACE_Message_Block mb ((char *)state->pktbuf_, len);
  mb.wr_ptr (len);
  protocol_object->send_frame (&mb);

  state->rtcp_avg_size_ += RTCP_SIZE_GAIN * (double (len + 28) - state->rtcp_avg_size_);

        /*
         * compute the time to the next report.  we do this here
         * because we need to know if there were any active TAO_AV_Sources
         * during the last report period (nrr above) & if we were
         * a TAO_AV_Source.  The bandwidth limit for rtcp traffic was set
         * on startup from the session bandwidth.  It is the inverse
         * of bandwidth (ie., ms/byte) to avoid a divide below.
         */
  double ibw = state->rtcp_inv_bw_;
  if (nrr) {
    /* there were active TAO_AV_Sources */
    if (we_sent) {
      ibw *= 1./RTCP_SENDER_BW_FRACTION;
      nsrc = nrr;
    } else {
      ibw *= 1./RTCP_RECEIVER_BW_FRACTION;
      nsrc -= nrr;
    }
  }
  double rint = state->rtcp_avg_size_ * double (nsrc) * ibw;
  if (rint < RTCP_MIN_RPT_TIME * 1000.)
    rint = RTCP_MIN_RPT_TIME * 1000.;
  state->rint_ = rint;
  callback->schedule (int (TAO_AV_RTCP::fmod (double (ACE_OS::rand ()), rint) + rint * .5 + .5));

  source_manager->CheckActiveSources (rint);
}

int
TAO_AV_RTCP::handle_input (ACE_Message_Block *data,
                           const ACE_Addr &peer_address,
                           rtcphdr &header,
                           TAO_AV_SourceManager *source_manager,
                           TAO_AV_RTP_State *state)
{
  int cc = data->length ();
  int size_phdr = ACE_static_cast (int, sizeof (rtcphdr));
  if (cc < size_phdr)
    {
      state->nrunt_++;
      ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::handle_input:invalid header\n"),-1);
    }
  if (peer_address == ACE_Addr::sap_any)
    ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::handle_input:get_peer_addr failed\n"),-1);
  // @@ We need to be careful of this.
  u_long addr = peer_address.hash ();
  header = * (rtcphdr*) (data->rd_ptr ());
  rtcphdr *rh = (rtcphdr *)data->rd_ptr ();
  /*
         * try to filter out junk: first thing in packet must be
         * sr, rr or bye & version number must be correct.
         */
  switch (ntohs (rh->rh_flags) & 0xc0ff)
    {
    case RTP_VERSION << 14 | RTCP_PT_SR:
    case RTP_VERSION << 14 | RTCP_PT_RR:
    case RTP_VERSION << 14 | RTCP_PT_BYE:
      break;
    default:
      /*
       * XXX should further categorize this error -- it is
       * likely that people mis-implement applications that
       * don't put something other than SR,RR,BYE first.
       */
      ++state->bad_version_;
      return -1;
    }
  /*
   * at this point we think the packet's valid.  Update our average
   * size estimator.  Also, there's valid ssrc so charge errors to it
   */


  double tmp = (cc + 28) - state->rtcp_avg_size_;
  tmp *= RTCP_SIZE_GAIN;
  state->rtcp_avg_size_ += ACE_static_cast (int, tmp);
  /*
   * First record in compound packet must be the ssrc of the
   * sender of the packet.  Pull it out here so we can use
   * it in the sdes parsing, since the sdes record doesn't
   * contain the ssrc of the sender (in the case of mixers).
   */
  ACE_UINT32 ssrc = rh->rh_ssrc;
  TAO_AV_Source* ps = source_manager->lookup (ssrc, ssrc, addr);
  if (ps == 0)
    return 0;

        /*
         * Outer loop parses multiple RTCP records of a "compound packet".
         * There is no framing between records.  Boundaries are implicit
         * and the overall length comes from UDP.
         */
  u_char* epack = (u_char*)rh + cc;
  while ( (u_char*)rh < epack) {
    u_int len = (ntohs (rh->rh_len) << 2) + 4;
    u_char* ep = (u_char*)rh + len;
    if (ep > epack) {
      ps->badsesslen (1);
      return 0;
    }
    u_int flags = ntohs (rh->rh_flags);
    if (flags >> 14 != RTP_VERSION) {
      ps->badsessver (1);
      return 0;
    }
    switch (flags & 0xff) {

    case RTCP_PT_SR:
      TAO_AV_RTCP::parse_sr (rh, flags, ep, ps, addr, source_manager);
      break;

    case RTCP_PT_RR:
      TAO_AV_RTCP::parse_rr (rh, flags, ep, ps, addr, source_manager);
      break;

    case RTCP_PT_SDES:
      TAO_AV_RTCP::parse_sdes (rh, flags, ep, ps, addr, ssrc, source_manager);
      break;

    case RTCP_PT_BYE:
      TAO_AV_RTCP::parse_bye (rh, flags, ep, ps, source_manager);
      break;

    default:
      ps->badsessopt (1);
      break;
    }
    rh = (rtcphdr*)ep;
  }
  return 0;
}

ACE_UINT32
TAO_AV_RTCP::alloc_srcid (ACE_UINT32 addr)
{
  ACE_Time_Value tv = ACE_OS::gettimeofday ();
  ACE_UINT32 srcid = ACE_UINT32 (tv.sec () + tv.usec ());
  srcid += (ACE_UINT32)ACE_OS::getuid();
  srcid += (ACE_UINT32)ACE_OS::getpid();
  srcid += addr;
  return (srcid);
}


double
TAO_AV_RTCP::fmod (double dividend, double divisor)
{
  //Method to calculate the fmod (x,y)
  int quotient = ACE_static_cast (int, (dividend / divisor));
  double product = quotient * divisor;
  double remainder = dividend - product;
  return remainder;
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
                                char * /* argv */ [])
{
  return 0;
}

TAO_AV_Protocol_Object*
TAO_AV_RTCP_Flow_Factory::make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                TAO_Base_StreamEndPoint *endpoint,
                                                TAO_AV_Flow_Handler *handler,
                                                TAO_AV_Transport *transport)
{
  TAO_AV_Callback *callback = 0;
  endpoint->get_control_callback (entry->flowname (),
                                  callback);
  if (callback == 0)
    ACE_NEW_RETURN (callback,
                    TAO_AV_RTCP_Callback,
                    0);
  TAO_AV_Protocol_Object *object = 0;
  ACE_NEW_RETURN (object,
                  TAO_AV_RTCP_Object (callback,
                                      transport),
                  0);
  callback->open (object,
                  handler);
  return object;
}

// TAO_AV_RTCP_Object
int
TAO_AV_RTCP_Object::handle_input (void)
{
  ACE_Message_Block *data;
  size_t bufsiz = 2*this->transport_->mtu ();
  ACE_NEW_RETURN (data,
                  ACE_Message_Block (bufsiz),
                  -1);
  int n = this->transport_->recv (data->rd_ptr (),bufsiz);
  if (n == 0)
    ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::handle_input:connection closed\n"),-1);
  if (n < 0)
    ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::handle_input:recv error\n"),-1);
  data->wr_ptr (n);
  ACE_Addr *peer_addr = this->transport_->get_peer_addr ();
  this->callback_->receive_control_frame (data,*peer_addr);
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

TAO_AV_RTCP_Object::TAO_AV_RTCP_Object (TAO_AV_Callback *callback,
                                        TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (callback,transport)
{
}

TAO_AV_RTCP_Object::~TAO_AV_RTCP_Object (void)
{
}

int
TAO_AV_RTCP_Object::destroy (void)
{
  this->callback_->handle_destroy ();
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
//   frame->rd_ptr ((size_t)0);
//   // Since the rd_ptr would have been moved ahead.
  return this->callback_->receive_frame (frame,
                                         0,
                                         peer_address);
}

// TAO_AV_RTCP_Callback
TAO_AV_RTCP_Callback::TAO_AV_RTCP_Callback (void)
{
  ACE_NEW (source_manager_,
           TAO_AV_SourceManager (this));

  ACE_NEW (this->state_,
           TAO_AV_RTP_State);
}

TAO_AV_RTCP_Callback::~TAO_AV_RTCP_Callback (void)
{
}

TAO_AV_SourceManager*
TAO_AV_RTCP_Callback::source_manager (void)
{
  return this->source_manager_;
}

TAO_AV_RTP_State*
TAO_AV_RTCP_Callback::state (void)
{
  return this->state_;
}

int
TAO_AV_RTCP_Callback::get_rtp_source (TAO_AV_Source *&source,
                                      ACE_UINT32 srcid,
                                      ACE_UINT32 ssrc,
                                      ACE_UINT32 addr)
{
  ACE_NEW_RETURN (source,
                  TAO_AV_Source (srcid,
                                 ssrc,
                                 addr),
                  -1);
  return 0;
}

void
TAO_AV_RTCP_Callback::schedule (int ms)
{
  this->timeout_ = ms;
}


int
TAO_AV_RTCP_Callback::handle_start (void)
{
  //
  /*
   * schedule a timer for our first report using half the
   * min rtcp interval.  This gives us some time before
   * our first report to learn about other sources so our
   * next report interval will account for them.  The avg
   * rtcp size was initialized to 128 bytes which is
   * conservative (it assumes everyone else is generating
   * SRs instead of RRs).
   */
  double rint = this->state_->rtcp_avg_size_ * this->state_->rtcp_inv_bw_;
  if (rint < RTCP_MIN_RPT_TIME / 2. * 1000.)
    rint = RTCP_MIN_RPT_TIME / 2. * 1000.;
  this->state_->rint_ = rint;
  this->timeout_ = int(TAO_AV_RTCP::fmod(double(ACE_OS::rand ()), rint) + rint * .5 + .5);
  return 0;
}

int
TAO_AV_RTCP_Callback::handle_stop (void)
{
  return 0;
}

int
TAO_AV_RTCP_Callback::handle_timeout (void * /*arg*/)
{
  // Here we do the send_report.
  TAO_AV_RTCP::send_report (0,
                            this->protocol_object_,
                            this->source_manager_,
                            this->state_,
                            this);
  return 0;
}

void
TAO_AV_RTCP_Callback::get_timeout (ACE_Time_Value *&tv,
                                   void *& /*arg*/)
{
  // Here we do the RTCP timeout calculation.
  ACE_NEW (tv,
           ACE_Time_Value (0,this->timeout_*ACE_ONE_SECOND_IN_MSECS));
}

int
TAO_AV_RTCP_Callback::handle_destroy (void)
{
  // Here we do the send_bye.
  TAO_AV_RTCP::send_report (1,
                            this->protocol_object_,
                            this->source_manager_,
                            this->state_,
                            this);
  return 0;
}

int
TAO_AV_RTCP_Callback::receive_frame (ACE_Message_Block *frame,
                                     TAO_AV_frame_info *,
                                     const ACE_Addr &peer_address)
{
  char *buf = frame->rd_ptr ();
  TAO_AV_RTP::rtphdr *rh = (TAO_AV_RTP::rtphdr *)buf;

  frame->rd_ptr (sizeof (TAO_AV_RTP::rtphdr));
  int result = this->demux (rh,
                            frame,
                            peer_address);
  frame->rd_ptr (buf);

  if (result < 0)
    return result;

  return 0;
}


int
TAO_AV_RTCP_Callback::receive_control_frame (ACE_Message_Block *frame,
                                             const ACE_Addr &peer_address)
{
  // Here we do the processing of the RTCP frames.
  TAO_AV_RTCP::rtcphdr header;
  int result = TAO_AV_RTCP::handle_input (frame,
                                          peer_address,
                                          header,
                                          this->source_manager_,
                                          this->state_);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTCP::handle_input failed\n"),-1);
  return 0;
}

int
TAO_AV_RTCP_Callback::demux (TAO_AV_RTP::rtphdr* rh,
                             ACE_Message_Block *data,
                             const ACE_Addr &address)
{
  char *bp = data->rd_ptr ();
  int cc = data->length ();
  if (cc < 0)
    {
      ++this->state_->nrunt_;
      return -1;
    }
  ACE_UINT32 srcid = rh->rh_ssrc;
  int flags = ntohs (rh->rh_flags);
  if ( (flags & RTP_X) != 0)
    {
      /*
       * the minimal-control audio/video profile
       * explicitly forbids extensions
       */
      ++this->state_->badext_;
      return -1;
    }

  // @@Naga:Maybe the framework itself could check for formats making use of
  // the property service to query the formats supported for this flow.
  /*
   * Check for illegal payload types.  Most likely this is
   * a session packet arriving on the data port.
   */
//   int fmt = flags & 0x7f;
//   if (!check_format (fmt))
//     {
//       ++state->badfmt_;
//       return;
//     }

  u_long addr = address.hash ();
  ACE_UINT16 seqno = ntohs (rh->rh_seqno);
  TAO_AV_Source* s = this->source_manager_->demux (srcid, addr, seqno);
  if (s == 0)
    /*
     * Takes a pair of validated packets before we will
     * believe the source.  This prevents a runaway
     * allocation of Source data structures for a
     * stream of garbage packets.
     */
    return -1;

  ACE_Time_Value now = ACE_OS::gettimeofday ();
  s->lts_data (now);
  s->sts_data (rh->rh_ts);

  int cnt = (flags >> 8) & 0xf;
  if (cnt > 0)
    {
      u_char* nh = (u_char*)rh + (cnt << 2);
      while (--cnt >= 0)
        {
          ACE_UINT32 csrc = * (ACE_UINT32*)bp;
          bp += 4;
          TAO_AV_Source* cs = this->source_manager_->lookup (csrc, srcid, addr);
          cs->lts_data (now);
          cs->action ();
        }
      /*XXX move header up so it's contiguous with data*/
      TAO_AV_RTP::rtphdr hdr = *rh;
      rh = (TAO_AV_RTP::rtphdr*)nh;
      *rh = hdr;
    }
  else
    s->action ();

  return 0;
  /*
   * This is a data packet.  If the source needs activation,
   * or the packet format has changed, deal with this.
   * Then, hand the packet off to the packet handler.
   * XXX might want to be careful about flip-flopping
   * here when format changes due to misordered packets
   * (easy solution -- keep rtp seqno of last fmt change).
   */
}

ACE_FACTORY_DEFINE (AV, TAO_AV_RTCP_Flow_Factory)
ACE_STATIC_SVC_DEFINE (TAO_AV_RTCP_Flow_Factory,
                       ACE_TEXT ("RTCP_Flow_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_AV_RTCP_Flow_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)
