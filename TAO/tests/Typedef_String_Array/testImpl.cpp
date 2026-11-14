#include "testImpl.h"
#include <array>

constexpr std::array<char const *, 15> data = {"Hi there",
                                               "This is a test",
                                               "of typedefed string",
                                               "arrays",
                                               "This test should work with either set",
                                               "of typedefs",
                                               "typedef string MyString",
                                               "typedef MyString ArrayOfString[15]",
                                               "or",
                                               "#define MyString string",
                                               "typedef MyString ArrayOfString[15]",
                                               "or",
                                               "typedef string MyString",
                                               "typedef string ArrayOfString[15]",
                                               "thats it"};

TestImpl::TestImpl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
TestImpl::get_strings (ArrayOfString_out strings)
{
  // The output parameter takes ownership of this IDL array.
  ACE_NEW (strings, ArrayOfString);

  for (CORBA::ULong i = 0; i < data.size (); ++i)
    {
      strings[i] = data[i];
    }

  orb_->shutdown ();
}
