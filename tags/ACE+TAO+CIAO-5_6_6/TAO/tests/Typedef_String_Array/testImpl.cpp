// $Id$

#include "testImpl.h"

static const CORBA::ULong maxd = 15;
static const char* data[maxd] = { "Hi there",
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
                                  "thats it"
                                };

TestImpl::TestImpl (CORBA::ORB_ptr orb)
 : orb_ (CORBA::ORB::_duplicate(orb))
{
}


void
TestImpl::get_strings (ArrayOfString_out strings)
{
  ACE_NEW (strings, ArrayOfString);

  for (CORBA::ULong i = 0; i < maxd; i++)
    {
      strings[i] = data[i];
    }

  orb_->shutdown();
}
