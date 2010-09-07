// $Id$

#include "tao/Monitor/Monitor_Impl.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

#include "ace/Monitor_Point_Registry.h"
#include "ace/Monitor_Control_Action.h"
#include "ace/Monitor_Control/Monitor_Control.h"

class TAO_Control_Action : public Control_Action
{
public:
  TAO_Control_Action (::Monitor::Subscriber_ptr sub)
    : sub_ (::Monitor::Subscriber::_duplicate (sub))
  {
  }

  virtual void execute (const char* /* command */)
  {
    try
      {
        // @todo, really want to send the value to the subscriber.
        ::Monitor::DataList d;
        sub_->push (d);
      }
    catch (const ::CORBA::Exception&)
      {
      }
  }

private:
  ::Monitor::Subscriber_var sub_;
};

Monitor_Impl::Monitor_Impl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

::Monitor::NameList *
Monitor_Impl::get_statistic_names (const char * filter)
{
  Monitor_Control_Types::NameList mc_names =
    Monitor_Point_Registry::instance ()->names ();

  ::Monitor::NameList *namelist = 0;
  ACE_NEW_THROW_EX (namelist,
                    ::Monitor::NameList (mc_names.size ()),
                    CORBA::NO_MEMORY ());

  CORBA::ULong index = 0;

  for (Monitor_Control_Types::NameList::Iterator i (mc_names);
       !i.done ();
       i.advance (), ++index)
    {
      ACE_CString *item = 0;
      i.next (item);

      if (ACE::wild_match (item->c_str (), filter, false))
        {
          CORBA::ULong const length = namelist->length ();
          namelist->length (length + 1);
          (*namelist)[length] = CORBA::string_dup (item->c_str ());
        }
    }

  return namelist;
}

::Monitor::DataList *
Monitor_Impl::get_statistics (const ::Monitor::NameList & names)
{
  ::Monitor::DataList *datalist = 0;
  ACE_NEW_THROW_EX (datalist,
                    ::Monitor::DataList (names.length ()),
                    CORBA::NO_MEMORY ());

  /// Get an instance of the MC service singleton.
  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");


  for (CORBA::ULong index = 0; index < names.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      ACE::Monitor_Control::Monitor_Base *monitor =
        mgr->admin ().monitor_point (names[index]);

      if (monitor != 0)
        {
          Monitor_Control_Types::Data d (monitor->type ());
          CORBA::ULong const length = datalist->length();
          datalist->length (length + 1);
          ::Monitor::Data data;
          TAO_Monitor::get_monitor_data (monitor, data, false);
          (*datalist)[length] = data;

          monitor->remove_ref ();
        }
    }

  return datalist;
}

::Monitor::DataList *
Monitor_Impl::get_and_clear_statistics (const ::Monitor::NameList & names)
{
  ::Monitor::DataList *datalist = 0;
  ACE_NEW_THROW_EX (datalist,
                    ::Monitor::DataList (names.length ()),
                    CORBA::NO_MEMORY ());

  /// Get an instance of the MC service singleton.
  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  for (CORBA::ULong index = 0; index < names.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      ACE::Monitor_Control::Monitor_Base *monitor =
        mgr->admin ().monitor_point (names[index]);

      if (monitor != 0)
        {
          CORBA::ULong const length = datalist->length();
          datalist->length (length + 1);
          ::Monitor::Data data;
          TAO_Monitor::get_monitor_data (monitor, data, true);
          (*datalist)[length] = data;

          monitor->remove_ref ();
        }
    }

  return datalist;
}

