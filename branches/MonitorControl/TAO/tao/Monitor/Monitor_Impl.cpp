//
// $Id$
//
#include "tao/Monitor/Monitor_Impl.h"
#include "ace/Monitor_Point_Registry.h"
#include "MonitorControl/MonitorControl.h"

ACE_RCSID (Monitor,
           Monitor_Impl,
           "$Id$")

Monitor_Impl::Monitor_Impl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

::Monitor::MC::NameList *
Monitor_Impl::get_statistic_names (void)
{
  MonitorControl_Types::NameList mc_names =
    Monitor_Point_Registry::instance ()->names ();

  ::Monitor::MC::NameList *namelist = 0;
  ACE_NEW_THROW_EX (namelist,
                    ::Monitor::MC::NameList (mc_names.size ()),
                    CORBA::NO_MEMORY ());

  namelist->length (mc_names.size ());

  CORBA::ULong index = 0;
  for (MonitorControl_Types::NameList::Iterator i (mc_names);
       !i.done ();
       i.advance (), ++index)
    {
      ACE_CString *item = 0;
      i.next (item);
      (*namelist)[index] = CORBA::string_dup (item->c_str ());
    }

  return namelist;
}

::Monitor::MC::Data
Monitor_Impl::get_statistic (const char * the_name)
{
  /// Get an instance of the MC service singleton.
  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  /// Call on the administrator class to look up the desired monitors.
  ACE::MonitorControl::Monitor_Base *monitor =
    mgr->admin ().monitor_point (the_name);

  if (monitor == 0)
    {
      Monitor::MC::NameList seq;
      seq.length (1);
      seq[0] = CORBA::string_dup (the_name);
      throw ::Monitor::MC::InvalidName (seq);
    }

  MonitorControl_Types::Data d;
  ::Monitor::MC::Data data;
  monitor->retrieve (d);
  data.value = d.value_;
  ACE_UINT64 usecs;
  d.timestamp_.to_usec (usecs);
  data.timestamp = usecs;

  return data;
}

::Monitor::MC::DataList *
Monitor_Impl::get_statistics (const ::Monitor::MC::NameList & names)
{
  ::Monitor::MC::DataList *datalist = 0;
  ACE_NEW_THROW_EX (datalist,
                    ::Monitor::MC::DataList (names.length ()),
                    CORBA::NO_MEMORY ());

  /// Get an instance of the MC service singleton.
  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  MonitorControl_Types::Data d;
  Monitor::MC::NameList seq;

  for (CORBA::ULong index = 0; index < names.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      ACE::MonitorControl::Monitor_Base *monitor =
        mgr->admin ().monitor_point (names[index]);

      if (monitor != 0)
        {
          if (seq.length () == 0)
            {
              ::Monitor::MC::Data data;
              monitor->retrieve (d);
              data.value = d.value_;
              ACE_UINT64 usecs;
              d.timestamp_.to_usec (usecs);
              data.timestamp = usecs;
              (*datalist)[index] = data;
            }
        }
      else
        {
          seq.length (seq.length() + 1);
          seq[seq.length() - 1] = CORBA::string_dup (names[index].in ());
        }
    }

  if (seq.length () > 0)
  {
    throw ::Monitor::MC::InvalidName (seq);
  }

  return datalist;
}

void
Monitor_Impl::check_names_i (const ::Monitor::MC::NameList & names)
{
  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  Monitor::MC::NameList seq;

  for (CORBA::ULong index = 0; index < names.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      ACE::MonitorControl::Monitor_Base *monitor =
        mgr->admin ().monitor_point (names[index]);
      if (!monitor)
        {
          seq.length (seq.length() + 1);
          seq[seq.length() - 1] = CORBA::string_dup (names[index].in ());
        }
    }

  if (seq.length () > 0)
    {
      throw ::Monitor::MC::InvalidName (seq);
    }
}

::Monitor::MC::DataList *
Monitor_Impl::get_and_clear_statistics (const ::Monitor::MC::NameList & names)
{
  ::Monitor::MC::DataList *datalist = 0;
  ACE_NEW_THROW_EX (datalist,
                    ::Monitor::MC::DataList (names.length ()),
                    CORBA::NO_MEMORY ());

  /// Get an instance of the MC service singleton.
  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  MonitorControl_Types::Data d;
  Monitor::MC::NameList seq;

  for (CORBA::ULong index = 0; index < names.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      ACE::MonitorControl::Monitor_Base *monitor =
        mgr->admin ().monitor_point (names[index]);

      if (monitor != 0)
        {
          if (seq.length () == 0)
            {
              ::Monitor::MC::Data data;
              monitor->retrieve (d);
              data.value = d.value_;
              ACE_UINT64 usecs;
              d.timestamp_.to_usec (usecs);
              data.timestamp = usecs;
              (*datalist)[index] = data;
            }
        }
      else
        {
          seq.length (seq.length() + 1);
          seq[seq.length() - 1] = CORBA::string_dup (names[index].in ());
        }
    }

  if (seq.length () > 0)
    {
      throw ::Monitor::MC::InvalidName (seq);
    }
  else
    {
      for (CORBA::ULong index = 0; index < names.length (); ++index)
        {
          /// Call on the administrator class to look up the desired monitors.
          ACE::MonitorControl::Monitor_Base *monitorb =
            mgr->admin ().monitor_point (names[index]);

          if (monitorb != 0)
            {
              monitorb->clear ();
            }
        }
    }

  return datalist;
}

void
Monitor_Impl::clear_statistics (const ::Monitor::MC::NameList & names)
{
  /// Get an instance of the MC service singleton.
  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  Monitor::MC::NameList seq;

  for (CORBA::ULong index = 0; index < names.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      ACE::MonitorControl::Monitor_Base *monitor =
        mgr->admin ().monitor_point (names[index]);

      if (monitor != 0)
        {
          monitor->clear ();
        }
      else
        {
          seq.length (seq.length() + 1);
          seq[seq.length() - 1] = CORBA::string_dup (names[index].in ());
        }
    }

  if (seq.length () > 0)
  {
    throw ::Monitor::MC::InvalidName (seq);
  }
}

