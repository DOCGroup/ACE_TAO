/* -*- C++ -*- */
// $Id$
/*
 * Copyright (c) 1994 Regents of the University of California.
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
 *
 * @(#) $Header$
 */

#ifndef TAO_AV_SOURCE_H
#define TAO_AV_SOURCE_H
#include "ace/pre.h"

#define RTCP_SDES_MAX 7

#include "ace/OS.h"
#include "tao/TAO.h"

class TAO_AV_SourceManager;

extern "C" 
{
  int compare_func (const void* v0, const void* v1);
}


#define TAO_AV_SOURCE_HASH 1024

class TAO_AV_Source
{
public:
  TAO_AV_Source (ACE_UINT32 srcid, ACE_UINT32 ssrc, ACE_UINT32 addr);
  virtual ~TAO_AV_Source (void);
  void deactivate (void);

  void action();
  // @@Naga:My additions.
  char *name ();
  int active (void);
  void active (int active);
  virtual void set_busy();
  ACE_UINT32 delvar () const;
  void delvar (ACE_UINT32 v);

  void lts_done (const timeval& now);
  void lts_data (const timeval& now);
  void lts_ctrl (const timeval& now);
  void sts_data (ACE_UINT32 now);
  void sts_ctrl (ACE_UINT32 now);

  const timeval& lts_ctrl (void) const;
  const timeval& lts_data (void) const;
  const timeval& lts_done (void) const;
  int sts_ctrl (void) const;
  int sts_data (void) const;

  const char* sdes (int t) const;
  virtual void sdes (int t, const char* value);
  ACE_UINT32 addr (void) const;
  void addr (ACE_UINT32 a);
  ACE_UINT32 srcid (void) const;
  void srcid (ACE_UINT32 s);
  ACE_UINT32 ssrc (void) const;
  void ssrc (ACE_UINT32 s);
  void format (int v);
  int format (void) const;
  void mute (int v);
  int mute (void) const;
  void ismixer (int v);
  int ismixer (void) const;
  void clear_counters (void);

  /*XXX should start at random values*/
  ACE_UINT32 nb (void) const;
  ACE_UINT32 nf (void) const;
  ACE_UINT32 np (void) const;
  ACE_UINT32 nm (void) const;
  ACE_UINT32 ns (void) const; /* no. expected */
  ACE_UINT32 ehs (void) const;
  ACE_UINT32 cs (void) const;
  ACE_UINT32 snp (void) const;
  ACE_UINT32 sns (void) const;
  ACE_UINT32 runt (void) const;
  ACE_UINT32 dups (void) const;
  ACE_UINT32 badsesslen (void) const;
  ACE_UINT32 badsessver (void) const;
  ACE_UINT32 badsessopt (void) const;
  ACE_UINT32 badsdes (void) const;
  ACE_UINT32 badbye (void) const;
  void nb (int v);
  void nf (int v);
  void np (int v);
  void nm (int v);
  void snp (int v);
  void sns (int v);
  void fs (int v);
  void runt (int v);
  void badsesslen (int v);
  void badsessver (int v);
  void badsessopt (int v);
  void badsdes (int v);
  void badbye (int v);
  int cs (ACE_UINT16 v);
  int checkseq (ACE_UINT16 v);
  virtual void lost (int);

  TAO_AV_Source* next_;          /* link for TAO_AV_SourceManager TAO_AV_Source list */
  TAO_AV_Source* hlink_;         /* link for TAO_AV_SourceManager hash table */
protected:
  char* stats (char* cp) const;

  // @@Naga:My additions
  int active_;
  char *name_;
  int delvar_;
  ACE_UINT32 srcid_;       /* rtp global src id  (CSRC), net order */
  ACE_UINT32 ssrc_;        /* rtp global sync src id  (SSRC), net order) */
  ACE_UINT32 addr_;        /* address of sender  (net order) */

  ACE_UINT32 sts_data_;    /* sndr ts from last data packet  (net order) */
  ACE_UINT32 sts_ctrl_;    /* sndr ts from last control packet */
  timeval lts_data_;      /* local unix time for last data packet */
  timeval lts_ctrl_;      /* local unix time for last control packet */
  timeval lts_done_;      /* local unix time for bye packet */

  ACE_UINT32 fs_;          /* first seq. no received */
  ACE_UINT32 cs_;          /* current  (most recent) seq. no received */
  ACE_UINT32 np_;          /* no. packets received  */
  ACE_UINT32 nf_;          /* no. video frames received */
  ACE_UINT32 nb_;          /* no. bytes received */
  ACE_UINT32 nm_;          /* no. misordered packets detected */
  ACE_UINT32 snp_;         /* last advertised no. pkts received */
  ACE_UINT32 sns_;         /* last advertised no. pkts exptected */
  ACE_UINT32 ndup_;        /* no. of duplicate packets  (via RTP seqno) */
  ACE_UINT32 nrunt_;       /* count of packets too small */

  /* following errors are from session  (rtcp) processing */
  ACE_UINT32 badsesslen_;  /* bad header length field */
  ACE_UINT32 badsessver_;  /* bad header version number */
  ACE_UINT32 badsessopt_;  /* unrecognized option */
  ACE_UINT32 badsdes_;     /* sdes cnt > available data */
  ACE_UINT32 badbye_;      /* bye cnt > available data */

  int format_;            /* RTP packet type */
  int mute_;              /* true if Source muted */
  int lost_;              /* true when signal lost */
  int busy_;              /* nonzero. during talk spurt */
  int ismixer_;           /* true if TAO_AV_Source has acted as a 'mixer' */

#define TAO_AV_SOURCE_NSEQ 64
  ACE_UINT16 seqno_[TAO_AV_SOURCE_NSEQ];
  char* sdes_[RTCP_SDES_MAX + 1];
};

int sdes_atoi (const char* s);
char* onestat (char* cp, const char* name, u_long v);
class TAO_Base_StreamEndPoint;
class TAO_AV_RTCP_Callback;

class TAO_AV_SourceManager
{
public:
  TAO_AV_SourceManager (TAO_AV_RTCP_Callback *callback);
  void init (ACE_UINT32 localid, ACE_UINT32 localaddr);
  TAO_AV_Source* lookup (ACE_UINT32 srcid, ACE_UINT32 ssrc, ACE_UINT32 addr);
  TAO_AV_Source* demux (ACE_UINT32 srcid, ACE_UINT32 addr, ACE_UINT16 seq);
  TAO_AV_Source* consult (ACE_UINT32 srcid);
  int nsources (void) const;
  TAO_AV_Source* sources (void) const;

  void CheckActiveSources (double msgint);
  void ListSources (void);

  ACE_UINT32 clock (void) const;
  TAO_AV_Source* localsrc (void) const;
  void localsrc (TAO_AV_Source* src);

  void sortactive (char*) const;
  void remove (TAO_AV_Source*);
  void keep_sites (int keep);
  void site_drop_time (int time);
  TAO_AV_Source* enter (TAO_AV_Source* s);
  static int compare (const void*, const void*);
protected:
  void remove_from_hashtable (TAO_AV_Source* s);

  TAO_AV_Source* lookup_duplicate (ACE_UINT32 srcid, ACE_UINT32 addr);

  int nsources_;
  TAO_AV_Source* sources_;
  ACE_UINT32 clock_;
  int keep_sites_;
  u_int site_drop_time_;
  TAO_AV_Source* localsrc_;
  TAO_AV_Source* hashtab_[TAO_AV_SOURCE_HASH];

  TAO_AV_RTCP_Callback *callback_;
};

#if defined (__ACE_INLINE__)
#include "source.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_AV_Source_H */
