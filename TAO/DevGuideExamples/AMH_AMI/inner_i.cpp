/* -*- C++ -*-  */
#include "amh_ami_pch.h"

#include "inner_i.h"
#include "ace/OS_NS_unistd.h"

Inner_i::Inner_i (int d)
  : divisor_ (d)
{
}

char *
Inner_i::answer (char const *question)
{
  ACE_UNUSED_ARG (question);

  CORBA::String_var answer (
    CORBA::string_dup ("I didn't know dogs could talk!"));

  int const delay = 5 / this->divisor_;

  ACE_OS::sleep (delay);
  return answer._retn ();
}
