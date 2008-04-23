// $Id$
#include "orbsvcs/Notify/MonitorControl/NotificationServiceMonitor_i.h"
#include "orbsvcs/Notify/MonitorControl/Statistic_Registry.h"
#include "orbsvcs/Notify/MonitorControl/Control_Registry.h"
#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

NotificationServiceMonitor_i::NotificationServiceMonitor_i (CORBA::ORB_ptr orb)
 : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CosNotification::NotificationServiceMonitorControl::NameList*
NotificationServiceMonitor_i::get_statistic_names (void)
{
  CosNotification::NotificationServiceMonitorControl::NameList* names = 0;
  ACE_NEW_RETURN (names,
                  CosNotification::NotificationServiceMonitorControl::NameList (
                    TAO_Statistic_Registry::instance ()->names ()),
                  0);
  return names;
}

CosNotification::NotificationServiceMonitorControl::Data*
NotificationServiceMonitor_i::get_statistic (const char* name)
{
  TAO_Statistic_Registry* registry = TAO_Statistic_Registry::instance ();

  CosNotification::NotificationServiceMonitorControl::NameList invalid;
  CosNotification::NotificationServiceMonitorControl::NameList names (1);
  names.length (1);
  names[0] = name;
  this->get_invalid_names (registry, names, invalid);

  if (invalid.length () > 0)
    throw CosNotification::NotificationServiceMonitorControl::InvalidName (invalid);

  CosNotification::NotificationServiceMonitorControl::Data* data = 0;
  ACE_NEW_THROW_EX (data,
                    CosNotification::NotificationServiceMonitorControl::Data,
                    CORBA::NO_MEMORY ());
  this->get_data (registry, name, *data);
  return data;
}

CosNotification::NotificationServiceMonitorControl::DataList*
NotificationServiceMonitor_i::get_statistics (const CosNotification::NotificationServiceMonitorControl::NameList& names)
{
  TAO_Statistic_Registry* registry = TAO_Statistic_Registry::instance ();

  CosNotification::NotificationServiceMonitorControl::NameList invalid;
  this->get_invalid_names (registry, names, invalid);
  if (invalid.length () > 0)
    throw CosNotification::NotificationServiceMonitorControl::InvalidName (invalid);

  CORBA::ULong length = names.length();
  CosNotification::NotificationServiceMonitorControl::DataList* data = 0;
  ACE_NEW_RETURN (data,
                  CosNotification::NotificationServiceMonitorControl::DataList (
                    length),
                  0);
  ACE_Auto_Basic_Ptr<CosNotification::NotificationServiceMonitorControl::DataList>
    safe_data (data);

  data->length (length);
  for(CORBA::ULong i = 0; i < length; i++)
    {
      this->get_data (registry, names[i], (*data)[i]);
    }
  return safe_data.release ();
}

CosNotification::NotificationServiceMonitorControl::DataList*
NotificationServiceMonitor_i::get_and_clear_statistics (const CosNotification::NotificationServiceMonitorControl::NameList& names)
{
  CosNotification::NotificationServiceMonitorControl::DataList* data =
    this->get_statistics (names);

  // We've gotten to the point where we have a list of data.
  // It is technically possible for the list of statistics names
  // to change at this point.  So, I'm bypassing the call to
  // clear_statistics() to avoid a possible exception.
  CORBA::ULong length = names.length();
  TAO_Statistic_Registry* registry = TAO_Statistic_Registry::instance ();
  for(CORBA::ULong i = 0; i < length; i++)
    {
      TAO_Statistic* statistic = registry->get (names[i].in ());
      if (statistic != 0)
        statistic->clear();
    }

  return data;
}

void
NotificationServiceMonitor_i::clear_statistics (const CosNotification::NotificationServiceMonitorControl::NameList& names)
{
  TAO_Statistic_Registry* registry = TAO_Statistic_Registry::instance ();

  CosNotification::NotificationServiceMonitorControl::NameList invalid;
  this->get_invalid_names (registry, names, invalid);
  if (invalid.length () > 0)
    throw CosNotification::NotificationServiceMonitorControl::InvalidName (invalid);

  CORBA::ULong length = names.length();
  for(CORBA::ULong i = 0; i < length; i++)
    {
      TAO_Statistic* statistic = registry->get (names[i].in ());
      if (statistic != 0)
        statistic->clear();
    }
}

