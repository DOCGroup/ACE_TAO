// $Id

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    helper.cpp
//
// = DESCRIPTION
//    Defines a helper class that can generate values for the parameters used
//    for the Param_Test example
//
// = AUTHORS
//    Aniruddha Gokhale
//
// ============================================================================

#include "helper.h"

Generator::Generator (void)
{
}

Generator::~Generator (void)
{
}

CORBA::Short
Generator::gen_short (void)
{
  return (CORBA::Short) (::lrand48 () % TEST_BUFSIZE);
}

CORBA::Long
Generator::gen_long (void)
{
  return ::lrand48 () % TEST_BUFSIZE;
}

char *
Generator::gen_string (void)
{
  CORBA::ULong len = (CORBA::ULong) (::lrand48 () % TEST_BUFSIZE);
  char *buf = CORBA::string_alloc (len);
  CORBA::ULong i = 0;

  while (i < len)
    {
      int c = lrand48 () % 128;
      if (isprint (c) && !isspace (c))
        {
          buf [i] = c;
          i++;
        }
    }
  buf[i] = 0;
  return buf;
}

const Param_Test::Fixed_Struct
Generator::gen_fixed_struct (void)
{
  this->fixed_struct_.l = lrand48 ();
  this->fixed_struct_.c = lrand48 () % 128;
  this->fixed_struct_.s = (CORBA::Short) lrand48 ();
  this->fixed_struct_.o = lrand48 () % 128;
  this->fixed_struct_.f = (CORBA::Float) drand48 ();
  this->fixed_struct_.b = (CORBA::Boolean) lrand48 () % 2;
  this->fixed_struct_.d = drand48 ();
  return this->fixed_struct_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Generator, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Generator, ACE_SYNCH_RECURSIVE_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
