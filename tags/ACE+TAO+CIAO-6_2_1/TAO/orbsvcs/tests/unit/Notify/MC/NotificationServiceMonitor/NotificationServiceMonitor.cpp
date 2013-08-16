// $Id$

#include "ace/Monitor_Base.h"
#include "ace/Monitor_Point_Registry.h"

#include "tao/TAO_Singleton_Manager.h"

#include "orbsvcs/Notify/MonitorControl/NotificationServiceMonitor_i.h"

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
          ACE_ERROR_RETURN ((LM_ERROR, "Monitor_Point_Registry::instance() failed\n"), 1);
        }

      // This can vary with the platform, so to make the test more
      // portable we get the starting size here and make sure that
      // the monitors we add subsequently are recognized.
      Monitor_Control_Types::NameList start_names = reg->names ();
      size_t start_size = start_names.size ();

      // Test registry addition.
      Monitor_Base* s = 0;
      ACE_NEW_RETURN (s,
                      Monitor_Base ("test1",
                                    Monitor_Control_Types::MC_COUNTER),
                      2);

      if (reg->add (s) == false)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "clean Monitor_Point_Registry::add() failed\n"), 1);
        }

      /// Index for several FOR loops below.
      size_t i = 0;

      for (i = 0; i < 10; ++i)
        {
          s->receive (0.0);
        }

      ACE_NEW_RETURN (s,
                      Monitor_Base ("test2",
                                    Monitor_Control_Types::MC_NUMBER),
                      2);

      if (reg->add (s) == false)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "second Monitor_Point_Registry::add() failed\n"), 1);
        }

      for (i = 0; i < 10; ++i)
        {
          s->receive  (i);
        }

      ACE_NEW_RETURN (s,
                      Monitor_Base ("test3",
                                    Monitor_Control_Types::MC_INTERVAL),
                      2);

      if (reg->add (s) == false)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "third Monitor_Point_Registry::add() failed\n"), 1);
        }

      for (i = 0; i < 10; ++i)
        {
          s->receive (i / .08);
        }

      NotificationServiceMonitor_i monitor;
      Monitor::NameList* names = monitor.get_statistic_names ();
      Monitor::NameList_var safe_names = names;

      // We added 3 to whatever was there already.
      if (names == 0 || names->length () != start_size + 3)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "get_statistic_names() returned the incorrect number of names\n"), 1);
        }

      CORBA::ULong index = start_size;
      Monitor::Data_var d =
        monitor.get_statistic (safe_names[index]);
      Monitor::Numeric num =
        d->data_union.num ();

      if (num.count != 10)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "get_statistic() returned the wrong data\n"), 1);
        }

      Monitor::DataList_var data =
        monitor.get_statistics (*names);

      // We added 3 monitors to whatever was there already.
      if (data.ptr () == 0 || data.in ().length () != start_size + 3)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "get_statistics() returned the incorrect number of data elements\n"), 1);
        }

      index = start_size + 1;
      num = data[index].data_union.num ();

      if (!ACE::is_equal (num.average, 4.5))
        {
          ACE_ERROR_RETURN ((LM_ERROR, "get_statistics() return the wrong data\n"), 1);
        }

      data = monitor.get_and_clear_statistics (*names);

      // We added 3 to whatever was there already.
      if (data.ptr () == 0 || data.in ().length () != start_size + 3)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "get_and_clear_statistics() returned the incorrect number of data elements\n"), 1);
        }

      // Skip the monitors not added by this test.
      for (index = start_size; i < data.in ().length (); ++index)
        {
          num = data[index].data_union.num ();

          if (num.count == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR, "get_and_clear_statistics() failed\n"), 1);
            }
        }

      // Test the clear_statistics method.
      for (i = 0; i < 10; ++i)
        {
          s->receive (i / .7);
        }

      monitor.clear_statistics (*names);

      if (s->count () != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR, "clear_statistics() did not clear the data\n"), 1);
        }

      TAO_Singleton_Manager::instance ()->fini ();
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Caught an unexpected exception type\n"), 1);
    }

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

  return 0;
}
