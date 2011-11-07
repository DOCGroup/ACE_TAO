// $Id$

#include "orbsvcs/Notify/MonitorControlExt/MonitorEventChannelFactory.h"
#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtC.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorEventChannel.h"
#include "orbsvcs/Notify/MonitorControl/Dynamic_Statistic.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Builder.h"

#include "ace/Monitor_Point_Registry.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// ******************************************************************
// Dynamic Statistic Classes
// ******************************************************************

class EventChannels
  : public TAO_Dynamic_Statistic<TAO_MonitorEventChannelFactory>
{
public:
  EventChannels (TAO_MonitorEventChannelFactory* ecf,
                 const ACE_CString& name,
                 Monitor_Control_Types::Information_Type type,
                 bool active)
    : TAO_Dynamic_Statistic<TAO_MonitorEventChannelFactory> (ecf,
                                                             name.c_str (),
                                                             type),
    active_ (active)
  {
  }

  virtual void update (void)
  {
    if (this->type () == Monitor_Control_Types::MC_LIST)
      {
        Monitor_Control_Types::NameList names;
        this->interf_->get_ecs (&names, this->active_);
        this->receive (names);
      }
    else
      {
        this->receive (this->interf_->get_ecs (0, this->active_));
      }
  }

private:
  bool active_;
};

// ******************************************************************
// TAO_MonitorEventChannelFactory Methods
// ******************************************************************

TAO_MonitorEventChannelFactory::TAO_MonitorEventChannelFactory (
  const char* name)
  : name_ (name)
{
  if (name != 0)
    {
      ACE_CString dir_name (this->name_ + "/");
      ACE_CString stat_name (dir_name +
                             NotifyMonitoringExt::ActiveEventChannelCount);

      EventChannels* event_channels = 0;
      ACE_NEW (event_channels,
               EventChannels (this,
                              stat_name,
                              Monitor_Control_Types::MC_NUMBER,
                              true));

      event_channels->add_to_registry ();
      event_channels->remove_ref ();

      this->stat_names_.push_back (stat_name);

      stat_name = dir_name + NotifyMonitoringExt::InactiveEventChannelCount;
      event_channels = 0;
      ACE_NEW (event_channels,
               EventChannels (this,
                              stat_name,
                              Monitor_Control_Types::MC_NUMBER,
                              false));

      event_channels->add_to_registry ();
      event_channels->remove_ref ();

      this->stat_names_.push_back (stat_name);

      stat_name = dir_name + NotifyMonitoringExt::ActiveEventChannelNames;
      event_channels = 0;
      ACE_NEW (event_channels,
               EventChannels (this,
                              stat_name,
                              Monitor_Control_Types::MC_LIST,
                              true));

      event_channels->add_to_registry ();
      event_channels->remove_ref ();

      this->stat_names_.push_back (stat_name);

      stat_name = dir_name + NotifyMonitoringExt::InactiveEventChannelNames;
      event_channels = 0;
      ACE_NEW (event_channels,
               EventChannels (this,
                              stat_name,
                              Monitor_Control_Types::MC_LIST,
                              false));

      event_channels->add_to_registry ();
      event_channels->remove_ref ();

      this->stat_names_.push_back (stat_name);

      stat_name = dir_name + NotifyMonitoringExt::EventChannelCreationTime;
      Monitor_Base* timestamp = 0;
      ACE_NEW (timestamp,
               Timestamp_Monitor (stat_name.c_str ()));

      ACE_Time_Value tv (ACE_OS::gettimeofday());
      timestamp->receive (tv.sec () + (tv.usec () / 1000000.0));

      timestamp->add_to_registry ();
      timestamp->remove_ref ();

      this->stat_names_.push_back (stat_name);

      Monitor_Point_Registry* instance =
        Monitor_Point_Registry::instance ();

      ACE_WRITE_GUARD (TAO_SYNCH_RW_MUTEX, guard, this->mutex_);

      Monitor_Base* names =
        instance->get (NotifyMonitoringExt::EventChannelFactoryNames);

      if (names == 0)
        {
          stat_name = NotifyMonitoringExt::EventChannelFactoryNames;
          ACE_NEW_THROW_EX (names,
                            Monitor_Base (stat_name.c_str (),
                                          Monitor_Control_Types::MC_LIST),
                            CORBA::NO_MEMORY ());
          names->add_to_registry ();
        }

      Monitor_Control_Types::NameList list = names->get_list ();
      list.push_back (this->name_);
      names->receive (list);
      names->remove_ref ();
    }
}

