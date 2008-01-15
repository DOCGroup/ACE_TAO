// $Id$

#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/Synch.h"
#include "ace/Task.h"

// Listing 1 code/ch12
class HA_Device_Repository
{
public:
  HA_Device_Repository ()
  { }

  void update_device (int device_id)
  {
    mutex_.acquire ();
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Updating device %d\n"),
                device_id));
    ACE_OS::sleep (1);
    mutex_.release ();
  }

private:
  ACE_Thread_Mutex mutex_;
};
// Listing 1
// Listing 2 code/ch12
class HA_CommandHandler : public ACE_Task_Base
{
public:
  enum {NUM_USES = 10};

  HA_CommandHandler (HA_Device_Repository& rep) : rep_(rep)
  { }

  virtual int svc (void)
  {
    ACE_DEBUG
      ((LM_DEBUG, ACE_TEXT ("(%t) Handler Thread running\n")));
    for (int i=0; i < NUM_USES; i++)
      this->rep_.update_device (i);
    return 0;
  }

private:
  HA_Device_Repository & rep_;
};

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  HA_Device_Repository rep;
  HA_CommandHandler handler1 (rep);
  HA_CommandHandler handler2 (rep);
  handler1.activate ();
  handler2.activate ();

  handler1.wait ();
  handler2.wait ();
  return 0;
}
// Listing 2

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */
