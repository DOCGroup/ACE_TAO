
//=============================================================================
/**
 *  @file    DLL_Test_Impl.cpp
 *
 *  This test illustrates the use of <ACE_DLL> wrapper class.
 *
 *  @author Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 */
//=============================================================================

#include "DLL_Test_Impl.h"
#include "ace/ACE.h"
#include "ace/OS_Errno.h"
#include "ace/svc_export.h"
#include "ace/OS_NS_string.h"
#include <utility>
#include <memory>

Hello_Impl::Hello_Impl ()
{
  ACE_DEBUG ((LM_DEBUG, "Hello_Impl::Hello_Impl\n"));
}

Hello_Impl::~Hello_Impl ()
{
  ACE_DEBUG ((LM_DEBUG, "Hello_Impl::~Hello_Impl\n"));
}

void
Hello_Impl::say_next ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("How are you?\n")));
}

ACE_TCHAR *
Hello_Impl::new_info ()
{
  return ACE::strnew (ACE_TEXT ("Hello_Impl::new_info() allocated by ACE::strnew()"));
}

ACE_TCHAR *
Hello_Impl::malloc_info ()
{
  return ACE_OS::strdup (ACE_TEXT ("Hello_Impl::new_info() allocated by ACE_OS::malloc()"));
}

void *
Hello_Impl::operator new (size_t bytes)
{
  ACE_DEBUG ((LM_INFO, "Hello_Impl::new\n"));
  return ::new char[bytes];
}

/// Overloaded new operator, nothrow_t variant.
void *
Hello_Impl::operator new (size_t bytes, const std::nothrow_t &nt)
{
  ACE_DEBUG ((LM_INFO, "Hello_Impl::new\n"));
  return ::new (nt) char[bytes];
}

void
Hello_Impl::operator delete (void *ptr, const std::nothrow_t&) throw ()
{
  ACE_DEBUG ((LM_INFO, "Hello_Impl::delete\n"));
  ::delete [] static_cast<char *> (ptr);
}

void
Hello_Impl::operator delete (void *ptr)
{
  ACE_DEBUG ((LM_INFO, "Hello_Impl::delete\n"));
  ::delete [] static_cast<char *> (ptr);
}

extern "C" ACE_Svc_Export Hello *
get_hello ()
{
  Hello *hello {};

  ACE_NEW_RETURN (hello,
                  Hello_Impl,
                  nullptr);

  return hello;
}

class Static_Constructor_Test
{
public:
  Static_Constructor_Test ()
  {
    ACE_DEBUG ((LM_DEBUG,
                "Static_Constructor_Test::Static_Constructor_Test\n"));
  }
  ~Static_Constructor_Test ()
  {
    ACE_DEBUG ((LM_DEBUG,
                "Static_Constructor_Test::~Static_Constructor_Test\n"));
  }
};

static Static_Constructor_Test the_instance;

// --------------------------------------------------------

Child::Child ()
{
}

Child::~Child ()
{
}

void
Child::test ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("child test called\n")));

  Data d;
  Data f(d);
  Data g;
  g = d;
  g = std::move(d);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("testing base exception\n")));

  std::unique_ptr<Base> base_excep (new Base ());
  try
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("throwing base exception\n")));
    base_excep->_raise();
  }
  catch (const Base&)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("caught base\n")));
  }

  std::unique_ptr<Derived> derived_excep (new Derived ());
  try
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("throwing derived exception\n")));
    derived_excep->_raise();
  }
  catch (const Derived&)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("caught derived\n")));
  }

  std::unique_ptr<Derived> derived_excep_base (new Derived ());
  try
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("throwing derived exception\n")));
    derived_excep_base->_raise();
  }
  catch (const Base&)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("caught derived as base\n")));
  }

  std::unique_ptr<Base> derived_excep_alloc (Derived::_alloc ());
  try
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("throwing derived exception allocated with _alloc\n")));
    derived_excep_base->_raise();
  }
  catch (const Derived&)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("caught derived exception\n")));
  }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("testing exceptions finished\n")));
}

// --------------------------------------------------------

// Test dynamic cast
extern "C" ACE_Svc_Export int
dynamic_cast_test (Parent *target)
{
  Child *c {};
  c = dynamic_cast<Child*> (target);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("dynamic_cast_test: parent %@; child %@\n"),
              target, c));
  return target == c ? 0 : -1;
}
