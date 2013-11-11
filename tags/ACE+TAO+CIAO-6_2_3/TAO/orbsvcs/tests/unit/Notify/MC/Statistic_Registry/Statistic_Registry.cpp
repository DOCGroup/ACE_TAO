// $Id$

#include "ace/Monitor_Base.h"
#include "ace/Monitor_Point_Registry.h"

#include "tao/TAO_Singleton_Manager.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

void
error (const char* msg)
{
#if defined (ACE_NLOGGING)
  ACE_UNUSED_ARG (msg);
#endif /* ACE_NLOGGING */
  ACE_ERROR ((LM_ERROR, "%s\n", msg));
  ACE_OS::exit (1);
}

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

int
ACE_TMAIN (int, ACE_TCHAR*[])
{
#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

  try
    {
      TAO_Singleton_Manager::instance ()->init ();

      // Test registry acquisition.
      Monitor_Point_Registry* reg = Monitor_Point_Registry::instance ();

      if (reg == 0)
        {
          error ("Monitor_Point_Registry::instance() failed");
        }

      // Test registry addition.
      Monitor_Base* s = 0;
      ACE_NEW_RETURN (s,
                      Monitor_Base ("test1",
                                    Monitor_Control_Types::MC_COUNTER),
                      2);

      if (reg->add (s) == false)
        {
          error ("clean Monitor_Point_Registry::add() failed");
        }

      if (reg->add (s) == true)
        {
          error ("duplicate Monitor_Point_Registry::add() failed");
        }

      // Test registry addition of null value - should get ACE_ERROR
      // message and return value of 'false'.
      if (reg->add (0) == true)
        {
          error ("Monitor_Point_Registry::add() of null succeeded");
        }

      // Test registry removal.
      if (reg->remove ("fake name") == true)
        {
          error ("non-existent Monitor_Point_Registry::remove() failed");
        }

      if (reg->remove ("test1") == false)
        {
          error ("existent Monitor_Point_Registry::remove() failed");
        }

      // Test destruction with registered statistics.
      ACE_NEW_RETURN (s,
                      Monitor_Base ("test1",
                                    Monitor_Control_Types::MC_COUNTER),
                      2);

      if (reg->add (s) == false)
        {
          error ("re-addition Monitor_Point_Registry::add() failed");
        }

      ACE_NEW_RETURN (s,
                      Monitor_Base ("test2",
                                    Monitor_Control_Types::MC_NUMBER),
                      2);

      if (reg->add (s) == false)
        {
          error ("second Monitor_Point_Registry::add() failed");
        }

      TAO_Singleton_Manager::instance ()->fini ();
    }
  catch (...)
    {
      error ("Caught an unexpected exception type");
    }

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}

