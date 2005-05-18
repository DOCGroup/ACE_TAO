// $Id$

#ifndef KOKYU_EC_H
#define KOKYU_EC_H

#include "ace/SString.h"
#include "ace/Task_T.h"
#include "ace/Sched_Params.h"
#include "ace/Select_Reactor_Base.h" //for ACE_Select_Reactor_Impl::DEFAULT_SIZE
#include "ace/OS_NS_unistd.h" //for ACE_OS::sleep()
#include "ace/OS_NS_sys_stat.h" //for ACE_OS::filesize()
#include "ace/Event_Handler.h"
#include "ace/Time_Value.h"
#include "ace/Reactor.h"

#include "tao/Utils/Servant_Var.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Gateway_IIOP_Factory.h"
#include "orbsvcs/Event/EC_Gateway_Sched.h"
#include "orbsvcs/Scheduler_Factory.h"

#include "RtSchedEventChannelS.h"
#include "ECSupplier.h"
#include "ECConsumer.h"

#include <vector>

class ECDriver; //forward decl

struct subtask_t
{
  int task_index;
  int subtask_index;
  ACE_Time_Value exec;
  ACE_Time_Value period;
  ACE_Time_Value phase;
}; //struct subtask_t

struct task_trigger_t
{
  ECSupplier_Timeout_Handler *handler;
  ACE_Time_Value period;
  ACE_Time_Value phase;
}; //struct task_trigger_t

struct name_t
{
  int operator== (const name_t& rhs) const
  {
    return !strcmp(this->name,rhs.name);
  }
  const char* name;
}; //struct name_t

class Kokyu_EC : public POA_RtEventChannelAdmin::RtSchedEventChannel
{
public:
  typedef std::vector<RtEventChannelAdmin::Event_Type> EventType_Vector;
  typedef std::vector<RtEventChannelAdmin::SchedInfo> QoSVector;

  Kokyu_EC(void);

  virtual ~Kokyu_EC(void);

  ACE_INLINE static RtecScheduler::Period_t time_val_to_period (const ACE_Time_Value &tv)
  {
    //100s of nanoseconds
    return (tv.sec () * 1000000 + tv.usec ())*10;
  }

  int init(bool time_master, const char* schedule_discipline, PortableServer::POA_ptr poa, ACE_Reactor * reactor = 0);

  virtual RtEventChannelAdmin::handle_t register_consumer (
                                                           const char * entry_point,
                                                           const RtEventChannelAdmin::SchedInfo & info,
                                                           RtecEventComm::EventType type,
                                                           RtecEventComm::PushConsumer_ptr consumer,
                                                           RtecEventChannelAdmin::ProxyPushSupplier_out proxy_supplier
                                                           ACE_ENV_ARG_DECL
                                                           )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ));

  virtual RtEventChannelAdmin::handle_t register_supplier (
                                                           const char * entry_point,
                                                           RtecEventComm::EventSourceID source,
                                                           const EventType_Vector& supp_types,
                                                           RtecEventComm::PushSupplier_ptr supplier,
                                                           RtecEventChannelAdmin::ProxyPushConsumer_out proxy_consumer
                                                           ACE_ENV_ARG_DECL
                                                           )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ));


  virtual void add_dependency (
                               RtEventChannelAdmin::handle_t handle,
                               RtEventChannelAdmin::handle_t dependency,
                               CORBA::Long number_of_calls,
                               RtecScheduler::Dependency_Type_t dependency_type
                               ACE_ENV_ARG_DECL
                               )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     , RtecScheduler::UNKNOWN_TASK
                     ));

  virtual void start (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ));

  virtual void notify_gateway_connection (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void set_start_time (RtEventChannelAdmin::Time start_time ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual RtecEventChannelAdmin::EventChannel_ptr event_channel (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual RtecScheduler::Scheduler_ptr scheduler (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  ///// CONVENIENCE FUNCTIONS /////
  int remote_gateways_connected(void);

  ///Takes ownership of ECSupplier and ECTimeout_Consumer
  void add_supplier_with_timeout(
                                 ECSupplier * supplier_impl,
                                 const char * supp_entry_point,
                                 const EventType_Vector& supp_types,
                                 ECSupplier_Timeout_Handler * timeout_handler_impl,
                                 ACE_Time_Value phase,
                                 ACE_Time_Value period,
                                 RtecScheduler::Criticality_t crit,
                                 RtecScheduler::Importance_t imp
                                 ACE_ENV_ARG_DECL
                                 )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ));

  ///Takes ownership of ECSupplier
  void add_supplier(
                    ECSupplier * supplier_impl,
                    const char * entry_point,
                    const EventType_Vector& supp_types
                    ACE_ENV_ARG_DECL
                    )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ));

  ///Takes ownership of ECConsumer and ECSupplier
  void add_consumer_with_supplier(
                                  ECConsumer * consumer_impl,
                                  const char * cons_entry_point,
                                  const QoSVector& qos_infos,
                                  /*
                                  ACE_Time_Value cons_period,
                                  const EventType_Vector& cons_types,
                                  RtecScheduler::Criticality_t cons_crit,
                                  RtecScheduler::Importance_t cons_imp,
                                  */
                                  ECSupplier * supplier_impl,
                                  const char * supp_entry_point,
                                  const EventType_Vector& supp_types
                                  ACE_ENV_ARG_DECL
                                  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ));

  ///Takes ownership of ECConsumer
  void add_consumer(
                    ECConsumer * consumer_impl,
                    const char * entry_point,
                    const QoSVector& qos_infos
                    /*
                    ACE_Time_Value period,
                    const EventType_Vector& cons_types,
                    RtecScheduler::Criticality_t crit,
                    RtecScheduler::Importance_t imp
                    */
                    ACE_ENV_ARG_DECL
                    )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ));

  void init_gateway (CORBA::ORB_ptr orb,
                     PortableServer::POA_ptr poa,
                     const char* consumer_ec_ior,
                     const char* ior_output_filename
                     ACE_ENV_ARG_DECL);

  static void init_remote_ec (const char *consumer_ec_ior,
                              CORBA::ORB_ptr orb,
                              RtEventChannelAdmin::RtSchedEventChannel_out consumer_ec);

  bool time_master(void);

  ACE_Time_Value start_time(void);

  typedef std::vector<RtEventChannelAdmin::RtSchedEventChannel_var> ECVector;

  ECVector* remote_ecs(void);

  /// Sets the EC's event types and gives it ownership of the vector.
  void setEventTypes(EventType_Vector *ev);

  /// Returns the EC's event types
  const EventType_Vector* getEventTypes();

  void set_name(const char *name);
  const char *get_name (void);

