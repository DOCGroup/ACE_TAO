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

#include "RTP.h"
#include "Nil.h"

int
TAO_AV_RTP::handle_input (TAO_AV_Transport *transport,
                          ACE_Message_Block *&data,
                          TAO_AV_frame_info *&frame_info,
                          ACE_Addr &addr,
                          TAO_AV_SourceManager *source_manager,
                          TAO_AV_RTP_State *state)
{
  size_t bufsiz = 2*transport->mtu ();
  if (data == 0)
    {
      ACE_NEW_RETURN (data,
                      ACE_Message_Block (bufsiz),
                      -1);
    }
  else
    if (data->size () < bufsiz)
      ACE_ERROR_RETURN ( (LM_ERROR,"data block size not enough\n"),-1);
  int n = transport->recv (data->rd_ptr (),bufsiz);
  if (n == 0)
    ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::handle_input:connection closed\n"),-1);
  if (n < 0)
    ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::handle_input:recv error\n"),-1);
  int size_phdr = ACE_static_cast (int, sizeof (rtphdr));
  if (n < size_phdr)
    ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::handle_input:invalid header\n"),-1);
  data->wr_ptr (n);
  ACE_Addr *addr_ptr = 0;
  addr_ptr = transport->get_peer_addr ();
  if (addr_ptr == 0)
    if (TAO_debug_level > 0) ACE_DEBUG ( (LM_ERROR,"TAO_AV_RTP::handle_input:get_peer_addr failed\n"));
  addr = *addr_ptr;
  rtphdr* rh = (rtphdr*)data->rd_ptr ();
  int version = * (u_char*)rh >> 6;
  if (version != 2)
    {
      state->bad_version_++;
      ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP:bad version\n"),-1);
    }
  TAO_AV_RTP::rtphdr header;
  header = *rh;
  ACE_UINT32 ip_addr;
  switch (addr_ptr->get_type ())
    {
    case AF_INET:
      {
        ACE_INET_Addr *inet_addr = ACE_static_cast (ACE_INET_Addr *,addr_ptr);
        ip_addr = inet_addr->get_ip_address ();
      }
      break;
    default:
      if (TAO_debug_level > 0) ACE_DEBUG ( (LM_DEBUG,"TAO_AV_RTP::Unknown address type\n"));
      break;
    }
  data->rd_ptr (sizeof (rtphdr));
  int result = TAO_AV_RTP::demux (rh,
                                  data,
                                  ip_addr,
                                  source_manager,
                                  state);
  if (result < 0)
    {
      data->release ();
      return -1;
    }
  ACE_NEW_RETURN (frame_info,
                  TAO_AV_frame_info,
                  -1);
  frame_info->boundary_marker = header.rh_flags & RTP_M;
  frame_info->timestamp = header.rh_ts;
  frame_info->ssrc = header.rh_ssrc;
  frame_info->sequence_num = header.rh_seqno;
  frame_info->format = header.rh_flags & 0x7f;
//   ACE_Message_Block *frame_info_mb;
//   ACE_NEW_RETURN (frame_info_mb,
//                   ACE_Message_Block ((char *)frame_info,
//                                      sizeof (TAO_AV_frame_info)),
//                   -1);
//   frame_info_mb->wr_ptr (sizeof(TAO_AV_frame_info));
//   frame_info_mb->cont (data);
//   data = frame_info_mb;
  return 0;
}

int
TAO_AV_RTP::demux (rtphdr* rh,
                   ACE_Message_Block *data,
                   ACE_UINT32 addr,
                   TAO_AV_SourceManager *source_manager,
                   TAO_AV_RTP_State *state)
{
  char *bp = data->rd_ptr ();
  int cc = data->length ();
  if (cc < 0)
    {
      ++state->nrunt_;
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
      ++state->badext_;
      return -1;
    }

  // @@Naga:Maybe the framework itself could check for formats making use of
  // the property service  to query the formats supported for this flow.
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

  ACE_UINT16 seqno = ntohs (rh->rh_seqno);
  TAO_AV_Source* s = source_manager->demux (srcid, addr, seqno);
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
          TAO_AV_Source* cs = source_manager->lookup (csrc, srcid, addr);
          cs->lts_data (now);
          cs->action ();
        }
      /*XXX move header up so it's contiguous with data*/
      rtphdr hdr = *rh;
      rh = (rtphdr*)nh;
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
//   PacketHandler* h = s->handler ();
//   if (h == 0)
//     h = s->activate (fmt);
//   else if (s->format () != fmt)
//     h = s->change_format (fmt);

