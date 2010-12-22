// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Dynamic_Test.cpp
//
// = DESCRIPTION
//    This tests the ACE_Dynamic class
//
// = AUTHORS
//    Johnny Willemsen  <jwillemsen@remedy.nl>
//
// ============================================================================

#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/Dynamic.h"
#include "ace/OS_Memory.h"



class A
{
public:
  A ();

  void *operator new (size_t n);

#if defined (ACE_HAS_NEW_NOTHROW)
  void *operator new (size_t n, const ACE_nothrow_t&) throw();
#if !defined (ACE_LACKS_PLACEMENT_OPERATOR_DELETE)
  void operator delete (void *p, const ACE_nothrow_t&) throw ();
#endif /* ACE_LACKS_PLACEMENT_OPERATOR_DELETE */
#endif

  void * operator new (size_t n, void *p);

  void operator delete (void *);

#if !defined (ACE_LACKS_PLACEMENT_OPERATOR_DELETE)
  void operator delete (void *, void *);
#endif /* ACE_LACKS_PLACEMENT_OPERATOR_DELETE */

  /// Have we been dynamically created?
  bool dynamic_;
};

void*
A::operator new (size_t n)
{
  ACE_Dynamic *const dynamic_instance = ACE_Dynamic::instance ();

  if (dynamic_instance == 0)
    {
      // If this ACE_ASSERT fails, it may be due to running of out TSS
      // keys.  Try using ACE_HAS_TSS_EMULATION, or increasing
      // ACE_DEFAULT_THREAD_KEYS if already using TSS emulation.
      ACE_ASSERT (dynamic_instance != 0);

      ACE_throw_bad_alloc;
    }
  else
    {
      // Allocate the memory and store it (usually in thread-specific
      // storage, depending on config flags).
      dynamic_instance->set ();

      return ::new char[n];
    }
}

#if defined (ACE_HAS_NEW_NOTHROW)
void*
A::operator new (size_t n, const ACE_nothrow_t&) throw()
{
  ACE_Dynamic *const dynamic_instance = ACE_Dynamic::instance ();

  if (dynamic_instance == 0)
    {
      // If this ACE_ASSERT fails, it may be due to running of out TSS
      // keys.  Try using ACE_HAS_TSS_EMULATION, or increasing
      // ACE_DEFAULT_THREAD_KEYS if already using TSS emulation.
      ACE_ASSERT (dynamic_instance != 0);

      return 0;
    }
  else
    {
      // Allocate the memory and store it (usually in thread-specific
      // storage, depending on config flags).
      dynamic_instance->set ();

      return ::new(ACE_nothrow) char[n];
    }
}

#if !defined (ACE_LACKS_PLACEMENT_OPERATOR_DELETE)
void
A::operator delete (void *p, const ACE_nothrow_t&) throw()
{
  ::delete [] static_cast <char *> (p);
}
#endif /* ACE_LACKS_PLACEMENT_OPERATOR_DELETE */

#endif /* ACE_HAS_NEW_NOTHROW */

void
A::operator delete (void *obj)
{
  ::delete [] static_cast <char *> (obj);
}

A::A()
{
  this->dynamic_ = ACE_Dynamic::instance ()->is_dynamic ();

  if (this->dynamic_)
    // Make sure to reset the flag.
    ACE_Dynamic::instance ()->reset ();
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Dynamic_Test"));
  A from_stack;
  A* heap = 0;
  ACE_NEW_RETURN (heap, A, 1);
  if (from_stack.dynamic_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          ACE_TEXT ("dynamic_ is true for an object on the stack\n")),
                        1);
    }

  if (!heap->dynamic_)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          ACE_TEXT ("dynamic_ is false for an object from the heap\n")),
                        1);
    }

  delete heap;
  ACE_END_TEST;
  return 0;
}

