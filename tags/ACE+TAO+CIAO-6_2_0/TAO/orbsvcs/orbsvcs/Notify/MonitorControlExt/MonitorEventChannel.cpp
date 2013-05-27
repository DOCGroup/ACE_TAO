// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorEventChannel.h"

#include "ace/Monitor_Point_Registry.h"

#include "orbsvcs/Notify/MonitorControlExt/MonitorConsumerAdmin.h"
#include "orbsvcs/Notify/MonitorControlExt/MonitorSupplierAdmin.h"
#include "orbsvcs/Notify/MonitorControl/Dynamic_Statistic.h"
#include "orbsvcs/Notify/MonitorControl/Timestamp_Monitor.h"
#include "orbsvcs/Notify/MonitorControl/Control_Registry.h"
#include "orbsvcs/Notify/Buffering_Strategy.h"
#include "orbsvcs/Notify/ProxySupplier.h"
#include "orbsvcs/Notify/ProxyConsumer.h"
#include "orbsvcs/Notify/ThreadPool_Task.h"

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// ******************************************************************
// Dynamic Statistic Classes
// ******************************************************************

class EventChannelConsumersSuppliers
  : public TAO_Dynamic_Statistic<TAO_MonitorEventChannel>
{
public:
  EventChannelConsumersSuppliers (TAO_MonitorEventChannel* ec,
                                  const ACE_CString& name,
                                  Monitor_Control_Types::Information_Type type,
                                  bool is_supplier = false)
    : TAO_Dynamic_Statistic<TAO_MonitorEventChannel> (ec,
                                                      name.c_str (),
                                                      type),
      is_supplier_ (is_supplier)
  {
  }

  virtual void update (void) {
    if (this->type () == Monitor_Control_Types::MC_LIST)
      {
        Monitor_Control_Types::NameList list;
        if (this->is_supplier_)
          this->interf_->get_suppliers (&list);
        else
          this->interf_->get_consumers (&list);
        this->receive (list);
      }
    else
      {
        if (this->is_supplier_)
          this->receive (this->interf_->get_suppliers (0));
        else
          this->receive (this->interf_->get_consumers (0));
      }
  }

private:
  bool is_supplier_;
};

class EventChannelTimedoutConsumers
  : public TAO_Dynamic_Statistic<TAO_MonitorEventChannel>
{
public:
  EventChannelTimedoutConsumers (TAO_MonitorEventChannel* ec,
                                 const ACE_CString& name,
                                 Monitor_Control_Types::Information_Type type)
    : TAO_Dynamic_Statistic<TAO_MonitorEventChannel> (ec,
                                                      name.c_str (),
                                                      type)
  {
  }

  virtual void update (void) {
    Monitor_Control_Types::NameList list;
    this->interf_->get_timedout_consumers (&list);
    this->receive (list);
  }
};

class EventChannelConsumerSupplierAdmins
  : public TAO_Dynamic_Statistic<TAO_MonitorEventChannel>
{
public:
  EventChannelConsumerSupplierAdmins (TAO_MonitorEventChannel* ec,
                                      const ACE_CString& name,
                                      Monitor_Control_Types::Information_Type type,
                                      bool is_supplier = false)
    : TAO_Dynamic_Statistic<TAO_MonitorEventChannel> (ec,
                                                      name.c_str (),
                                                      type),
    is_supplier_ (is_supplier)
  {
  }

  virtual void update (void)
  {
    if (this->type () == Monitor_Control_Types::MC_LIST)
      {
        Monitor_Control_Types::NameList list;

        if (this->is_supplier_)
          {
            this->interf_->get_supplieradmins (&list);
          }
        else
          {
            this->interf_->get_consumeradmins (&list);
          }

        this->receive (list);
      }
    else
      {
        if (this->is_supplier_)
          {
            this->receive (this->interf_->get_supplieradmins (0));
          }
        else
          {
            this->receive (this->interf_->get_consumeradmins (0));
          }
      }
  }

private:
  bool is_supplier_;
};

class QueuedEvents
  : public TAO_Dynamic_Statistic<TAO_MonitorEventChannel>
{
public:
  QueuedEvents (TAO_MonitorEventChannel* ec,
                const ACE_CString& name,
                Monitor_Control_Types::Information_Type type,
                bool count)
    : TAO_Dynamic_Statistic<TAO_MonitorEventChannel> (ec,
                                                      name.c_str (),
                                                      type),
      count_ (count)
  {
  }

  virtual void update (void)
  {
    this->receive (this->interf_->calculate_queue_size (this->count_));
  }

private:
  bool count_;
};

class OldestEvent
  : public TAO_Dynamic_Statistic<TAO_MonitorEventChannel>
{
public:
  OldestEvent (TAO_MonitorEventChannel* ec,
               const ACE_CString& name,
               Monitor_Control_Types::Information_Type type)
    : TAO_Dynamic_Statistic<TAO_MonitorEventChannel> (ec,
                                                      name.c_str (),
                                                      type)
  {
  }

  virtual void update (void)
  {
    this->receive (this->interf_->get_oldest_event ());
  }
};

