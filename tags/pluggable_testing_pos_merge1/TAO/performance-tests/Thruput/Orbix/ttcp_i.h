/* -*- C++ -*- */
// $Id$


// ttcp_i.h
//
// This header file provides facility to use it either as a BOA approach or as
// a TIE approach.
//
#ifndef TTCP_I_H
#define TTCP_I_H

#include "ttcp.h"
#include "ttcp_decl.h"

/* define the data types to be sent */
#define SEND_SHORT ((unsigned long)(1))
#define SEND_LONG ((unsigned long)(2))
#define SEND_CHAR ((unsigned long)(3))
#define SEND_OCTET ((unsigned long)(4))
#define SEND_DOUBLE ((unsigned long)(5))
#define SEND_STRUCT ((unsigned long)(6))
#define SEND_COMPOSITE ((unsigned long)(7))


#if defined(USE_TIE)
class ttcp_sequence_i
#else   // use BOA
class ttcp_sequence_i
        : public virtual ttcp_sequenceBOAImpl
#endif
{
public:
  ttcp_sequence_i();

  virtual void sendShortSeq (const ttcp_sequence::ShortSeq& ttcp_seq,
        CORBA::Environment &IT_env=CORBA::default_environment);
  virtual void sendLongSeq (const ttcp_sequence::LongSeq& ttcp_seq,
        CORBA::Environment &IT_env=CORBA::default_environment);
  virtual void sendDoubleSeq (const ttcp_sequence::DoubleSeq& ttcp_seq,
        CORBA::Environment &IT_env=CORBA::default_environment);
  virtual void sendCharSeq (const ttcp_sequence::CharSeq& ttcp_seq,
        CORBA::Environment &IT_env=CORBA::default_environment);
  virtual void sendStructSeq (const ttcp_sequence::StructSeq& ttcp_seq,
        CORBA::Environment &IT_env=CORBA::default_environment);
  virtual void sendOctetSeq (const ttcp_sequence::OctetSeq& ttcp_seq,
        CORBA::Environment &IT_env=CORBA::default_environment);
  virtual void sendCompositeSeq (const ttcp_sequence::CompositeSeq& ttcp_seq,
        CORBA::Environment &IT_env=CORBA::default_environment);

  /* Routines to calculate the time required to transfer */
  virtual void start_timer (CORBA::Environment &IT_env=
        CORBA::default_environment);
  virtual void stop_timer (CORBA::Environment &IT_env=
        CORBA::default_environment);
private:
  unsigned long nbytes_;
};

#if defined(USE_TIE)
// ttcp_sequence_i implements ttcp_sequence
DEF_TIE(ttcp_sequence, ttcp_sequence_i);
#endif

#endif // defined (TTCP_I_H)
