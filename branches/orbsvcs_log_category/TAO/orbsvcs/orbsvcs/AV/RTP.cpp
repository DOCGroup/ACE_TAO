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

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/AV/RTP.h"
#include "orbsvcs/AV/RTCP.h"

#include "tao/debug.h"
#include "ace/OS_NS_arpa_inet.h"
#include "ace/OS_NS_strings.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// RTP_Packet

// Constructor for RTP packets received
RTP_Packet::RTP_Packet(char* buffer, int length)
{
  // skip the standard header info
  int index = 12;

  ACE_OS::memcpy(this->packet_, buffer, length);

  for (int j=0; j<(int)this->cc(); j++)
    {
      this->host_byte_order_csrc_list_[j] = ntohl(*(ACE_UINT32*)&buffer[index]);
      index+=4;
    }

  // ignore the header extension if there is one
  if (this->ext())
    {
      index+=2;
      int extension_data_size = ntohs(*(ACE_UINT16*)&buffer[index]);
      index+=2;
      index+=extension_data_size;

      this->extension_bytes_ = 4 + extension_data_size;
    }
  else
    this->extension_bytes_ = 0;

  this->packet_size_ = static_cast<ACE_UINT16> (length);
  this->payload_size_ = static_cast<ACE_UINT16> (length-index);

  // This is necessary only for payload types that have 16 bit values to correct
  //  the network byte ordering.
  if ((this->pt() == RTP_PT_L16_OTHER) ||
      (this->pt() == RTP_PT_L16_STEREO) ||
      (this->pt() == RTP_PT_L16_MONO))
    {
      for (int i=0; i < payload_size_; i+=2)
        {
          *(ACE_UINT16*)&this->host_byte_order_payload_[i] = ntohs(*(ACE_UINT16*)&this->packet_[index]);
          index+=2;
        }
    }
  else
    for (int i=0; i<this->payload_size_; i++)
      {
        this->host_byte_order_payload_[i] = this->packet_[index];
        index++;
      }
}

RTP_Packet::RTP_Packet(unsigned char padding,
                       unsigned char marker,
                       unsigned char payload_type,
                       ACE_UINT32 seq_num,
                       ACE_UINT32 timestamp,
                       ACE_UINT32 ssrc,
                       unsigned char csrc_count,
                       ACE_UINT32 *csrc_list,
                       char *data,
                       ACE_UINT16 data_size)
  :extension_bytes_(0)
{
  //size of header (in octets) without contributing sources
  ACE_UINT16 size = 3*4;
  int index = 0;

  if (data_size > RTP_MTU-12)
  {
    data_size = RTP_MTU-12;
    ORBSVCS_DEBUG ((LM_DEBUG, "\n(%N,%l) RTP_Packet: Warning - packet truncated\n"));
  }

  if (csrc_count > 15)
    csrc_count = 15; // Only 15 contributing sources can be specified.

  if (csrc_list != 0)
    for (unsigned char i=0; i<csrc_count; i++)
      {
        size+=4;
        this->host_byte_order_csrc_list_[i] = csrc_list[i];
      }

  this->packet_size_ = size + data_size;

  index = 0;
  this->packet_[index] = ((RTP_VERSION & 0x3) << 6) |
                          ((padding & 0x1) << 5) |
                          ((0 & 0x1) << 4) |  // extension bit
                          ((csrc_count & 0xf));

  index++;
  this->packet_[index] = ((marker & 0x1) << 7 ) |
                         ((payload_type & 0x7f));
  index++;
  *((ACE_UINT16*)&this->packet_[index]) = (ACE_UINT16)htons(static_cast<u_short> (seq_num));
  index+=2;
  *((ACE_UINT32*)&this->packet_[index]) = (ACE_UINT32)htonl(timestamp);
  index+=4;
  *((ACE_UINT32*)&this->packet_[index]) = htonl(ssrc);
  index+=4;

  for (int i=0; i<csrc_count; i++)
    {
      *((ACE_UINT32*)&this->packet_[index]) = htonl(this->host_byte_order_csrc_list_[i]);
      index+=4;
    }

    ACE_OS::memcpy (this->host_byte_order_payload_, data, data_size);

  this->payload_size_ = data_size;

  // Correct byte ordering for multi-byte payload types.
  if ((this->pt() == RTP_PT_L16_OTHER) ||
      (this->pt() == RTP_PT_L16_STEREO) ||
      (this->pt() == RTP_PT_L16_MONO))
    {
      for (int i=0; i<this->payload_size_; i+=2)
        {
          *(ACE_UINT16*)&this->packet_[index] = htons(*(ACE_UINT16*)&data[i]);
          index+=2;
        }
    }
  else
    for (int i=0; i<this->payload_size_; i++)
      {
        this->packet_[index] = data[i];
        index++;
      }
}

