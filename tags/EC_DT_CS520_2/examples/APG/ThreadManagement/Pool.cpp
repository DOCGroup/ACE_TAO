// $Id$

#include "ace/Task.h"
#include "ace/Log_Msg.h"

// Listing 1 code/ch13
class HA_CommandHandler : public ACE_Task<ACE_MT_SYNCH>
{
public:
  virtual int svc (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) starting up \n")));
    ACE_Message_Block *mb;
    if (this->getq (mb) == -1)
      return -1;
    // ... do something with the message.
    return 0;
  }
};
// Listing 1
// Listing 2 code/ch13
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  HA_CommandHandler handler;

  // Create 4 threads.
  handler.activate (THR_NEW_LWP | THR_JOINABLE, 4);
  handler.wait ();
  return 0;
}
// Listing 2