//   /*
//    * XXX bit rate doesn't include rtpv1 options;
//    * but v1 is going away anyway.
//    */
//   int dup = s->cs (seqno);
//   s->np (1);
//   s->nb (cc + sizeof (*rh));
//   if (dup)
//     return;
//   if (flags & RTP_M)
//     s->nf (1);

//   int hlen = h->hdrlen ();
//   cc -= hlen;
//   if (cc < 0)
//     {
//       s->runt (1);
//       return;
//     }
//   if (!s->mute ())
//     h->recv (rh, bp + hlen, cc);
}

ACE_INLINE
int
TAO_AV_RTP::write_header (rtphdr &header,
                          int format,
                          ACE_UINT16 &sequence_num,
                          ACE_UINT32 ts,
                          ACE_UINT32 ssrc,
                          CORBA::Boolean boundary_marker)
{
  int flags = RTP_VERSION << 14 | format;
  if (boundary_marker)
    flags |= RTP_M;
  header.rh_flags = ntohs (flags);
  header.rh_seqno = htons (sequence_num);
  sequence_num++;
  header.rh_ts = htonl (ts);
  header.rh_ssrc = ssrc;
  return 0;
}

ACE_INLINE
int
TAO_AV_RTP::send_frame (TAO_AV_Transport *transport,
                        rtphdr &header,
                        ACE_Message_Block *frame)
{
  ACE_Message_Block mb ((char *)&header,
                        sizeof (header));
  mb.wr_ptr (sizeof (header));
  mb.cont (frame);
  int result = transport->send (&mb);
  if (result < 0)
    ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::send_frame failed\n"),result);
  return 0;
}

//ACE_INLINE
int
TAO_AV_RTP::send_frame (TAO_AV_Transport *transport,
                        rtphdr &header,
                        const iovec *iov,
                        int iovcnt)
{
  iovec send_iov[IOV_MAX];
  send_iov [0].iov_base = (char *)&header;
  send_iov [0].iov_len  = sizeof(header);
  for (int i=1;i<=iovcnt; i++)
    send_iov [i] = iov [i-1];
  int result = transport->send (send_iov,
                                iovcnt+1);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTP::send_frame failed\n"),result);
  return 0;
}

//------------------------------------------------------------
// TAO_AV_RTP_UDP_Protocol_Factory
//------------------------------------------------------------

int
TAO_AV_RTP_UDP_Protocol_Factory::match_protocol (TAO_AV_Core::Protocol protocol)
{
  return (protocol == TAO_AV_Core::TAO_AV_RTP_UDP);
}

TAO_AV_Acceptor*
TAO_AV_RTP_UDP_Protocol_Factory::make_acceptor (void)
{
  TAO_AV_RTP_UDP_Acceptor *acceptor;

  ACE_NEW_RETURN (acceptor,
                  TAO_AV_RTP_UDP_Acceptor,
                  0);
  return acceptor;
}

TAO_AV_Connector*
TAO_AV_RTP_UDP_Protocol_Factory::make_connector (void)
{
  TAO_AV_RTP_UDP_Connector *connector;

  ACE_NEW_RETURN (connector,
                  TAO_AV_RTP_UDP_Connector,
                  0);
  return connector;
}

//------------------------------------------------------------
// TAO_AV_RTP_UDP_Acceptor
//------------------------------------------------------------

TAO_AV_RTP_UDP_Acceptor::TAO_AV_RTP_UDP_Acceptor (void)
  :make_rtp_handler_ (1)
{
}

TAO_AV_RTP_UDP_Acceptor::~TAO_AV_RTP_UDP_Acceptor (void)
{
}

int
TAO_AV_RTP_UDP_Acceptor::make_svc_handler (TAO_AV_UDP_Flow_Handler *&handler)
{
  if (this->make_rtp_handler_)
    {
      this->make_rtp_handler_ = 0;
      TAO_AV_Callback *callback = 0;
      this->endpoint_->get_callback (this->flowname_.c_str (),
                                     callback);
      ACE_NEW_RETURN (handler,
                      TAO_AV_RTP_UDP_Flow_Handler (callback),
                      -1);
      callback->transport (handler->transport ());
      TAO_AV_Protocol_Object *object = 0;
      ACE_NEW_RETURN (object,
                      TAO_AV_RTP_Object (callback,
                                         handler->transport ()),
                      -1);
      callback->protocol_object (object);
      this->endpoint_->set_protocol_object (this->flowname_.c_str (),
                                            object);
      this->endpoint_->set_handler (this->flowname_.c_str (),handler);
      this->entry_->protocol_object (object);
    }
  else
    {
      // Now make rtcp handler.
      ACE_NEW_RETURN (handler,
                      TAO_AV_RTCP_UDP_Flow_Handler (this->reactor_,
                                                    this->endpoint_,
                                                    this->entry_->flowname ()),
                      -1);
    }
  return 0;
}