RTP_Packet::~RTP_Packet(void)
{
}

ACE_UINT16
RTP_Packet::packet_size(void)
{
    return this->packet_size_;
}

ACE_UINT16
RTP_Packet::payload_size(void)
{
    return this->payload_size_;
}

unsigned int
RTP_Packet::ver (void)
{
  return ( this->packet_[0] & 0xC0 ) >> 6;
}

unsigned int
RTP_Packet::pad (void)
{
  return ( this->packet_[0] & 0x20 ) >> 5;
}

unsigned int
RTP_Packet::ext (void)
{
  return ( this->packet_[0] & 0x10 ) >> 4;
}

unsigned int
RTP_Packet::cc  (void)
{
  return ( this->packet_[0] & 0x0F ) ;
}

unsigned int
RTP_Packet::mrk (void)
{
  return ( this->packet_[1] & 0x80 ) >> 7;
}

unsigned int
RTP_Packet::pt  (void)
{
  return ( this->packet_[1] & 0x7F ) ;
}

ACE_UINT16
RTP_Packet::sn  (void)
{
  return ntohs(*(ACE_UINT16*)(&this->packet_[2])) ;
}

ACE_UINT32
RTP_Packet::ts  (void)
{
  return ntohl(*(ACE_UINT32*)(&this->packet_[4])) ;
}

ACE_UINT32
RTP_Packet::ssrc (void)
{
  return ntohl(*(ACE_UINT32*)(&this->packet_[8])) ;
}

unsigned int
RTP_Packet::ext_bytes (void)
{
  return this->extension_bytes_;
}

void
RTP_Packet::get_frame_info (TAO_AV_frame_info *frame_info)
{
  frame_info->timestamp = this->mrk();
  frame_info->timestamp = this->ts();
  frame_info->ssrc = this->ssrc();
  frame_info->sequence_num = this->sn();
  frame_info->format = static_cast<CORBA::Octet> (this->pt());
}

int
RTP_Packet::is_valid (void)
{
  // taken from RFC 1889 - Appendix A.1

  // make sure the RTP version is correct
  if (this->ver() != RTP_VERSION)
    return 0;

  // make sure the payload type is not SR or RR
  if ((this->pt() == RTCP_PT_SR) || (this->pt() == RTCP_PT_RR))
    return 0;

  // if the p bit is set, the last octet of the packet must contain a valid
  // octet count, in particular, less than the total packet length minus
  // the header size.
  if (this->pad() != 0)
    if ((unsigned int)this->packet_[this->packet_size_] >=
        (this->packet_size_ - (12 + this->cc() + this->extension_bytes_)))
      return 0;

  // If there is an extension, it is ignored (taken care of in constructor)

  // The length of the packet must be consistent with CC and payload type (if
  // payloads have a known length)

  return 1;
}

void
RTP_Packet::get_csrc_list (ACE_UINT32 **csrc_list, ACE_UINT16 &length)
{
  *csrc_list = this->host_byte_order_csrc_list_;
  length = static_cast<ACE_UINT16> (this->cc ());
}

void
RTP_Packet::get_payload (char **payload, ACE_UINT16 &length)
{
  *payload = this->host_byte_order_payload_ ;
  length = this->payload_size_;
}

