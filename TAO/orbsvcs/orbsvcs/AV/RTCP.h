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
#include "orbsvcs/AV/source.h"
#include "orbsvcs/AV/AVStreams_i.h"
#include "orbsvcs/AV/UDP.h"
#include <math.h>
#include <stdlib.h>
#include "orbsvcs/AV/RTP.h"

/**
 * @class TAO_AV_RTP_State
 * @brief Encapsulate the state of an RTP session
 */
class TAO_AV_Export TAO_AV_RTP_State
{
public:
  TAO_AV_RTP_State (void);
  int bad_version_;
  u_int badoptions_;
  u_int badfmt_;
  u_int badext_;
  u_int nrunt_;
  ACE_UINT32 last_np_;
  ACE_UINT32 sdes_seq_;
  double rtcp_inv_bw_;
  double rtcp_avg_size_;        /* (estimated) average size of rtcp packets */
  double rint_;         /* current session report rate (in ms) */
  int confid_;
  u_char* pktbuf_;
};

class TAO_AV_RTCP_Callback;
class TAO_AV_SourceManager;

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

  /*
 * Sender report.
 */
  struct rtcp_sr {
    ntp64 sr_ntp;               /* 64-bit ntp timestamp */
    ACE_UINT32 sr_ts;   /* reference media timestamp */
    ACE_UINT32 sr_np;   /* no. packets sent */
    ACE_UINT32 sr_nb;   /* no. bytes sent */
  };

  /*
 * Receiver report.
 * Time stamps are middle 32-bits of ntp timestamp.
 */
  struct rtcp_rr {
    ACE_UINT32 rr_srcid;        /* sender being reported */
    ACE_UINT32 rr_loss; /* loss stats (8:fraction, 24:cumulative)*/
    ACE_UINT32 rr_ehsr; /* ext. highest seqno received */
    ACE_UINT32 rr_dv;   /* jitter (delay variance) */
    ACE_UINT32 rr_lsr;  /* orig. ts from last rr from this src  */
    ACE_UINT32 rr_dlsr; /* time from recpt of last rr to xmit time */
  };

  static int handle_input (ACE_Message_Block *data,
                           const ACE_Addr &peer_address,
                           rtcphdr &header,
                           TAO_AV_SourceManager *source_manager,
                           TAO_AV_RTP_State *state);

  static int build_bye (rtcphdr* rh,
                        TAO_AV_Source& local);

  static int build_sdes (rtcphdr* rh,
                         TAO_AV_Source& s,
                         TAO_AV_RTP_State *state);

  static u_char* build_sdes_item (u_char* p,
                                  int code,
                                  TAO_AV_Source&);

  static void parse_sr (rtcphdr* rh,
                        int flags,
                        u_char* ep,
                        TAO_AV_Source* ps,
                        ACE_UINT32 addr,
                        TAO_AV_SourceManager *source_manager);

  static void parse_rr (rtcphdr* rh,
                        int flags,
                        u_char* ep,
                        TAO_AV_Source* ps,
                        ACE_UINT32 addr,
                        TAO_AV_SourceManager *source_manager);

  static void parse_rr_records (ACE_UINT32 ssrc,
                                rtcp_rr* r,
                                int cnt,
                                const u_char* ep,
                                ACE_UINT32 addr);

  static int sdesbody (ACE_UINT32* p,
                       u_char* ep,
                       TAO_AV_Source* ps,
                       ACE_UINT32 addr,
                       ACE_UINT32 ssrc,
                       TAO_AV_SourceManager *source_manager);

  static void parse_sdes (rtcphdr* rh,
                          int flags,
                          u_char* ep,
                          TAO_AV_Source* ps,
                          ACE_UINT32 addr,
                          ACE_UINT32 ssrc,
                          TAO_AV_SourceManager *source_manager);

  static void parse_bye (rtcphdr* rh,
                         int flags,
                         u_char* ep,
                         TAO_AV_Source* ps,
                         TAO_AV_SourceManager *source_manager);

  static void send_report (int bye,
                           TAO_AV_Protocol_Object *object,
                           TAO_AV_SourceManager *source_manager,
                           TAO_AV_RTP_State *state,
                           TAO_AV_RTCP_Callback *callback);

  static ACE_UINT32 alloc_srcid (ACE_UINT32 addr);

  static double fmod (double dividend, double divisor);
};

class TAO_AV_Callback;

/**
 * @class TAO_AV_RTCP_Object
 * @brief TAO_AV_Protocol_Object for RTCP protocol
 */
class TAO_AV_Export TAO_AV_RTCP_Object 
  : public TAO_AV_Protocol_Object
{
public:
  /// constructor.
  TAO_AV_RTCP_Object (TAO_AV_Callback *callback,
                      TAO_AV_Transport *transport = 0);

  /// Destructor
  virtual ~TAO_AV_RTCP_Object (void);

  virtual int handle_input (void);
  virtual int handle_control_input (ACE_Message_Block *frame,
                                    const ACE_Addr &peer_address);

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
};

/**
 * @class TAO_AV_Flow_Protocol_Factory
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

  virtual int receive_control_frame (ACE_Message_Block *frame,
                                     const ACE_Addr &peer_address = ACE_Addr::sap_any);

  /// Called during Streamctrl->destroy i.e tear_down  of the stream
  /// @@coryan:Call it handle_destroy or handle_close.
  virtual int handle_destroy (void);

  /// Called to get the timeout. If tv is 0 then the framework stop
  /// calling this.
  virtual void get_timeout (ACE_Time_Value *&tv,
                            void *&arg);

  int demux (TAO_AV_RTP::rtphdr* rh,
             ACE_Message_Block *data,
            const ACE_Addr &peer_address);

  virtual int get_rtp_source (TAO_AV_Source *&source,
                              ACE_UINT32 srcid,
                              ACE_UINT32 ssrc,
                              ACE_UINT32 addr);
  void schedule (int ms);
  TAO_AV_SourceManager *source_manager (void);
  TAO_AV_RTP_State *state (void);
protected:
  TAO_AV_SourceManager *source_manager_;
  TAO_AV_RTP_State *state_;
  int timeout_;
};

ACE_STATIC_SVC_DECLARE (TAO_AV_RTCP_Flow_Factory)
ACE_FACTORY_DECLARE (TAO_AV, TAO_AV_RTCP_Flow_Factory)

#include "ace/post.h"
#endif /* TAO_AV_RTCP_H */