int
TAO_AV_RTP_UDP_Acceptor::open (TAO_Base_StreamEndPoint *endpoint,
                               TAO_AV_Core *av_core,
                               TAO_FlowSpec_Entry *entry)
{
  this->reactor_ = av_core->reactor ();
  int result = TAO_AV_UDP_Acceptor::open (endpoint,
                                          av_core,
                                          entry);
  if (result < 0)
    return result;
  TAO_String_Hash_Key handler_key (entry->flowname ());
  TAO_AV_RTCP::RTCP_UDP_Map::ENTRY *handler_entry = 0;
  TAO_AV_RTP_UDP_Flow_Handler *rtp_handler = ACE_dynamic_cast (TAO_AV_RTP_UDP_Flow_Handler *,
                                                               this->handler_);
  TAO_AV_RTCP_UDP_Flow_Handler *rtcp_handler = 0;
  if (TAO_AV_RTCP::rtcp_udp_map_.find (handler_key,
                                       handler_entry) == 0)
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_ERROR,"RTCP Object already created for this flow\n"));
      rtcp_handler = ACE_dynamic_cast (TAO_AV_RTCP_UDP_Flow_Handler*,
                                       handler_entry->int_id_);
    }
  else
    {
      // Now do the creation of RTCP handler.
      // The address of RTCP will always be one port higher than RTP.
      ACE_INET_Addr *entry_addr = ACE_static_cast (ACE_INET_Addr *,
                                                   entry->address ());
      if (entry_addr == 0)
        return -1;
      ACE_INET_Addr local_addr (entry_addr->get_port_number ()+1,
                                entry_addr->get_ip_address ());
      TAO_AV_UDP_Flow_Handler *handler = 0;
      result = this->acceptor_.open (this,
                                     av_core->reactor (),
                                     local_addr,
                                     handler);
      rtcp_handler = ACE_dynamic_cast (TAO_AV_RTCP_UDP_Flow_Handler*,
                                       handler);
      result = TAO_AV_RTCP::rtcp_udp_map_.bind (handler_key,rtcp_handler);
      if (result < 0)
        if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"rtcp_map::bind failed\n"));
    }
  rtp_handler->rtcp_handler (rtcp_handler);
  endpoint->set_rtcp_info (entry->flowname (),
                           rtcp_handler->source_manager (),
                           rtcp_handler->state ());
  return 0;
}

int
TAO_AV_RTP_UDP_Acceptor::open_default (TAO_Base_StreamEndPoint *endpoint,
                                       TAO_AV_Core *av_core,
                                       TAO_FlowSpec_Entry *entry)
{
  this->reactor_ = av_core->reactor ();
  int result = TAO_AV_UDP_Acceptor::open (endpoint,
                                          av_core,
                                          entry);
  if (result < 0)
    return result;
  // Now do the creation of RTCP handler.
  // Since its a default open we do a default open for rtcp also.
  ACE_INET_Addr local_addr;
  TAO_String_Hash_Key handler_key (entry->flowname ());
  TAO_AV_RTCP::RTCP_UDP_Map::ENTRY *handler_entry = 0;
  TAO_AV_RTP_UDP_Flow_Handler *rtp_handler = ACE_dynamic_cast (TAO_AV_RTP_UDP_Flow_Handler *,
                                                               this->handler_);
  TAO_AV_RTCP_UDP_Flow_Handler *rtcp_handler = 0;
  if (TAO_AV_RTCP::rtcp_udp_map_.find (handler_key,
                                   handler_entry) == 0)
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_ERROR,"RTCP Object already created for this flow\n"));
      rtcp_handler = ACE_dynamic_cast (TAO_AV_RTCP_UDP_Flow_Handler*,
                                       handler_entry->int_id_);
    }
  else
    {
      TAO_AV_UDP_Flow_Handler *handler = 0;
      result = this->acceptor_.open (this,
                                     av_core->reactor (),
                                     local_addr,
                                     handler);
      TAO_AV_RTCP_UDP_Flow_Handler *rtcp_handler = ACE_dynamic_cast (TAO_AV_RTCP_UDP_Flow_Handler*,
                                                                     handler);
      result = TAO_AV_RTCP::rtcp_udp_map_.bind (handler_key,rtcp_handler);
      if (result < 0)
        if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"rtcp_map::bind failed\n"));
    }
  rtp_handler->rtcp_handler (rtcp_handler);
  endpoint->set_rtcp_info (entry->flowname (),
                           rtcp_handler->source_manager (),
                           rtcp_handler->state ());
  return 0;
}