void
RTP_Packet::get_packet_data (char **packet, ACE_UINT16 &length)
{
  *packet = this->packet_;
  length = this->packet_size_;
}


// TAO_AV_RTP_Object

int
TAO_AV_RTP_Object::handle_input (void)
{
  TAO_AV_frame_info frame_info;

  if (TAO_debug_level > 1)
    ORBSVCS_DEBUG ((LM_DEBUG,
                "\nTAO_AV_RTP_Object::handle_input\n"));

  // Handles the incoming RTP packet input.
  this->frame_.rd_ptr (this->frame_.base ());

  int n = this->transport_->recv (this->frame_.rd_ptr (),
                                  this->frame_.size ());
  if (n == 0)
    ORBSVCS_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::handle_input:connection closed\n"),-1);
  if (n < 0)
    {
      if ((errno == EADDRNOTAVAIL) || (errno == ECONNRESET))
        {
          this->connection_gone_ = 1;
          return -1;
        }
      else
        ORBSVCS_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::handle_input:recv error\n"),-1);
    }

  this->frame_.wr_ptr (this->frame_.rd_ptr () + n);
  ACE_Addr *addr = this->transport_->get_peer_addr ();

  if(this->control_object_)
    this->control_object_->handle_control_input (&this->frame_, *addr);

  // Get payload that has been converted to host byte order
  char *data_ptr;
  ACE_UINT16 length;
  RTP_Packet rtp_packet(this->frame_.rd_ptr (),
                        static_cast<int> (this->frame_.length ()));

  rtp_packet.get_frame_info (&frame_info);
  rtp_packet.get_payload(&data_ptr, length);

  this->frame_.rd_ptr (this->frame_.base ());
  ACE_OS::memcpy (this->frame_.rd_ptr (), data_ptr, length);
  this->frame_.wr_ptr (this->frame_.rd_ptr() + length);

  this->callback_->receive_frame (&this->frame_, &frame_info, *addr);

  return 0;
}

