// $Id$
#include "orbsvcs/Notify/MonitorControl/Statistic_Registry.h"
#include "orbsvcs/Notify/MonitorControl/Statistic.h"
#include "orbsvcs/Notify/MonitorControl/NotificationServiceMonitor_i.h"
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
      for(int i = 0; i < 10; i++)
        s->receive();

      ACE_NEW_RETURN (s,
                      TAO_Statistic ("test2",
                                     TAO_Statistic::TS_NUMBER),
                      2);
      if (reg->add(s) == false)
        error("second TAO_Statistic_Registry::add() failed");
      for(int i = 0; i < 10; i++)
        s->receive(i);

      ACE_NEW_RETURN (s,
                      TAO_Statistic ("test3",
                                     TAO_Statistic::TS_INTERVAL),
                      2);
      if (reg->add(s) == false)
        error("third TAO_Statistic_Registry::add() failed");
      for(int i = 0; i < 10; i++)
        s->receive(i / .08);

      NotificationServiceMonitor_i monitor;
      CosNotification::NotificationServiceMonitorControl::NameList* names =
        monitor.get_statistic_names ();
      if (names == 0 || names->length () != 3)
        error("get_statistic_names() returned the incorrect number of names");

      CosNotification::NotificationServiceMonitorControl::Data_var d =
        monitor.get_statistic ((*names)[0]);
      CosNotification::NotificationServiceMonitorControl::Numeric num = d->num ();
      if (num.count != 10)
        error("get_statistic() returned the wrong data");

      CosNotification::NotificationServiceMonitorControl::DataList* data =
        monitor.get_statistics (*names);
      if (data == 0 || data->length () != 3)
        error("get_statistics() returned the incorrect number of data elements");
      num = (*data)[1].num ();
      if (num.average != 4.5)
        error("get_statistics() return the wrong data");
      delete data;

      data = monitor.get_and_clear_statistics (*names);
      if (data == 0 || data->length () != 3)
        error("get_and_clear_statistics() returned the incorrect number of data elements");
      for(CORBA::ULong i = 0; i < data->length (); i++)
        {
          num = (*data)[i].num ();
          if (num.count == 0)
            error("get_and_clear_statistics() failed");
        }
      delete data;

      // Test the clear_statistics method
      for(int i = 0; i < 10; i++)
        s->receive(i / .7);
      monitor.clear_statistics (*names);
      if (s->count() != 0)
        error("clear_statistics() did not clear the data");

      delete names;

      TAO_Singleton_Manager::instance ()->fini ();
    }
  catch(...)
    {
      error("Caught an unexpected exception type");
    }

  return 0;
}