//------------------------------------------------------------
// TAO_AV_RTP_UDP_Connector
//------------------------------------------------------------

TAO_AV_RTP_UDP_Connector::TAO_AV_RTP_UDP_Connector (void)
  :make_rtp_handler_ (1)
{
}

TAO_AV_RTP_UDP_Connector::~TAO_AV_RTP_UDP_Connector (void)
{
}

int
TAO_AV_RTP_UDP_Connector::make_svc_handler (TAO_AV_UDP_Flow_Handler *&handler)
{
  if (this->make_rtp_handler_)
    {
      make_rtp_handler_ = 0;
      TAO_AV_Callback *callback = 0;
      this->endpoint_->get_callback (this->flowname_.c_str (),
                                     callback);
      ACE_NEW_RETURN (handler,
                      TAO_AV_RTP_UDP_Flow_Handler (callback),
                      -1);
      callback->transport (handler->transport ());
      TAO_AV_Protocol_Object *object = 0;
      ACE_NEW_RETURN (object,
                      TAO_AV_RTP_Object (callback,
                                         handler->transport ()),
                      -1);
      callback->protocol_object (object);
      this->endpoint_->set_protocol_object (this->flowname_.c_str (),
                                            object);
      this->endpoint_->set_handler (this->flowname_.c_str (),handler);
      this->entry_->protocol_object (object);
    }
  else
    {
      // Now make rtcp handler.
      ACE_NEW_RETURN (handler,
                      TAO_AV_RTCP_UDP_Flow_Handler (this->av_core_->reactor (),
                                                    this->endpoint_,
                                                    this->entry_->flowname ()),
                      -1);
    }
  return 0;
}

int
TAO_AV_RTP_UDP_Connector::connect (TAO_FlowSpec_Entry *entry,
                                   TAO_AV_Transport *&transport)
{
  int result = TAO_AV_UDP_Connector::connect (entry,
                                              transport);
  TAO_String_Hash_Key handler_key (entry->flowname ());
  TAO_AV_RTCP::RTCP_UDP_Map::ENTRY *handler_entry = 0;
  TAO_AV_RTP_UDP_Flow_Handler *rtp_handler = ACE_dynamic_cast (TAO_AV_RTP_UDP_Flow_Handler *,
                                                               this->handler_);
  TAO_AV_RTCP_UDP_Flow_Handler *rtcp_handler = 0;
  if (TAO_AV_RTCP::rtcp_udp_map_.find (handler_key,
                                       handler_entry) == 0)
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_ERROR,"RTCP Object already created for this flow\n"));
      rtcp_handler = ACE_dynamic_cast (TAO_AV_RTCP_UDP_Flow_Handler*,
                                       handler_entry->int_id_);
    }
  else
    {

      // Now do the creation of RTCP handler.
      // The address of RTCP will always be one port higher than RTP.
      ACE_INET_Addr *entry_addr = ACE_static_cast (ACE_INET_Addr *,
                                                   entry->address ());
      if (entry_addr == 0)
        return -1;
      ACE_INET_Addr remote_addr (entry_addr->get_port_number ()+1,
                                 entry_addr->get_ip_address ());

      ACE_INET_Addr local_addr;
      TAO_AV_UDP_Flow_Handler *handler;
      result = this->connector_.connect (handler,
                                         remote_addr,
                                         local_addr);
      rtcp_handler = ACE_dynamic_cast (TAO_AV_RTCP_UDP_Flow_Handler*,
                                       handler);
      result = TAO_AV_RTCP::rtcp_udp_map_.bind (handler_key,rtcp_handler);
      if (result < 0)
        if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"rtcp_map::bind failed\n"));

    }
  rtp_handler->rtcp_handler (rtcp_handler);
  this->endpoint_->set_rtcp_info (entry->flowname (),
                                  rtcp_handler->source_manager (),
                                  rtcp_handler->state ());
  return 0;
}

