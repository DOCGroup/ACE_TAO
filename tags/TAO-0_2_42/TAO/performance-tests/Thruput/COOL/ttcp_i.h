// $Id$

// ============================================================================
//
// = COOL tests
//    Throughput measurement using the TTCP benchmark adapted to work using COOL
//
// = FILENAME
//   ttcp_i.h
//
// = AUTHOR
//    Aniruddha Gokhale
//    Sergio Flores-Gaitan   Ported to COOL
//
// ============================================================================

#ifndef TTCP_I_H
#define TTCP_I_H

#include "ttcp.H"

class ttcp_sequence_i
{
public:
  ttcp_sequence_i (const char *obj_name = 0);

  virtual void sendShortSeq (const ttcp_sequence::ShortSeq& ttcp_seq,
                             CORBA::Environment &IT_env);
  virtual void sendLongSeq (const ttcp_sequence::LongSeq& ttcp_seq,
                            CORBA::Environment &IT_env);
  virtual void sendDoubleSeq (const ttcp_sequence::DoubleSeq& ttcp_seq,
                              CORBA::Environment &IT_env);
  virtual void sendCharSeq (const ttcp_sequence::CharSeq& ttcp_seq,
                            CORBA::Environment &IT_env);
  virtual void sendStructSeq (const ttcp_sequence::StructSeq& ttcp_seq,
                              CORBA::Environment &IT_env);
  virtual void sendOctetSeq (const ttcp_sequence::OctetSeq& ttcp_seq,
                             CORBA::Environment &IT_env);

  /* Routines to calculate the time required to transfer */
  virtual void start_timer (CORBA::Environment &IT_env);
  virtual void stop_timer (CORBA::Environment &IT_env);

private:
  unsigned long nbytes_;
};

#endif // defined (TTCP_I_H)

