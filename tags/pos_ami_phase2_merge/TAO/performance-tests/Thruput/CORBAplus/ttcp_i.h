// $Id$

// ============================================================================
//
// = TAO tests
//    Throughput measurement using the TTCP benchmark adapted to work using
//    CORBAplus PowerBroker from Expersoft
// = FILENAME
//   ttcp_i.h
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef TTCP_I_H
#define TTCP_I_H

#include <pbroker/corba/lifecycl/actvtr.h>
#include <pbroker/corba/lifecycl/impldecl.h>
#include "pbroker/corba/orb.h"
#include "pbroker/corba/request.h"
#include "pbroker/corba/environ.h"

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ttcp_s.h"

class ttcp_sequence_i: public ttcp_sequence_base_impl
{
public:
  //  XPS_CAST1_DECL(ttcp_sequence_i,ttcp_sequence_base_impl)
  //  XPS_DEFINE_IMPLEMENTATION(ttcp_sequence_decl)

  ttcp_sequence_i(const char *obj_name);

  ttcp_sequence_i (void)
    { ttcp_sequence_i ("keyone");}

  ttcp_sequence_i (const CORBA_ReferenceData & refData)
    { ttcp_sequence_i ("keyone");}

  virtual void sendShortSeq (const ttcp_sequence::ShortSeq& ttcp_seq);
  virtual void sendLongSeq (const ttcp_sequence::LongSeq& ttcp_seq);
  virtual void sendDoubleSeq (const ttcp_sequence::DoubleSeq& ttcp_seq);
  virtual void sendCharSeq (const ttcp_sequence::CharSeq& ttcp_seq);
  virtual void sendStructSeq (const ttcp_sequence::StructSeq& ttcp_seq);
  virtual void sendOctetSeq (const ttcp_sequence::OctetSeq& ttcp_seq);

  /* Routines to calculate the time required to transfer */
  virtual void start_timer (void);
  virtual void stop_timer (void);

  const char *_get_name (void);
  // get the key of the object.
private:
  unsigned long nbytes_;
  const char *key_;
};

#endif // defined (TTCP_I_H)

