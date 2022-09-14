
//=============================================================================
/**
 *  @file    Dynamic_Test.cpp
 *
 *  This tests the ACE_Dynamic class
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#include "test_config.h"
#include "ace/OS_NS_string.h"
#include "ace/Dynamic.h"
#include "ace/OS_Memory.h"


class A
{
public:
  A ();

  void *operator new (size_t n);

  void *operator new (size_t n, const std::nothrow_t&) throw();
  void operator delete (void *p, const std::nothrow_t&) throw ();
  void * operator new (size_t n, void *p);

  void operator delete (void *);

  void operator delete (void *, void *);

  /// Have we been dynamically created?
  bool dynamic_;
};

void*
A::operator new (size_t n)
{
  ACE_Dynamic *const dynamic_instance = ACE_Dynamic::instance ();

  if (dynamic_instance == 0)
    {
      // If this ACE_TEST_ASSERT fails, it may be due to running of out TSS
      // keys.  Try using ACE_HAS_TSS_EMULATION, or increasing
      // ACE_DEFAULT_THREAD_KEYS if already using TSS emulation.
      ACE_TEST_ASSERT (dynamic_instance != 0);

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

void*
A::operator new (size_t n, const std::nothrow_t&) throw()
{
  ACE_Dynamic *const dynamic_instance = ACE_Dynamic::instance ();

  if (dynamic_instance == 0)
    {
      // If this ACE_TEST_ASSERT fails, it may be due to running of out TSS
      // keys.  Try using ACE_HAS_TSS_EMULATION, or increasing
      // ACE_DEFAULT_THREAD_KEYS if already using TSS emulation.
      ACE_TEST_ASSERT (dynamic_instance != 0);

      return 0;
    }
  else
    {
      // Allocate the memory and store it (usually in thread-specific
      // storage, depending on config flags).
      dynamic_instance->set ();

      return ::new(std::nothrow) char[n];
    }
}

void
A::operator delete (void *p, const std::nothrow_t&) throw()
{
  ::delete [] static_cast <char *> (p);
}

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

