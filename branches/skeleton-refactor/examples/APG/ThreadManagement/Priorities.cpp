// $Id$

#include "ace/config-lite.h"

#if defined (ACE_HAS_THREADS)

#include "ace/Task.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

// Listing 2 code/ch13
class HA_CommandHandler : public ACE_Task<ACE_MT_SYNCH>
{
public:
  HA_CommandHandler (const char *name) : name_ (name)
  { }

  virtual int svc (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) starting up %C\n"),
                name_));

    ACE_OS::sleep (2);
    ACE_Message_Block *mb;
    while (this->getq (mb) != -1)
      process_message (mb);
    return 0;
  }

  void process_message (ACE_Message_Block *)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Processing message %C\n"),
                name_));
    // Simulate compute bound task.
    for (int i = 0; i < 100; i++)
      ;
  }

private:
  const char *name_;
};
// Listing 2

// This should be fixed in ACE... There's no _MAX, _MIN values for
// thread priorities.
#if defined (ACE_WIN32) && !defined (ACE_THR_PRI_OTHER_MAX)
#  define ACE_THR_PRI_OTHER_MAX ((ACE_THR_PRI_OTHER_DEF) + 1)
#endif

// Listing 1 code/ch13
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  HA_CommandHandler hp_handler ("HighPriority");
  hp_handler.activate (THR_NEW_LWP | THR_JOINABLE,
                       1, 1, ACE_THR_PRI_OTHER_MAX);

  HA_CommandHandler lp_handler ("LowPriority");
  lp_handler.activate (THR_NEW_LWP | THR_JOINABLE,
                       1, 1, ACE_THR_PRI_OTHER_DEF);

  ACE_Message_Block mb;
  for (int i = 0; i < 100; i++)
    {
      hp_handler.putq (&mb);
      lp_handler.putq (&mb);
    }

  hp_handler.wait ();
  lp_handler.wait ();

  return 0;
}
// Listing 1

#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */
