/* -*- C++ -*- */
// $Id$


// ttcp_i.C
// implementation of the ttcp and Profile_Logger objects.

#ifndef TTCP_I_H
#define TTCP_I_H

// This is a total hack...
#define private public
#include "ttcp_s.hh"
#include "ttcp_c.hh"

class ttcp_string_i : public ttcp_string_impl {
public:

  ttcp_string_i();

  virtual void send(const CORBA::String& ttcp_string);
  virtual void send_hack(const CORBA::String& ttcp_string);
  virtual void start_timer();
  virtual void stop_timer();

private:
  unsigned long nbytes_;
};

class ttcp_sequence_i : public ttcp_sequence_impl {
public:

  ttcp_sequence_i();

  virtual void send(const ttcp_sequence::my_sequence& ttcp_seq);
  virtual void send_hack(const CORBA::String& ttcp_string);
  virtual void start_timer();
  virtual void stop_timer();

private:
  unsigned long nbytes_;
};


#endif