TAO_MonitorEventChannelFactory::~TAO_MonitorEventChannelFactory (void)
{
  Monitor_Point_Registry* instance = Monitor_Point_Registry::instance ();
  size_t size = this->stat_names_.size ();

  for (size_t i = 0; i < size; ++i)
    {
      instance->remove (this->stat_names_[i].c_str ());
    }
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_MonitorEventChannelFactory::create_named_channel (
   const CosNotification::QoSProperties& initial_qos,
   const CosNotification::AdminProperties& initial_admin,
   CosNotifyChannelAdmin::ChannelID_out id,
   const char* name)
{
  if (ACE_OS::strlen (name) == 0)
    {
      throw NotifyMonitoringExt::NameMapError ();
    }

  ACE_CString sname (this->name_ + "/");
  sname += name;

  ACE_WRITE_GUARD_RETURN (TAO_SYNCH_RW_MUTEX, guard, this->mutex_, 0);

  // Make sure the name isn't already used.
  if (this->map_.find (sname) == 0)
    {
      throw NotifyMonitoringExt::NameAlreadyUsed ();
    }

  CosNotifyChannelAdmin::EventChannel_var ec =
    TAO_Notify_Properties::instance ()->builder ()->build_event_channel (
      this,
      initial_qos,
      initial_admin,
      id,
      sname.c_str ());

  if (CORBA::is_nil (ec.in ()))
    {
      return CosNotifyChannelAdmin::EventChannel::_nil ();
    }

  // Event channel creation was successful, try to bind it in our map.
  if (this->map_.bind (sname, id) != 0)
    {
      throw NotifyMonitoringExt::NameMapError ();
    }

  // The destructor of this object will unbind the
  // name from the map unless release is called.
  Unbinder unbinder (this->map_, sname);

  this->self_change ();

  unbinder.release ();
  return ec._retn ();
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_MonitorEventChannelFactory::create_channel (
  const CosNotification::QoSProperties& initial_qos,
  const CosNotification::AdminProperties& initial_admin,
  CosNotifyChannelAdmin::ChannelID_out id)
{
  CosNotifyChannelAdmin::EventChannel_var ec =
    this->TAO_Notify_EventChannelFactory::create_channel (initial_qos,
                                                          initial_admin,
                                                          id);

  if (CORBA::is_nil (ec.in ()))
    {
      return CosNotifyChannelAdmin::EventChannel::_nil ();
    }

  // Make sure we can get down to the real event channel
  TAO_MonitorEventChannel* mec =
    dynamic_cast<TAO_MonitorEventChannel*> (ec->_servant ());

  if (mec == 0)
    {
      throw CORBA::INTERNAL ();
    }

  // Construct the name using the new id
  ACE_CString sname (this->name_ + "/");
  char name[64];
  ACE_OS::sprintf(name, "%d", id);
  sname += name;

  ACE_WRITE_GUARD_RETURN (TAO_SYNCH_RW_MUTEX, guard, this->mutex_, 0);

  // Make sure the name isn't already used.
  if (this->map_.find (sname) == 0)
    throw NotifyMonitoringExt::NameAlreadyUsed ();

  // Event channel creation was successful, try to bind it in our map
  if (this->map_.bind (sname, id) != 0)
    {
      throw NotifyMonitoringExt::NameMapError ();
    }

  // Set the name and statistics on the event channel.
  mec->add_stats (sname.c_str ());

  return ec._retn ();
}

void
TAO_MonitorEventChannelFactory::remove (TAO_Notify_EventChannel* channel)
{
  TAO_MonitorEventChannel* mec =
    dynamic_cast<TAO_MonitorEventChannel*> (channel);

  if (mec != 0)
    {
      ACE_WRITE_GUARD (TAO_SYNCH_RW_MUTEX, guard, this->mutex_);
      this->map_.unbind (mec->name ());
    }

  this->TAO_Notify_EventChannelFactory::remove (channel);
}

size_t
TAO_MonitorEventChannelFactory::get_suppliers (
  CosNotifyChannelAdmin::ChannelID id)
{
  size_t count = 0;
  CosNotifyChannelAdmin::EventChannel_var ec =
    this->get_event_channel (id);

  if (!CORBA::is_nil (ec.in ()))
    {
      CosNotifyChannelAdmin::AdminIDSeq_var supadmin_ids =
        ec->get_all_supplieradmins ();
      CORBA::ULong length = supadmin_ids->length ();

      for (CORBA::ULong j = 0; j < length; ++j)
        {
          CosNotifyChannelAdmin::SupplierAdmin_var admin =
            ec->get_supplieradmin (supadmin_ids[j]);

          if (!CORBA::is_nil (admin.in ()))
            {
              CosNotifyChannelAdmin::ProxyIDSeq_var proxys =
                admin->push_consumers ();
              count += proxys->length ();
            }
        }
    }

  return count;
}

size_t
TAO_MonitorEventChannelFactory::get_consumers (
  CosNotifyChannelAdmin::ChannelID id)
{
  size_t count = 0;
  CosNotifyChannelAdmin::EventChannel_var ec =
    this->get_event_channel (id);

  if (!CORBA::is_nil (ec.in ()))
    {
      CosNotifyChannelAdmin::AdminIDSeq_var conadmin_ids =
        ec->get_all_consumeradmins ();
      CORBA::ULong length = conadmin_ids->length ();

      for (CORBA::ULong j = 0; j < length; ++j)
        {
          CosNotifyChannelAdmin::ConsumerAdmin_var admin =
            ec->get_consumeradmin (conadmin_ids[j]);

          if (!CORBA::is_nil (admin.in ()))
            {
              CosNotifyChannelAdmin::ProxyIDSeq_var proxys =
                admin->push_suppliers ();
              count += proxys->length ();
            }
        }
    }

  return count;
}

size_t
TAO_MonitorEventChannelFactory::get_ecs (
  Monitor_Control_Types::NameList* names,
  bool active)
{
  size_t count = 0;
  CosNotifyChannelAdmin::ChannelIDSeq_var ids = this->get_all_channels ();

  CORBA::ULong total = ids->length ();

  for (CORBA::ULong i = 0; i < total; ++i)
    {
      CosNotifyChannelAdmin::ChannelID id = ids[i];
      bool want_event_channel = !active;

      // Check for connected consumers.
      size_t consumers = this->get_consumers (id);

      if (consumers > 0)
        {
          want_event_channel = active;
        }

      if ((!active && want_event_channel)
          || (active && !want_event_channel))
        {
          // Check for connected suppliers
          size_t suppliers = this->get_suppliers (id);

          if (suppliers > 0)
            {
              want_event_channel = active;
            }
        }

      if (want_event_channel)
        {
          count++;

          if (names != 0)
            {
              ACE_READ_GUARD_RETURN (TAO_SYNCH_RW_MUTEX,
                                     guard,
                                     this->mutex_,
                                     0);

              Map::iterator itr (this->map_);
              Map::value_type* entry = 0;

              while (itr.next (entry))
                {
                  if (id == entry->item ())
                    {
                      names->push_back (entry->key ());
                    }

                  itr.advance ();
                }
            }
        }
    }

  return count;
}

// ******************************************************************
// Unbinder Helper
// ******************************************************************

TAO_MonitorEventChannelFactory::Unbinder::Unbinder (
  TAO_MonitorEventChannelFactory::Map& map,
  const ACE_CString& name)
  : map_ (map),
     name_ (name),
     released_ (false)
{
}

TAO_MonitorEventChannelFactory::Unbinder::~Unbinder (void)
{
  if (!this->released_)
    {
      this->map_.unbind (this->name_);
    }
}

void
TAO_MonitorEventChannelFactory::Unbinder::release (void)
{
  this->released_ = true;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

