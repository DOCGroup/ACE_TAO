
//=============================================================================
/**
 *  @file    helper.cpp
 *
 *  Defines a helper class that can generate values for the parameters used
 *  for the Param_Test example
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#include "helper.h"

#include <cstdlib>

#include "ace/OS_NS_ctype.h"

const CORBA::ULong TEST_BUFSIZE = 128;

Generator::Generator ()
{
}

Generator::~Generator ()
{
}

CORBA::Short
Generator::gen_short ()
{
  return (CORBA::Short) (std::rand () % TEST_BUFSIZE);
}

CORBA::Long
Generator::gen_long ()
{
  return std::rand () % TEST_BUFSIZE;
}

char *
Generator::gen_string ()
{
  return gen_string (TEST_BUFSIZE);
}

char *
Generator::gen_string (int max_length)
{
  CORBA::ULong len = (CORBA::ULong) (std::rand () % max_length);
  char *buf = CORBA::string_alloc (len);
  CORBA::ULong i = 0;

  while (i < len)
    {
      int c = std::rand () % 128;
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
Generator::gen_wstring ()
{
  return gen_wstring (TEST_BUFSIZE);
}

CORBA::WChar *
Generator::gen_wstring (int max_length)
{
  CORBA::ULong len = (CORBA::ULong) (std::rand () % max_length);
  CORBA::WChar *buf = CORBA::wstring_alloc (len);
  CORBA::ULong i = 0;
  CORBA::WChar limit =
    ACE_OutputCDR::wchar_maxbytes() == 1 ? ACE_OCTET_MAX : ACE_WCHAR_MAX;
  while (i < len)
    {
      CORBA::WChar wc = std::rand () % limit;
      if (wc)
        {
          buf[i] = wc;
          i++;
        }
    }

  buf[i] = 0;
  return buf;
}

const Param_Test::Fixed_Struct
Generator::gen_fixed_struct ()
{
  this->fixed_struct_.l = std::rand ();
  this->fixed_struct_.c = std::rand () % 128;
  this->fixed_struct_.s = (CORBA::Short) std::rand ();
  this->fixed_struct_.o = std::rand () % 128;
  this->fixed_struct_.f = (CORBA::Float) (std::rand () * 1.0);
  this->fixed_struct_.b = (CORBA::Boolean) (std::rand () % 2);
  this->fixed_struct_.d = (std::rand () * 1.0);
  return this->fixed_struct_;
}

const Param_Test::Step
Generator::gen_step ()
{
  this->step_.name.id = this->gen_string ();
  this->step_.name.kind = this->gen_string ();
  this->step_.process = (CORBA::Boolean) (std::rand () % 2);
  return this->step_;
}

ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_Singleton, Generator,  ACE_Recursive_Thread_Mutex);
