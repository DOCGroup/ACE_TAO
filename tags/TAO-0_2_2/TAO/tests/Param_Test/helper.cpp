// $Id$

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

ACE_RCSID(Param_Test, helper, "$Id$")

Generator::Generator (void)
{
}

Generator::~Generator (void)
{
}

CORBA::Short
Generator::gen_short (void)
{
  return (CORBA::Short) (ACE_OS::rand () % TEST_BUFSIZE);
}

CORBA::Long
Generator::gen_long (void)
{
  return ::ACE_OS::rand () % TEST_BUFSIZE;
}

char *
Generator::gen_string (void)
{
  return gen_string (TEST_BUFSIZE);
}

char *
Generator::gen_string (int max_length)
{
  CORBA::ULong len = (CORBA::ULong) (::ACE_OS::rand () % max_length);
  char *buf = CORBA::string_alloc (len);
  CORBA::ULong i = 0;

  while (i < len)
    {
      int c = ACE_OS::rand () % 128;
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
  this->fixed_struct_.l = ACE_OS::rand ();
  this->fixed_struct_.c = ACE_OS::rand () % 128;
  this->fixed_struct_.s = (CORBA::Short) ACE_OS::rand ();
  this->fixed_struct_.o = ACE_OS::rand () % 128;
  this->fixed_struct_.f = (CORBA::Float) (ACE_OS::rand () * 1.0);
  this->fixed_struct_.b = (CORBA::Boolean) ACE_OS::rand () % 2;
  this->fixed_struct_.d = (ACE_OS::rand () * 1.0);
  return this->fixed_struct_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Generator, ACE_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Generator, ACE_SYNCH_RECURSIVE_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
