
//=============================================================================
/**
 *  @file    helper.cpp
 *
 *  $Id$
 *
 *  Defines a helper class that can generate values for the parameters used
 *  for the Alt_Mapping example
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#include "helper.h"
#include "ace/OS_NS_ctype.h"

const CORBA::ULong TEST_BUFSIZE = 128;

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
      if (ACE_OS::ace_isprint (c) && !ACE_OS::ace_isspace (c))
        {
          buf [i] = c;
          i++;
        }
    }

  buf[i] = 0;
  return buf;
}

CORBA::WChar *
Generator::gen_wstring (void)
{
  return gen_wstring (TEST_BUFSIZE);
}

CORBA::WChar *
Generator::gen_wstring (int max_length)
{
  CORBA::ULong len = (CORBA::ULong) (::ACE_OS::rand () % max_length);
  CORBA::WChar *buf = CORBA::wstring_alloc (len);
  CORBA::ULong i = 0;
  CORBA::WChar limit =
    ACE_OutputCDR::wchar_maxbytes() == 1 ? ACE_OCTET_MAX : ACE_WCHAR_MAX;
  while (i < len)
    {
      CORBA::WChar wc = ACE_OS::rand () % limit;
      if (wc)
        {
          buf[i] = wc;
          i++;
        }
    }

  buf[i] = 0;
  return buf;
}

const Alt_Mapping::Fixed_Struct
Generator::gen_fixed_struct (void)
{
  this->fixed_struct_.l = ACE_OS::rand ();
  this->fixed_struct_.c = ACE_OS::rand () % 128;
  this->fixed_struct_.s = (CORBA::Short) ACE_OS::rand ();
  this->fixed_struct_.o = ACE_OS::rand () % 128;
  this->fixed_struct_.f = (CORBA::Float) (ACE_OS::rand () * 1.0);
  this->fixed_struct_.b = (CORBA::Boolean) (ACE_OS::rand () % 2);
  this->fixed_struct_.d = (ACE_OS::rand () * 1.0);
  return this->fixed_struct_;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<Generator, ACE_Recursive_Thread_Mutex> *ACE_Singleton<Generator, ACE_Recursive_Thread_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
