// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    DLL_Test.cpp
//
// = DESCRIPTION
//    This test illustrates the use of <ACE_DLL> wrapper class.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "DLL_Test_Impl.h"

ACE_RCSID(tests, DLL_Test_Impl, "$Id$")

Hello_Impl::Hello_Impl (void)
{
  ACE_DEBUG ((LM_DEBUG, "Hello_Impl::Hello_Impl\n"));
}

Hello_Impl::~Hello_Impl (void)
{
  ACE_DEBUG ((LM_DEBUG, "Hello_Impl::~Hello_Impl\n"));
}

void
Hello_Impl::say_next (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("How are you?\n")));
}

extern "C" ACE_Svc_Export Hello *
get_hello (void)
{
  Hello *hello = 0;

  ACE_NEW_RETURN (hello,
                  Hello_Impl,
                  NULL);

  return hello;
}

class Static_Constructor_Test
{
public:
  Static_Constructor_Test (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Static_Constructor_Test::Static_Constructor_Test\n"));
  }
  ~Static_Constructor_Test (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                "Static_Constructor_Test::~Static_Constructor_Test\n"));
  }
};

static Static_Constructor_Test the_instance;
