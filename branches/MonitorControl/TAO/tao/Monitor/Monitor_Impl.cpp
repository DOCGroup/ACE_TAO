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

::Monitor::NameList *
Monitor_Impl::get_statistic_names (const char * /* filter */)
{
  MonitorControl_Types::NameList mc_names =
    Monitor_Point_Registry::instance ()->names ();

  ::Monitor::NameList *namelist = 0;
  ACE_NEW_THROW_EX (namelist,
                    ::Monitor::NameList (mc_names.size ()),
                    CORBA::NO_MEMORY ());

  CORBA::ULong index = 0;
  for (MonitorControl_Types::NameList::Iterator i (mc_names);
       !i.done ();
       i.advance (), ++index)
    {
      CORBA::ULong length = namelist->length ();
      namelist->length (length + 1);
      ACE_CString *item = 0;
      i.next (item);
      // @todo Check filter
      (*namelist)[length] = CORBA::string_dup (item->c_str ());
    }

  return namelist;
}

::Monitor::DataItemList *
Monitor_Impl::get_statistics (const ::Monitor::NameList & names)
{
  ::Monitor::DataItemList *datalist = 0;
  ACE_NEW_THROW_EX (datalist,
                    ::Monitor::DataItemList (names.length ()),
                    CORBA::NO_MEMORY ());

  /// Get an instance of the MC service singleton.
  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  MonitorControl_Types::Data d;

  for (CORBA::ULong index = 0; index < names.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      ACE::MonitorControl::Monitor_Base *monitor =
        mgr->admin ().monitor_point (names[index]);

      if (monitor != 0)
        {
          CORBA::ULong const length = datalist->length();
          datalist->length (length + 1);
          ::Monitor::DataItem *dataitem = 0;
          ACE_NEW_THROW_EX (dataitem,
                            ::Monitor::DataItem,
                            CORBA::NO_MEMORY ());
          dataitem->itemname = CORBA::string_dup (names[index].in());
          dataitem->dlist.length (1);
          ::Monitor::Data data;
          monitor->retrieve (d);
          data.value = d.value_;
          ACE_UINT64 usecs;
          d.timestamp_.to_usec (usecs);
          data.timestamp = usecs;
          (*dataitem).dlist[0] = data;
          (*datalist)[length] = *dataitem;
        }
    }

  return datalist;
}

::Monitor::DataItemList *
Monitor_Impl::get_and_clear_statistics (const ::Monitor::NameList & names)
{
  ::Monitor::DataItemList *datalist = 0;
  ACE_NEW_THROW_EX (datalist,
                    ::Monitor::DataItemList (names.length ()),
                    CORBA::NO_MEMORY ());

  /// Get an instance of the MC service singleton.
  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  MonitorControl_Types::Data d;

  for (CORBA::ULong index = 0; index < names.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      ACE::MonitorControl::Monitor_Base *monitor =
        mgr->admin ().monitor_point (names[index]);

      if (monitor != 0)
        {
          CORBA::ULong const length = datalist->length();
          datalist->length (length + 1);
          ::Monitor::DataItem *dataitem = 0;
          ACE_NEW_THROW_EX (dataitem,
                            ::Monitor::DataItem,
                            CORBA::NO_MEMORY ());
          dataitem->itemname = CORBA::string_dup (names[index].in());
          dataitem->dlist.length (1);
          ::Monitor::Data data;
          monitor->retrieve_and_clear (d);
          data.value = d.value_;
          ACE_UINT64 usecs;
          d.timestamp_.to_usec (usecs);
          data.timestamp = usecs;
          (*dataitem).dlist[0] = data;
          (*datalist)[length] = *dataitem;
        }
    }

  return datalist;
}

::Monitor::NameList *
Monitor_Impl::clear_statistics (const ::Monitor::NameList & names)
{
  ::Monitor::NameList *namelist = 0;
  ACE_NEW_THROW_EX (namelist,
                    ::Monitor::NameList (names.length ()),
                    CORBA::NO_MEMORY ());
  namelist->length (0UL);

  /// Get an instance of the MC service singleton.
  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  for (CORBA::ULong index = 0; index < names.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      ACE::MonitorControl::Monitor_Base *monitor =
        mgr->admin ().monitor_point (names[index]);

      if (monitor != 0)
        {
          CORBA::ULong const length = namelist->length ();
          namelist->length (length + 1);
          (*namelist)[length] = CORBA::string_dup (names[index].in ());
          monitor->clear ();
        }
    }

  return namelist;
}

::Monitor::ConstraintId
Monitor_Impl::register_constraint (
      const ::Monitor::NameList & names,
      const char * cs,
      ::Monitor::Subscriber_ptr sub)
{
}

void
Monitor_Impl::unregister_constraint ( ::Monitor::ConstraintId constaint)
{

}