class SlowestConsumers
  : public TAO_Dynamic_Statistic<TAO_MonitorEventChannel>
{
public:
  SlowestConsumers (TAO_MonitorEventChannel* ec,
                    const ACE_CString& name,
                    Monitor_Control_Types::Information_Type type)
    : TAO_Dynamic_Statistic<TAO_MonitorEventChannel> (ec,
                                                      name.c_str (),
                                                      type)
  {
  }

  virtual void update (void)
  {
    Monitor_Control_Types::NameList list;
    this->interf_->determine_slowest_consumer (&list);
    this->receive (list);
  }
};

class QueueOverflows:
  public Monitor_Base
{
public:
  QueueOverflows (const ACE_CString& name)
  : Monitor_Base (
      name.c_str (),
      Monitor_Control_Types::MC_COUNTER) {
  }
};

class ShutdownControl : public TAO_NS_Control
{
public:
  ShutdownControl (TAO_MonitorEventChannel* ec,
                   const ACE_CString& name)
    : TAO_NS_Control (name.c_str ()),
      ec_ (ec)
  {
  }

  virtual bool execute (const char* command)
  {
    if (ACE_OS::strcmp (command, TAO_NS_CONTROL_SHUTDOWN) == 0)
      {
        this->ec_->destroy ();
      }
    else
      {
        return false;
      }

    return true;
  }

private:
  TAO_MonitorEventChannel* ec_;
};

class RemoveConsumerSupplierControl : public TAO_NS_Control
{
public:
  RemoveConsumerSupplierControl (TAO_MonitorEventChannel* ec,
                                 const ACE_CString& name,
                                 CosNotifyChannelAdmin::ProxyID id,
                                 bool is_supplier)
    : TAO_NS_Control (name.c_str ()),
      ec_ (ec),
      id_ (id),
      is_supplier_ (is_supplier)
  {
  }

  virtual bool execute (const char* command)
  {
    bool status = true;

    if (this->is_supplier_)
      {
        if (ACE_OS::strcmp (command, TAO_NS_CONTROL_REMOVE_SUPPLIER) == 0)
          {
            status = this->ec_->destroy_supplier (this->id_);
          }
        else
          {
            // Indicate that the command was unknown.
            return false;
          }
      }
    else
      {
        if (ACE_OS::strcmp (command, TAO_NS_CONTROL_REMOVE_CONSUMER) == 0)
          {
            status = this->ec_->destroy_consumer (this->id_);
          }
        else
          {
            // Indicate that the command was unknown.
            return false;
          }
      }

    return status;
  }

private:
  TAO_MonitorEventChannel* ec_;
  CosNotifyChannelAdmin::ProxyID id_;
  bool is_supplier_;
};

// ******************************************************************
// TAO_MonitorEventChannel Methods
// ******************************************************************

TAO_MonitorEventChannel::TAO_MonitorEventChannel (const char* name)
  : name_ (name)
{
  this->add_stats ();
}

TAO_MonitorEventChannel::~TAO_MonitorEventChannel (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->names_mutex_);

  Monitor_Point_Registry* instance = Monitor_Point_Registry::instance ();
  size_t size = this->stat_names_.size ();

  for (size_t i = 0; i < size; ++i)
    {
      instance->remove (this->stat_names_[i].c_str ());
    }

  TAO_Control_Registry* cinstance = TAO_Control_Registry::instance ();
  size = this->control_names_.size ();

  for (size_t i = 0; i < size; ++i)
    {
      cinstance->remove (this->control_names_[i]);
    }
}

const ACE_CString&
TAO_MonitorEventChannel::name (void) const
{
  return this->name_;
}

bool
TAO_MonitorEventChannel::register_statistic (const ACE_CString& name,
                                             Monitor_Base* stat)
{
  // Add the statistic to the registry and
  // add the name to the statistic names list.
  bool added = Monitor_Point_Registry::instance ()->add (stat);

  if (added)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->names_mutex_, added);

      this->stat_names_.push_back (name);
    }

  return added;
}

bool
TAO_MonitorEventChannel::unregister_statistic (const ACE_CString& name)
{
  // Remove the statistic from the registry and if it's successful
  // remove the name from the statistic names list.
  bool removed = Monitor_Point_Registry::instance ()->remove (name.c_str ());

  if (removed)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->names_mutex_, removed);

      this->remove_list_name (this->stat_names_, name);
    }

  return removed;
}

void
TAO_MonitorEventChannel::map_supplier_proxy (
  CosNotifyChannelAdmin::ProxyID id,
  const ACE_CString& name)
{
  if (name.length () == 0)
    {
      throw NotifyMonitoringExt::NameMapError ();
    }

  ACE_CString full = this->name_ + "/" + name;

  ACE_WRITE_GUARD (TAO_SYNCH_RW_MUTEX, guard, this->supplier_mutex_);

  if (this->is_duplicate_name (this->supplier_map_, full))
    {
      throw NotifyMonitoringExt::NameAlreadyUsed ();
    }

  if (this->supplier_map_.bind (id, full) != 0)
    {
      throw NotifyMonitoringExt::NameMapError ();
    }

  // The view from the outside sees a supplier proxy as a consumer.
  // So, we'll register a RemoveConsumerSupplierControl for each supplier
  // proxy indicating a consumer (false)
  RemoveConsumerSupplierControl* rcsc = 0;
  ACE_NEW_THROW_EX (rcsc,
                    RemoveConsumerSupplierControl (this, full, id, false),
                    CORBA::NO_MEMORY ());

  TAO_Control_Registry* cinstance = TAO_Control_Registry::instance ();

  if (cinstance->add (rcsc))
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->names_mutex_);

      this->control_names_.push_back (full);
    }
  else
    {
      delete rcsc;
      ORBSVCS_ERROR ((LM_ERROR,
                  "Unable to add control: %s\n",
                  full.c_str ()));
    }
}

