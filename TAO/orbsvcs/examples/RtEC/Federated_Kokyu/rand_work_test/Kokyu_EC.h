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
#include "ace/Vector_T.h"
#include "ace/Reactor.h"

#include "tao/Utils/Servant_Var.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Gateway_IIOP_Factory.h"
#include "orbsvcs/Event/EC_Gateway_Sched.h"
#include "orbsvcs/Scheduler_Factory.h"

#include "RtSchedEventChannelS.h"
#include "Supplier.h"
#include "Consumer.h"

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
  Supplier_Timeout_Handler *handler;
  ACE_Time_Value period;
  ACE_Time_Value phase;
}; //struct task_trigger_t

class Kokyu_EC : public POA_RtEventChannelAdmin::RtSchedEventChannel
{
public:
  Kokyu_EC(void);

  virtual ~Kokyu_EC(void);

  ACE_INLINE static RtecScheduler::Period_t time_val_to_period (const ACE_Time_Value &tv)
  {
    //100s of nanoseconds
    return (tv.sec () * 1000000 + tv.usec ())*10;
  }

  int init(const char* schedule_discipline, PortableServer::POA_ptr poa, ACE_Reactor * reactor = 0);

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
                                                           RtecEventComm::EventType type,
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

  virtual RtecEventChannelAdmin::EventChannel_ptr event_channel (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual RtecScheduler::Scheduler_ptr scheduler (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  ///// CONVENIENCE FUNCTIONS /////
  int remote_gateways_connected(void);

  ///Takes ownership of Supplier and Timeout_Consumer
  void add_supplier_with_timeout(
                                 Supplier * supplier_impl,
                                 const char * supp_entry_point,
                                 RtecEventComm::EventType supp_type,
                                 Supplier_Timeout_Handler * timeout_handler_impl,
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

  /*
  ///Takes ownership of Supplier and Timeout_Consumer
  void add_supplier_with_timeout(
                                 Supplier * supplier_impl,
                                 const char * supp_entry_point,
                                 RtecEventComm::EventType supp_type,
                                 Timeout_Consumer * timeout_consumer_impl,
                                 const char * timeout_entry_point,
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

  ///Takes ownership of Timeout_Consumer
  void add_timeout_consumer(
                            Supplier * supplier_impl,
                            Timeout_Consumer * timeout_consumer_impl,
                            const char * timeout_entry_point,
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
  */
  ///Takes ownership of Supplier
  void add_supplier(
                    Supplier * supplier_impl,
                    const char * entry_point,
                    RtecEventComm::EventType type
                    ACE_ENV_ARG_DECL
                    )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ));

  ///Takes ownership of Consumer and Supplier
  void add_consumer_with_supplier(
                                  Consumer * consumer_impl,
                                  const char * cons_entry_point,
                                  ACE_Time_Value cons_period,
                                  RtecEventComm::EventType cons_type,
                                  RtecScheduler::Criticality_t cons_crit,
                                  RtecScheduler::Importance_t cons_imp,
                                  Supplier * supplier_impl,
                                  const char * supp_entry_point,
                                  RtecEventComm::EventType supp_type
                                  ACE_ENV_ARG_DECL
                                  )
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ));

  ///Takes ownership of Consumer
  void add_consumer(
                    Consumer * consumer_impl,
                    const char * entry_point,
                    ACE_Time_Value period,
                    RtecEventComm::EventType cons_type,
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

  void set_up_last_subtask (subtask_t subtask,
                            RtecEventComm::EventType in_type ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ));

  void set_up_first_subtask (subtask_t subtask,
                             RtecEventComm::EventSourceID supp_id1, RtecEventComm::EventSourceID supp_id2,
                             RtecEventComm::EventType in_type, RtecEventComm::EventType out_type ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ));

  void set_up_middle_subtask (subtask_t subtask,
                              RtecEventComm::EventSourceID supp_id, RtecEventComm::EventSourceID next_supp_id,
                              RtecEventComm::EventType in_type, RtecEventComm::EventType out_type ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
                     CORBA::SystemException
                     , RtecScheduler::UNKNOWN_TASK
                     , RtecScheduler::INTERNAL
                     , RtecScheduler::SYNCHRONIZATION_FAILURE
                     ));

  void init_gateway(CORBA::ORB_ptr orb,
                    PortableServer::POA_ptr poa,
                    const char* consumer_ec_ior,
                    const char* ior_output_filename
                    ACE_ENV_ARG_DECL);

private:
  TAO::Utils::Servant_Var<POA_RtecScheduler::Scheduler> scheduler_impl_;
  TAO::Utils::Servant_Var<TAO_EC_Event_Channel> ec_impl_;
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin_;
  RtecScheduler::Scheduler_var scheduler_;

  ACE_Vector<Supplier*> suppliers_;
  //ACE_Vector<Supplier_Timeout_Handler*> timeout_handlers_;
  ACE_Vector<Consumer*> consumers_;
  ACE_Vector<RtecEventComm::PushConsumer_var> consumer_proxies_;
  ACE_Vector<RtecEventChannelAdmin::ProxyPushSupplier_var> push_suppliers_;
  ACE_Vector<RtecEventComm::PushSupplier_var> supplier_proxies_;

  ACE_Vector<task_trigger_t> task_triggers_; //also keeps track of timeout_handlers

  ACE_Reactor *reactor_;
  ACE_Vector<long> timer_handles_;

  bool started_;
  int remote_gateways_connected_;

  //need to handle multiple gateways!
  typedef ACE_Map_Manager<const char*,TAO_EC_Gateway_Sched*,ACE_Thread_Mutex> Gateway_Map;

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
  typedef ACE_Vector<const char*> Filename_Array;

  Gateway_Initializer(void);

  ~Gateway_Initializer(void);

  int init(CORBA::ORB_var orb, PortableServer::POA_var poa, Kokyu_EC *ec, const ACE_CString& ior_output_filename,
           const Filename_Array& ior_input_files);

  virtual int handle_timeout (const ACE_Time_Value &,
                              const void *);

private:
  Kokyu_EC *ec_;
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  ACE_CString ior_output_filename_;
  Filename_Array ior_input_files_;
}; //class Gateway_Initializer

#endif //KOKYU_EC_H
