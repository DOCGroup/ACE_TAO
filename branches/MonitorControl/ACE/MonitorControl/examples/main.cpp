
#include "MonitorControl/MonitorControl.h"

#include "ace/OS_NS_unistd.h"

int main (int argc, char *argv [])
{
  try
  {
    START_MC_SERVICE;
    ADD_PERIODIC_MONITOR (CPU_LOAD_MONITOR, 2000);
    START_PERIODIC_MONITORS;
    
    for (int i = 0; i < 10; ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "looping\n"));
        ACE_OS::sleep (1);
      }

    STOP_PERIODIC_MONITORS;
  }
  catch (const GenericRegistry::MapError &e)
  {
    switch (e.why_)
    {
      case GenericRegistry::MapError::MAP_ERROR_BIND_FAILURE:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Monitor add failed\n"),
                          -1);
        break;
      case GenericRegistry::MapError::MAP_ERROR_INVALID_VALUE:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Invalid monitor\n"),
                          -1);
        break;
      default:
        break;
    }
  }

  return 0;
}