// $Id$
#include "orbsvcs/Notify/MonitorControlExt/MonitorEventChannel.h"
#include "orbsvcs/Notify/MonitorControlExt/NotifyMonitoringExtS.h"
#include "orbsvcs/Notify/MonitorControl/Statistic_Registry.h"
#include "orbsvcs/Notify/MonitorControl/Dynamic_Statistic.h"
#include "orbsvcs/Notify/MonitorControl/Control_Registry.h"
#include "orbsvcs/Notify/Buffering_Strategy.h"
#include "orbsvcs/Notify/ConsumerAdmin.h"
#include "orbsvcs/Notify/ThreadPool_Task.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// ******************************************************************
// Dynamic Statistic Classes
// ******************************************************************

class EventChannelConsumers:
  public TAO_Dynamic_Statistic<TAO_MonitorEventChannel>
{
public:
  EventChannelConsumers (TAO_MonitorEventChannel* ec,
                         const ACE_CString& name,
                         TAO_Statistic::Information_Type type)
  : TAO_Dynamic_Statistic<TAO_MonitorEventChannel> (ec,
                                                    name.c_str (),
                                                    type) {
  }
  virtual void calculate (void) {
    if (this->type () == TAO_Statistic::TS_LIST)
      {
        TAO_Statistic::List list;
        this->interf_->get_consumers (&list);
        this->receive (list);
      }
    else
      this->receive (static_cast<double> (this->interf_->get_consumers (0)));
  }
};
class EventChannelSuppliers:
  public TAO_Dynamic_Statistic<TAO_MonitorEventChannel>
{
public:
  EventChannelSuppliers (TAO_MonitorEventChannel* ec,
                         const ACE_CString& name,
                         TAO_Statistic::Information_Type type)
  : TAO_Dynamic_Statistic<TAO_MonitorEventChannel> (ec,
                                                    name.c_str (),
                                                    type) {
  }
  virtual void calculate (void) {
    if (this->type () == TAO_Statistic::TS_LIST)
      {
        TAO_Statistic::List list;
        this->interf_->get_suppliers (&list);
        this->receive (list);
      }
    else
      this->receive (static_cast<double> (this->interf_->get_suppliers (0)));
  }
};
class QueuedEvents:
  public TAO_Dynamic_Statistic<TAO_MonitorEventChannel>
{
public:
  QueuedEvents (TAO_MonitorEventChannel* ec,
                const ACE_CString& name,
                TAO_Statistic::Information_Type type,
                bool count)
  : TAO_Dynamic_Statistic<TAO_MonitorEventChannel> (ec,
                                                    name.c_str (),
                                                    type),
    count_ (count) {
  }

  virtual void calculate (void) {
    this->receive (
      static_cast<double> (
        this->interf_->calculate_queue_size (this->count_)));
  }

private:
  bool count_;
};
class OldestEvent:
  public TAO_Dynamic_Statistic<TAO_MonitorEventChannel>
{
public:
  OldestEvent (TAO_MonitorEventChannel* ec,
               const ACE_CString& name,
               TAO_Statistic::Information_Type type)
  : TAO_Dynamic_Statistic<TAO_MonitorEventChannel> (ec,
                                                    name.c_str (),
                                                    type) {
  }

  virtual void calculate (void) {
    this->receive (this->interf_->get_oldest_event ());
  }
};
class SlowestConsumers:
  public TAO_Dynamic_Statistic<TAO_MonitorEventChannel>
{
public:
  SlowestConsumers (TAO_MonitorEventChannel* ec,
                    const ACE_CString& name,
                    TAO_Statistic::Information_Type type)
  : TAO_Dynamic_Statistic<TAO_MonitorEventChannel> (ec,
                                                    name.c_str (),
                                                    type) {
  }
  virtual void calculate (void) {
    TAO_Statistic::List list;
    this->interf_->determine_slowest_consumer (&list);
    this->receive (list);
  }
};
class ShutdownControl: public TAO_NS_Control
{
public:
  ShutdownControl (TAO_MonitorEventChannel* ec,
                   const ACE_CString& name)
  : TAO_NS_Control (name.c_str ()),
    ec_ (ec) {
  }

  virtual void execute (const char* command) {
    if (ACE_OS::strcmp (command, TAO_NS_CONTROL_SHUTDOWN) == 0)
      {
        this->ec_->destroy ();
      }
  }

private:
  TAO_MonitorEventChannel* ec_;
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
  TAO_Statistic_Registry* instance =
    TAO_Statistic_Registry::instance ();
  size_t size = this->stat_names_.size ();
  for(size_t i = 0; i < size; i++)
    {
      instance->remove (this->stat_names_[i]);
    }

  TAO_Control_Registry* cinstance =
    TAO_Control_Registry::instance ();
  size = this->control_names_.size ();
  for(size_t i = 0; i < size; i++)
    {
      cinstance->remove (this->control_names_[i]);
    }
}

const ACE_CString&
TAO_MonitorEventChannel::name (void) const
{
  return this->name_;
}

void
TAO_MonitorEventChannel::map_supplier_proxy (
                                    CosNotifyChannelAdmin::ProxyID id,
                                    const ACE_CString& name)
{
  if (name.length () == 0)
    throw NotifyMonitoringExt::NameMapError ();

  ACE_WRITE_GUARD (ACE_SYNCH_RW_MUTEX, guard, this->supplier_mutex_);
  if (this->is_duplicate_name (this->supplier_map_, name))
    throw NotifyMonitoringExt::NameAlreadyUsed ();

  int status = this->supplier_map_.bind (id, name);
  if (status != 0)
    throw NotifyMonitoringExt::NameMapError ();
}

void
TAO_MonitorEventChannel::map_consumer_proxy (
                                    CosNotifyChannelAdmin::ProxyID id,
                                    const ACE_CString& name)
{
  if (name.length () == 0)
    throw NotifyMonitoringExt::NameMapError ();

  ACE_WRITE_GUARD (ACE_SYNCH_RW_MUTEX, guard, this->consumer_mutex_);
  if (this->is_duplicate_name (this->consumer_map_, name))
    throw NotifyMonitoringExt::NameAlreadyUsed ();

  int status = this->consumer_map_.bind (id, name);
  if (status != 0)
    throw NotifyMonitoringExt::NameMapError ();
}

void
TAO_MonitorEventChannel::cleanup_proxy (CosNotifyChannelAdmin::ProxyID id
                                        , bool is_supplier)
{
  if (is_supplier) {
    this->unmap_supplier_proxy (id);
  }
  else {
    this->unmap_consumer_proxy (id);
  }
}

void
TAO_MonitorEventChannel::unmap_supplier_proxy
(CosNotifyChannelAdmin::ProxyID id)
{
  if (this->supplier_map_.unbind (id) != 0) {
    throw NotifyMonitoringExt::NameMapError ();
  }
}

void
TAO_MonitorEventChannel::unmap_consumer_proxy
(CosNotifyChannelAdmin::ProxyID id)
{
  if (this->consumer_map_.unbind (id) != 0) {
    throw NotifyMonitoringExt::NameMapError ();
  }
}

