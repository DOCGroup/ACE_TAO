// $Id$

#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Condition_T.h"

// Listing 1 code/ch12
class HA_Device_Repository
{
public:
  HA_Device_Repository() : owner_(0)
  { }

  int is_free (void)
    { return (this->owner_ == 0); }

  int is_owner (ACE_Task_Base* tb)
    { return (this->owner_ == tb); }

  ACE_Task_Base *get_owner (void)
    { return this->owner_; }

  void set_owner (ACE_Task_Base *owner)
    { this->owner_ = owner; }

  int update_device (int device_id);

private:
  ACE_Task_Base * owner_;
};
// Listing 1

class HA_CommandHandler : public ACE_Task_Base
{
public:
  enum {NUM_USES = 10};

  HA_CommandHandler (HA_Device_Repository& rep,
                     ACE_Condition<ACE_Thread_Mutex> &wait,
                     ACE_Thread_Mutex& mutex)
    : rep_(rep), waitCond_(wait), mutex_(mutex)
  { }

  virtual int svc (void);

private:
  HA_Device_Repository &rep_;
  ACE_Condition<ACE_Thread_Mutex> &waitCond_;
  ACE_Thread_Mutex &mutex_;
};
// Listing 2 code/ch12
int
HA_CommandHandler::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Handler Thread running\n")));

  for (int i = 0; i < NUM_USES; i++)
    {
      this->mutex_.acquire ();
      while (!this->rep_.is_free ())
        this->waitCond_.wait ();
      this->rep_.set_owner (this);
      this->mutex_.release ();

      this->rep_.update_device (i);

      ACE_ASSERT (this->rep_.is_owner (this));
      this->rep_.set_owner (0);

      this->waitCond_.signal ();
    }

  return 0;
}
// Listing 2
int
HA_Device_Repository::update_device (int device_id)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Updating device %d\n"),
              device_id));

  ACE_OS::sleep (1);
  return 0;
}
// Listing 3 code/ch12
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  HA_Device_Repository rep;
  ACE_Thread_Mutex rep_mutex;

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Condition<ACE_Thread_Mutex> wait (rep_mutex);
  //FUZZ: enable check_for_lack_ACE_OS

  HA_CommandHandler handler1 (rep, wait, rep_mutex);
  HA_CommandHandler handler2 (rep, wait, rep_mutex);

  handler1.activate ();
  handler2.activate ();

  handler1.wait ();
  handler2.wait ();

  return 0;
}
// Listing 3

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */
