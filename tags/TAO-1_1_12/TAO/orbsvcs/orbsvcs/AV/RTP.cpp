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
#include "RTCP.h"
#include "Nil.h"

int
TAO_AV_RTP::handle_input (ACE_Message_Block *&data,
                          TAO_AV_frame_info *&frame_info)
{
  int n = data->length ();
  int size_phdr = ACE_static_cast (int, sizeof (rtphdr));
  if (n < size_phdr)
    ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::handle_input:invalid header\n"),-1);
  rtphdr* rh = (rtphdr*)data->rd_ptr ();
  int version = * (u_char*)rh >> 6;
  if (version != 2)
    {
      //      state->bad_version_++;
      ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP:bad version\n"),-1);
    }
  TAO_AV_RTP::rtphdr header;
  header = *rh;
  //  data->rd_ptr (sizeof (rtphdr));
  ACE_NEW_RETURN (frame_info,
                  TAO_AV_frame_info,
                  -1);
  frame_info->boundary_marker = header.rh_flags & RTP_M;
  frame_info->timestamp = header.rh_ts;
  frame_info->ssrc = header.rh_ssrc;
  frame_info->sequence_num = header.rh_seqno;
  frame_info->format = header.rh_flags & 0x7f;
  return 0;
}


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

// TAO_AV_RTP_Object

int
TAO_AV_RTP_Object::handle_input (void)
{
  TAO_AV_frame_info *frame_info = 0;

  // Handles the incoming RTP packet input.

  this->frame_.rd_ptr (this->frame_.base ());

  int n = this->transport_->recv (this->frame_.rd_ptr (),
                                  this->frame_.size ());
  if (n == 0)
    ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::handle_input:connection closed\n"),-1);
  if (n < 0)
    ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::handle_input:recv error\n"),-1);
  this->frame_.wr_ptr (this->frame_.rd_ptr () + n);
  ACE_Addr *addr = this->transport_->get_peer_addr ();

  ACE_Message_Block* data = &this->frame_;
  int result = TAO_AV_RTP::handle_input (data,
                                         frame_info);
  if (result < 0)
    return 0;
  this->control_object_->handle_control_input (data,
                                               *addr);

  this->frame_.rd_ptr (sizeof(TAO_AV_RTP::rtphdr));
  result = this->callback_->receive_frame (&this->frame_,
                                           frame_info,
                                           *addr);

  return 0;
}

int
TAO_AV_RTP_Object::send_frame (ACE_Message_Block *frame,
                               TAO_AV_frame_info *frame_info)
{
  TAO_AV_RTP::rtphdr header;
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

  ACE_Message_Block mb ((char *)&header,
                        sizeof (header));
  mb.wr_ptr (sizeof (header));
  mb.cont (frame);
  result = this->transport_->send (&mb);
  if (result < 0)
    ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP::send_frame failed\n"),result);
  return 0;
}

int
TAO_AV_RTP_Object::send_frame (const iovec *iov,
                               int iovcnt,
                               TAO_AV_frame_info *frame_info)
{
  TAO_AV_RTP::rtphdr header;
  //  int format = 0;
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

  iovec send_iov[IOV_MAX];
  send_iov [0].iov_base = (char *)&header;
  send_iov [0].iov_len  = sizeof(header);
  for (int i=1;i<=iovcnt; i++)
    send_iov [i] = iov [i-1];
  result = this->transport_->send (send_iov,
                                   iovcnt+1);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"TAO_AV_RTP::send_frame failed\n"),result);
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
   sequence_num_ (0),
   control_object_ (0)
{
  this->sequence_num_ = ACE_OS::rand ();

  this->frame_.size (2 * this->transport_->mtu ());
}

TAO_AV_RTP_Object::~TAO_AV_RTP_Object (void)
{
}

int
TAO_AV_RTP_Object::destroy (void)
{
  this->control_object_->destroy ();
  this->callback_->handle_destroy ();
  return 0;
}

int
TAO_AV_RTP_Object::set_policies (const TAO_AV_PolicyList &policy_list)
{
  this->policy_list_ = policy_list;
  u_int num_policies = this->policy_list_.length ();
  TAO_AV_Policy *policy = 0;
  for (u_int i=0; i< num_policies;i++)
    {
      policy = this->policy_list_ [i];
      switch (policy->type ())
        {
        case TAO_AV_PAYLOAD_TYPE_POLICY:
          {
            TAO_AV_Payload_Type_Policy *payload_policy =
              ACE_static_cast (TAO_AV_Payload_Type_Policy *,policy);
            if (payload_policy == 0)
              ACE_ERROR_RETURN ( (LM_ERROR,"TAO_AV_RTP_Object::send_frame:Payload policy not defined\n"),-1);
            this->format_ = payload_policy->value ();
          }
          break;
        case TAO_AV_SSRC_POLICY:
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

  return 0;
}

void
TAO_AV_RTP_Object::control_object (TAO_AV_Protocol_Object *object)
{
  this->control_object_ = object;
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
                                char * /* argv */ [])
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
  endpoint->get_callback (entry->flowname (),
                          callback);

  TAO_AV_Protocol_Object *object = 0;
  ACE_NEW_RETURN (object,
                  TAO_AV_RTP_Object (callback,
                                     transport),
                  0);
  callback->open (object,
                  handler);
  endpoint->set_protocol_object (entry->flowname (),
                                 object);
  return object;
}

int
TAO_AV_RTP_Flow_Factory::match_protocol (const char *flow_string)
{
  if (ACE_OS::strncasecmp (flow_string,"RTP",3) == 0)
    return 1;
  return 0;
}

const char *
TAO_AV_RTP_Flow_Factory::control_flow_factory (void)
{
  return "RTCP";
}

ACE_FACTORY_DEFINE (AV, TAO_AV_RTP_Flow_Factory)
ACE_STATIC_SVC_DEFINE (TAO_AV_RTP_Flow_Factory,
                       ACE_TEXT ("RTP_Flow_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_AV_RTP_Flow_Factory),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)