int
TAO_AV_RTP_Object::send_frame (ACE_Message_Block *frame,
                               TAO_AV_frame_info *frame_info)
{
//  ACE_Addr *addr = this->transport_->get_peer_addr ();

  if (this->connection_gone_)
    {
      errno = ECONNRESET;
      return -1;
    }

  int result = -1;
  RTP_Packet *rtp_packet;
  ACE_UINT32 csrc_count = 0;  // Assume for now no mixers/translators
  ACE_UINT32 *csrc_list = 0;

  if (frame_info != 0)
    {
      if (frame_info->format != this->format_)
        ORBSVCS_DEBUG ((LM_DEBUG,
                    "TAO_AV_RTP_Object::send_frame - error: format type mismatch"));
      if (frame_info->ssrc != 0)
        this->ssrc_ = frame_info->ssrc;

      TAO_AV_RTCP_Object *rtcp_prot_obj = dynamic_cast<TAO_AV_RTCP_Object*> (this->control_object_);
      // set the ssrc on the control object so the RTCP traffic can be matched
      // to the RTP traffic
      rtcp_prot_obj->ssrc(this->ssrc_);

      ACE_NEW_RETURN (rtp_packet,
                      RTP_Packet (0,                            // padding
                                  frame_info->boundary_marker,  // marker
                                  static_cast<unsigned char> (this->format_),                // payload type
                                  frame_info->sequence_num,     // sequence num
                                  frame_info->timestamp,        // time stamp
                                  this->ssrc_,                  // ssrc
                                  static_cast<unsigned char> (csrc_count),                   // csrc count
                                  csrc_list,                    // csrc list
                                  frame->rd_ptr (),             // data
                                  (ACE_UINT16)frame->length ()),// data size
                                  -1);

      frame_info->sequence_num ++;
    }
  else
    {
      // TODO: For periodic RTP packets (constant rate), the RFC suggests
      //       increasing the clock by the number of samples each frame rather
      //       than relying on the system time

      // The RFC specifies at least one timestamp unit per sample as well as a
      //  random offset.  It used to be in milliseconds so I left it that way
      //  for non-audio streams.
      unsigned int samples_per_sec;
      double samples_per_usec;

      switch (this->format_)
      {
        case RTP_PT_PCMU:
        case RTP_PT_CELP:
        case RTP_PT_G721:
        case RTP_PT_GSM:
        case RTP_PT_DVI:
        case RTP_PT_LPC:
        case RTP_PT_PCMA:
        case RTP_PT_G722:
          samples_per_sec = 8000;
          break;
        case RTP_PT_L16_STEREO:
        case RTP_PT_L16_MONO:
          samples_per_sec = 44100;
          break;
        default:
          samples_per_sec = 1000000;
      };

      samples_per_usec = samples_per_sec/1000000.0;

      ACE_Time_Value now = ACE_OS::gettimeofday();

      ACE_UINT32 ts = (ACE_UINT32)
                      (now.sec () * samples_per_sec +
                       ((double)now.usec () * samples_per_usec) +
                       this->timestamp_offset_);

      ACE_NEW_RETURN (rtp_packet,
                      RTP_Packet (0,                            // padding
                                  0,                            // marker
                                  static_cast<unsigned char> (this->format_),                // payload type
                                  this->sequence_num_,          // sequence num
                                  ts,                           // time stamp
                                  this->ssrc_,                  // ssrc
                                  static_cast<unsigned char> (csrc_count),                   // csrc count
                                  csrc_list,                    // csrc list
                                  frame->rd_ptr (),             // data
                                  (ACE_UINT16)frame->length ()),// data size
                                  -1);

      this->sequence_num_ ++;
    }

  char *data_ptr;
  ACE_UINT16 data_length;
  rtp_packet->get_packet_data (&data_ptr, data_length);

  ACE_Message_Block mb (data_ptr, data_length);
  mb.wr_ptr (data_length);

  result = this->transport_->send (&mb);
  if (result < 0)
    ORBSVCS_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::send_frame failed\n"),result);

  TAO_AV_RTCP_Object *rtcp_prot_obj = dynamic_cast<TAO_AV_RTCP_Object*> (this->control_object_);
  if (rtcp_prot_obj)
    rtcp_prot_obj->handle_control_output (&mb);

  delete rtp_packet;

  return 0;
}

