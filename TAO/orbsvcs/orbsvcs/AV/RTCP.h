/* -*- C++ -*- */
/*-
 * Copyright (c) 1993-1994 The Regents of the University of California.
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

//=============================================================================
/**
 *  @file   RTCP.h
 *
 *  $Id$
 *
 *  @author Nagarajan Surendran <naga@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_AV_RTCP_H
#define TAO_AV_RTCP_H
#include "ace/pre.h"

// FUZZ: disable check_for_math_include
#include "ace/OS.h"
#include "ace/Hash_Map_Manager.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/UDP.h"
#include <math.h>
#include <stdlib.h>
#include "orbsvcs/AV/RTP.h"

#include "RTCP_Channel.h"

class TAO_AV_RTCP_Callback;

/**
 * @class TAO_AV_RTCP
 * @brief Encapsulate the header format for the Real Time Control
 *        Protocol (RTCP)
 */
class TAO_AV_Export TAO_AV_RTCP
{
public:
  struct rtcphdr
  {
    ACE_UINT16 rh_flags;        /* T:2 P:1 CNT:5 PT:8 */
    ACE_UINT16 rh_len;  /* length of message (in bytes) */
    ACE_UINT32 rh_ssrc; /* synchronization src id */
  };

  struct ntp64
  {
    ACE_UINT32 upper;   /* more significant 32 bits */
    ACE_UINT32 lower;   /* less significant 32 bits */
  };

  struct md5_string
  {
    int type;
    ACE_Time_Value tv;
    pid_t pid;
    pid_t pgid;
    pid_t ppid;
    uid_t uid;
    gid_t gid;
  };

  static void send_report (ACE_Message_Block *mb);

  static ACE_UINT32 alloc_srcid (ACE_UINT32 addr);

  static double rtcp_interval (int members,
                               int senders,
                               double rtcp_bw,
                               int we_sent,
                               int packet_size,
                               int *avg_rtcp_size,
                               int initial);
};



/**
 * @class TAO_AV_RTCP_Flow_Factory
 * @brief
 */
class TAO_AV_Export TAO_AV_RTCP_Flow_Factory
  :public TAO_AV_Flow_Protocol_Factory
{
public:
  TAO_AV_RTCP_Flow_Factory (void);
  virtual ~TAO_AV_RTCP_Flow_Factory (void);
  virtual int init (int argc, char *argv[]);
  virtual int match_protocol (const char *flow_string);
  virtual TAO_AV_Protocol_Object* make_protocol_object (TAO_FlowSpec_Entry *entry,
                                                        TAO_Base_StreamEndPoint *endpoint,
                                                        TAO_AV_Flow_Handler *handler,
                                                        TAO_AV_Transport *transport);
};

class TAO_AV_Callback;


/**
 * @class TAO_AV_RTCP_Callback
 * @brief TAO_AV_Callback for RTCP protocol
 */
class TAO_AV_Export TAO_AV_RTCP_Callback : public TAO_AV_Callback
{
public:
  /// RTCP callback.
  TAO_AV_RTCP_Callback (void);

  /// virtual destructor.
  virtual ~TAO_AV_RTCP_Callback (void);

  /// Called during Streamctrl->start.
  virtual int handle_start (void);

  /// Called during Streamctrl->stop.
  virtual int handle_stop (void);

  /// Called during timeout for Flow Producers.
  virtual int handle_timeout (void *arg);

  /// Called when a frame arrives for a FlowConsumer.
  virtual int receive_frame (ACE_Message_Block *frame,
                             TAO_AV_frame_info *frame_info = 0,
                             const ACE_Addr &peer_address = ACE_Addr::sap_any);
  int send_frame (ACE_Message_Block *frame);

  virtual int receive_control_frame (ACE_Message_Block *frame,
                                     const ACE_Addr &peer_address = ACE_Addr::sap_any);

  /// Called during Streamctrl->destroy i.e tear_down  of the stream
  /// @@coryan:Call it handle_destroy or handle_close.
  virtual int handle_destroy (void);

  /// Called to get the timeout. If tv is 0 then the framework stop
  /// calling this.
  virtual void get_timeout (ACE_Time_Value *&tv,
                            void *&arg);

  int send_report(int bye);
  void schedule (int ms);
  TAO_AV_RTP_State *state (void);
  void ts_offset (ACE_UINT32 offset);

protected:
  ACE_Hash_Map_Manager<ACE_UINT32, RTCP_Channel_In*, ACE_Null_Mutex> inputs_;
  RTCP_Channel_Out output_;
  int timeout_;
  int timestamp_offset_;
  int sdes_count_;

  int is_initial_timeout_;
  int avg_rtcp_size_;
  int packet_size_;
};

class RTP_Packet;


/**
 * @class TAO_AV_RTCP_Object
 * @brief TAO_AV_Protocol_Object for RTCP protocol
 */
class TAO_AV_Export TAO_AV_RTCP_Object
  : public TAO_AV_Protocol_Object
{
public:
  /// constructor.
  TAO_AV_RTCP_Object (TAO_AV_Callback *client_cb,
                      TAO_AV_RTCP_Callback *&rtcp_cb,
                      TAO_AV_Transport *transport = 0);

  /// Destructor
  virtual ~TAO_AV_RTCP_Object (void);

  virtual int handle_input (void);
  virtual int handle_control_input (ACE_Message_Block *frame,
                                    const ACE_Addr &peer_address);
  virtual int handle_control_output (ACE_Message_Block *frame);

  /// set/get policies.
  virtual int set_policies (const TAO_AV_PolicyList &policy_list);

  /// start/stop the flow.
  virtual int start (void);
  virtual int stop (void);

  /// send a data frame.
  virtual int send_frame (ACE_Message_Block *frame,
                          TAO_AV_frame_info *frame_info = 0);

  /// send a frame in iovecs.
  virtual int send_frame (const iovec *iov,
                          int iovcnt,
                          TAO_AV_frame_info *frame_info = 0);

  virtual int send_frame (const char*buf,
                          size_t len);

  /// end the stream.
  virtual int destroy (void);

  void ssrc (ACE_UINT32 ssrc) {this->ssrc_ = ssrc; }
  ACE_UINT32 ssrc (void) { return this->ssrc_; }

  void ts_offset (ACE_UINT32 ts_offset);


private:
  TAO_AV_Callback *client_cb_;
  TAO_AV_RTCP_Callback rtcp_cb_;
  ACE_UINT32 ssrc_;
};



ACE_STATIC_SVC_DECLARE (TAO_AV_RTCP_Flow_Factory)
ACE_FACTORY_DECLARE (TAO_AV, TAO_AV_RTCP_Flow_Factory)

#include "ace/post.h"
#endif /* TAO_AV_RTCP_H */
