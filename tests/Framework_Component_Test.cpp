// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    This program tests both the ACE_Framework_Compondent and ACE_Repository.
//
// = AUTHOR
//    Don Hinton <dhinton@ieee.org>
//
// ============================================================================

#include "ace/Framework_Component.h"

#include "ace/OS.h"
#include "ace/Get_Opt.h"
#include "ace/ARGV.h"
#include "ace/SString.h"
#include "tests/test_config.h"

ACE_RCSID(tests, Framework_Component_Test, "$Id$")

/// This is a singleton we will use to test the ACE_Framework_Repository
class My_Singleton
{
public:
  My_Singleton (void)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("My_Singleton::ctor\n")));
    }
  virtual ~My_Singleton (void)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("My_Singleton::dtor\n")));
    }

  static My_Singleton * instance (void);

  static void close_singleton (void);

private:
  static My_Singleton *instance_;
};

My_Singleton *My_Singleton::instance_ = 0;

void
My_Singleton::close_singleton (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("My_Singleton::close_singleton\n")));
  delete My_Singleton::instance_;
  My_Singleton::instance_ = 0;
}

My_Singleton *
My_Singleton::instance (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("My_Singleton::instance\n")));

  if (My_Singleton::instance_ == 0)
    {
      // Perform Double-Checked Locking Optimization.
      ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                                *ACE_Static_Object_Lock::instance (), 0));
      if (My_Singleton::instance_ == 0)
        {
          ACE_NEW_RETURN (My_Singleton::instance_,
                          My_Singleton (),
                          0);
          ACE_REGISTER_FRAMEWORK_COMPONENT(My_Singleton, My_Singleton::instance_);
        }
    }
  return My_Singleton::instance_;
}


int
main (int, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_LIB_TEXT("Framework_Component_Test"));
  ACE_UNUSED_ARG (argv);

  // Create an instance of My_Singleton that will be managed
  // by the ACE_Framework_Repository and keep a pointer to
  // instance.
  My_Singleton *first = My_Singleton::instance ();

  // Now, close down the repository.
  ACE_Framework_Repository::close_singleton ();

  ACE_Framework_Repository::instance ();

  // Now get another pointer to My_Singleton and make sure
  // that it's different.  We could also test other pointers
  // like ACE_Reactor, ACE_Proactor, or the repository itself
  // if we wanted.
  My_Singleton *second = My_Singleton::instance ();

  if (first == second)
    ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("Pointer are equal, but shouldn't be.\n")));

  // Close down the repository again since we are writing debug messages.
  // And want all of them to go to the log.
  ACE_Framework_Repository::close_singleton ();
  
  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Framework_Component_T<My_Singleton>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Framework_Component_T<My_Singleton>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