// TAO_AV_RTP_UDP_Flow_Handler
TAO_AV_RTP_UDP_Flow_Handler::TAO_AV_RTP_UDP_Flow_Handler (TAO_AV_Callback *callback)
  :TAO_AV_Flow_Handler (callback),
   TAO_AV_UDP_Flow_Handler (callback)
{
}

void
TAO_AV_RTP_UDP_Flow_Handler::rtcp_handler (TAO_AV_RTCP_UDP_Flow_Handler *handler)
{
  this->rtcp_handler_ = handler;
  this->source_manager_ = handler->source_manager ();
  this->state_ = handler->state ();
}

int
TAO_AV_RTP_UDP_Flow_Handler::handle_input (ACE_HANDLE /*fd*/)
{
  ACE_Message_Block *data = 0;
  ACE_INET_Addr addr;
  TAO_AV_frame_info *frame_info = 0;
  // Handles the incoming RTP packet input.
  int result = TAO_AV_RTP::handle_input (this->transport_,
                                         data,
                                         frame_info,
                                         addr,
                                         this->source_manager_,
                                         this->state_);
  if (result < 0)
    return 0;
  result = this->callback_->receive_frame (data,
                                           frame_info);
  return 0;
}

int
TAO_AV_RTP_Object::send_frame (ACE_Message_Block *frame,
                               TAO_AV_frame_info *frame_info)
{
  TAO_AV_RTP::rtphdr header;
  int format = 0;
  int result = -1;
  if (frame_info != 0)
    {
      result = TAO_AV_RTP::write_header (header,
                                         frame_info->format,
                                         this->sequence_num_,
                                         frame_info->timestamp,
                                         frame_info->ssrc,
                                         frame_info->boundary_marker);
      frame_info->sequence_num = this->sequence_num_;
    }
  else
    {
      result = TAO_AV_RTP::write_header (header,
                                         0,
                                         this->sequence_num_,
                                         0,
                                         0,
                                         0);
    }
  if (result < 0)
    return result;
  result = TAO_AV_RTP::send_frame (this->transport_,
                                   header,
                                   frame);
  if (result < 0)
    return result;

  return 0;
}

int
TAO_AV_RTP_Object::send_frame (const iovec *iov,
                               int iovcnt,
                               TAO_AV_frame_info *frame_info)
{
  TAO_AV_RTP::rtphdr header;
  int format = 0;
  int result = -1;
  if (frame_info != 0)
    {
      result = TAO_AV_RTP::write_header (header,
                                         frame_info->format,
                                         this->sequence_num_,
                                         frame_info->timestamp,
                                         frame_info->ssrc,
                                         frame_info->boundary_marker);
      frame_info->sequence_num = this->sequence_num_;
    }
  else
    {
      result = TAO_AV_RTP::write_header (header,
                                         0,
                                         this->sequence_num_,
                                         0,
                                         0,
                                         0);
    }
  if (result < 0)
    return result;
  result = TAO_AV_RTP::send_frame (this->transport_,
                                   header,
                                   iov,
                                   iovcnt);
  if (result < 0)
    return result;

  return 0;
}

