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
#include "ace/OS.h"
#include "RTCP.h"
#include "source.h"

#if !defined (__ACE_INLINE__)
#include "source.i"
#endif /* __ACE_INLINE__ */

/* gray out src if no ctrl msgs for this many consecutive update intervals */
#define CTRL_IDLE 8.

#define SHASH(a) ((int)((((a) >> 20) ^ ((a) >> 10) ^ (a)) & (TAO_AV_SOURCE_HASH-1)))

extern "C" int 
compare_func (const void* v0, const void* v1)
{
  return TAO_AV_SourceManager::compare (v0, v1);
}

TAO_AV_Source::TAO_AV_Source (ACE_UINT32 srcid, ACE_UINT32 ssrc, ACE_UINT32 addr)
  : next_ (0),
    hlink_ (0),
    srcid_ (srcid),
    ssrc_ (ssrc),
    addr_ (addr),
    sts_data_ (0),
    sts_ctrl_ (0),
    fs_ (0),
    cs_ (0),
    np_ (0),
    nf_ (0),
    nb_ (0),
    nm_ (0),
    snp_ (0),
    sns_ (0),
    ndup_ (0),
    nrunt_ (0),
    badsesslen_ (0),
    badsessver_ (0),
    badsessopt_ (0),
    badsdes_ (0),
    badbye_ (0),
    format_ (-1),
    mute_ (0),
    lost_ (0),
    busy_ (0),
    ismixer_ (0)
{
  lts_data_.tv_sec = 0;
  lts_data_.tv_usec = 0;
  lts_ctrl_.tv_sec = 0;
  lts_ctrl_.tv_usec = 0;
  lts_done_.tv_sec = 0;
  lts_done_.tv_usec = 0;
  /*
   * Put an invalid seqno in each slot to guarantee that
   * we don't count any initial dups by mistake.
   */
  int i;
  for  (i = 0; i < TAO_AV_SOURCE_NSEQ; ++i)
    seqno_[i] = i + 1;

  for  (i = 0; i <= RTCP_SDES_MAX; ++i)
    sdes_[i] = 0;
}

TAO_AV_Source::~TAO_AV_Source ()
{
  int i;
  for  (i = 0; i <= RTCP_SDES_MAX; ++i)
    delete sdes_[i];
}

void
TAO_AV_Source::sdes (int t, const char* s)
{
  char** p = &sdes_[t];
  if  (*p != 0 && strcmp (*p, s) == 0)
    /* no change */
    return;

  delete *p;
  int n = strlen (s);
  if  (n > 254)
    n = 254;
  ACE_NEW (*p,
           char[n + 1]);
  strncpy (*p, s, n + 1);
}

char*
onestat (char* cp, const char* name, u_long v)
{
  sprintf (cp, "%s %lu ", name, v);
  return  (cp + strlen (cp));
}

char*
TAO_AV_Source::stats (char* cp) const
{
  cp = onestat (cp, "Kilobits", nb () >>  (10-3));
  cp = onestat (cp, "Frames", nf ());
  cp = onestat (cp, "Packets", np ());
  int missing = ns () - np ();
  if  (missing < 0)
    missing = 0;
  cp = onestat (cp, "Missing", missing);
  cp = onestat (cp, "Misordered", nm ());
  cp = onestat (cp, "Runts", runt ());
  cp = onestat (cp, "Dups", dups ());
  cp = onestat (cp, "Bad-S-Len", badsesslen ());
  cp = onestat (cp, "Bad-S-Ver", badsessver ());
  cp = onestat (cp, "Bad-S-Opt", badsessopt ());
  cp = onestat (cp, "Bad-Sdes", badsdes ());
  cp = onestat (cp, "Bad-Bye", badbye ());
  *--cp = 0;
  return  (cp);
}

