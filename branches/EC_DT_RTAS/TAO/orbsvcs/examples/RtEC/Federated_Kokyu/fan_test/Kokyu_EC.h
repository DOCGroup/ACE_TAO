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

    int init(const char* schedule_discipline, PortableServer::POA_ptr poa);

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

  ACE_Vector<Supplier*> suppliers_;
  ACE_Vector<Timeout_Consumer*> timeout_consumers_;
  ACE_Vector<Consumer*> consumers_;
}; //class Kokyu_EC

class Reactor_Task : public ACE_Task<ACE_SYNCH>
{
public:
  /// Constructor
  Reactor_Task (void)
    : initialized_(0)
    , react_(0)
  {}


  ~Reactor_Task (void)
  {
    delete react_;
  }

  int initialize(void)
  {
    //We need to set the ACE_Reactor::instance() to be the ORB
    //reactor so Kokyu's RG implementation can use it w/o creating
    //an extra thread to run the reactor event loop. I hope this
    //doesn't screw something else up!
    //use Select_Reactor explicitly?
    ACE_Reactor *reactor; //TODO: how clean up reactor and stop thread?
    ACE_NEW_RETURN(reactor,
                   ACE_Reactor,
                   -1);
    reactor->open(ACE_Select_Reactor_Impl::DEFAULT_SIZE);
    ACE_Reactor::instance(reactor);

    this->react_ = reactor;

    this->initialized_ = 1;

    return 0;
  }

  ACE_Reactor *reactor(void)
  {
    return this->react_;
  }

  /// Process the events in the queue.
  int svc (void)
  {
    ACE_DEBUG((LM_DEBUG,"Reactor_Task (%P|%t) svc(): ENTER\n"));

    if (!this->initialized_)
      {
        this->initialize();
      }

    this->react_->owner(ACE_Thread::self()); //set this thread as owner

    int err = this->react_->run_reactor_event_loop();
    if (err < 0)
      {
        ACE_DEBUG((LM_ERROR,"Reactor_Task (%t) error running Reactor event loop\n"));
      }

    ACE_DEBUG((LM_DEBUG,"Reactor_Task (%P|%t) svc(): LEAVE\n"));
    return 0;
  }

private:
  int initialized_;

  ACE_Reactor *react_;
}; //class Reactor_Task

#endif
