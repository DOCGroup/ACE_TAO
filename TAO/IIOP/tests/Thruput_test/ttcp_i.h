/* -*- C++ -*- */

// ttcp_i.h
// 
// This header file provides facility to use it either as a BOA approach or as
// a TIE approach.
//
#if !defined (TTCP_I_H)
#define TTCP_I_H

#include "ttcp_decl.h"
#include "ttcpS.h"

/* define the data types to be sent */
#define SEND_SHORT ((unsigned long)(1))
#define SEND_LONG ((unsigned long)(2))
#define SEND_CHAR ((unsigned long)(3))
#define SEND_OCTET ((unsigned long)(4))
#define SEND_DOUBLE ((unsigned long)(5))
#define SEND_STRUCT ((unsigned long)(6))


class ttcp_sequence_i: public _skel_ttcp_sequence 
{
public:
  ttcp_sequence_i(const char *obj_name = 0);

  virtual void sendShortSeq (const ttcp_sequence::ShortSeq& ttcp_seq, 
	CORBA_Environment &IT_env);
  virtual void sendLongSeq (const ttcp_sequence::LongSeq& ttcp_seq, 
	CORBA_Environment &IT_env);
  virtual void sendDoubleSeq (const ttcp_sequence::DoubleSeq& ttcp_seq, 
	CORBA_Environment &IT_env);
  virtual void sendCharSeq (const ttcp_sequence::CharSeq& ttcp_seq, 
	CORBA_Environment &IT_env);
  virtual void sendStructSeq (const ttcp_sequence::StructSeq& ttcp_seq, 
	CORBA_Environment &IT_env);
  virtual void sendOctetSeq (const ttcp_sequence::OctetSeq& ttcp_seq, 
	CORBA_Environment &IT_env);

  /* Routines to calculate the time required to transfer */
  virtual void start_timer (CORBA_Environment &IT_env);
  virtual void stop_timer (CORBA_Environment &IT_env);

private:
  unsigned long nbytes_;
};

#endif // defined (TTCP_I_H)