void
TAO_Monitor::get_monitor_data (
   ACE::Monitor_Control::Monitor_Base *monitor,
   Monitor::Data& data,
   bool clear)
{
  // If it's not a counter, we need to make sure that we have
  // the most up-to-date information.  A counter will always have
  // the correct value.
  if (monitor->type () != Monitor_Control_Types::MC_COUNTER)
    {
      monitor->update ();
    }

  // Populate the data structure based on the type of statistic
  if (monitor->type () == Monitor_Control_Types::MC_LIST)
    {
      Monitor_Control_Types::NameList slist (monitor->get_list ());
      CORBA::ULong size = static_cast<CORBA::ULong> (slist.size ());
      Monitor::NameList list (size);
      list.length (size);

      for (CORBA::ULong i = 0; i < size; ++i)
        {
          list[i] = CORBA::string_dup (slist[i].c_str ());
        }

      data.data_union._d (::Monitor::DATA_TEXT);
      data.data_union.list (list);
    }
  else
    {
      Monitor::Numeric num;
      num.count = static_cast<CORBA::ULong> (monitor->count ());
      num.minimum = monitor->minimum_sample ();
      num.maximum = monitor->maximum_sample ();
      num.last = monitor->last_sample ();
      num.dlist.length (1);
      num.dlist[0].value = monitor->last_sample ();
      Monitor_Control_Types::Data d (monitor->type ());
      if (clear)
        {
          monitor->retrieve_and_clear (d);
        }
      else
        {
          monitor->retrieve (d);
        }
      ACE_UINT64 usecs;
      d.timestamp_.to_usec (usecs);

      if (monitor->type() == Monitor_Control_Types::MC_COUNTER)
        {
          num.average = 0;
          num.sum_of_squares = 0;
        }
      else
        {
          num.average = monitor->average ();
          num.sum_of_squares = monitor->sum_of_squares ();
        }

      data.data_union._d (::Monitor::DATA_NUMERIC);
      data.data_union.num (num);
      data.data_union.num ().dlist[0].value = d.value_;
      data.data_union.num ().dlist[0].timestamp = usecs;
    }
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
      ACE::Monitor_Control::Monitor_Base *monitor =
        mgr->admin ().monitor_point (names[index]);

      if (monitor != 0)        {
          CORBA::ULong const length = namelist->length ();
          namelist->length (length + 1);
          (*namelist)[length] = CORBA::string_dup (names[index].in ());
          monitor->clear ();

          monitor->remove_ref ();
        }
    }

  return namelist;
}

::Monitor::ConstraintStructList *
Monitor_Impl::register_constraint (
      const ::Monitor::NameList & names,
      const char * cs,
      ::Monitor::Subscriber_ptr sub)
{
  ::Monitor::ConstraintStructList *constraintlist = 0;
  ACE_NEW_THROW_EX (constraintlist,
                    ::Monitor::ConstraintStructList (names.length ()),
                    CORBA::NO_MEMORY ());
  constraintlist->length (0UL);

  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  for (CORBA::ULong index = 0; index < names.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      ACE::Monitor_Control::Monitor_Base *monitor =
        mgr->admin ().monitor_point (names[index]);

      if (monitor != 0)
        {
          TAO_Control_Action* ca = new TAO_Control_Action (sub);
          long id = monitor->add_constraint (cs, ca);
          CORBA::ULong const length = constraintlist->length ();
          constraintlist->length (length + 1);
          (*constraintlist)[length].id = id;
          (*constraintlist)[length].itemname =
            CORBA::string_dup (names[index].in ());

          monitor->remove_ref ();
        }
    }

  return constraintlist;
}

void
Monitor_Impl::unregister_constraints (
  const ::Monitor::ConstraintStructList & constraint)
{
  MC_ADMINMANAGER* mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  for (CORBA::ULong index = 0; index < constraint.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      ACE::Monitor_Control::Monitor_Base *monitor =
        mgr->admin ().monitor_point (constraint[index].itemname.in ());

      if (monitor != 0)
        {
          ACE::Monitor_Control::Control_Action *action =
            monitor->remove_constraint (constraint[index].id);

          if (action != 0)
            {
              action->remove_ref ();
            }

          monitor->remove_ref ();
        }
    }
}

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

