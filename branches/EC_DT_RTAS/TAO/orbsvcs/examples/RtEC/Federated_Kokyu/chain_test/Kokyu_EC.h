// $Id$

#ifndef KOKYU_EC_H
#define KOKYU_EC_H

#include "Supplier.h"
#include "Consumer.h"
#include "ace/Vector_T.h"
#include "ace/Task_T.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor_Base.h" //for ACE_Select_Reactor_Impl::DEFAULT_SIZE

#include "RtSchedEventChannelS.h"
#include "tao/Utils/Servant_Var.h"
#include "orbsvcs/Event/EC_Event_Channel.h"

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

    virtual RtecEventChannelAdmin::EventChannel_ptr event_channel (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual RtecScheduler::Scheduler_ptr scheduler (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    ///Takes ownership of Supplier and Timeout_Consumer
    void add_supplier_with_timeout(
                                   Supplier * supplier_impl,
                                   const char * supp_entry_point,
                                   RtecEventComm::EventType supp_type,
                                   Supplier_Timeout_Handler * timeout_handler_impl, 
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

/*
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

private:
  TAO::Utils::Servant_Var<POA_RtecScheduler::Scheduler> scheduler_impl_;
  TAO::Utils::Servant_Var<TAO_EC_Event_Channel> ec_impl_;
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin_;
  RtecScheduler::Scheduler_var scheduler_;

  ACE_Vector<task_trigger_t> task_triggers_; //also keeps track of timeout_handlers

  ACE_Reactor *reactor_;
  ACE_Vector<long> timer_handles_;

  ACE_Vector<Supplier*> suppliers_;
//  ACE_Vector<Timeout_Consumer*> timeout_consumers_;
  ACE_Vector<Consumer*> consumers_;
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

#endif
