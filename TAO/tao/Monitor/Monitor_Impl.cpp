#include "tao/Monitor/Monitor_Impl.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

#include "ace/Monitor_Point_Registry.h"
#include "ace/Monitor_Control_Action.h"
#include "ace/Monitor_Control/Monitor_Control.h"

#include <memory>

class TAO_Control_Action : public Control_Action
{
public:
  explicit TAO_Control_Action (::Monitor::Subscriber_ptr sub)
    : sub_ (::Monitor::Subscriber::_duplicate (sub))
  {
  }

  TAO_Control_Action (TAO_Control_Action const &) = delete;
  TAO_Control_Action &operator= (TAO_Control_Action const &) = delete;
  TAO_Control_Action (TAO_Control_Action &&) = delete;
  TAO_Control_Action &operator= (TAO_Control_Action &&) = delete;

  void execute (char const* /* command */) override
  {
    try
      {
        // @todo, really want to send the value to the subscriber.
        ::Monitor::DataList d;
        sub_->push (d);
      }
    catch (::CORBA::Exception const &)
      {
      }
  }

private:
  ::Monitor::Subscriber_var sub_;
};

struct TAO_Monitor_Point_Deleter
{
  void operator() (ACE::Monitor_Control::Monitor_Base *monitor) const
  {
    monitor->remove_ref ();
  }
};

struct TAO_Control_Action_Deleter
{
  void operator() (ACE::Monitor_Control::Control_Action *action) const
  {
    action->remove_ref ();
  }
};

using TAO_Monitor_Point_Ptr = std::unique_ptr<ACE::Monitor_Control::Monitor_Base,
                                              TAO_Monitor_Point_Deleter>;
using TAO_Control_Action_Ptr = std::unique_ptr<ACE::Monitor_Control::Control_Action,
                                               TAO_Control_Action_Deleter>;

Monitor_Impl::Monitor_Impl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

::Monitor::NameList *
Monitor_Impl::get_statistic_names (char const *filter)
{
  Monitor_Control_Types::NameList mc_names =
    Monitor_Point_Registry::instance ()->names ();

  ::Monitor::NameList *namelist {};
  ACE_NEW_THROW_EX (namelist,
                    ::Monitor::NameList (static_cast<CORBA::ULong> (mc_names.size ())),
                    CORBA::NO_MEMORY ());
  std::unique_ptr<::Monitor::NameList> namelist_owner (namelist);

  for (Monitor_Control_Types::NameList::Iterator i (mc_names);
       !i.done ();
       i.advance ())
    {
      ACE_CString *item {};
      i.next (item);

      if (ACE::wild_match (item->c_str (), filter, false))
        {
          CORBA::ULong const length = namelist->length ();
          namelist->length (length + 1);
          (*namelist)[length] = CORBA::string_dup (item->c_str ());
        }
    }

  return namelist_owner.release ();
}

::Monitor::DataList *
Monitor_Impl::get_statistics (::Monitor::NameList const &names)
{
  ::Monitor::DataList *datalist {};
  ACE_NEW_THROW_EX (datalist,
                    ::Monitor::DataList (names.length ()),
                    CORBA::NO_MEMORY ());
  std::unique_ptr<::Monitor::DataList> datalist_owner (datalist);

  /// Get an instance of the MC service singleton.
  MC_ADMINMANAGER * const mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  for (CORBA::ULong index {}; index < names.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      TAO_Monitor_Point_Ptr monitor (
        mgr->admin ().monitor_point (names[index]));

      if (monitor != nullptr)
        {
          CORBA::ULong const length = datalist->length();
          datalist->length (length + 1);
          ::Monitor::Data data {};
          TAO_Monitor::get_monitor_data (monitor.get (), data, false);
          (*datalist)[length] = data;
        }
    }

  return datalist_owner.release ();
}

