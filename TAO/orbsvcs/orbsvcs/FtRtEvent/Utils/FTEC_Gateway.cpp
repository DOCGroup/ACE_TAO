#include "FTEC_Gateway.h"
#include "activate_with_id.h"
#include "UUID.h"
#include "resolve_init.h"
#include "orbsvcs/FtRtecEventCommS.h"

ACE_RCSID (Utils, 
           FTEC_Gateway,
           "$Id$")

namespace TAO_FTRTEC
{
  class TAO_FtRtEvent_Export
    FTEC_Gateway_ConsumerAdmin :
      public POA_RtecEventChannelAdmin::ConsumerAdmin
  {
  public:
      FTEC_Gateway_ConsumerAdmin(FTEC_Gateway_Impl *impl);
      ~FTEC_Gateway_ConsumerAdmin();

      // = The RtecEventChannelAdmin::ConsumerAdmin methods...
      virtual RtecEventChannelAdmin::ProxyPushSupplier_ptr
        obtain_push_supplier
          (ACE_ENV_SINGLE_ARG_DECL_NOT_USED) ACE_THROW_SPEC((CORBA::SystemException));
      FTEC_Gateway_Impl *impl_;
  };

  class FTEC_Gateway_SupplierAdmin :
    public POA_RtecEventChannelAdmin::SupplierAdmin
  {
  public:
      FTEC_Gateway_SupplierAdmin(FTEC_Gateway_Impl *impl);
      ~FTEC_Gateway_SupplierAdmin(void);

      // = The RtecEventChannelAdmin::SupplierAdmin methods...
      virtual RtecEventChannelAdmin::ProxyPushConsumer_ptr
        obtain_push_consumer
          (ACE_ENV_SINGLE_ARG_DECL_NOT_USED) ACE_THROW_SPEC((CORBA::SystemException));
      FTEC_Gateway_Impl *impl_;
  };