private:
  TAO::Utils::Servant_Var<POA_RtecScheduler::Scheduler> scheduler_impl_;
  TAO::Utils::Servant_Var<TAO_EC_Event_Channel> ec_impl_;
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin_;
  RtecScheduler::Scheduler_var scheduler_;

  std::vector<ECSupplier*> suppliers_;
  //std::vector<ECSupplier_Timeout_Handler*> timeout_handlers_;
  std::vector<ECConsumer*> consumers_;
  std::vector<RtecEventComm::PushConsumer_var> consumer_proxies_;
  std::vector<RtecEventChannelAdmin::ProxyPushSupplier_var> push_suppliers_;
  std::vector<RtecEventComm::PushSupplier_var> supplier_proxies_;

  std::vector<task_trigger_t> task_triggers_; //also keeps track of timeout_handlers

  ACE_Reactor *reactor_;
  std::vector<long> timer_handles_;

  bool started_;
  int remote_gateways_connected_;

  bool time_master_;
  ACE_Time_Value start_time_;
  ECVector remote_ecs_;

  EventType_Vector* etypes_;

  ACE_CString name_;

  //need to handle multiple gateways!
  typedef ACE_Map_Manager<name_t,TAO_EC_Gateway_Sched*,ACE_Thread_Mutex> Gateway_Map;

  Gateway_Map gateways;

}; //class Kokyu_EC

class Reactor_Task : public ACE_Task<ACE_SYNCH>
{
public:
  /// Constructor
  Reactor_Task (void);

  ~Reactor_Task (void);

  int initialize(void);

  ACE_Reactor *reactor(void);

  /// Process the events in the queue.
  int svc (void);

private:
  int initialized_;

  ACE_Reactor *react_;
}; //class Reactor_Task

class Gateway_Initializer : public ACE_Event_Handler
{
public:
  typedef std::vector<const char*> FileNameVector;

  Gateway_Initializer(void);

  ~Gateway_Initializer(void);

  int init(CORBA::ORB_var orb,
           PortableServer::POA_var poa,
           Kokyu_EC *ec,
           ECDriver *drv,
           const ACE_CString& ior_filename,
           const FileNameVector& ior_output_files,
           int numInputECs);

  // arg is a ECSupplier::EventTypeVector*
  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *arg);

private:
  //RtEventChannelAdmin::RtSchedEventChannel_var ec_;
  Kokyu_EC *ec_;
  ECDriver *driver_;
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  ACE_CString ior_filename_;
  FileNameVector ior_output_files_;
  int numInputECs_;
}; //class Gateway_Initializer

#endif //KOKYU_EC_H