void
NotificationServiceMonitor_i::shutdown_event_channel (const char* name)
{
  this->send_control_command (name, TAO_NS_CONTROL_SHUTDOWN);
}

void
NotificationServiceMonitor_i::remove_consumer (const char* name)
{
  this->send_control_command (name, TAO_NS_CONTROL_REMOVE_CONSUMER);
}

void
NotificationServiceMonitor_i::remove_supplier (const char* name)
{
  this->send_control_command (name, TAO_NS_CONTROL_REMOVE_SUPPLIER);
}

void
NotificationServiceMonitor_i::remove_consumeradmin (const char* name)
{
  this->send_control_command (name, TAO_NS_CONTROL_REMOVE_CONSUMERADMIN);
}

void
NotificationServiceMonitor_i::remove_supplieradmin (const char* name)
{
  this->send_control_command (name, TAO_NS_CONTROL_REMOVE_SUPPLIERADMIN);
}

void
NotificationServiceMonitor_i::send_control_command (const char* name,
                                                    const char* cmd)
{
  TAO_Control_Registry* instance = TAO_Control_Registry::instance ();
  TAO_NS_Control* control = instance->get (name);

  // If we didn't find a control object with the given name, or the
  // execution of the control object failed, we must throw an exception.
  // The control object execution should only return false when the
  // command given does not correspond to one that is supported by the
  // control object.
  if (control == 0 || !control->execute (cmd))
    {
      CosNotification::NotificationServiceMonitorControl::NameList invalid (1);
      invalid.length (1);
      invalid[0] = name; 
      throw CosNotification::NotificationServiceMonitorControl::InvalidName (invalid);
    }
}

void
NotificationServiceMonitor_i::shutdown (void)
{
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown ();
}

void
NotificationServiceMonitor_i::get_data (
                     TAO_Statistic_Registry* registry,
                     const char* name,
                     CosNotification::NotificationServiceMonitorControl::Data& data)
{
  // Get the statistic by name
  TAO_Statistic* statistic = registry->get (name);

  if (statistic == 0)
    {
      // At the time that the list of names were acquired, this
      // statistic was available.  However, we have failed to
      // retrieve it.  The only reason that could happen is if
      // some one removed the statistic from the registry.
      CosNotification::NotificationServiceMonitorControl::Numeric num;
      num.count = 0;
      num.average = 0;
      num.sum_of_squares = 0;
      num.minimum = 0;
      num.maximum = 0;
      num.last = 0;
      data.num (num);
    }
  else
    {
      // If it's not a counter, we need to make sure that we have
      // the most up-to-date information.  A counter will always have
      // the correct value.
      if (statistic->type () != TAO_Statistic::TS_COUNTER)
        statistic->calculate ();

      // Populate the data structure based on the type of statistic
      if (statistic->type () == TAO_Statistic::TS_LIST)
        {
          TAO_Statistic::List slist (statistic->get_list ());
          CORBA::ULong size = static_cast<CORBA::ULong> (slist.size ());
          CosNotification::NotificationServiceMonitorControl::NameList
            list (size);
          list.length (size);
          for(CORBA::ULong i = 0; i < size; i++)
            {
              list[i] = CORBA::string_dup (slist[i].c_str ());
            }
          data.list (list);
        }
      else
        {
          CosNotification::NotificationServiceMonitorControl::Numeric num;
          num.count = static_cast<CORBA::ULong> (statistic->count ());
          num.minimum = statistic->minimum_sample ();
          num.maximum = statistic->maximum_sample ();
          num.last = statistic->last_sample ();

          if (statistic->type() == TAO_Statistic::TS_COUNTER)
            {
              num.average = 0;
              num.sum_of_squares = 0;
            }
          else
            {
              num.average = statistic->average ();
              num.sum_of_squares = statistic->sum_of_squares();
            }
          data.num (num);
        }
    }
}

void
NotificationServiceMonitor_i::get_invalid_names (
           TAO_Statistic_Registry* registry,
           const CosNotification::NotificationServiceMonitorControl::NameList& names,
           CosNotification::NotificationServiceMonitorControl::NameList& invalid)
{
  invalid.length (0);

  CORBA::ULong ilength = 0;
  CORBA::ULong length  = names.length ();
  for(CORBA::ULong i = 0; i < length; i++)
    {
      if (registry->get (names[i].in ()) == 0)
        {
          invalid.length (ilength + 1);
          invalid[ilength++] = CORBA::string_dup (names[i]);
        }
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
