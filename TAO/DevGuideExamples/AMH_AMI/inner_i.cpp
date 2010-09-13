/* -*- C++ -*- $Id$ */
#include "amh_ami_pch.h"

#include "inner_i.h"

Inner_i::Inner_i (int d)
  : divisor_ (d)
{
}

char *
Inner_i::answer (const char * question)
{
  ACE_UNUSED_ARG (question); // doesn't matter

  CORBA::String_var answer =
    CORBA::string_dup ("I didn't know dogs could talk!");

  int delay = 5/this->divisor_; // throw exception if divisor = 0

  ACE_OS::sleep(delay); // make the caller wait a bit
  return answer._retn();
}