  class FTEC_Gateway_ProxyPushSupplier :
    public POA_RtecEventChannelAdmin::ProxyPushSupplier
  {
  public:
      FTEC_Gateway_ProxyPushSupplier(FTEC_Gateway_Impl *impl);
      ~FTEC_Gateway_ProxyPushSupplier(void);

      // = The RtecEventChannelAdmin::ProxyPushSupplier methods...
      virtual void connect_push_consumer
        (
          RtecEventComm::PushConsumer_ptr
            push_consumer, const
              RtecEventChannelAdmin::ConsumerQOS &
                qos ACE_ENV_ARG_DECL_NOT_USED
        )  ACE_THROW_SPEC((
                            CORBA::SystemException,
                            RtecEventChannelAdmin::
                            AlreadyConnected,
                            RtecEventChannelAdmin::
                            TypeError
                ));
      virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)  ACE_THROW_SPEC((CORBA::SystemException));
      virtual void suspend_connection (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)  ACE_THROW_SPEC((CORBA::SystemException));
      virtual void resume_connection (ACE_ENV_SINGLE_ARG_DECL_NOT_USED) ACE_THROW_SPEC((CORBA::SystemException));
      FTEC_Gateway_Impl *impl_;
  };

  class FTEC_Gateway_ProxyPushConsumer :
    public POA_RtecEventChannelAdmin::ProxyPushConsumer
  {
  public:
      FTEC_Gateway_ProxyPushConsumer(FTEC_Gateway_Impl *impl);
      ~FTEC_Gateway_ProxyPushConsumer(void);

      virtual void push
        (
          const RtecEventComm::EventSet &data
            ACE_ENV_ARG_DECL_WITH_DEFAULTS
        )  ACE_THROW_SPEC((CORBA::SystemException));

      // = The RtecEventChannelAdmin::ProxyPushConsumer methods...
      virtual void connect_push_supplier
        (
          RtecEventComm::PushSupplier_ptr
            push_supplier, const
              RtecEventChannelAdmin::SupplierQOS &
                qos ACE_ENV_ARG_DECL_NOT_USED
        )  ACE_THROW_SPEC((
                            CORBA::SystemException,
                            RtecEventChannelAdmin::
                            AlreadyConnected
                    ));
      virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)  ACE_THROW_SPEC((CORBA::SystemException));

      FTEC_Gateway_Impl *impl_;
  };

  class PushConsumerHandler :
    public POA_FtRtecEventComm::AMI_PushConsumerHandler
  {
  public:
      PushConsumerHandler(void);
      ~PushConsumerHandler(void);
      virtual void push ()  ACE_THROW_SPEC((CORBA::SystemException));

      virtual void push_excep
        (
          FtRtecEventComm::AMI_PushConsumerExceptionHolder *
          excep_holder
        ) ACE_THROW_SPEC((CORBA::SystemException));
  };

  struct FTEC_Gateway_Impl
  {
    CORBA::ORB_var          orb;
    PortableServer::POA_var poa;
    FtRtecEventChannelAdmin::EventChannel_var
      ftec;
    RtecEventChannelAdmin::ConsumerAdmin_var
      consumer_admin;
    RtecEventChannelAdmin::SupplierAdmin_var
      supplier_admin;
    FTEC_Gateway_ConsumerAdmin
      consumer_admin_servant;
    FTEC_Gateway_SupplierAdmin
      supplier_admin_servant;
    FTEC_Gateway_ProxyPushSupplier
      proxy_supplier_servant;
    FTEC_Gateway_ProxyPushConsumer
      proxy_consumer_servant;
    PushConsumerHandler push_handler_servant;
    FtRtecEventComm::AMI_PushConsumerHandler_var
      push_handler;
    FTEC_Gateway_Impl();
  };

  FTEC_Gateway_Impl::FTEC_Gateway_Impl (void) :
    consumer_admin_servant(this),
    supplier_admin_servant(this),
    proxy_supplier_servant(this),
    proxy_consumer_servant(this)
  {
  }

  FTEC_Gateway::FTEC_Gateway (CORBA::ORB_ptr  orb,
                              FtRtecEventChannelAdmin::EventChannel_ptr
                                ftec) :
  impl_(new FTEC_Gateway_Impl)
  {
    impl_->orb = CORBA::ORB::_duplicate(orb);
    impl_->ftec = FtRtecEventChannelAdmin::EventChannel::_duplicate(ftec);
  }

  FTEC_Gateway::~
  FTEC_Gateway (void)
  {
    delete impl_;
  }

  RtecEventChannelAdmin::EventChannel_ptr
    FTEC_Gateway::activate
      (
        PortableServer::POA_ptr root_poa
          ACE_ENV_ARG_DECL
      )
  {
    PortableServer::IdUniquenessPolicy_var
      id_uniqueness_policy = root_poa->
        create_id_uniqueness_policy
          (
            PortableServer::MULTIPLE_ID
              ACE_ENV_ARG_PARAMETER
          );
    ACE_CHECK_RETURN(0);

    PortableServer::LifespanPolicy_var  lifespan =
      root_poa->create_lifespan_policy
        (
          PortableServer::PERSISTENT
            ACE_ENV_ARG_PARAMETER
        );
    ACE_CHECK_RETURN(0);

    // create a USER_ID IdAssignmentPolicy object
    PortableServer::IdAssignmentPolicy_var
      assign = root_poa->
        create_id_assignment_policy
          (
            PortableServer::USER_ID
              ACE_ENV_ARG_PARAMETER
          );
    ACE_CHECK_RETURN(0);

    CORBA::PolicyList policy_list;
    policy_list.length(3);

    policy_list[0] = PortableServer::
      IdUniquenessPolicy::_duplicate
        (
          id_uniqueness_policy.in()
            ACE_ENV_ARG_PARAMETER
        );
    ACE_CHECK_RETURN(0);
    policy_list[1] = PortableServer::
      LifespanPolicy::_duplicate
        (lifespan.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);
    policy_list[2] = PortableServer::
      IdAssignmentPolicy::_duplicate
        (assign.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);

    PortableServer::POAManager_var  mgr =
      root_poa->the_POAManager
        (ACE_ENV_SINGLE_ARG_PARAMETER);

    impl_->poa = root_poa->create_POA
      (
        "gateway_poa", mgr.in(),
        policy_list
          ACE_ENV_ARG_PARAMETER
      );
    ACE_CHECK_RETURN(0);

    id_uniqueness_policy->destroy();
    lifespan->destroy();
    assign->destroy();

    PortableServer::ObjectId  oid;
    oid.length(16);
    UUID::create(oid.get_buffer());

    RtecEventChannelAdmin::EventChannel_var
      gateway;

    activate_object_with_id
      (
        gateway.out(),
        impl_->poa, this, oid
          ACE_ENV_ARG_PARAMETER
      );
    ACE_CHECK_RETURN(0);
    ++oid[9];
    activate_object_with_id
      (
        impl_->consumer_admin.out(),
        impl_->poa, &
          impl_->consumer_admin_servant, oid
            ACE_ENV_ARG_PARAMETER
      );
    ACE_CHECK_RETURN(0);
    ++oid[9];
    activate_object_with_id
      (
        impl_->supplier_admin.out(),
        impl_->poa, &
          impl_->supplier_admin_servant, oid
            ACE_ENV_ARG_PARAMETER
      );
    ACE_CHECK_RETURN(0);

    return gateway._retn();
  }

  //= The RtecEventChannelAdmin::EventChannel methods
  RtecEventChannelAdmin::ConsumerAdmin_ptr FTEC_Gateway::
    for_consumers(ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return RtecEventChannelAdmin::ConsumerAdmin::_duplicate(impl_->consumer_admin.in());
  }

  RtecEventChannelAdmin::SupplierAdmin_ptr FTEC_Gateway::
    for_suppliers(ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    ACE_DEBUG((LM_DEBUG, "FTEC_Gateway::for_suppliers\n"));
    return RtecEventChannelAdmin::SupplierAdmin::_duplicate(impl_->supplier_admin.in());
  }

  void FTEC_Gateway:: destroy
    (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    impl_->ftec->destroy();
  }

  RtecEventChannelAdmin::Observer_Handle
    FTEC_Gateway::append_observer
      (
        RtecEventChannelAdmin::Observer_ptr
          observer ACE_ENV_ARG_DECL
      )
  ACE_THROW_SPEC ((
                    CORBA::SystemException,
                    RtecEventChannelAdmin::EventChannel::
                    SYNCHRONIZATION_ERROR,
                    RtecEventChannelAdmin::
                    EventChannel::CANT_APPEND_OBSERVER
        ))
  {
    return impl_->ftec->append_observer(observer ACE_ENV_ARG_PARAMETER);
  }

  void FTEC_Gateway:: remove_observer
    (
      RtecEventChannelAdmin::Observer_Handle handle
        ACE_ENV_ARG_DECL
    )
  ACE_THROW_SPEC ((
                    CORBA::SystemException,
                    RtecEventChannelAdmin::EventChannel::
                    SYNCHRONIZATION_ERROR,
                    RtecEventChannelAdmin::
                    EventChannel::CANT_REMOVE_OBSERVER
        ))
  {
    impl_->ftec->remove_observer(handle ACE_ENV_ARG_PARAMETER);
  }

  /// FTEC_Gateway_ConsumerAdmin
  FTEC_Gateway_ConsumerAdmin::FTEC_Gateway_ConsumerAdmin (FTEC_Gateway_Impl *impl) :
  impl_(impl)
  {
  }

  FTEC_Gateway_ConsumerAdmin::~
  FTEC_Gateway_ConsumerAdmin (void)
  {
  }

  RtecEventChannelAdmin::ProxyPushSupplier_ptr FTEC_Gateway_ConsumerAdmin::
    obtain_push_supplier
      (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    PortableServer::ObjectId **remote_proxy_oid_ptr;
    ACE_NEW_THROW_EX(remote_proxy_oid_ptr,
                     PortableServer::ObjectId *,
                     CORBA::NO_MEMORY());

    PortableServer::ObjectId  local_oid;
    local_oid.length(sizeof(remote_proxy_oid_ptr));
    memcpy(&local_oid[0],
           &remote_proxy_oid_ptr,
           sizeof(remote_proxy_oid_ptr));

    RtecEventChannelAdmin::ProxyPushSupplier_ptr
      result;
    activate_object_with_id(result,
                            impl_->poa,
                            &impl_->proxy_supplier_servant,
                            local_oid ACE_ENV_ARG_PARAMETER);
    return result;
  }

  /// FTEC_Gateway_SupplierAdmin
  FTEC_Gateway_SupplierAdmin::FTEC_Gateway_SupplierAdmin (FTEC_Gateway_Impl *impl) :
  impl_(impl)
  {
  }

  FTEC_Gateway_SupplierAdmin::~
  FTEC_Gateway_SupplierAdmin (void)
  {
  }

  // = The RtecEventChannelAdmin::SupplierAdmin methods...
  RtecEventChannelAdmin::ProxyPushConsumer_ptr FTEC_Gateway_SupplierAdmin::
    obtain_push_consumer
      (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    PortableServer::ObjectId **remote_proxy_oid_ptr;
    ACE_NEW_THROW_EX(remote_proxy_oid_ptr,
                     PortableServer::ObjectId *,
                     CORBA::NO_MEMORY());

    PortableServer::ObjectId  local_oid;
    local_oid.length(sizeof(remote_proxy_oid_ptr));
    memcpy(&local_oid[0],
           &remote_proxy_oid_ptr,
           sizeof(remote_proxy_oid_ptr));

    RtecEventChannelAdmin::ProxyPushConsumer_ptr
      result;
    activate_object_with_id(result,
                            impl_->poa,
                            &impl_->proxy_consumer_servant,
                            local_oid ACE_ENV_ARG_PARAMETER);
    return result;
  }

  PortableServer::ObjectId **
  get_remote_oid_ptr
    (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL_NOT_USED)
  {
    PortableServer::Current_var current =
      resolve_init < PortableServer::Current >
        (orb, "POACurrent"ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    PortableServer::ObjectId_var  object_id =
      current->get_object_id
        (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    PortableServer::ObjectId **result;
    memcpy
      (
        &result, &object_id[0], sizeof
          (PortableServer::ObjectId **)
      );
    return result;
  }

  /// FTEC_Gateway_ProxyPushSupplier
  FTEC_Gateway_ProxyPushSupplier::FTEC_Gateway_ProxyPushSupplier (FTEC_Gateway_Impl *impl) :
  impl_(impl)
  {
  }

  FTEC_Gateway_ProxyPushSupplier::~
  FTEC_Gateway_ProxyPushSupplier ()
  {
  }

  // = The RtecEventChannelAdmin::ProxyPushSupplier methods...
  void FTEC_Gateway_ProxyPushSupplier::
    connect_push_consumer
      (
        RtecEventComm::PushConsumer_ptr
          push_consumer, const RtecEventChannelAdmin::ConsumerQOS &qos
            ACE_ENV_ARG_DECL_NOT_USED
      )
  ACE_THROW_SPEC ((
                    CORBA::SystemException,
                    RtecEventChannelAdmin::AlreadyConnected,
                    RtecEventChannelAdmin::TypeError
            ))
  {
    PortableServer::ObjectId **oid_ptr = get_remote_oid_ptr(impl_->orb.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    *oid_ptr = impl_->ftec->connect_push_consumer(push_consumer,
                                                  qos ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void FTEC_Gateway_ProxyPushSupplier::
    disconnect_push_supplier
      (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    PortableServer::ObjectId **oid_ptr = get_remote_oid_ptr(impl_->orb.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    impl_->ftec->disconnect_push_supplier(**oid_ptr ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    delete *oid_ptr;
    delete oid_ptr;
  }

  void FTEC_Gateway_ProxyPushSupplier::
    suspend_connection
      (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    PortableServer::ObjectId **oid_ptr = get_remote_oid_ptr(impl_->orb.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    impl_->ftec->suspend_push_supplier(**oid_ptr ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void FTEC_Gateway_ProxyPushSupplier::
    resume_connection
      (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    PortableServer::ObjectId **oid_ptr = get_remote_oid_ptr(impl_->orb.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    impl_->ftec->resume_push_supplier(**oid_ptr ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  /// FTEC_Gateway_ProxyPushConsumer
  FTEC_Gateway_ProxyPushConsumer::FTEC_Gateway_ProxyPushConsumer (FTEC_Gateway_Impl *impl) :
  impl_(impl)
  {
  }

  FTEC_Gateway_ProxyPushConsumer::~
  FTEC_Gateway_ProxyPushConsumer (void)
  {
  }

  // = The RtecEventChannelAdmin::ProxyPushConsumer methods...
  void FTEC_Gateway_ProxyPushConsumer:: push
    (
      const RtecEventComm::EventSet &data
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
    )
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    PortableServer::ObjectId **oid_ptr = get_remote_oid_ptr(impl_->orb.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (CORBA::is_nil(impl_->push_handler.in()))
    {
      impl_->push_handler = impl_->push_handler_servant._this();
    }

    impl_->ftec->sendc_push(impl_->push_handler.in(),
                            **oid_ptr,
                            data ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void FTEC_Gateway_ProxyPushConsumer::
    connect_push_supplier
      (
        RtecEventComm::PushSupplier_ptr
          push_supplier, const RtecEventChannelAdmin::SupplierQOS &qos
            ACE_ENV_ARG_DECL_NOT_USED
      )
  ACE_THROW_SPEC ((
                    CORBA::SystemException,
                    RtecEventChannelAdmin::AlreadyConnected
              ))
  {
    PortableServer::ObjectId **oid_ptr = get_remote_oid_ptr(impl_->orb.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    *oid_ptr = impl_->ftec->connect_push_supplier(push_supplier,
                                                  qos ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  void FTEC_Gateway_ProxyPushConsumer::
    disconnect_push_consumer
      (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
    PortableServer::ObjectId **oid_ptr = get_remote_oid_ptr(impl_->orb.in() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    impl_->ftec->disconnect_push_consumer(**oid_ptr ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    delete *oid_ptr;
    delete oid_ptr;
  }

  PushConsumerHandler::PushConsumerHandler (void)
  {
  }

  PushConsumerHandler::~
  PushConsumerHandler (void)
  {
  }

  void PushConsumerHandler::  push()
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

  void PushConsumerHandler::  push_excep
    (
      FtRtecEventComm::AMI_PushConsumerExceptionHolder *
        excep_holder
    )
  ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }
}
