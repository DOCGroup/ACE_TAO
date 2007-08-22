// $Id$
#include "ace/Log_Msg.h"
#include "orbsvcs/Notify/MonitorControl/Control.h"

static ACE_CString command;

class ControlTest: public TAO_NS_Control
{
public:
  ControlTest (const ACE_CString& name)
   : TAO_NS_Control (name.c_str ()) {
  }

  virtual void execute (const char* cmd) {
    command = cmd;
  }
};

void
error(const char* msg)
{
  ACE_ERROR ((LM_ERROR, "%s\n", msg));
  ACE_OS::exit(1);
}

int
ACE_TMAIN (int, ACE_TCHAR*[])
{
  try
    {
      ACE_CString name ("Control Test");
      ControlTest ct (name);

      if (name != ct.name ())
        error ("The TAO_NS_Control name does not work");

      ct.execute (TAO_NS_CONTROL_SHUTDOWN);
      if (command != TAO_NS_CONTROL_SHUTDOWN)
        error ("The TAO_NS_Control callback does not work");
    }
  catch(...)
    {
      error("Caught an unexpected exception type");
    }

  return 0;
}
