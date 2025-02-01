// -*- C++ -*-
#include "test_i.h"

// part of the famous caesium 133 atom transition for a
// second. Cannot represent the last part since the value gets out of
// range.
const CORBA::ULong freq = 919263;

Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

Simple_Server_i::Simple_Server_i ()
  : orb_ (0)
{
}

CORBA::ULongLong
Simple_Server_i::remote_call ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Print out from process id (%P) hosting the servant\n"));

  return freq;
}


void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (false);
}
