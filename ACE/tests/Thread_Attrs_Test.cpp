
//=============================================================================
/**
 *  @file    Thread_Attrs_Test.cpp
 *
 *   This test program ensures that attributes set on a thread via the
 *   ACE_Task/ACE_Thread_Manager are honored.
 *
 *  @author Steve Huston <shuston@riverace.com>
 */
//=============================================================================

#include "test_config.h"
#include "ace/Task.h"

#if defined (ACE_HAS_THREADS)

class Cancel_Check : public ACE_Task<ACE_MT_SYNCH>
{
public:
  // Create a checker with the state, type requested.
  Cancel_Check (bool enable, bool async);

  //FUZZ: disable check_for_lack_ACE_OS
  // Spawn the thread
  int open (void * = 0) override;
  //FUZZ: enable check_for_lack_ACE_OS

  // Check the cancel settings against what is expected then exit.
  int svc () override;

  /// Returns true iff settings match what was requested.
  bool operator! ();

private:
  bool enable_req_;
  bool async_req_;
  bool failed_;
};

bool
Cancel_Check::operator!()
{
  return this->failed_;
}

Cancel_Check::Cancel_Check (bool enable, bool async)
  : enable_req_ (enable), async_req_(async), failed_ (false)
{
}

int
Cancel_Check::svc ()
{
#if defined (ACE_HAS_PTHREADS) && !defined (ACE_LACKS_PTHREAD_CANCEL)
  int state;
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &state);
  if (state == PTHREAD_CANCEL_ENABLE && !this->enable_req_)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Cancel found enabled, should not be\n")));
      this->failed_ = true;
    }
  else if (state == PTHREAD_CANCEL_DISABLE && this->enable_req_)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Cancel found disabled, should not be\n")));
      this->failed_ = true;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Cancel found %s; ok\n"),
                  state == PTHREAD_CANCEL_ENABLE ? ACE_TEXT ("enabled") :
                                                   ACE_TEXT ("disabled")));
    }

  int type;
  pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &type);
  if (type == PTHREAD_CANCEL_ASYNCHRONOUS && !this->async_req_)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Cancel type async, should not be\n")));
      this->failed_ = true;
    }
  else if (type == PTHREAD_CANCEL_DEFERRED && this->async_req_)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Cancel type deferred, should not be\n")));
      this->failed_ = true;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Cancel type %s; ok\n"),
                  type == PTHREAD_CANCEL_DEFERRED ? ACE_TEXT ("deferred") :
                                                    ACE_TEXT ("asynchronous")));
    }

#endif
  return 0;
}

int
Cancel_Check::open (void *)
{
  long flags = THR_NEW_LWP | THR_JOINABLE;
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Flags before cancels: 0x%x\n"), flags));
  flags |= (this->enable_req_ ? THR_CANCEL_ENABLE : THR_CANCEL_DISABLE);
  flags |= (this->async_req_ ? THR_CANCEL_ASYNCHRONOUS : THR_CANCEL_DEFERRED);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Flags after cancels: 0x%x\n"), flags));
  if (this->activate (flags) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Cancel_Check activate failed")),
                      -1);
  return 0;
}


/**
 * @class Stack_Size_Check
 *
 * @brief Defines a task that verifies its stack size.
 */
class Stack_Size_Check : public ACE_Task<ACE_MT_SYNCH>
{
public:
  /// Create the thread with specified stack size
  Stack_Size_Check (size_t stack_size);

  //FUZZ: disable check_for_lack_ACE_OS
  /// Spawn the thread
  int open (void * = 0) override;
  //FUZZ: enable check_for_lack_ACE_OS

  /// Check the stack size against what is expected then exit.
  int svc () override;

  /// Returns true iff failed_ == false.
  bool operator! ();

private:
  size_t stack_size_;

  /// Flag indicating the test failed.
  bool failed_;
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
Stack_Size_Check::svc ()
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

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Cancel flags sanity check:\n")
              ACE_TEXT ("  THR_CANCEL_ENABLE: 0x%x\n")
              ACE_TEXT ("  THR_CANCEL_DISABLE: 0x%x\n")
              ACE_TEXT ("  THR_CANCEL_DEFERRED: 0x%x\n")
              ACE_TEXT ("  THR_CANCEL_ASYNCHRONOUS: 0x%x\n"),
              THR_CANCEL_ENABLE,
              THR_CANCEL_DISABLE,
              THR_CANCEL_DEFERRED,
              THR_CANCEL_ASYNCHRONOUS));
  // Cancel check args:  enable (yes/no), async/deferred
  Cancel_Check check1 (true, true);
  Cancel_Check check2 (true, false);
  Cancel_Check check3 (false, true);
  Cancel_Check check4 (false, false);
  if (check1.open(0) == 0)
    {
      check1.wait ();
      if (!check1)
        status = 1;

      check2.open (0);
      check2.wait ();
      if (!check2)
        status = 1;

      check3.open (0);
      check3.wait ();
      if (!check3)
        status = 1;

      check4.open (0);
      check4.wait ();
      if (!check4)
        status = 1;
    }
  else
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("Cancel_Check open")));
      status = 1;
    }
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return status;
}
