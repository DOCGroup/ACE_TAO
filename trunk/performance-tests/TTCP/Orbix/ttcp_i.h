/* -*- C++ -*- */
// $Id$


// ttcp_i.C
//

#ifndef TTCP_I_H
#define TTCP_I_H

#include "ttcp.hh"

class ttcp_sequence_i
  : virtual public ttcp_sequenceBOAImpl
{
public:
  ttcp_sequence_i();

  virtual long send (const ttcp_sequence::my_sequence& ttcp_seq, CORBA::Environment &IT_env=CORBA::default_environment);
  virtual void start_timer (CORBA::Environment &IT_env=CORBA::default_environment);
  virtual void stop_timer (CORBA::Environment &IT_env=CORBA::default_environment);
private:
  unsigned long nbytes_;
};

// DEF_TIE_ttcp_sequence (ttcp_sequence_i);

class ttcp_string_i
  : virtual public ttcp_stringBOAImpl
{
public:
  ttcp_string_i();

  virtual long send (const char * ttcp_string, CORBA::Environment &IT_env=CORBA::default_environment);
  virtual void start_timer (CORBA::Environment &IT_env=CORBA::default_environment);
  virtual void stop_timer (CORBA::Environment &IT_env=CORBA::default_environment);
private:
  unsigned long nbytes_;
};

#endif defined (TTCP_I_H)