::Monitor::DataList *
Monitor_Impl::get_and_clear_statistics (::Monitor::NameList const &names)
{
  ::Monitor::DataList *datalist {};
  ACE_NEW_THROW_EX (datalist,
                    ::Monitor::DataList (names.length ()),
                    CORBA::NO_MEMORY ());
  std::unique_ptr<::Monitor::DataList> datalist_owner (datalist);

  /// Get an instance of the MC service singleton.
  MC_ADMINMANAGER * const mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  for (CORBA::ULong index {}; index < names.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      TAO_Monitor_Point_Ptr monitor (
        mgr->admin ().monitor_point (names[index]));

      if (monitor != nullptr)
        {
          CORBA::ULong const length = datalist->length();
          datalist->length (length + 1);
          ::Monitor::Data data {};
          TAO_Monitor::get_monitor_data (monitor.get (), data, true);
          (*datalist)[length] = data;
        }
    }

  return datalist_owner.release ();
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
      CORBA::ULong const size = static_cast<CORBA::ULong> (slist.size ());
      Monitor::NameList list (size);
      list.length (size);

      for (CORBA::ULong i {}; i < size; ++i)
        {
          list[i] = CORBA::string_dup (slist[i].c_str ());
        }

      data.data_union.list (list);
    }
  else
    {
      Monitor::Numeric num {};
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
      ACE_UINT64 usecs {};
      d.timestamp_.to_usec (usecs);

      if (monitor->type () != Monitor_Control_Types::MC_COUNTER)
        {
          num.average = monitor->average ();
          num.sum_of_squares = monitor->sum_of_squares ();
        }

      data.data_union.num (num);
      data.data_union.num ().dlist[0].value = d.value_;
      data.data_union.num ().dlist[0].timestamp = usecs;
    }
}

::Monitor::NameList *
Monitor_Impl::clear_statistics (::Monitor::NameList const &names)
{
  ::Monitor::NameList *namelist {};
  ACE_NEW_THROW_EX (namelist,
                    ::Monitor::NameList (names.length ()),
                    CORBA::NO_MEMORY ());
  std::unique_ptr<::Monitor::NameList> namelist_owner (namelist);
  namelist->length (0UL);

  /// Get an instance of the MC service singleton.
  MC_ADMINMANAGER * const mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  for (CORBA::ULong index {}; index < names.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      TAO_Monitor_Point_Ptr monitor (
        mgr->admin ().monitor_point (names[index]));

      if (monitor != nullptr)
        {
          CORBA::ULong const length = namelist->length ();
          namelist->length (length + 1);
          (*namelist)[length] = CORBA::string_dup (names[index].in ());
          monitor->clear ();
        }
    }

  return namelist_owner.release ();
}

::Monitor::ConstraintStructList *
Monitor_Impl::register_constraint (
      ::Monitor::NameList const &names,
      char const *cs,
      ::Monitor::Subscriber_ptr sub)
{
  ::Monitor::ConstraintStructList *constraintlist {};
  ACE_NEW_THROW_EX (constraintlist,
                    ::Monitor::ConstraintStructList (names.length ()),
                    CORBA::NO_MEMORY ());
  std::unique_ptr<::Monitor::ConstraintStructList> constraintlist_owner (constraintlist);
  constraintlist->length (0UL);

  MC_ADMINMANAGER * const mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  for (CORBA::ULong index {}; index < names.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      TAO_Monitor_Point_Ptr monitor (
        mgr->admin ().monitor_point (names[index]));

      if (monitor != nullptr)
        {
          TAO_Control_Action_Ptr ca (new TAO_Control_Action (sub));
          long const id = monitor->add_constraint (cs, ca.get ());
          CORBA::ULong const length = constraintlist->length ();
          constraintlist->length (length + 1);
          (*constraintlist)[length].id = id;
          (*constraintlist)[length].itemname =
            CORBA::string_dup (names[index].in ());
        }
    }

  return constraintlist_owner.release ();
}

void
Monitor_Impl::unregister_constraints (
  ::Monitor::ConstraintStructList const &constraint)
{
  MC_ADMINMANAGER * const mgr =
    ACE_Dynamic_Service<MC_ADMINMANAGER>::instance ("MC_ADMINMANAGER");

  for (CORBA::ULong index {}; index < constraint.length (); ++index)
    {
      /// Call on the administrator class to look up the desired monitors.
      TAO_Monitor_Point_Ptr monitor (
        mgr->admin ().monitor_point (constraint[index].itemname.in ()));

      if (monitor != nullptr)
        {
          TAO_Control_Action_Ptr action (
            monitor->remove_constraint (constraint[index].id));
        }
    }
}

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

