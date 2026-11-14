#include "ace/config-lite.h"

#if defined (ACE_HAS_THREADS)

#include "ace/Task.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

// Listing 2 code/ch13
class HA_CommandHandler : public ACE_Task<ACE_MT_SYNCH>
{
public:
  explicit HA_CommandHandler (char const *name) : name_ (name)
  { }
  HA_CommandHandler (HA_CommandHandler const &) = delete;
  HA_CommandHandler (HA_CommandHandler &&) = delete;
  HA_CommandHandler &operator= (HA_CommandHandler const &) = delete;
  HA_CommandHandler &operator= (HA_CommandHandler &&) = delete;

  int svc () override
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) starting up %C\n"),
                name_));

    ACE_OS::sleep (2);
    ACE_Message_Block *mb {};
    while (this->getq (mb) != -1)
      {
        if (mb->msg_type () == ACE_Message_Block::MB_BREAK)
          {
            mb->release ();
            break;
          }
        process_message (mb);
        mb->release ();
      }
    return 0;
  }

  void process_message (ACE_Message_Block *)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Processing message %C\n"),
                name_));
    // Simulate compute bound task.
    for (int i = 0; i < 100; ++i)
      ;
  }

private:
  char const *const name_;
};
// Listing 2

#if !defined (ACE_THR_PRI_OTHER_MAX)
// This should be fixed in ACE... There's no _MAX, _MIN values for
// thread priorities.
#if defined (ACE_WIN32)
#  define ACE_THR_PRI_OTHER_MAX ((ACE_THR_PRI_OTHER_DEF) + 1)
#elif defined (VXWORKS)
#  define ACE_THR_PRI_OTHER_MAX 0
#endif
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
  for (int i = 0; i < 100; ++i)
    {
      // putq takes ownership of each clone; the handlers release them after dequeue.
      ACE_Message_Block *const mb_hp = mb.clone ();
      ACE_Message_Block *const mb_lp = mb.clone ();
      hp_handler.putq (mb_hp);
      lp_handler.putq (mb_lp);
    }

  ACE_Message_Block stop {0, ACE_Message_Block::MB_BREAK};
  ACE_Message_Block *const hp_stop = stop.clone ();
  ACE_Message_Block *const lp_stop = stop.clone ();
  hp_handler.putq (hp_stop);
  lp_handler.putq (lp_stop);
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
