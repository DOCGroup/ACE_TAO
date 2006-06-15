// $Id$

#include "ace/Local_Tokens.h"
#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"

#if defined (ACE_HAS_TOKENS_LIBRARY)

// Listing 1 code/ch14
class ThreadOne : public ACE_Task_Base
{
public:
  virtual int svc (void)
  {
    ACE_Local_Mutex mutex1 (ACE_TEXT ("resource1"),
                            0, // Deadlock detection enabled.
                            1);// Debugging enabled.
    mutex1.acquire ();
    ACE_OS::sleep (2);
    ACE_Local_Mutex mutex2 (ACE_TEXT ("resource2"), 0, 1);
    mutex2.acquire ();
    return 0;
  }
};

class ThreadTwo : public ACE_Task_Base
{
public:
  virtual int svc (void)
  {
    ACE_Local_Mutex mutex2 (ACE_TEXT ("resource2"),
                            0, // Deadlock detection enabled.
                            1);// Debugging enabled.
    mutex2.acquire ();
    ACE_OS::sleep (2);
    ACE_Local_Mutex mutex1 (ACE_TEXT ("resource1"),
                            0, // Deadlock detection enabled.
                            1);// Debugging enabled.
    mutex1.acquire ();
    return 0;
  }
};

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ThreadOne t1;
  ThreadTwo t2;

  t1.activate ();
  ACE_OS::sleep (1);
  t2.activate ();
  t1.wait ();
  t2.wait ();

  return 0;
}
// Listing 1

#else /* defined (ACE_HAS_TOKENS_LIBRARY) */

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR,
              ACE_TEXT ("local tokens not supported ")
              ACE_TEXT ("on this platform\n")));
  return 0;
}

#endif /* defined (ACE_HAS_TOKENS_LIBRARY) */
