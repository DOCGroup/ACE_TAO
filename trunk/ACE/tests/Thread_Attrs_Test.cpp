// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Thread_Attrs_Test.cpp
//
// = DESCRIPTION
//     This test program ensures that attributes set on a thread via the
//     ACE_Task/ACE_Thread_Manager are honored.
//
// = AUTHOR
//    Steve Huston <shuston@riverace.com>
//
// ============================================================================

#include "test_config.h"
#include "ace/Task.h"



#if defined (ACE_HAS_THREADS)

namespace
{
  // Change this to 'true' if you want lots of debugging messages in the log
  const bool PRINT_DEBUG_MSGS = true;
}

class Stack_Size_Check : public ACE_Task<ACE_MT_SYNCH>
{
  // = TITLE
  //   Defines a task that verifies its stack size.

public:
  Stack_Size_Check (size_t stack_size);
  // Create the thread with specified stack size

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void * = 0);
  // Spawn the thread
  //FUZZ: enable check_for_lack_ACE_OS

  virtual int svc (void);
  // Check the stack size against what is expected then exit.

  bool operator! ();
  // Returns true iff failed_ == false.

private:
  size_t stack_size_;

  bool failed_;
  // Flag indicating the test failed.
};

bool
Stack_Size_Check::operator!()
{
  return this->failed_;
}

Stack_Size_Check::Stack_Size_Check (size_t stack_size)
  : stack_size_ (stack_size), failed_ (false)
{
}

int
Stack_Size_Check::svc (void)
{
  size_t my_size = 0;

#ifdef __USE_GNU
  pthread_attr_t my_attrs;
  pthread_getattr_np (pthread_self (), &my_attrs);
  pthread_attr_getstacksize (&my_attrs, &my_size);
  pthread_attr_destroy (&my_attrs);
#else
  // No known way to do this yet... feel free to fill this in.
  my_size = this->stack_size_;
#endif /* __USE_GNU */

  // The Posix docs say that the size set for the threads stack will be the
  // *minimum* size allocated (the actual size may be bigger because of
  // a) pagesize rounding, b) guardsize addition) so we can really only
  // check if we have gotten *at least* what we asked for.
  if (my_size < this->stack_size_)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%t: My stack size attr %B; expected %B\n"),
                  my_size, this->stack_size_));
      this->failed_ = true;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("%t: My stack size attr %B; correct.\n"),
                my_size));
  return 0;
}

int
Stack_Size_Check::open (void *)
{
  if (this->activate (THR_NEW_LWP | THR_JOINABLE,
                      1,
                      0,
                      ACE_DEFAULT_THREAD_PRIORITY,
                      -1,
                      0,
                      0,
                      0,
                      &stack_size_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Stack_Size_Check activate failed")),
                      -1);

  return 0;
}

#endif /* ACE_HAS_THREADS */


int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Thread_Attrs_Test"));

  int status = 0;
#if defined (ACE_HAS_THREADS)
  Stack_Size_Check size_checker (40*1024);
  status = size_checker.open(0);
  if (status == 0)
    {
      if (size_checker.wait () == -1)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("size_checker %p\n"),
                             ACE_TEXT ("wait")),
                            1);
      if (!size_checker)
        status = 1;
    }
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return status;
}
