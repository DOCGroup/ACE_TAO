
#include "MonitorControl/MonitorControl.h"

int main (int argc, char *argv [])
{
  ACE_Service_Config::open (argc,
                            argv,
                            ACE_DEFAULT_LOGGER_KEY,
                            false);
  try
  {
    ADD_MONITOR (CPU_LOAD_MONITOR);
    ADD_MONITOR (CPU_LOAD_MONITOR);
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