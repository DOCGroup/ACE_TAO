/* -*- C++ -*- */
//$Id$
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

ACE_INLINE int 
TAO_AV_Source::checkseq (ACE_UINT16 v)
{
  int k = v & (TAO_AV_SOURCE_NSEQ-1);
  if (seqno_[k] != v) {
    seqno_[k] = v;
    return (0);
  } else {
    ++ndup_;
    return (-1);
  }
}

ACE_INLINE int
TAO_AV_Source::cs (ACE_UINT16 v)
{
  /*
   * This routine updates a 32 bit sequence number based on
   * the rtp packet's 16 bit seq. no.
   */
  register int c = cs_;
  register int d = v - c;
  if (d < -1024 || d > 1024) {
    cs_ = v;
    if (v < 512 && c > 0x10000-512) {
      /*
       * seq no. wrapped - subtract 64k from fs to
       * account for it.
       */
      fs_ -= 0x10000;
    } else {
      /*
       * the seq. no made a very large jump.  assume
       * that the other side restarted without telling
       * us about it so just re-sync (i.e., pretend
       * this was the first packet).
       */
      fs_ = v - 1;
      np_ = 0;
      nf_ = 0;
      nb_ = 0;
      snp_ = 0;
      nm_ = 0;
    }
  } else if (d > 0) {
    /* d <= 0 means duplicate or reordered packet - ignore */
    cs_ = v;
    if (d < 0)
      /* out of order */
      ++nm_;
  }
  return (checkseq (v));
}

ACE_INLINE void
TAO_AV_Source::action (void)
{
  if (!busy_) set_busy(); 
}

ACE_INLINE char*
TAO_AV_Source::name (void)
{
  return ACE_OS::strdup ("");
}

ACE_INLINE int
TAO_AV_Source::active (void)
{
  return this->active_;
}

ACE_INLINE void
TAO_AV_Source::active (int active)
{
  this->active_ = active;
}

ACE_INLINE ACE_UINT32
TAO_AV_Source::delvar (void) const
{
  return this->delvar_;
}

ACE_INLINE void
TAO_AV_Source::delvar (ACE_UINT32 v)
{
  this->delvar_ = v;
}

ACE_INLINE const timeval& 
TAO_AV_Source::lts_ctrl (void) const
{ 
  return (lts_ctrl_); 
}

ACE_INLINE void
TAO_AV_Source::lts_ctrl (const timeval& now)
{
  this->lts_ctrl_ = now;
}

ACE_INLINE const timeval& 
TAO_AV_Source::lts_data (void) const
{ 
  return (lts_data_); 
}

ACE_INLINE void
TAO_AV_Source::lts_data (const timeval& now)
{
  this->lts_data_ = now;
}

ACE_INLINE const timeval& 
TAO_AV_Source::lts_done (void) const
{ 
  return (lts_done_); 
}

ACE_INLINE void
TAO_AV_Source::lts_done (const timeval &now)
{
  this->lts_done_ = now;
}

ACE_INLINE int 
TAO_AV_Source::sts_ctrl (void) const
{ 
  return (sts_ctrl_); 
}

ACE_INLINE void
TAO_AV_Source::sts_ctrl (ACE_UINT32 now)
{
  this->sts_ctrl_ = now;
}

ACE_INLINE int
TAO_AV_Source::sts_data (void) const
{ 
  return (sts_data_); 
}

ACE_INLINE void
TAO_AV_Source::sts_data (ACE_UINT32 now)
{
  this->sts_data_ = now;
}