int
TAO_AV_RTP_Object::send_frame (const iovec *iov,
                               int iovcnt,
                               TAO_AV_frame_info *frame_info)
{
  int result = -1;
  RTP_Packet *rtp_packet = 0;
  ACE_UINT32 csrc_count = 0;  // Assume for now no mixers/translators
  ACE_UINT32 *csrc_list = 0;

  if (this->connection_gone_)
    {
      errno = ECONNRESET;
      return -1;
    }

  if (frame_info != 0)
    {
      if (frame_info->format != this->format_)
        ORBSVCS_DEBUG ((LM_DEBUG,
                    "TAO_AV_RTP_Object::send_frame - error: format type mismatch"));
      this->sequence_num_ = static_cast<ACE_UINT16> (frame_info->sequence_num);
      if (frame_info->ssrc != 0)
        this->ssrc_ = frame_info->ssrc;

      TAO_AV_RTCP_Object *rtcp_prot_obj = dynamic_cast<TAO_AV_RTCP_Object*> (this->control_object_);
      // set the ssrc on the control object so the RTCP traffic can be matched
      // to the RTP traffic
      rtcp_prot_obj->ssrc(this->ssrc_);
      ACE_UINT16 data_size = static_cast<ACE_UINT16> (iov[0].iov_len);

      ACE_NEW_RETURN (rtp_packet,
                      RTP_Packet (0,                            // padding
                                  frame_info->boundary_marker,  // marker
                                  static_cast<unsigned char> (this->format_),                // payload type
                                  frame_info->sequence_num,     // sequence num
                                  frame_info->timestamp,        // time stamp
                                  this->ssrc_,                  // ssrc
                                  static_cast<unsigned char> (csrc_count),                   // csrc count
                                  csrc_list,                    // csrc list
                                  (char *)iov[0].iov_base,      // data
                                  data_size),                   // data size
                                  -1);

      frame_info->sequence_num ++;
    }
  else
    {
      // TODO: For periodic RTP packets (constant rate), the RFC suggests
      //       increasing the clock by the number of samples each frame rather
      //       than relying on the system time

      // The RFC specifies at least one timestamp unit per sample as well as a
      //  random offset.  It used to be in milliseconds so I left it that way
      //  for non-audio streams.

      unsigned int samples_per_sec;
      double samples_per_usec;

      switch (this->format_)
      {
        case RTP_PT_PCMU:
        case RTP_PT_CELP:
        case RTP_PT_G721:
        case RTP_PT_GSM:
        case RTP_PT_DVI:
        case RTP_PT_LPC:
        case RTP_PT_PCMA:
        case RTP_PT_G722:
          samples_per_sec = 8000;
          break;
        case RTP_PT_L16_STEREO:
        case RTP_PT_L16_MONO:
          samples_per_sec = 44100;
          break;
        default:
          samples_per_sec = 1000000;
      };

      samples_per_usec = samples_per_sec/1000000.0;

      ACE_Time_Value now = ACE_OS::gettimeofday();

      ACE_UINT32 ts = (ACE_UINT32)
                      (now.sec () * samples_per_sec +
                       ((double)now.usec () * samples_per_usec) +
                       this->timestamp_offset_);
      ACE_UINT16 data_size = static_cast<ACE_UINT16> (iov[0].iov_len);

      ACE_NEW_RETURN (rtp_packet,
                      RTP_Packet (0,                            // padding
                                  0,                            // marker
                                  static_cast<unsigned char> (this->format_),                // payload type
                                  this->sequence_num_,          // sequence num
                                  ts,                           // time stamp
                                  this->ssrc_,                  // ssrc
                                  static_cast<unsigned char> (csrc_count),                   // csrc count
                                  csrc_list,                    // csrc list
                                  (char *)iov[0].iov_base,      // data
                                  data_size),                   // data size
                                  -1);

      this->sequence_num_ ++;
    }

  char *data_ptr;
  ACE_UINT16 data_length;
  rtp_packet->get_packet_data (&data_ptr, data_length);

  iovec send_iov[ACE_IOV_MAX];
  send_iov [0].iov_base = data_ptr;
  send_iov [0].iov_len  = data_length;
  for (int i=1;i<iovcnt; i++)
    send_iov [i] = iov [i];
  result = this->transport_->send (send_iov, iovcnt);

  delete rtp_packet;

  if (result < 0)
    ORBSVCS_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTP::send_frame failed\n"),result);

  return 0;
}

int
TAO_AV_RTP_Object::send_frame (const char*,
                               size_t)
{
  return 0;
}


TAO_AV_RTP_Object::TAO_AV_RTP_Object (TAO_AV_Callback *callback,
                                      TAO_AV_Transport *transport)
  :TAO_AV_Protocol_Object (callback,transport),
   control_object_ (0),
   connection_gone_ (0)
{
  this->sequence_num_ = static_cast<ACE_UINT16> (ACE_OS::rand ());
  this->timestamp_offset_ = ACE_OS::rand ();

  char buf [BUFSIZ];
  int result = ACE_OS::hostname (buf, BUFSIZ);
  unsigned long ipaddr = 0;
  if (result == 0)
    ipaddr = ACE_OS::inet_addr (buf);
  this->ssrc_ = TAO_AV_RTCP::alloc_srcid (ipaddr);

  this->frame_.size (2 * this->transport_->mtu ());
}

TAO_AV_RTP_Object::~TAO_AV_RTP_Object (void)
{
}

int
TAO_AV_RTP_Object::destroy (void)
{
  if(this->control_object_)
     this->control_object_->destroy ();

  this->callback_->handle_destroy ();
  delete this;

  return 0;
}

