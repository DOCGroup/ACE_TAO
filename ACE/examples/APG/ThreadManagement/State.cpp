// $Id$

#include "ace/Task.h"

class HA_CommandHandler : public ACE_Task_Base
{
public:
  virtual int svc (void)
  {
    ACE_DEBUG
      ((LM_DEBUG, ACE_TEXT ("(%t) Handler Thread running\n")));
    return 0;
  }
};


int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Main Thread running\n")));
// Listing 1 code/ch13
  HA_CommandHandler handler;
  int result = handler.activate (THR_NEW_LWP |
                                 THR_JOINABLE |
                                 THR_SUSPENDED);
  ACE_ASSERT (result == 0);

  ACE_UNUSED_ARG (result);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) The current thread count is %d\n"),
              handler.thr_count ()));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) The group identifier is %d\n"),
              handler.grp_id ()));
  handler.resume ();
  handler.wait ();
// Listing 1
  return 0;
}