void
TAO_MonitorEventChannel::map_consumer_proxy (
  CosNotifyChannelAdmin::ProxyID id,
  const ACE_CString& name)
{
  if (name.length () == 0)
    {
      throw NotifyMonitoringExt::NameMapError ();
    }

  ACE_CString full = this->name_ + "/" + name;

  ACE_WRITE_GUARD (TAO_SYNCH_RW_MUTEX, guard, this->consumer_mutex_);

  if (this->is_duplicate_name (this->consumer_map_, full))
    {
      throw NotifyMonitoringExt::NameAlreadyUsed ();
    }

  if (this->consumer_map_.bind (id, full) != 0)
    {
      throw NotifyMonitoringExt::NameMapError ();
    }

  // The view from the outside sees a consumer proxy as a supplier.
  // So, we'll register a RemoveConsumerSupplierControl for each consumer
  // proxy indicating a supplier (true)
  RemoveConsumerSupplierControl* rcsc = 0;
  ACE_NEW_THROW_EX (rcsc,
                    RemoveConsumerSupplierControl (this, full, id, true),
                    CORBA::NO_MEMORY ());

  TAO_Control_Registry* cinstance = TAO_Control_Registry::instance ();

  if (cinstance->add (rcsc))
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->names_mutex_);

      this->control_names_.push_back (full);
    }
  else
    {
      delete rcsc;
      ORBSVCS_ERROR ((LM_ERROR,
                  "Unable to add control: %s\n",
                  full.c_str ()));
    }
}

void
TAO_MonitorEventChannel::cleanup_proxy (CosNotifyChannelAdmin::ProxyID id,
                                        bool is_supplier,
                                        bool experienced_timeout)
{
  ACE_CString name;

  if (is_supplier)
    {
      ACE_WRITE_GUARD (TAO_SYNCH_RW_MUTEX, guard, this->supplier_mutex_);

      // It may seem like a good idea to throw a
      // NotifyMonitoringExt::NameMapError() exception if we can't unbind the
      // id from the map, but we can't.  This method is called indirectly
      // from CORBA calls (e.g., disconnect_structured_push_supplier) and at
      // best it will cause a CORBA::UNKNOWN exception and at worst will
      // cause an unexpected exception call which will cause the program to
      // abort.
      this->supplier_map_.unbind (id, name);

      // If this supplier proxy is being cleaned up because it
      // experienced a timeout, we need to add it to the timeout supplier
      // map.
      if (experienced_timeout && name.length () != 0)
        {
          ACE_WRITE_GUARD (ACE_SYNCH_RW_MUTEX, tguard,
                           this->timedout_supplier_mutex_);
          this->timedout_supplier_map_.bind (id, name);
        }
    }
  else
    {
      ACE_WRITE_GUARD (TAO_SYNCH_RW_MUTEX, guard, this->consumer_mutex_);

      // The same goes for this one too.
      this->consumer_map_.unbind (id, name);
    }

  if (name.length () != 0)
    {
      TAO_Control_Registry* cinstance = TAO_Control_Registry::instance ();
      cinstance->remove (name);

      ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->names_mutex_);

      this->remove_list_name (this->control_names_, name);
    }
}

void
TAO_MonitorEventChannel::remove_consumeradmin (
  CosNotifyChannelAdmin::ProxyID id)
{
  ACE_WRITE_GUARD (TAO_SYNCH_RW_MUTEX, guard, this->consumeradmin_mutex_);

  this->consumeradmin_map_.unbind (id);
}

void
TAO_MonitorEventChannel::remove_supplieradmin (
  CosNotifyChannelAdmin::ProxyID id)
{
  ACE_WRITE_GUARD (TAO_SYNCH_RW_MUTEX, guard, this->supplieradmin_mutex_);

  this->supplieradmin_map_.unbind (id);
}

