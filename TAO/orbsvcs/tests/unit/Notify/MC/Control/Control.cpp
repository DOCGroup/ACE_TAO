// $Id$

#include "ace/Log_Msg.h"

#include "orbsvcs/orbsvcs/Notify/MonitorControl/Control.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

static ACE_CString command;

class ControlTest : public TAO_NS_Control
{
public:
  ControlTest (const ACE_CString& name)
    : TAO_NS_Control (name.c_str ())
  {
  }

  virtual bool execute (const char* cmd)
  {
    command = cmd;
    return (ACE_OS::strcmp (cmd, TAO_NS_CONTROL_SHUTDOWN) == 0);
  }
};

void
error (const char* msg)
{
  ACE_ERROR ((LM_ERROR, "%s\n", msg));
  ACE_OS::exit (1);
}

int
ACE_TMAIN (int, ACE_TCHAR*[])
{
  try
    {
      ACE_CString name ("Control Test");
      ControlTest ct (name);

      if (name != ct.name ())
        {
          error ("The TAO_NS_Control name does not work");
        }

      if (ct.execute (TAO_NS_CONTROL_REMOVE_CONSUMER))
        {
          error ("The control object should have returned false");
        }

      if (!ct.execute (TAO_NS_CONTROL_SHUTDOWN))
        {
          error ("The control object shouldn't have returned false");
        }

      if (command != TAO_NS_CONTROL_SHUTDOWN)
        {
          error ("The TAO_NS_Control callback does not work");
        }
    }
  catch(...)
    {
      error ("Caught an unexpected exception type");
    }

  return 0;
}

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