void
TAO_MonitorEventChannel::add_stats (const char* name)
{
  if (name != 0 && this->name_.length () == 0)
    this->name_ = name;

  if (this->name_.length () != 0)
    {
      TAO_Statistic_Registry* instance =
        TAO_Statistic_Registry::instance ();

      ACE_CString dir_name (this->name_ + "/");
      ACE_CString stat_name = dir_name +
                              NotifyMonitoringExt::EventChannelCreationTime;
      TAO_Statistic* timestamp = 0;
      ACE_NEW_THROW_EX (timestamp,
                        TAO_Statistic (stat_name.c_str (),
                                       TAO_Statistic::TS_TIME),
                        CORBA::NO_MEMORY ());
      ACE_Time_Value tv (ACE_OS::gettimeofday());
      timestamp->receive (tv.sec () + (tv.usec () / 1000000.0));
      bool added = instance->add (timestamp);
      if (added)
        this->stat_names_.push_back (stat_name);
      else
        {
          delete timestamp;
          ACE_ERROR ((LM_ERROR, "Unable to add statistic: %s\n",
                      stat_name.c_str ()));
        }

      stat_name = dir_name +
                  NotifyMonitoringExt::EventChannelConsumerCount;
      EventChannelConsumers* consumers = 0;
      ACE_NEW_THROW_EX (consumers,
                        EventChannelConsumers (this,
                                               stat_name.c_str (),
                                               TAO_Statistic::TS_NUMBER),
                        CORBA::NO_MEMORY ());
      added = instance->add (consumers);
      if (added)
        this->stat_names_.push_back (stat_name);
      else
        {
          delete consumers;
          ACE_ERROR ((LM_ERROR, "Unable to add statistic: %s\n",
                      stat_name.c_str ()));
        }

      stat_name = dir_name +
                  NotifyMonitoringExt::EventChannelConsumerNames;
      consumers = 0;
      ACE_NEW_THROW_EX (consumers,
                        EventChannelConsumers (this,
                                               stat_name.c_str (),
                                               TAO_Statistic::TS_LIST),
                        CORBA::NO_MEMORY ());
      added = instance->add (consumers);
      if (added)
        this->stat_names_.push_back (stat_name);
      else
        {
          delete consumers;
          ACE_ERROR ((LM_ERROR, "Unable to add statistic: %s\n",
                      stat_name.c_str ()));
        }

      stat_name = dir_name +
                  NotifyMonitoringExt::EventChannelSupplierCount;
      EventChannelSuppliers* suppliers = 0;
      ACE_NEW_THROW_EX (suppliers,
                        EventChannelSuppliers (this,
                                               stat_name.c_str (),
                                               TAO_Statistic::TS_NUMBER),
                        CORBA::NO_MEMORY ());
      added = instance->add (suppliers);
      if (added)
        this->stat_names_.push_back (stat_name);
      else
        {
          delete suppliers;
          ACE_ERROR ((LM_ERROR, "Unable to add statistic: %s\n",
                      stat_name.c_str ()));
        }

      stat_name = dir_name +
                  NotifyMonitoringExt::EventChannelSupplierNames;
      suppliers = 0;
      ACE_NEW_THROW_EX (suppliers,
                        EventChannelSuppliers (this,
                                               stat_name.c_str (),
                                               TAO_Statistic::TS_LIST),
                        CORBA::NO_MEMORY ());
      added = instance->add (suppliers);
      if (added)
        this->stat_names_.push_back (stat_name);
      else
        {
          delete suppliers;
          ACE_ERROR ((LM_ERROR, "Unable to add statistic: %s\n",
                      stat_name.c_str ()));
        }

      stat_name = dir_name + NotifyMonitoringExt::EventChannelQueueSize;
      QueuedEvents* events = 0;
      ACE_NEW_THROW_EX (events,
                        QueuedEvents (this, stat_name.c_str (),
                                      TAO_Statistic::TS_NUMBER, false),
                        CORBA::NO_MEMORY ());
      added = instance->add (events);
      if (added)
        this->stat_names_.push_back (stat_name);
      else
        {
          delete events;
          ACE_ERROR ((LM_ERROR, "Unable to add statistic: %s\n",
                      stat_name.c_str ()));
        }

      stat_name = dir_name +
                  NotifyMonitoringExt::EventChannelQueueElementCount;
      events = 0;
      ACE_NEW_THROW_EX (events,
                        QueuedEvents (this, stat_name.c_str (),
                                      TAO_Statistic::TS_NUMBER, true),
                        CORBA::NO_MEMORY ());
      added = instance->add (events);
      if (added)
        this->stat_names_.push_back (stat_name);
      else
        {
          delete events;
          ACE_ERROR ((LM_ERROR, "Unable to add statistic: %s\n",
                      stat_name.c_str ()));
        }

      stat_name = dir_name + NotifyMonitoringExt::EventChannelOldestEvent;
      OldestEvent* oldest = 0;
      ACE_NEW_THROW_EX (oldest,
                        OldestEvent (this, stat_name.c_str (),
                                     TAO_Statistic::TS_TIME),
                        CORBA::NO_MEMORY ());
      added = instance->add (oldest);
      if (added)
        this->stat_names_.push_back (stat_name);
      else
        {
          delete oldest;
          ACE_ERROR ((LM_ERROR, "Unable to add statistic: %s\n",
                      stat_name.c_str ()));
        }

      stat_name = dir_name +
                  NotifyMonitoringExt::EventChannelSlowestConsumers;
      SlowestConsumers* slowest = 0;
      ACE_NEW_THROW_EX (slowest,
                        SlowestConsumers (this, stat_name.c_str (),
                                          TAO_Statistic::TS_LIST),
                        CORBA::NO_MEMORY ());
      added = instance->add (slowest);
      if (added)
        this->stat_names_.push_back (stat_name);
      else
        {
          delete oldest;
          ACE_ERROR ((LM_ERROR, "Unable to add statistic: %s\n",
                      stat_name.c_str ()));
        }

      TAO_Control_Registry* cinstance =
        TAO_Control_Registry::instance ();

      ShutdownControl* sd = 0;
      ACE_NEW_THROW_EX (sd,
                        ShutdownControl (this,
                                         this->name_),
                        CORBA::NO_MEMORY ());
      added = cinstance->add (sd);
      if (added)
        this->control_names_.push_back (this->name_);
      else
        {
          delete sd;
          ACE_ERROR ((LM_ERROR, "Unable to add control: %s\n",
                      this->name_.c_str ()));
        }
    }
}

