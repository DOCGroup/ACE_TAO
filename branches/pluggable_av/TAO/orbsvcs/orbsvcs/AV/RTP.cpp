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

int TAO_AV_RTP::bad_version_;
int TAO_AV_RTCP::nrunt_;
int TAO_AV_RTCP::badversion_;
int TAO_AV_RTCP::rtcp_avg_size_;

int
TAO_AV_RTP::handle_input (TAO_AV_Transport *transport,
                          rtphdr &header,
                          ACE_Message_Block *&data,
                          ACE_Addr &addr)
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
      ACE_ERROR_RETURN ((LM_ERROR,"data block size not enough\n"),-1);
  int n = transport->recv (data->rd_ptr (),bufsiz);
  if (n == 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTP::handle_input:connection closed\n"),-1);
  if (n < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTP::handle_input:recv error\n"),-1);
  if (n < sizeof (rtphdr))
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTP::handle_input:invalid header\n"),-1);
  data->wr_ptr (n);
  int result = transport->get_peer_addr (addr);
  if (result < 0)
    ACE_DEBUG ((LM_ERROR,"TAO_AV_RTP::handle_input:get_peer_addr failed\n"));
  rtphdr* rh = (rtphdr*)data->rd_ptr ();
  int version = *(u_char*)rh >> 6;
  if (version != 2)
    {
      bad_version_++;
      ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTP:bad version\n"),-1);
    }
  header = *rh;
  data->rd_ptr (sizeof (header));
  return 0;
}

int
TAO_AV_RTCP::handle_input (TAO_AV_Transport *transport,
                           rtcphdr &header)
{
  ACE_Message_Block *data;
  size_t bufsiz = 2*transport->mtu ();
  ACE_NEW_RETURN (data,
                  ACE_Message_Block (bufsiz),
                  -1);
  int n = transport->recv (data->rd_ptr (),bufsiz);
  if (n == 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTP::handle_input:connection closed\n"),-1);
  if (n < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTP::handle_input:recv error\n"),-1);
  if (n < sizeof (rtcphdr))
    {
      nrunt_++;
      ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTP::handle_input:invalid header\n"),-1);
    }
  ACE_Addr peer_addr;
  int result = transport->get_peer_addr (peer_addr);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTP::handle_input:get_peer_addr failed\n"),-1);
  header = *(rtcphdr*) (data->rd_ptr ());
  /*
         * try to filter out junk: first thing in packet must be
         * sr, rr or bye & version number must be correct.
         */
  switch(ntohs(header.rh_flags) & 0xc0ff)
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
      ++badversion_;
      return -1;
    }
  /*
   * at this point we think the packet's valid.  Update our average
   * size estimator.  Also, there's valid ssrc so charge errors to it
   */
  /*
   * at this point we think the packet's valid.  Update our average
   * size estimator.  Also, there's valid ssrc so charge errors to it
   */
  rtcp_avg_size_ += RTCP_SIZE_GAIN * (double(n + 28) - rtcp_avg_size_);
  ACE_UINT32 addr = 0;
  switch (peer_addr.get_type ())
    {
    case AF_INET:
      {
        ACE_INET_Addr *inet_addr =
          ACE_dynamic_cast (ACE_INET_Addr *,&peer_addr);
        addr = inet_addr->get_ip_address ();
      }
      break;
    default:
      break;
    }
  /*
   * First record in compound packet must be the ssrc of the
   * sender of the packet.  Pull it out here so we can use
   * it in the sdes parsing, since the sdes record doesn't
   * contain the ssrc of the sender (in the case of mixers).
   */
  // @@ Still to be completed.
  return 0;
}

int
TAO_AV_RTP::write_header (rtphdr &header,
                          int format,
                          ACE_UINT16 &sequence_num,
                          ACE_UINT32 ts,
                          ACE_UINT32 ssrc)
{
  int flags = RTP_VERSION << 14 | format;
  header.rh_flags = ntohs(flags);
  header.rh_seqno = htons(sequence_num);
  sequence_num++;
  header.rh_ts = htonl(ts);
  header.rh_ssrc = ssrc;
  return 0;
}

int
TAO_AV_RTP::send_frame (TAO_AV_Transport *transport,
                        rtphdr &header,
                        ACE_Message_Block *frame)
{
  iovec iov [2];
  iov [0].iov_base = (char *)&header;
  iov [0].iov_len = sizeof (header);
  iov [1].iov_base = frame->rd_ptr ();
  iov [1].iov_len = frame->length ();
  int result = transport->send (iov,
                                2);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTP::send_frame failed\n"),result);
  return 0;
}

//------------------------------------------------------------
// TAO_AV_RTP_UDP_Protocol_Factory
//------------------------------------------------------------

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
{
}

TAO_AV_RTP_UDP_Acceptor::~TAO_AV_RTP_UDP_Acceptor (void)
{
}

int
TAO_AV_RTP_UDP_Acceptor::make_svc_handler (TAO_AV_UDP_Flow_Handler *&handler)
{
  TAO_AV_Callback *callback = 0;
  this->endpoint_->get_callback (this->flowname_.c_str (),
                                 callback);
  ACE_NEW_RETURN (handler,
                  TAO_AV_RTP_UDP_Flow_Handler (callback),
                  -1);
  TAO_AV_Protocol_Object *object = 0;
  ACE_NEW_RETURN (object,
                  TAO_AV_RTP_Object (callback,
                                     handler->transport ()),
                  -1);
  this->endpoint_->set_protocol_object (this->flowname_.c_str (),
                                        object);
  this->entry_->protocol_object (object);
  return 0;
}

//------------------------------------------------------------
// TAO_AV_RTP_UDP_Connector
//------------------------------------------------------------

TAO_AV_RTP_UDP_Connector::TAO_AV_RTP_UDP_Connector (void)
{
}

TAO_AV_RTP_UDP_Connector::~TAO_AV_RTP_UDP_Connector (void)
{
}

int
TAO_AV_RTP_UDP_Connector::make_svc_handler (TAO_AV_UDP_Flow_Handler *&handler)
{
  TAO_AV_Callback *callback = 0;
  this->endpoint_->get_callback (this->flowname_.c_str (),
                                 callback);
  ACE_NEW_RETURN (handler,
                  TAO_AV_RTP_UDP_Flow_Handler (callback),
                  -1);
  TAO_AV_Protocol_Object *object = 0;
  ACE_NEW_RETURN (object,
                  TAO_AV_RTP_Object (callback,
                                     handler->transport ()),
                  -1);
  this->endpoint_->set_protocol_object (this->flowname_.c_str (),
                                        object);
  this->entry_->protocol_object (object);
  return 0;
}

// TAO_AV_RTP_UDP_Flow_Handler
TAO_AV_RTP_UDP_Flow_Handler::TAO_AV_RTP_UDP_Flow_Handler (TAO_AV_Callback *callback)
  :TAO_AV_Flow_Handler (callback),
   TAO_AV_UDP_Flow_Handler (callback)
{
}

int
TAO_AV_RTP_UDP_Flow_Handler::handle_input (ACE_HANDLE fd)
{
  TAO_AV_RTP::rtphdr header;
  ACE_Message_Block *data = 0;
  ACE_INET_Addr addr;
  // Handles the incoming RTP packet input.
  int result = TAO_AV_RTP::handle_input (this->transport_,
                                         header,
                                         data,
                                         addr);
  if (result < 0)
    return result;
  // @@ What should we do with the header.
  // Answer:Use it for RTCP statistics. But then the
  // application needs the format of the data to decode it.
  result = this->callback_->receive_frame (data);
  return 0;
}

//TAO_AV_RTCP_UDP_Flow_Handler

TAO_AV_RTCP_UDP_Flow_Handler::TAO_AV_RTCP_UDP_Flow_Handler (void)
  :TAO_AV_Flow_Handler (0),
   TAO_AV_UDP_Flow_Handler (0)
{
}

int
TAO_AV_RTCP_UDP_Flow_Handler::handle_input (ACE_HANDLE fd)
{
  TAO_AV_RTCP::rtcphdr rtcp_header;
  int result = TAO_AV_RTCP::handle_input (this->transport_,
                                          rtcp_header);
  if (result < 0)
    return result;
  return 0;
}

int
TAO_AV_RTCP_UDP_Flow_Handler::handle_timeout (const ACE_Time_Value &tv,
                                              const void *arg)
{
  return -1;
}

int
TAO_AV_RTP_Object::send_frame (ACE_Message_Block *frame,
                               ACE_UINT32 timestamp)
{
  rtphdr header;
  int format;
  CORBA::ULong ssrc;
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
              ACE_dynamic_cast (TAO_AV_Payload_Type_Policy *,policy);
            if (payload_policy == 0)
              ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTP_Object::send_frame:Payload policy not defined\n"),-1);
            format = payload_policy->value ();
          }
          break;
        case TAO_AV_Policy::TAO_AV_SSRC_POLICY:
          {
            TAO_AV_SSRC_Policy *ssrc_policy =
              ACE_dynamic_cast (TAO_AV_SSRC_Policy *,policy);
            if (ssrc_policy == 0)
              ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTP_Object::send_frame:SSRC policy not defined\n"),-1);
            ssrc = ssrc_policy->value ();;
          }
          break;
        }
    }
  int result = TAO_AV_RTP::write_header (header,
                                         format,
                                         this->sequence_num_,
                                         timestamp,
                                         ssrc);
  if (result < 0)
    return result;
  result = TAO_AV_RTP::send_frame (this->transport_,
                                   header,
                                   frame);
  if (result < 0)
    return result;
}

TAO_AV_RTP_Object::TAO_AV_RTP_Object (TAO_AV_Callback *callback,
                                      TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (callback,transport),
   sequence_num_ (0)
{
  // We have to initialize the sequence number to a non-zero
  // random number.
}

int
TAO_AV_RTP_Object::end_stream (void)
{
  return -1;
}