int
sdes_atoi (const char* s)
{
  if  (ACE_OS::strcasecmp (s, "cname") == 0)
    return  (RTCP_SDES_CNAME);
  if  (ACE_OS::strcasecmp (s, "name") == 0)
    return  (RTCP_SDES_NAME);
  if  (ACE_OS::strcasecmp (s, "email") == 0)
    return  (RTCP_SDES_EMAIL);
  if  (ACE_OS::strcasecmp (s, "phone") == 0)
    return  (RTCP_SDES_PHONE);
  if  (ACE_OS::strcasecmp (s, "loc") == 0 || ACE_OS::strcasecmp (s, "location") == 0)
    return  (RTCP_SDES_LOC);
  if  (ACE_OS::strcasecmp (s, "tool") == 0)
    return  (RTCP_SDES_TOOL);
  if  (ACE_OS::strcasecmp (s, "note") == 0)
    return  (RTCP_SDES_NOTE);
  return  (-1);
}

void
TAO_AV_Source::set_busy()
{
  busy_ = 1;
}

void
TAO_AV_Source::clear_counters ()
{
  np_ = 0;
  nf_ = 0;
  nb_ = 0;
  nm_ = 0;
  snp_ = 0;
  sns_ = 0;
  ndup_ = 0;
  nrunt_ = 0;

  lts_data_.tv_sec = 0;
  lts_data_.tv_usec = 0;
  lts_ctrl_.tv_sec = 0;
  lts_ctrl_.tv_usec = 0;
  lts_done_.tv_sec = 0;
  lts_done_.tv_usec = 0;
}

TAO_AV_SourceManager::TAO_AV_SourceManager  (TAO_AV_RTCP_Callback *callback)
  :nsources_ (0),
   sources_ (0),
   clock_ (0),
   keep_sites_ (0),
   site_drop_time_ (0),
   localsrc_ (0),
   callback_ (callback)
{
  memset ( (char*)hashtab_, 0, sizeof (hashtab_));
}

void
TAO_AV_SourceManager::remove_from_hashtable (TAO_AV_Source* s)
{
  /* delete the TAO_AV_Source from hash table */
  ACE_UINT32 srcid = s->srcid ();
  int h = SHASH (srcid);
  TAO_AV_Source** p = &hashtab_[h];
  while  (*p != s)
    p = & (*p)->hlink_;
  *p =  (*p)->hlink_;
}

void
TAO_AV_SourceManager::init (ACE_UINT32 localid, ACE_UINT32 localaddr)
{
  /*
   * create the local object.  remove it from the hash
   * table since collision resolution changes local id
   *  (we special case detection of our own loopbed back packets)
   */
  // Make an upcall to get the source.
  this->callback_->get_rtp_source (localsrc_,
                                   localid,
                                   localid,
                                   localaddr);
  enter (localsrc_);
  remove_from_hashtable (localsrc_);
  /*
         * hack to prevent local TAO_AV_Source from turning gray at startup.
         * we don't need to do a similar thing for external TAO_AV_Sources,
         * because they are only created when a packet arrives.
         */
  localsrc_->lts_ctrl (ACE_OS::gettimeofday ());
}

TAO_AV_Source*
TAO_AV_SourceManager::enter (TAO_AV_Source* s)
{
  s->next_ = sources_;
  sources_ = s;

  int h = SHASH (s->srcid ());
  s->hlink_ = hashtab_[h];
  hashtab_[h] = s;

  ++nsources_;

  return  (s);
}

TAO_AV_Source*
TAO_AV_SourceManager::consult (ACE_UINT32 srcid)
{
  int h = SHASH (srcid);
  for  (TAO_AV_Source* s = hashtab_[h]; s != 0; s = s->hlink_) {
    /*XXX pulling these values into variable seems
      to work around a DEC cpp bug */
    ACE_UINT32 id = s->srcid ();
    if  (id == srcid)
      return  (s);
  }
  return  (0);
}

