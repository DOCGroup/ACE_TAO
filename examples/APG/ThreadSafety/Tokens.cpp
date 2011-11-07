// $Id$

#include "ace/Local_Tokens.h"
#include "ace/Token_Manager.h"
#include "ace/Task.h"
#include "ace/OS_NS_time.h"

#if defined (ACE_HAS_TOKENS_LIBRARY)

class Device;

// Listing 1 code/ch14
class HA_Device_Repository
{
public:

  enum { N_DEVICES = 100 };

  HA_Device_Repository ()
  {
    for (int i = 0; i < N_DEVICES; i++)
      tokens_[i] = new ACE_Local_Mutex (0, 0, 1);
  }

  ~HA_Device_Repository ()
  {
    for (int i = 0; i < N_DEVICES; i++)
      delete tokens_[i];
  }

  int update_device (int device_id, char *commands)
  {
    this->tokens_[device_id]->acquire ();

    Device *curr_device = this->devices_[device_id];
    internal_do (curr_device);

    this->tokens_[device_id]->release ();

    return 0;
  }

  void internal_do (Device *device);

private:
  Device *devices_[N_DEVICES];
  ACE_Local_Mutex *tokens_[N_DEVICES];
  unsigned int seed_;
};
// Listing 1

void
HA_Device_Repository::internal_do (Device *device)
{
  ACE_UNUSED_ARG (device);    // Real code would use this.
  ACE_Time_Value tv (0, ACE_OS::rand_r (&this->seed_) % 10000);
  timespec_t t  = (timespec_t)tv;
  ACE_OS::nanosleep (&t);
}

// Listing 2 code/ch14
class HA_CommandHandler : public ACE_Task_Base
{
public:
  enum { N_THREADS = 5 };

  HA_CommandHandler (HA_Device_Repository &rep) : rep_(rep)
  { }

  int svc (void)
  {
    for (int i = 0; i < HA_Device_Repository::N_DEVICES; i++)
      rep_.update_device (i, "");
    return 0;
  }

private:
  HA_Device_Repository &rep_;
};

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  HA_Device_Repository rep;
  HA_CommandHandler handler (rep);
  handler.activate (THR_NEW_LWP | THR_JOINABLE,
                    HA_CommandHandler::N_THREADS);
  handler.wait ();
  return 0;
}
// Listing 2

#else /* defined (ACE_HAS_TOKENS_LIBRARY) */

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("local tokens not supported ")
              ACE_TEXT ("on this platform\n")));
  return 0;
}

#endif /* defined (ACE_HAS_TOKENS_LIBRARY) */
