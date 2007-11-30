// $Id$
#include "orbsvcs/Notify/MonitorControl/Statistic_Registry.h"
#include "orbsvcs/Notify/MonitorControl/Statistic.h"
#include "tao/TAO_Singleton_Manager.h"

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
      TAO_Singleton_Manager::instance ()->init ();

      // Test registry acquisition
      TAO_Statistic_Registry* reg = TAO_Statistic_Registry::instance();
      if (reg == 0)
        error("TAO_Statistic_Registry::instance() failed");

      // Test registry addition
      TAO_Statistic* s = 0;
      ACE_NEW_RETURN (s,
                      TAO_Statistic ("test1",
                                     TAO_Statistic::TS_COUNTER),
                      2);

      if (reg->add(s) == false)
        error("clean TAO_Statistic_Registry::add() failed");

      if (reg->add(s) == true)
        error("duplicate TAO_Statistic_Registry::add() failed");

      try
        {
          reg->add(0);
          error("exception TAO_Statistic_Registry::add() failed");
        }
      catch (const TAO_Statistic_Registry::Map_Error& ex)
        {
          if (ex.why_ != TAO_Statistic_Registry::Map_Error::MAP_ERROR_INVALID_VALUE)
            {
              error("TAO_Statistic_Registry::add() threw exception "
                    "with an incorrect reason");
            }
        }

      // Test registry removal
      if (reg->remove("fake name") == true)
        error("non-existent TAO_Statistic_Registry::remove() failed");

      if (reg->remove("test1") == false)
        error("existent TAO_Statistic_Registry::remove() failed");

      // Test destruction with registered statistics
      ACE_NEW_RETURN (s,
                      TAO_Statistic ("test1",
                                     TAO_Statistic::TS_COUNTER),
                      2);
      if (reg->add(s) == false)
        error("re-addition TAO_Statistic_Registry::add() failed");

      ACE_NEW_RETURN (s,
                      TAO_Statistic ("test2",
                                     TAO_Statistic::TS_NUMBER),
                      2);
      if (reg->add(s) == false)
        error("second TAO_Statistic_Registry::add() failed");

      TAO_Singleton_Manager::instance ()->fini ();
    }
  catch(...)
    {
      error("Caught an unexpected exception type");
    }

  return 0;
}