TAO_AV_Source*
TAO_AV_SourceManager::lookup (ACE_UINT32 srcid, ACE_UINT32 ssrc, ACE_UINT32 addr)
{
  TAO_AV_Source* s = consult (srcid);
  if  (s == 0) {
    if  (srcid == ssrc)
      /*
       * Heuristic: handle application re-starts
       * gracefully.  We have a new TAO_AV_Source that's
       * not via a mixer.  Try to find an existing
       * entry from the same host.
       */
      s = lookup_duplicate (srcid, addr);

    if  (s == 0) {
      this->callback_->get_rtp_source (s,
                                       srcid,
                                       ssrc,
                                       addr);
//       ACE_NEW_RETURN (s,
//                       TAO_AV_Source (srcid, ssrc, addr),
//                       0);
      enter (s);
    }
  }
  return  (s);
}

/*
 * Demux data packet to its TAO_AV_Source table entry.   (We don't want an extra
 * SSRC arg here because CSRC's via a mixer don't have their own data
 * packets.)  If we haven't seen this TAO_AV_Source yet, allocate it but
 * wait until we see two in-order packets accepting the flow.
 */
TAO_AV_Source*
TAO_AV_SourceManager::demux (ACE_UINT32 srcid, ACE_UINT32 addr, ACE_UINT16 seq)
{
  TAO_AV_Source* s = consult (srcid);
  if  (s == 0) {
    s = lookup_duplicate (srcid, addr);
    if  (s == 0) {
      /* CSRC=SSRC for data stream */

      this->callback_->get_rtp_source (s,
                                       srcid,
                                       srcid,
                                       addr);
//       ACE_NEW_RETURN (s,
//                       TAO_AV_Source (srcid, srcid, addr),
//                       0);
      enter (s);
    }
    /* it takes two in-seq packets to activate TAO_AV_Source */
    s->fs (seq);
    s->cs (seq);
    //   @@@Naga:We should take care of this using a policy for the
    // number of packets to be validated before we accept a source.
    return  (0);
  } else {
    /*
     * check for a srcid conflict or loop:
     *  - believe the new guy if the old guy said he's done.
     *  - otherwise, don't believe the new guy if we've heard
     *    from the old guy 'recently'.
     */
    if  (s->addr () != addr) {
      ACE_UINT32 t = s->lts_done ().tv_sec;
      if  (t == 0) {
        t = s->lts_data ().tv_sec;
        ACE_UINT32 now = ACE_OS::gettimeofday ().sec ();
        if  (t && int (now - t) <= 2)
          return  (0);
        t = s->lts_ctrl ().tv_sec;
        if  (t && int (now - t) <= 30)
          return  (0);
      }
      s->addr (addr);
      s->clear_counters ();
      s->lost (0);
    }
    if  (s->np () == 0 && s->nb () == 0) {
      /*
       * make sure we get 2 in-seq packets before
       * accepting TAO_AV_Source.
       */
      if  ( (ACE_UINT32) ( (ACE_UINT32)seq - s->cs () + 31) > 63) {
        s->fs (seq);
        s->cs (seq);
        return  (0);
      }
    }
  }
  return  (s);
}

/*
 * Try to find an entry in the TAO_AV_Source table with the same network
 * address  (i.e.,  a "duplicate entry") but possibly different srcid.
 * As a side effect, refile the TAO_AV_Source under the new srcid.
 *
 * The idea here is to gracefully handle sites that restart  (with
 * a new srcid).  If we assume that it takes a couple seconds to
 * restart
 *
 */
TAO_AV_Source*
TAO_AV_SourceManager::lookup_duplicate (ACE_UINT32 srcid, ACE_UINT32 addr)
{
  /*XXX - should eventually be conditioned on cname not ipaddr */
  /*
         * could use hashing here, but this is rarely called.
         */
  register TAO_AV_Source* s;
  for  (s = sources_; s != 0; s = s->next_) {
    /*
     * if addresses match, take old entry if:
     *  - it sent a 'done', or
     *  - it hasn't sent any data for 2 seconds and
     *    and any control for 30 seconds.
                 */
    if  (s->addr () == addr) {
      if  (s->lts_done ().tv_sec != 0)
        break;
      ACE_UINT32 now = ACE_OS::gettimeofday ().sec ();
      ACE_UINT32 t = s->lts_data ().tv_sec;
      if  (t == 0 || int (now - t) > 2) {
        t = s->lts_ctrl ().tv_sec;
        if  (t == 0 || int (now - t) > 30)
          break;
      }
    }
  }
  if  (s) {
    remove_from_hashtable (s);
    s->srcid (srcid);
    s->ssrc (srcid);
    int h = SHASH (srcid);
    s->hlink_ = hashtab_[h];
    hashtab_[h] = s;
    s->clear_counters ();
    s->lost (0);
  }
  return  (s);
}