size_t
TAO_MonitorEventChannel::get_consumers (TAO_Statistic::List* names)
{
  size_t count = 0;
  CosNotifyChannelAdmin::AdminIDSeq_var conadmin_ids =
    this->get_all_consumeradmins ();
  CORBA::ULong length = conadmin_ids->length ();
  for(CORBA::ULong j = 0; j < length; j++)
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
              ACE_READ_GUARD_RETURN (ACE_SYNCH_RW_MUTEX, guard,
                                     this->supplier_mutex_, 0);

              for(CORBA::ULong i = 0; i < plen; i++)
                {
                  if (names == 0)
                    {
                      if (this->supplier_map_.find (proxys[i]) == 0)
                        count++;
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

size_t
TAO_MonitorEventChannel::get_suppliers (TAO_Statistic::List* names)
{
  size_t count = 0;
  CosNotifyChannelAdmin::AdminIDSeq_var supadmin_ids =
    this->get_all_supplieradmins ();
  CORBA::ULong length = supadmin_ids->length ();
  for(CORBA::ULong j = 0; j < length; j++)
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
              ACE_READ_GUARD_RETURN (ACE_SYNCH_RW_MUTEX, guard,
                                     this->consumer_mutex_, 0);

              for(CORBA::ULong i = 0; i < plen; i++)
                {
                  if (names == 0)
                    {
                      if (this->consumer_map_.find (proxys[i]) == 0)
                        count++;
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
          return dynamic_cast<TAO_Notify_ThreadPool_Task*> (
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
  for(CORBA::ULong j = 0; j < length; j++)
    {
      TAO_Notify_ThreadPool_Task* task =
        this->get_threadpool_task (conadmin_ids[j]);
      if (task != 0)
        {
          TAO_Notify_Message_Queue* queue =
            task->msg_queue ();
          if (count)
            size += queue->message_count ();
          else
            // The message blocks stored in this queue are of size
            // zero.  However, each message block is a
            // TAO_Notify_Event which has an associated set of data
            // which can be used to estimate the amount of memory
            // allocated to the message queue
            size += (queue->message_count () * sizeof (TAO_Notify_Event));
        }
    }

  return size;
}

void
TAO_MonitorEventChannel::determine_slowest_consumer (
                                         TAO_Statistic::List* names)
{
  size_t largest = 0;
  CosNotifyChannelAdmin::AdminID id = 0;

  CosNotifyChannelAdmin::AdminIDSeq_var conadmin_ids =
    this->get_all_consumeradmins ();
  CORBA::ULong length = conadmin_ids->length ();
  for(CORBA::ULong j = 0; j < length; j++)
    {
      TAO_Notify_ThreadPool_Task* task =
        this->get_threadpool_task (conadmin_ids[j]);
      if (task != 0)
        {
          TAO_Notify_Message_Queue* queue =
            task->msg_queue ();
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
              ACE_READ_GUARD (ACE_SYNCH_RW_MUTEX,
                              guard, this->supplier_mutex_);
              for(CORBA::ULong i = 0; i < plen; i++)
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
  ACE_Time_Value tv (ACE_Time_Value::max_time);
  CosNotifyChannelAdmin::AdminIDSeq_var conadmin_ids =
    this->get_all_consumeradmins ();
  CORBA::ULong length = conadmin_ids->length ();
  for(CORBA::ULong j = 0; j < length; j++)
    {
      TAO_Notify_ThreadPool_Task* task =
        this->get_threadpool_task (conadmin_ids[j]);
      if (task != 0)
        {
          ACE_Time_Value old =
            task->buffering_strategy ()->oldest_event ();
          if (old < tv)
            tv = old;
        }
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

TAO_END_VERSIONED_NAMESPACE_DECL
