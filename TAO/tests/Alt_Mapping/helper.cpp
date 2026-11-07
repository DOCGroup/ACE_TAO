
//=============================================================================
/**
 *  @file    helper.cpp
 *
 *  Defines a helper class that can generate values for the parameters used
 *  for the Alt_Mapping example
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#include "helper.h"
#include <cctype>
#include <cstdlib>

CORBA::ULong constexpr TEST_BUFSIZE = 128;

CORBA::Long
Generator::gen_long () const
{
  return static_cast<CORBA::Long> (std::rand () % TEST_BUFSIZE);
}

std::string
Generator::gen_string () const
{
  return gen_string (TEST_BUFSIZE);
}

std::string
Generator::gen_string (CORBA::ULong max_length) const
{
  if (max_length == 0)
    {
      return {};
    }

  CORBA::ULong const len = static_cast<CORBA::ULong> (std::rand ()) % max_length;
  std::string result;
  result.reserve (len);

  while (result.length () < len)
    {
      int const c = std::rand () % 128;
      if (std::isprint (static_cast<unsigned char> (c)) &&
          !std::isspace (static_cast<unsigned char> (c)))
        {
          result.push_back (static_cast<char> (c));
        }
    }

  return result;
}

Alt_Mapping::Fixed_Struct
Generator::gen_fixed_struct () const
{
  Alt_Mapping::Fixed_Struct fixed_struct {};
  fixed_struct.l = std::rand ();
  fixed_struct.c = static_cast<CORBA::Char> (std::rand () % 128);
  fixed_struct.s = static_cast<CORBA::Short> (std::rand ());
  fixed_struct.o = static_cast<CORBA::Octet> (std::rand () % 128);
  fixed_struct.f = static_cast<CORBA::Float> (std::rand () * 1.0);
  fixed_struct.b = static_cast<CORBA::Boolean> (std::rand () % 2);
  fixed_struct.d = std::rand () * 1.0;
  return fixed_struct;
}

ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_Singleton, Generator,  ACE_Recursive_Thread_Mutex);