ACE_INLINE const char* 
TAO_AV_Source::sdes (int t) const
{ 
  return (sdes_[t]); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::addr (void) const
{ 
  return (addr_); 
}
ACE_INLINE void 
TAO_AV_Source::addr (ACE_UINT32 a) 
{ 
  addr_ = a;
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::srcid (void) const
{ 
  return (srcid_); 
}

ACE_INLINE void 
TAO_AV_Source::srcid (ACE_UINT32 s)
{ 
  srcid_ = s; 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::ssrc (void) const
{ 
  return (ssrc_); 
}

ACE_INLINE void 
TAO_AV_Source::ssrc (ACE_UINT32 s)
{ 
  ssrc_ = s; 
}

ACE_INLINE void 
TAO_AV_Source::format (int v)
{ 
  format_ = v; 
}

ACE_INLINE int 
TAO_AV_Source::format (void) const
{ 
  return (format_); 
}

ACE_INLINE void 
TAO_AV_Source::mute (int v)
{ 
  mute_ = v; 
}

ACE_INLINE int 
TAO_AV_Source::mute (void) const
{ 
  return (mute_); 
}

ACE_INLINE void 
TAO_AV_Source::ismixer (int v)
{ 
  ismixer_ = v; 
}

ACE_INLINE int 
TAO_AV_Source::ismixer (void) const
{ 
  return (ismixer_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::nb (void) const
{ 
  return (nb_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::nf (void) const
{ 
  return (nf_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::np (void) const
{ 
  return (np_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::nm (void) const
{ 
  return (nm_); 
}

/* no. expected */
ACE_INLINE ACE_UINT32 
TAO_AV_Source::ns (void) const
{ 
  return (cs_ - fs_); 
} 

ACE_INLINE ACE_UINT32 
TAO_AV_Source::ehs (void) const
{ 
  return ((fs_ & 0xffff0000) | cs_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::cs (void) const
{ 
  return (cs_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::snp (void) const
{ 
  return (snp_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::sns (void) const
{ 
  return (sns_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::runt (void) const
{ 
  return (nrunt_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::dups (void) const
{ 
  return (ndup_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::badsesslen (void) const
{ 
  return (badsesslen_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::badsessver (void) const
{ 
  return (badsessver_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::badsessopt (void) const
{ 
  return (badsessopt_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::badsdes (void) const
{ 
  return (badsdes_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_Source::badbye (void) const
{ 
  return (badbye_); 
}

ACE_INLINE void 
TAO_AV_Source::nb (int v)
{ 
  nb_ += v; 
}

ACE_INLINE void 
TAO_AV_Source::nf (int v)
{ 
  nf_ += v; 
}

ACE_INLINE void 
TAO_AV_Source::np (int v)
{ 
  np_ += v; 
}

ACE_INLINE void 
TAO_AV_Source::nm (int v)
{ 
  nm_ += v; 
}

ACE_INLINE void 
TAO_AV_Source::snp (int v)
{ 
  snp_ = v; 
}

ACE_INLINE void 
TAO_AV_Source::sns (int v)
{ 
  sns_ = v; 
}

ACE_INLINE void 
TAO_AV_Source::fs (int v)
{ 
  fs_ = v; 
}

ACE_INLINE void 
TAO_AV_Source::runt (int v)
{ 
  nrunt_ += v; 
}

ACE_INLINE void 
TAO_AV_Source::badsesslen (int v)
{ 
  badsesslen_ += v; 
}

ACE_INLINE void 
TAO_AV_Source::badsessver (int v)
{ 
  badsessver_ += v; 
}

ACE_INLINE void 
TAO_AV_Source::badsessopt (int v)
{ 
  badsessopt_ += v; 
}

ACE_INLINE void 
TAO_AV_Source::badsdes (int v)
{ 
  badsdes_ += v; 
}

ACE_INLINE void 
TAO_AV_Source::badbye (int v)
{ 
  badbye_ += v; 
}

ACE_INLINE void 
TAO_AV_Source::lost (int v)
{
  if (lost_ != v)
    lost_ = v;
}

//------------------------------------------------------------
// TAO_AV_SourceManager
//------------------------------------------------------------

ACE_INLINE int 
TAO_AV_SourceManager::nsources (void) const
{ 
  return (this->nsources_); 
}

ACE_INLINE TAO_AV_Source* 
TAO_AV_SourceManager::sources (void) const
{ 
  return (this->sources_); 
}

ACE_INLINE ACE_UINT32 
TAO_AV_SourceManager::clock (void) const
{ 
  return (clock_); 
}

ACE_INLINE TAO_AV_Source* 
TAO_AV_SourceManager::localsrc (void) const
{ 
  return (localsrc_); 
}

ACE_INLINE void
TAO_AV_SourceManager::localsrc (TAO_AV_Source *src)
{
  this->localsrc_ = src;
}

ACE_INLINE void
TAO_AV_SourceManager::keep_sites (int keep) 
{
  this->keep_sites_ = keep;
}

ACE_INLINE void
TAO_AV_SourceManager::site_drop_time (int time)
{
  this->site_drop_time_ = time;
}