TAO_AV_RTP_Object::TAO_AV_RTP_Object (TAO_AV_Callback *callback,
                                      TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (callback,transport),
   sequence_num_ (0)
{
  // @@Naga:We have to initialize the sequence number to a non-zero
  // random number.
  this->sequence_num_ = ACE_OS::rand ();
}

int
TAO_AV_RTP_Object::end_stream (void)
{
  this->callback_->handle_end_stream ();
  return 0;
}

int
TAO_AV_RTP_Object::set_policies (const PolicyList &policy_list)
{
  this->policy_list_ = policy_list;
  u_int num_policies = this->policy_list_.length ();
  TAO_AV_Policy *policy = 0;
  for (u_int i=0; i< num_policies;i++)
    {
      policy = this->policy_list_ [i];
      switch (policy->type ())
        {
        case TAO_AV_Policy::TAO_AV_PAYLOAD_TYPE_POLICY:
          {
            TAO_AV_Payload_Type_Policy *payload_policy =
              ACE_static_cast (TAO_AV_Payload_Type_Policy *,policy);
            if (payload_policy == 0)
              ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP_Object::send_frame:Payload policy not defined\n"),-1);
            this->format_ = payload_policy->value ();
          }
          break;
        case TAO_AV_Policy::TAO_AV_SSRC_POLICY:
          {
            TAO_AV_SSRC_Policy *ssrc_policy =
              ACE_static_cast (TAO_AV_SSRC_Policy *,policy);
            if (ssrc_policy == 0)
              ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP_Object::send_frame:SSRC policy not defined\n"),-1);
            this->ssrc_ = ssrc_policy->value ();;
          }
          break;
        default:
          break;
        }
    }
}

//------------------------------------------------------------
// TAO_AV_RTP_UDP_MCast_Protocol_Factory
//------------------------------------------------------------

int
TAO_AV_RTP_UDP_MCast_Protocol_Factory::match_protocol (TAO_AV_Core::Protocol protocol)
{
  return (protocol == TAO_AV_Core::TAO_AV_RTP_UDP_MCAST);
}

TAO_AV_Acceptor*
TAO_AV_RTP_UDP_MCast_Protocol_Factory::make_acceptor (void)
{
  TAO_AV_RTP_UDP_MCast_Acceptor *acceptor;

  ACE_NEW_RETURN (acceptor,
                  TAO_AV_RTP_UDP_MCast_Acceptor,
                  0);
  return acceptor;
}

TAO_AV_Connector*
TAO_AV_RTP_UDP_MCast_Protocol_Factory::make_connector (void)
{
  TAO_AV_RTP_UDP_MCast_Connector *connector;

  ACE_NEW_RETURN (connector,
                  TAO_AV_RTP_UDP_MCast_Connector,
                  0);
  return connector;
}

//------------------------------------------------------------
// TAO_AV_RTP_UDP_MCast_Acceptor
//------------------------------------------------------------

TAO_AV_RTP_UDP_MCast_Acceptor::TAO_AV_RTP_UDP_MCast_Acceptor (void)
  :make_rtp_handler_ (1)
{
}

TAO_AV_RTP_UDP_MCast_Acceptor::~TAO_AV_RTP_UDP_MCast_Acceptor (void)
{
}

int
TAO_AV_RTP_UDP_MCast_Acceptor::make_svc_handler (TAO_AV_UDP_MCast_Flow_Handler *&handler)
{
  if (this->make_rtp_handler_)
    {
      make_rtp_handler_ = 0;
      TAO_AV_Callback *callback = 0;
      this->endpoint_->get_callback (this->flowname_.c_str (),
                                     callback);
      ACE_NEW_RETURN (handler,
                      TAO_AV_RTP_UDP_MCast_Flow_Handler (callback),
                      -1);
      callback->transport (handler->transport ());
      TAO_AV_Protocol_Object *object = 0;
      ACE_NEW_RETURN (object,
                      TAO_AV_RTP_Object (callback,
                                         handler->transport ()),
                      -1);
      callback->protocol_object (object);
      this->endpoint_->set_protocol_object (this->flowname_.c_str (),
                                            object);
      this->endpoint_->set_handler (this->flowname_.c_str (),handler);
      this->entry_->protocol_object (object);
    }
  else
    {
      // Now make rtcp handler.
      ACE_NEW_RETURN (handler,
                      TAO_AV_RTCP_UDP_MCast_Flow_Handler (this->reactor_,
                                                          this->endpoint_,
                                                          this->entry_->flowname ()),
                      -1);
    }

  return 0;
}

int
TAO_AV_RTP_UDP_MCast_Acceptor::open (TAO_Base_StreamEndPoint *endpoint,
                                     TAO_AV_Core *av_core,
                                     TAO_FlowSpec_Entry *entry)
{
  this->reactor_ = av_core->reactor ();
  int result = TAO_AV_UDP_MCast_Acceptor::open (endpoint,
                                                av_core,
                                                entry);
  if (result < 0)
    return result;
  TAO_String_Hash_Key handler_key (entry->flowname ());
  TAO_AV_RTCP::RTCP_MCast_Map::ENTRY *handler_entry = 0;
  TAO_AV_RTP_UDP_MCast_Flow_Handler *rtp_handler = ACE_static_cast (TAO_AV_RTP_UDP_MCast_Flow_Handler *,
                                                                    this->handler_);
  TAO_AV_RTCP_UDP_MCast_Flow_Handler *rtcp_handler = 0;
  if (TAO_AV_RTCP::rtcp_mcast_map_.find (handler_key,
                                         handler_entry) == 0)
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_ERROR,"RTCP Object already created for this flow\n"));
      rtcp_handler = ACE_dynamic_cast (TAO_AV_RTCP_UDP_MCast_Flow_Handler*,
                                       handler_entry->int_id_);
    }
  else
    {
      // Now do the creation of RTCP handler.
      // The address of RTCP will always be one port higher than RTP.
      ACE_INET_Addr *entry_addr = ACE_static_cast (ACE_INET_Addr *,
                                                   entry->address ());
      if (entry_addr == 0)
        return -1;
      ACE_INET_Addr *local_addr = 0;
      ACE_NEW_RETURN (local_addr,
                      ACE_INET_Addr (entry_addr->get_port_number ()+1,
                                     entry_addr->get_ip_address ()),
                      -1);
      TAO_AV_UDP_MCast_Flow_Handler *handler = 0;
      result = this->open_i (av_core->reactor (),
                             local_addr,
                             handler);
      rtcp_handler = ACE_dynamic_cast (TAO_AV_RTCP_UDP_MCast_Flow_Handler*,
                                       handler);
      result = TAO_AV_RTCP::rtcp_mcast_map_.bind (handler_key,rtcp_handler);
      if (result < 0)
        if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"rtcp_map::bind failed\n"));
    }
  rtp_handler->rtcp_handler (rtcp_handler);
  endpoint->set_rtcp_info (entry->flowname (),
                           rtcp_handler->source_manager (),
                           rtcp_handler->state ());
}

