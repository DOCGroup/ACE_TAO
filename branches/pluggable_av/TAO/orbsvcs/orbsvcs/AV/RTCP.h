/* -*- C++ -*- */
// $Id$
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

// ============================================================================
//
// = LIBRARY
//   ORBSVCS AVStreams
//
// = FILENAME
//   RTCP.h
//
// = AUTHOR
//    Nagarajan Surendran <naga@cs.wustl.edu>
//
//
// ============================================================================

#ifndef TAO_AV_RTCP_H
#define TAO_AV_RTCP_H

#include "source.h"
#include "AVStreams_i.h"
#include <math.h>
#include <stdlib.h>

// TAO_AV_RTP_State
class TAO_AV_RTP_State
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

class TAO_AV_RTCP_Flow_Handler
{
public:
  TAO_AV_RTCP_Flow_Handler (ACE_Reactor *reactor,
                            TAO_Base_StreamEndPoint *endpoint,
                            const char *flowname);
  virtual ~TAO_AV_RTCP_Flow_Handler (void);
  void init (void);
  // initialize the timer.
  virtual ACE_Event_Handler *event_handler (void) = 0;
  void schedule (int value);
  TAO_AV_SourceManager *source_manager (void);
  TAO_AV_RTP_State *state (void);
  TAO_Base_StreamEndPoint *endpoint (void);
protected:
  TAO_AV_SourceManager *source_manager_;
  TAO_AV_RTP_State state_;
  ACE_Reactor *reactor_;
  TAO_Base_StreamEndPoint *endpoint_;
  CORBA::String_var flowname_;
};

class TAO_AV_RTCP_UDP_Flow_Handler
  :public TAO_AV_RTCP_Flow_Handler,
   public TAO_AV_UDP_Flow_Handler
{
public:
  TAO_AV_RTCP_UDP_Flow_Handler (ACE_Reactor *reactor,
                                TAO_Base_StreamEndPoint *endpoint,
                                const char *flowname);
  virtual ~TAO_AV_RTCP_UDP_Flow_Handler (void);
  virtual ACE_Event_Handler *event_handler (void);
  virtual int handle_input (ACE_HANDLE fd);
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);
};

class TAO_AV_RTCP_UDP_MCast_Flow_Handler
  :public TAO_AV_RTCP_Flow_Handler,
   public TAO_AV_UDP_MCast_Flow_Handler
{
public:
  TAO_AV_RTCP_UDP_MCast_Flow_Handler (ACE_Reactor *reactor,
                                      TAO_Base_StreamEndPoint *endpoint,
                                      const char *flowname);
  virtual ~TAO_AV_RTCP_UDP_MCast_Flow_Handler (void);
  virtual ACE_Event_Handler *event_handler (void);
  virtual int handle_input (ACE_HANDLE fd);
  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg = 0);
};

class TAO_AV_RTCP
{
public:
  struct rtcphdr
  {
    ACE_UINT16 rh_flags;        /* T:2 P:1 CNT:5 PT:8 */
    ACE_UINT16 rh_len;  /* length of message (in bytes) */
    ACE_UINT32 rh_ssrc; /* synchronization src id */
  };

  typedef struct
  {
    ACE_UINT32 upper;   /* more significant 32 bits */
    ACE_UINT32 lower;   /* less significant 32 bits */
  } ntp64;

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

  static int handle_input (TAO_AV_Transport *transport,
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
                           TAO_AV_Transport *transport,
                           TAO_AV_SourceManager *source_manager,
                           TAO_AV_RTP_State *state,
                           TAO_AV_RTCP_Flow_Handler *handler);

  static ACE_UINT32 alloc_srcid (ACE_UINT32 addr);

  typedef ACE_Hash_Map_Manager <TAO_String_Hash_Key,TAO_AV_RTCP_UDP_MCast_Flow_Handler*,ACE_Null_Mutex> RTCP_MCast_Map;
  typedef ACE_Hash_Map_Manager <TAO_String_Hash_Key,TAO_AV_RTCP_UDP_Flow_Handler*,ACE_Null_Mutex> RTCP_UDP_Map;
  static RTCP_MCast_Map rtcp_mcast_map_;
  static RTCP_UDP_Map rtcp_udp_map_;
};

#endif /* TAO_AV_RTCP_H */