int
TAO_AV_RTP_Object::set_policies (const TAO_AV_PolicyList &policy_list)
{
  this->policy_list_ = policy_list;
  CORBA::ULong const num_policies = this->policy_list_.length ();
  TAO_AV_Policy *policy = 0;

  for (u_int i=0; i< num_policies;i++)
    {
      policy = this->policy_list_ [i];
      switch (policy->type ())
        {
        case TAO_AV_PAYLOAD_TYPE_POLICY:
          {
            TAO_AV_Payload_Type_Policy *payload_policy =
              static_cast<TAO_AV_Payload_Type_Policy *> (policy);
            if (payload_policy == 0)
              ORBSVCS_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP_Object::send_frame:Payload policy not defined\n"),-1);
            this->format_ = payload_policy->value ();
          }
          break;
        case TAO_AV_SSRC_POLICY:
          {
            TAO_AV_SSRC_Policy *ssrc_policy =
              static_cast<TAO_AV_SSRC_Policy *> (policy);
            if (ssrc_policy == 0)
              ORBSVCS_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP_Object::send_frame:SSRC policy not defined\n"),-1);
            this->ssrc_ = ssrc_policy->value ();;
          }
          break;
        default:
          break;
        }
    }

  return 0;
}

void
TAO_AV_RTP_Object::control_object (TAO_AV_Protocol_Object *object)
{
  this->control_object_ = object;
  TAO_AV_RTCP_Object *rtcp_prot_obj = dynamic_cast<TAO_AV_RTCP_Object*> (this->control_object_);
  rtcp_prot_obj->ssrc (this->ssrc_);
  rtcp_prot_obj->ts_offset (this->timestamp_offset_);
}

int
TAO_AV_RTP_Object::start (void)
{
  this->control_object_->start ();
  return this->callback_->handle_start ();
}

int
TAO_AV_RTP_Object::stop (void)
{
  this->control_object_->stop ();
  return this->callback_->handle_stop ();
}

// TAO_AV_RTP_Flow_Factory
TAO_AV_RTP_Flow_Factory::TAO_AV_RTP_Flow_Factory (void)
{
}

TAO_AV_RTP_Flow_Factory::~TAO_AV_RTP_Flow_Factory (void)
{
}

int
TAO_AV_RTP_Flow_Factory::init (int /* argc */,
                               ACE_TCHAR * /* argv */ [])
{
  return 0;
}

TAO_AV_Protocol_Object*
TAO_AV_RTP_Flow_Factory::make_protocol_object (TAO_FlowSpec_Entry *entry,
                                               TAO_Base_StreamEndPoint *endpoint,
                                               TAO_AV_Flow_Handler *handler,
                                               TAO_AV_Transport *transport)
{
  TAO_AV_Callback *callback = 0;

  if( endpoint->get_callback (entry->flowname (), callback) ) {
    ORBSVCS_ERROR_RETURN ((LM_ERROR, "(%N,%l) Invalid callback\n"), 0);
  }

  TAO_AV_Protocol_Object *object = 0;
  ACE_NEW_RETURN (object,
                  TAO_AV_RTP_Object (callback,
                                     transport),
                  0);
  callback->open (object,
                  handler);
  endpoint->set_protocol_object (entry->flowname (),
                                 object);

  endpoint->protocol_object_set ();
  return object;
}

int
TAO_AV_RTP_Flow_Factory::match_protocol (const char *flow_string)
{
  if (ACE_OS::strncasecmp (flow_string,"RTP",3) == 0)
    {
      return 1;
    }
  return 0;
}

const char *
TAO_AV_RTP_Flow_Factory::control_flow_factory (void)
{
  return "RTCP";
}

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_AV, TAO_AV_RTP_Flow_Factory)
ACE_STATIC_SVC_DEFINE (TAO_AV_RTP_Flow_Factory,
                       ACE_TEXT ("RTP_Flow_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_AV_RTP_Flow_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)