int
TAO_AV_RTP_UDP_MCast_Acceptor::open_default (TAO_Base_StreamEndPoint *endpoint,
                                             TAO_AV_Core *av_core,
                                             TAO_FlowSpec_Entry *entry)
{
  this->reactor_ = av_core->reactor ();
  int result = TAO_AV_UDP_MCast_Acceptor::open (endpoint,
                                                av_core,
                                                entry);
  if (result < 0)
    return result;
  TAO_String_Hash_Key handler_key (entry->flowname ());
  TAO_AV_RTCP::RTCP_MCast_Map::ENTRY *handler_entry = 0;
  TAO_AV_RTP_UDP_MCast_Flow_Handler *rtp_handler = ACE_static_cast (TAO_AV_RTP_UDP_MCast_Flow_Handler *,
                                                                    this->handler_);
  TAO_AV_RTCP_UDP_MCast_Flow_Handler *rtcp_handler = 0;
  if (TAO_AV_RTCP::rtcp_mcast_map_.find (handler_key,
                                         handler_entry) == 0)
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_ERROR,"RTCP Object already created for this flow\n"));
      rtcp_handler = ACE_dynamic_cast (TAO_AV_RTCP_UDP_MCast_Flow_Handler*,
                                       handler_entry->int_id_);
    }
  else
    {
      // Now do the creation of RTCP handler.
      // Since its a default open we do a default open for rtcp also.
      ACE_INET_Addr *local_addr = 0;
      TAO_AV_UDP_MCast_Flow_Handler *handler = 0;
      result = this->open_i (av_core->reactor (),
                             local_addr,
                             handler);
      rtcp_handler = ACE_dynamic_cast (TAO_AV_RTCP_UDP_MCast_Flow_Handler*,
                                       handler);
      result = TAO_AV_RTCP::rtcp_mcast_map_.bind (handler_key,rtcp_handler);
      if (result < 0)
        if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"rtcp_map::bind failed\n"));
    }
  rtp_handler->rtcp_handler (rtcp_handler);
  endpoint->set_rtcp_info (entry->flowname (),
                           rtcp_handler->source_manager (),
                           rtcp_handler->state ());
}

//------------------------------------------------------------
// TAO_AV_RTP_UDP_MCast_Connector
//------------------------------------------------------------

TAO_AV_RTP_UDP_MCast_Connector::TAO_AV_RTP_UDP_MCast_Connector (void)
  :make_rtp_handler_ (1)
{
}

TAO_AV_RTP_UDP_MCast_Connector::~TAO_AV_RTP_UDP_MCast_Connector (void)
{
}

int
TAO_AV_RTP_UDP_MCast_Connector::make_svc_handler (TAO_AV_UDP_MCast_Flow_Handler *&handler)
{
  if (this->make_rtp_handler_)
    {
      make_rtp_handler_ = 0;

      TAO_AV_Callback *callback = 0;
      this->endpoint_->get_callback (this->flowname_.c_str (),
                                     callback);
      ACE_NEW_RETURN (handler,
                      TAO_AV_RTP_UDP_MCast_Flow_Handler (callback),
                      -1);
      callback->transport (handler->transport ());
      TAO_AV_Protocol_Object *object = 0;
      ACE_NEW_RETURN (object,
                      TAO_AV_RTP_Object (callback,
                                         handler->transport ()),
                      -1);
      callback->protocol_object (object);
      this->endpoint_->set_protocol_object (this->flowname_.c_str (),
                                            object);
      this->endpoint_->set_handler (this->flowname_.c_str (),handler);
      this->entry_->protocol_object (object);
    }
  else
    {
      // Now make rtcp handler.
      ACE_NEW_RETURN (handler,
                      TAO_AV_RTCP_UDP_MCast_Flow_Handler (this->av_core_->reactor (),
                                                          this->endpoint_,
                                                          this->entry_->flowname ()),
                      -1);
    }
  return 0;
}