void
TAO_MonitorEventChannel::add_stats (const char* name)
{
  if (name != 0 && this->name_.length () == 0)
    {
      this->name_ = name;
    }

  if (this->name_.length () != 0)
    {
      ACE_CString dir_name (this->name_ + "/");
      ACE_CString stat_name = dir_name +
                              NotifyMonitoringExt::EventChannelCreationTime;
      Monitor_Base* timestamp = 0;
      ACE_NEW_THROW_EX (timestamp,
                        Timestamp_Monitor (stat_name.c_str ()),
                        CORBA::NO_MEMORY ());
      ACE_Time_Value tv (ACE_OS::gettimeofday ());
      timestamp->receive (tv.sec () + (tv.usec () / 1000000.0));

      if (!this->register_statistic (stat_name, timestamp))
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Unable to add statistic %s\n",
                      stat_name.c_str ()));
        }

      // Registry manages refcount, so we do this regardless.
      timestamp->remove_ref ();

      stat_name = dir_name
                  + NotifyMonitoringExt::EventChannelConsumerCount;
      EventChannelConsumersSuppliers* consumers = 0;
      ACE_NEW_THROW_EX (consumers,
                        EventChannelConsumersSuppliers (
                          this,
                          stat_name.c_str (),
                          Monitor_Control_Types::MC_NUMBER),
                        CORBA::NO_MEMORY ());

      if (!this->register_statistic (stat_name, consumers))
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Unable to add statistic %s\n",
                      stat_name.c_str ()));
        }

      // Registry manages refcount, so we do this regardless.
      consumers->remove_ref ();

      stat_name = dir_name
                  + NotifyMonitoringExt::EventChannelConsumerNames;
      consumers = 0;
      ACE_NEW_THROW_EX (consumers,
                        EventChannelConsumersSuppliers (
                          this,
                          stat_name.c_str (),
                          Monitor_Control_Types::MC_LIST),
                        CORBA::NO_MEMORY ());

      if (!this->register_statistic (stat_name, consumers))
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Unable to add statistic %s\n",
                      stat_name.c_str ()));
        }

      // Registry manages refcount, so we do this regardless.
      consumers->remove_ref ();

      stat_name = dir_name
                  + NotifyMonitoringExt::EventChannelTimedoutConsumerNames;
      EventChannelTimedoutConsumers* tconsumers = 0;
      ACE_NEW_THROW_EX (tconsumers,
                        EventChannelTimedoutConsumers (
                          this,
                          stat_name.c_str (),
                          Monitor_Control_Types::MC_LIST),
                        CORBA::NO_MEMORY ());

      if (!this->register_statistic (stat_name, tconsumers))
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Unable to add statistic %s\n",
                      stat_name.c_str ()));
        }

      // Registry manages refcount, so we do this regardless.
      tconsumers->remove_ref ();

      stat_name = dir_name
                  + NotifyMonitoringExt::EventChannelSupplierCount;
      EventChannelConsumersSuppliers* suppliers = 0;
      ACE_NEW_THROW_EX (suppliers,
                        EventChannelConsumersSuppliers (
                          this,
                          stat_name.c_str (),
                          Monitor_Control_Types::MC_NUMBER,
                          true),
                        CORBA::NO_MEMORY ());

      if (!this->register_statistic (stat_name, suppliers))
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Unable to add statistic %s\n",
                      stat_name.c_str ()));
        }

      // Registry manages refcount, so we do this regardless.
      suppliers->remove_ref ();

      stat_name = dir_name
                  + NotifyMonitoringExt::EventChannelSupplierNames;
      suppliers = 0;
      ACE_NEW_THROW_EX (suppliers,
                        EventChannelConsumersSuppliers (
                          this,
                          stat_name.c_str (),
                          Monitor_Control_Types::MC_LIST,
                          true),
                        CORBA::NO_MEMORY ());

      if (!this->register_statistic (stat_name, suppliers))
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Unable to add statistic %s\n",
                      stat_name.c_str ()));
        }

      // Registry manages refcount, so we do this regardless.
      suppliers->remove_ref ();

      stat_name = dir_name
                  + NotifyMonitoringExt::EventChannelConsumerAdminCount;
      EventChannelConsumerSupplierAdmins* conadmins = 0;
      ACE_NEW_THROW_EX (conadmins,
                        EventChannelConsumerSupplierAdmins (
                          this,
                          stat_name.c_str (),
                          Monitor_Control_Types::MC_NUMBER),
                        CORBA::NO_MEMORY ());

      if (!this->register_statistic (stat_name, conadmins))
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Unable to add statistic %s\n",
                      stat_name.c_str ()));
        }

      // Registry manages refcount, so we do this regardless.
      conadmins->remove_ref ();

      stat_name = dir_name
                  + NotifyMonitoringExt::EventChannelConsumerAdminNames;
      conadmins = 0;
      ACE_NEW_THROW_EX (conadmins,
                        EventChannelConsumerSupplierAdmins (
                          this,
                          stat_name.c_str (),
                          Monitor_Control_Types::MC_LIST),
                        CORBA::NO_MEMORY ());

      if (!this->register_statistic (stat_name, conadmins))
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Unable to add statistic %s\n",
                      stat_name.c_str ()));
        }

      // Registry manages refcount, so we do this regardless.
      conadmins->remove_ref ();

      stat_name = dir_name
                  + NotifyMonitoringExt::EventChannelSupplierAdminCount;
      EventChannelConsumerSupplierAdmins* supadmins = 0;
      ACE_NEW_THROW_EX (supadmins,
                        EventChannelConsumerSupplierAdmins (
                          this,
                          stat_name.c_str (),
                          Monitor_Control_Types::MC_NUMBER,
                          true),
                        CORBA::NO_MEMORY ());

      if (!this->register_statistic (stat_name, supadmins))
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Unable to add statistic %s\n",
                      stat_name.c_str ()));
        }

      // Registry manages refcount, so we do this regardless.
      supadmins->remove_ref ();

      stat_name = dir_name
                  + NotifyMonitoringExt::EventChannelSupplierAdminNames;
      supadmins = 0;
      ACE_NEW_THROW_EX (supadmins,
                        EventChannelConsumerSupplierAdmins (
                          this,
                          stat_name.c_str (),
                          Monitor_Control_Types::MC_LIST,
                          true),
                        CORBA::NO_MEMORY ());

      if (!this->register_statistic (stat_name, supadmins))
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Unable to add statistic %s\n",
                      stat_name.c_str ()));
        }

      // Registry manages refcount, so we do this regardless.
      supadmins->remove_ref ();

      stat_name = dir_name + NotifyMonitoringExt::EventChannelQueueSize;
      QueuedEvents* events = 0;
      ACE_NEW_THROW_EX (events,
                        QueuedEvents (this,
                                      stat_name.c_str (),
                                      Monitor_Control_Types::MC_NUMBER,
                                      false),
                        CORBA::NO_MEMORY ());

      if (!this->register_statistic (stat_name, events))
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Unable to add statistic %s\n",
                      stat_name.c_str ()));
        }

      // Registry manages refcount, so we do this regardless.
      events->remove_ref ();

      stat_name = dir_name
                  + NotifyMonitoringExt::EventChannelQueueElementCount;
      events = 0;
      ACE_NEW_THROW_EX (events,
                        QueuedEvents (this,
                                      stat_name.c_str (),
                                      Monitor_Control_Types::MC_NUMBER,
                                      true),
                        CORBA::NO_MEMORY ());

      events->add_to_registry ();
      events->remove_ref ();

      stat_name = dir_name + NotifyMonitoringExt::EventChannelOldestEvent;
      OldestEvent* oldest = 0;
      ACE_NEW_THROW_EX (oldest,
                        OldestEvent (this, stat_name.c_str (),
                                     Monitor_Control_Types::MC_TIME),
                        CORBA::NO_MEMORY ());

      if (!this->register_statistic (stat_name, oldest))
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Unable to add statistic %s\n",
                      stat_name.c_str ()));
        }

      // Registry manages refcount, so we do this regardless.
      oldest->remove_ref ();

      stat_name = dir_name
                  + NotifyMonitoringExt::EventChannelSlowestConsumers;
      SlowestConsumers* slowest = 0;
      ACE_NEW_THROW_EX (slowest,
                        SlowestConsumers (this,
                                          stat_name.c_str (),
                                          Monitor_Control_Types::MC_LIST),
                        CORBA::NO_MEMORY ());

      if (!this->register_statistic (stat_name, slowest))
        {
          ORBSVCS_ERROR ((LM_ERROR,
                      "Unable to add statistic %s\n",
                      stat_name.c_str ()));
        }

      // Registry manages refcount, so we do this regardless.
      slowest->remove_ref ();

      stat_name = dir_name +
                  NotifyMonitoringExt::EventChannelQueueOverflows;
      QueueOverflows* overflows = 0;
      ACE_NEW_THROW_EX (overflows,
                        QueueOverflows (stat_name.c_str ()),
                        CORBA::NO_MEMORY ());
      if (!this->register_statistic (stat_name, overflows))
        {
          delete overflows;
          ORBSVCS_ERROR ((LM_ERROR, "Unable to add statistic: %s\n",
                      stat_name.c_str ()));
        }

      TAO_Control_Registry* cinstance =
        TAO_Control_Registry::instance ();

      ShutdownControl* sd = 0;
      ACE_NEW_THROW_EX (sd,
                        ShutdownControl (this,
                                         this->name_),
                        CORBA::NO_MEMORY ());

      if (cinstance->add (sd))
        {
          ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->names_mutex_);
          this->control_names_.push_back (this->name_);
        }
      else
        {
          delete sd;
          ORBSVCS_ERROR ((LM_ERROR,
                      "Unable to add control: %s\n",
                      this->name_.c_str ()));
        }
    }
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_MonitorEventChannel::new_for_consumers (
  CosNotifyChannelAdmin::InterFilterGroupOperator op,
  CosNotifyChannelAdmin::AdminID_out id)
{
  return this->named_new_for_consumers (op, id, 0);
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_MonitorEventChannel::named_new_for_consumers (
  CosNotifyChannelAdmin::InterFilterGroupOperator op,
  CosNotifyChannelAdmin::AdminID_out id,
  const char* name)
{
  if (name != 0 && name[0] == 0)
    {
      throw NotifyMonitoringExt::NameMapError ();
    }

  CosNotifyChannelAdmin::ConsumerAdmin_var admin =
    this->TAO_Notify_EventChannel::new_for_consumers (op, id);

  // WARNING: Internal knowledge of TAO.
  TAO_MonitorConsumerAdmin* low_admin =
    dynamic_cast<TAO_MonitorConsumerAdmin*> (admin->_servant ());

  if (low_admin == 0)
    {
      // This shouldn't happen.
      throw CORBA::INTERNAL ();
    }
  else
    {
      // Build up the full name.
      ACE_CString full = this->name_ + "/";

      if (name == 0)
        {
          char idname[64];
          ACE_OS::sprintf (idname, "%d", id);
          full += idname;
        }
      else
        {
          full += name;
        }

      ACE_WRITE_GUARD_RETURN (TAO_SYNCH_RW_MUTEX, guard,
                              this->consumeradmin_mutex_,
                              CosNotifyChannelAdmin::ConsumerAdmin::_nil ());

      if (this->is_duplicate_name (this->consumeradmin_map_, full))
        {
          throw NotifyMonitoringExt::NameAlreadyUsed ();
        }

      if (this->consumeradmin_map_.bind (id, full) != 0)
        {
          throw NotifyMonitoringExt::NameMapError ();
        }

      low_admin->register_stats_controls (this, full);
    }

  return admin._retn ();
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_MonitorEventChannel::new_for_suppliers (
  CosNotifyChannelAdmin::InterFilterGroupOperator op,
  CosNotifyChannelAdmin::AdminID_out id)
{
  return this->named_new_for_suppliers (op, id, 0);
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_MonitorEventChannel::named_new_for_suppliers (
  CosNotifyChannelAdmin::InterFilterGroupOperator op,
  CosNotifyChannelAdmin::AdminID_out id,
  const char* name)
{
  if (name != 0 && name[0] == 0)
    {
      throw NotifyMonitoringExt::NameMapError ();
    }

  CosNotifyChannelAdmin::SupplierAdmin_var admin =
    this->TAO_Notify_EventChannel::new_for_suppliers (op, id);

  // WARNING: Internal knowledge of TAO.
  TAO_MonitorSupplierAdmin* low_admin =
    dynamic_cast<TAO_MonitorSupplierAdmin*> (admin->_servant ());

  if (low_admin == 0)
    {
      // This shouldn't happen
      throw CORBA::INTERNAL ();
    }
  else
    {
      // Build up the full name
      ACE_CString full = this->name_ + "/";

      if (name == 0)
        {
          char idname[64];
          ACE_OS::sprintf (idname, "%d", id);
          full += idname;
        }
      else
        {
          full += name;
        }

      ACE_WRITE_GUARD_RETURN (TAO_SYNCH_RW_MUTEX, guard,
                              this->supplieradmin_mutex_,
                              CosNotifyChannelAdmin::SupplierAdmin::_nil ());

      if (this->is_duplicate_name (this->supplieradmin_map_, full))
        {
          throw NotifyMonitoringExt::NameAlreadyUsed ();
        }

      if (this->supplieradmin_map_.bind (id, full) != 0)
        {
          throw NotifyMonitoringExt::NameMapError ();
        }

      low_admin->register_stats_controls (this, full);
    }

  return admin._retn ();
}

size_t
TAO_MonitorEventChannel::get_consumers (
  Monitor_Control_Types::NameList* names)
{
  size_t count = 0;
  CosNotifyChannelAdmin::AdminIDSeq_var conadmin_ids =
    this->get_all_consumeradmins ();
  CORBA::ULong length = conadmin_ids->length ();

  for (CORBA::ULong j = 0; j < length; ++j)
    {
      CosNotifyChannelAdmin::ConsumerAdmin_var admin =
        this->get_consumeradmin (conadmin_ids[j]);

      if (!CORBA::is_nil (admin.in ()))
        {
          CosNotifyChannelAdmin::ProxyIDSeq_var proxys =
            admin->push_suppliers ();
          CORBA::ULong plen = proxys->length ();

          if (plen > 0)
            {
              ACE_READ_GUARD_RETURN (TAO_SYNCH_RW_MUTEX,
                                     guard,
                                     this->supplier_mutex_,
                                     0);

              for (CORBA::ULong i = 0; i < plen; ++i)
                {
                  if (names == 0)
                    {
                      if (this->supplier_map_.find (proxys[i]) == 0)
                        {
                          count++;
                        }
                    }
                  else
                    {
                      ACE_CString name;

                      if (this->supplier_map_.find (proxys[i], name) == 0)
                        {
                          count++;
                          names->push_back (name);
                        }
                    }
                }
            }
        }
    }

  return count;
}

void
TAO_MonitorEventChannel::get_timedout_consumers (
                           Monitor_Control_Types::NameList* names)
{
  ACE_READ_GUARD (ACE_SYNCH_RW_MUTEX, guard, this->timedout_supplier_mutex_);

  Map::const_iterator itr (this->timedout_supplier_map_);
  Map::value_type* entry = 0;
  while (itr.next (entry))
    {
      names->push_back (entry->item ());
      itr.advance ();
    }
}

size_t
TAO_MonitorEventChannel::get_suppliers (
  Monitor_Control_Types::NameList* names)
{
  size_t count = 0;
  CosNotifyChannelAdmin::AdminIDSeq_var supadmin_ids =
    this->get_all_supplieradmins ();
  CORBA::ULong length = supadmin_ids->length ();

  for (CORBA::ULong j = 0; j < length; ++j)
    {
      CosNotifyChannelAdmin::SupplierAdmin_var admin =
        this->get_supplieradmin (supadmin_ids[j]);

      if (!CORBA::is_nil (admin.in ()))
        {
          CosNotifyChannelAdmin::ProxyIDSeq_var proxys =
            admin->push_consumers ();
          CORBA::ULong plen = proxys->length ();

          if (plen > 0)
            {
              ACE_READ_GUARD_RETURN (TAO_SYNCH_RW_MUTEX,
                                     guard,
                                     this->consumer_mutex_,
                                     0);

              for (CORBA::ULong i = 0; i < plen; ++i)
                {
                  if (names == 0)
                    {
                      if (this->consumer_map_.find (proxys[i]) == 0)
                        {
                          count++;
                        }
                    }
                  else
                    {
                      ACE_CString name;

                      if (this->consumer_map_.find (proxys[i], name) == 0)
                        {
                          count++;
                          names->push_back (name);
                        }
                    }
                }
            }
        }
    }

  return count;
}

size_t
TAO_MonitorEventChannel::get_consumeradmins (
  Monitor_Control_Types::NameList* names)
{
  ACE_READ_GUARD_RETURN (TAO_SYNCH_RW_MUTEX,
                         guard,
                         this->consumeradmin_mutex_,
                         0);

  CosNotifyChannelAdmin::AdminIDSeq_var admin_ids =
    this->get_all_consumeradmins ();

  return this->get_admins (this->consumeradmin_map_,
                           admin_ids.in (),
                           names);
}

size_t
TAO_MonitorEventChannel::get_supplieradmins (
  Monitor_Control_Types::NameList* names)
{
  ACE_READ_GUARD_RETURN (TAO_SYNCH_RW_MUTEX,
                         guard,
                         this->supplieradmin_mutex_,
                         0);

  CosNotifyChannelAdmin::AdminIDSeq_var admin_ids =
    this->get_all_supplieradmins ();

  return this->get_admins (this->supplieradmin_map_,
                           admin_ids.in (),
                           names);
}

size_t
TAO_MonitorEventChannel::get_admins (
  TAO_MonitorEventChannel::Map& map,
  const CosNotifyChannelAdmin::AdminIDSeq& admin_ids,
  Monitor_Control_Types::NameList* names)
{
  size_t count = 0;
  CORBA::ULong length = admin_ids.length ();

  for (CORBA::ULong j = 0; j < length; ++j)
    {
      if (names == 0)
        {
          if (map.find (admin_ids[j]) == 0)
            {
              count++;
            }
        }
      else
        {
          ACE_CString name;

          if (map.find (admin_ids[j], name) == 0)
            {
              count++;
              names->push_back (name);
            }
        }
    }

  return count;
}

TAO_Notify_ThreadPool_Task*
TAO_MonitorEventChannel::get_threadpool_task (
  CosNotifyChannelAdmin::AdminID id)
{
  CosNotifyChannelAdmin::ConsumerAdmin_var admin =
    this->get_consumeradmin (id);

  if (!CORBA::is_nil (admin.in ()))
    {
      // WARNING: Internal knowledge of TAO and the Notification
      // Service implementation.
      TAO_Notify_ConsumerAdmin* low_admin =
        dynamic_cast<TAO_Notify_ConsumerAdmin*> (admin->_servant ());

      if (low_admin != 0)
        {
          return
            dynamic_cast<TAO_Notify_ThreadPool_Task*> (
              low_admin->get_worker_task ());
        }
    }

  return 0;
}

size_t
TAO_MonitorEventChannel::calculate_queue_size (bool count)
{
  size_t size = 0;
  CosNotifyChannelAdmin::AdminIDSeq_var conadmin_ids =
    this->get_all_consumeradmins ();
  CORBA::ULong length = conadmin_ids->length ();

  for (CORBA::ULong j = 0; j < length; j++)
    {
      TAO_Notify_ThreadPool_Task* task =
        this->get_threadpool_task (conadmin_ids[j]);

      if (task != 0)
        {
          TAO_Notify_Message_Queue* queue = task->msg_queue ();

          if (count)
            {
              size += queue->message_count ();
            }
          else
            {
              // The message blocks stored in this queue are of size
              // zero.  However, each message block is a
              // TAO_Notify_Event which has an associated set of data
              // which can be used to estimate the amount of memory
              // allocated to the message queue
              size += (queue->message_count () /* * sizeof (TAO_Notify_Event) */);
            }
        }
    }

  return size;
}

void
TAO_MonitorEventChannel::determine_slowest_consumer (
  Monitor_Control_Types::NameList* names)
{
  size_t largest = 0;
  CosNotifyChannelAdmin::AdminID id = 0;

  CosNotifyChannelAdmin::AdminIDSeq_var conadmin_ids =
    this->get_all_consumeradmins ();
  CORBA::ULong length = conadmin_ids->length ();

  for (CORBA::ULong j = 0; j < length; ++j)
    {
      TAO_Notify_ThreadPool_Task* task =
        this->get_threadpool_task (conadmin_ids[j]);

      if (task != 0)
        {
          TAO_Notify_Message_Queue* queue = task->msg_queue ();
          size_t count = queue->message_count ();

          if (count > largest)
            {
              largest = count;
              id = conadmin_ids[j];
            }
        }
    }

  if (largest > 0)
    {
      CosNotifyChannelAdmin::ConsumerAdmin_var admin =
        this->get_consumeradmin (id);

      if (!CORBA::is_nil (admin.in ()))
        {
          CosNotifyChannelAdmin::ProxyIDSeq_var proxys =
            admin->push_suppliers ();
          CORBA::ULong plen = proxys->length ();

          if (plen > 0)
            {
              ACE_READ_GUARD (TAO_SYNCH_RW_MUTEX,
                              guard,
                              this->supplier_mutex_);

              for (CORBA::ULong i = 0; i < plen; ++i)
                {
                  ACE_CString name;

                  if (this->supplier_map_.find (proxys[i], name) == 0)
                    {
                      names->push_back (name);
                    }
                }
            }
        }
    }
}

double
TAO_MonitorEventChannel::get_oldest_event (void)
{
  CosNotifyChannelAdmin::AdminIDSeq_var conadmin_ids =
    this->get_all_consumeradmins ();
  CORBA::ULong length = conadmin_ids->length ();

  // If there are no consumers, we will return the special case indicator.
  if (length == 0)
    {
      return 0.0;
    }

  // Loop through each consumer admins thread pool task and have the
  // buffering strategy give us the time of the oldest event.
  ACE_Time_Value tv (ACE_Time_Value::max_time);

  for (CORBA::ULong j = 0; j < length; ++j)
    {
      TAO_Notify_ThreadPool_Task* task =
        this->get_threadpool_task (conadmin_ids[j]);

      if (task != 0)
        {
          ACE_Time_Value old =
            task->buffering_strategy ()->oldest_event ();

          if (old < tv)
            {
              tv = old;
            }
        }
    }

  // If there was no message in any of the queues, our time value will be
  // ACE_Time_Value::max_time.  For this situation, we must return the
  // special case indicator.
  if (tv == ACE_Time_Value::max_time)
    {
      return 0.0;
    }

  return tv.sec () + (tv.usec () / 1000000.0);
}

bool
TAO_MonitorEventChannel::is_duplicate_name (
  const TAO_MonitorEventChannel::Map& map,
  const ACE_CString& name) const
{
  Map::const_iterator itr (map);
  Map::value_type* entry = 0;

  while (itr.next (entry))
    {
      if (name == entry->item ())
        {
          return true;
        }

      itr.advance ();
    }

  return false;
}

void
TAO_MonitorEventChannel::remove_list_name (
  Monitor_Control_Types::NameList& list,
  const ACE_CString& name)
{
  size_t size = list.size ();

  for (size_t i = 0; i < size; ++i)
    {
      if (list[i] == name)
        {
          if (size == 1)
            {
              list.clear ();
            }
          else
            {
              if (i != size - 1)
                {
                  list[i] = list[size - 1];
                }

              list.resize (size - 1, "");
            }

          break;
        }
    }
}

bool
TAO_MonitorEventChannel::destroy_consumer (
  CosNotifyChannelAdmin::ProxyID id)
{
  // Since we just have a proxy id, we have to search through all of the
  // consumer admins.  We have no idea to which the proxy will belong.
  CosNotifyChannelAdmin::AdminIDSeq_var conadmin_ids =
    this->get_all_consumeradmins ();
  CORBA::ULong length = conadmin_ids->length ();

  for (CORBA::ULong j = 0; j < length; ++j)
    {
      CosNotifyChannelAdmin::ConsumerAdmin_var admin =
        this->get_consumeradmin (conadmin_ids[j]);

      if (!CORBA::is_nil (admin.in ()))
        {
          try
            {
              CosNotifyChannelAdmin::ProxySupplier_var supplier =
                            admin->get_proxy_supplier(id);

              if (!CORBA::is_nil(supplier.in()))
                {
                  // WARNING: Internal knowledge of TAO and the
                  // Notification Service implementation.
                  TAO_Notify_ProxySupplier* low_proxy =
                    dynamic_cast<TAO_Notify_ProxySupplier*> (
                      supplier->_servant ());

                  low_proxy->destroy ();
                  return true;
                }
            }
          catch(const CosNotifyChannelAdmin::ProxyNotFound&)
            {
            }
        }
    }

  return false;
}

bool
TAO_MonitorEventChannel::destroy_supplier (
  CosNotifyChannelAdmin::ProxyID id)
{
  // Since we just have a proxy id, we have to search through all of the
  // supplier admins.  We have no idea to which the proxy will belong.
  CosNotifyChannelAdmin::AdminIDSeq_var supadmin_ids =
    this->get_all_supplieradmins ();
  CORBA::ULong length = supadmin_ids->length ();

  for (CORBA::ULong j = 0; j < length; ++j)
    {
      CosNotifyChannelAdmin::SupplierAdmin_var admin =
        this->get_supplieradmin (supadmin_ids[j]);

      if (!CORBA::is_nil (admin.in ()))
        {
          try
            {
              CosNotifyChannelAdmin::ProxyConsumer_var consumer =
                            admin->get_proxy_consumer(id);

              if (!CORBA::is_nil(consumer.in()))
                {
                  // WARNING: Internal knowledge of TAO and the
                  // Notification Service implementation.
                  TAO_Notify_ProxyConsumer* low_proxy =
                    dynamic_cast<TAO_Notify_ProxyConsumer*> (
                      consumer->_servant ());

                  low_proxy->destroy ();
                  return true;
                }
            }
          catch(const CosNotifyChannelAdmin::ProxyNotFound&)
            {
            }
        }
    }

  return false;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