void
TAO_AV_SourceManager::remove (TAO_AV_Source* s)
{
  --nsources_;

  remove_from_hashtable (s);

  /* delete the TAO_AV_Source from list */
  TAO_AV_Source** p = &sources_;
  while  (*p != s)
    p = & (*p)->next_;
  *p =  (*p)->next_;

  delete s;

}

/*
 * Go through all the TAO_AV_Sources and see if any should be "grayed out"
 * or removed altogether.  'msgint' is the current "report interval"
 * in ms.
 */
void
TAO_AV_SourceManager::CheckActiveSources (double msgint)
{
  ACE_UINT32 now = ACE_OS::gettimeofday ().sec ();
  u_int max_idle = u_int (msgint *  (CTRL_IDLE / 1000.));
  if  (max_idle == 0)
    max_idle = 1;

  TAO_AV_Source* n;
  for  (TAO_AV_Source* s = sources_; s != 0; s = n) {
    n = s->next_;
    ACE_UINT32 t = s->lts_done ().tv_sec;
    if  (t != 0) {
      if  (keep_sites_)
        s->lost (1);
      else
        remove (s);
      continue;
    }
    t = s->lts_ctrl ().tv_sec;
    if  (t == 0)
      /*
       * No session packets?  Probably ivs or nv sender.
       * Revert to the data time stamp.
       */
      t = s->lts_data ().tv_sec;

    if  (u_int (now - t) > max_idle) {
      if  (keep_sites_ || site_drop_time_ == 0 ||
          u_int (now - t) < site_drop_time_)
        s->lost (1);
      else
        remove (s);
    } else
      s->lost (0);
  }
}

int
TAO_AV_SourceManager::compare (const void* v0, const void* v1)
{
  const TAO_AV_Source* x = * (TAO_AV_Source**)v0;
  const TAO_AV_Source* y = * (TAO_AV_Source**)v1;

  const char* yn = y->sdes (RTCP_SDES_NAME);
  if  (yn == 0) {
    yn = y->sdes (RTCP_SDES_CNAME);
    if  (yn == 0)
      return  (-1);
  }
  const char* xn = x->sdes (RTCP_SDES_NAME);
  if  (xn == 0) {
    xn = x->sdes (RTCP_SDES_CNAME);
    if  (xn == 0)
      return  (1);
  }
  return  (strcmp (xn, yn));
}

/*
 * Sort the sources by name and format a corresponding list
 * of ascii srcid's in the input buffer.
 */
void
TAO_AV_SourceManager::sortactive (char* cp) const
{
  static int ntab;
  static TAO_AV_Source** srctab;
  if  (srctab == 0) {
    ntab = 2 * nsources_;
    if  (ntab < 32)
      ntab = 32;
    ACE_NEW (srctab,
             TAO_AV_Source*[ntab]);
  } else if  (ntab < nsources_) {
    ntab = 2 * nsources_;
    delete srctab;
    ACE_NEW (srctab,
             TAO_AV_Source*[ntab]);
  }
  int n = 0;
  for  (TAO_AV_Source* s = sources_; s != 0; s = s->next_)
    if  (s->active () != 0)
      /* source is active if it has a PacketHandler */
      srctab[n++] = s;

  if  (n == 0) {
    *cp = 0;
    return;
  }
  ACE_OS::qsort (srctab, n, sizeof (*srctab), compare_func);
  for  (int i = 0; i < n; ++i) {
    strcpy (cp, srctab[i]->name ());
    cp += strlen (cp);
    *cp++ = ' ';
  }
  /* nuke trailing space */
  cp[-1] = 0;
}