int
TAO_AV_RTP_UDP_MCast_Connector::connect (TAO_FlowSpec_Entry *entry,
                                         TAO_AV_Transport *&transport)
{
  int result = TAO_AV_UDP_MCast_Connector::connect (entry,
                                                    transport);
  TAO_String_Hash_Key handler_key (entry->flowname ());
  TAO_AV_RTCP::RTCP_MCast_Map::ENTRY *handler_entry = 0;
  TAO_AV_RTP_UDP_MCast_Flow_Handler *rtp_handler = ACE_dynamic_cast (TAO_AV_RTP_UDP_MCast_Flow_Handler *,
                                                                     this->handler_);
  TAO_AV_RTCP_UDP_MCast_Flow_Handler *rtcp_handler = 0;
  if (TAO_AV_RTCP::rtcp_mcast_map_.find (handler_key,
                                   handler_entry) == 0)
    {
      if (TAO_debug_level > 0) ACE_DEBUG ((LM_ERROR,"RTCP Object already created for this flow\n"));
      rtcp_handler = ACE_dynamic_cast (TAO_AV_RTCP_UDP_MCast_Flow_Handler*,
                                       handler_entry->int_id_);
    }
  else
    {

      // Now do the creation of RTCP handler.
      // The address of RTCP will always be one port higher than RTP.
      ACE_INET_Addr *entry_addr = ACE_static_cast (ACE_INET_Addr *,
                                                   entry->address ());
      if (entry_addr == 0)
        return -1;
      ACE_INET_Addr remote_addr (entry_addr->get_port_number ()+1,
                                 entry_addr->get_ip_address ());

      ACE_INET_Addr *address = &remote_addr;
      TAO_AV_UDP_MCast_Flow_Handler *handler = 0;
      result = this->connect_i (this->av_core_->reactor (),
                                address,
                                handler);
      if (result < 0)
        return result;
      rtcp_handler = ACE_dynamic_cast (TAO_AV_RTCP_UDP_MCast_Flow_Handler*,
                                       handler);
      result = TAO_AV_RTCP::rtcp_mcast_map_.bind (handler_key,rtcp_handler);
      if (result < 0)
        if (TAO_debug_level > 0) ACE_DEBUG ((LM_DEBUG,"rtcp_map::bind failed\n"));
    }
  rtp_handler->rtcp_handler (rtcp_handler);
  this->endpoint_->set_rtcp_info (entry->flowname (),
                                  rtcp_handler->source_manager (),
                                  rtcp_handler->state ());
  return 0;
}

// TAO_AV_RTP_UDP_MCast_Flow_Handler
TAO_AV_RTP_UDP_MCast_Flow_Handler::TAO_AV_RTP_UDP_MCast_Flow_Handler (TAO_AV_Callback *callback)
  :TAO_AV_Flow_Handler (callback),
   TAO_AV_UDP_MCast_Flow_Handler (callback),
   source_manager_ (0),
   rtcp_handler_ (0),
   state_ (0)
{
}

void
TAO_AV_RTP_UDP_MCast_Flow_Handler::rtcp_handler (TAO_AV_RTCP_UDP_MCast_Flow_Handler *handler)
{
  this->rtcp_handler_ = handler;
  this->source_manager_ = handler->source_manager ();
  this->state_ = handler->state ();
}

int
TAO_AV_RTP_UDP_MCast_Flow_Handler::handle_input (ACE_HANDLE /*fd*/)
{
  ACE_Message_Block *data = 0;
  ACE_INET_Addr addr;
  TAO_AV_frame_info *frame_info = 0;
  // Handles the incoming RTP packet input.
  int result = TAO_AV_RTP::handle_input (this->transport_,
                                         data,
                                         frame_info,
                                         addr,
                                         this->source_manager_,
                                         this->state_);
  if (result < 0)
    return 0;
  // @@ What should we do with the header.
  // Answer:Use it for RTCP statistics. But then the
  // application needs the format of the data to decode it.
  result = this->callback_->receive_frame (data,
                                           frame_info);
  return 0;
}
