// $Id$

#include "ace/Dynamic_Service.h"
#include "FTEC_Event_Channel.h"
#include "FTEC_Event_Channel_Impl.h"
#include "../Utils/activate_with_id.h"
#include "../Utils/resolve_init.h"
#include "../Utils/UUID.h"
#include "Fault_Detector_Loader.h"
#include "Fault_Detector.h"
#include "Request_Context_Repository.h"
#include "Replication_Service.h"
#include "Identification_Service.h"
#include "create_persistent_poa.h"
#include "tao/Utils/PolicyList_Destroyer.h"
#include "GroupInfoPublisher.h"
#include "../Utils/Log.h"

ACE_RCSID (EventChannel,
           TAO_FTEC_Event_Channel,
           "$Id$")

TAO_FTEC_Event_Channel::TAO_FTEC_Event_Channel(CORBA::ORB_var orb,
                                               PortableServer::POA_var poa)
:    orb_(orb)
,    poa_(poa)
,    ec_impl_(NULL)
{
}

TAO_FTEC_Event_Channel::~TAO_FTEC_Event_Channel()
{
    delete ec_impl_;
}


void setup_object_group(TAO_FTEC_Event_Channel* es,
                       CosNaming::NamingContext_ptr naming_context,
                       TAO_FTEC_Event_Channel::MEMBERSHIP membership,
                       FtRtecEventChannelAdmin::EventChannel_ptr ec
                       ACE_ENV_ARG_DECL)
{
    if (membership != TAO_FTEC_Event_Channel::NONE) {// register to naming service
      FTRT::ManagerInfoList member_list;
      member_list.length(1);
      member_list[0].the_location = Fault_Detector::instance()->my_location();
      member_list[0].ior = FTRT::ObjectGroupManager::_duplicate(ec);

      if (membership == TAO_FTEC_Event_Channel::PRIMARY)
        es->create_group(member_list, 0
            ACE_ENV_ARG_PARAMETER);

      else { // BACKUP
        FtRtecEventChannelAdmin::EventChannel_var primary =
          resolve<FtRtecEventChannelAdmin::EventChannel>(naming_context,
            FTRTEC::Identification_Service::instance()->name()
            ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        TAO_FTRTEC::Log(1, "Got Primary address from Naming Service\n");

        primary->join_group(member_list[0] ACE_ENV_ARG_PARAMETER);
      }
      ACE_CHECK;
    }
}


FtRtecEventChannelAdmin::EventChannel_ptr
TAO_FTEC_Event_Channel::activate(TAO_FTEC_Event_Channel::MEMBERSHIP membership
                                 ACE_ENV_ARG_DECL)
{
    FTRTEC::Fault_Detector_Loader* detector_loader =
      ACE_Dynamic_Service<FTRTEC::Fault_Detector_Loader>::instance("FTRTEC_Fault_Detector");

    detector_loader->init(0, 0);

    if (FTRTEC::Identification_Service::instance() == 0)
      ACE_ERROR_RETURN((LM_ERROR, "No Identification\n"), 0);

    // initialize naming_contex
    CosNaming::NamingContext_var naming_context
      = resolve_init<CosNaming::NamingContext>(orb_.in(), "NameService"
                                                ACE_ENV_ARG_PARAMETER);
    // initialize group info publisher
    GroupInfoPublisher::instance()->set_naming_context(naming_context);

    if (FTRTEC::Replication_Service::instance()->init(0,0) == -1)
      return 0;

    GroupInfoPublisher::instance()->subscribe(FTRTEC::Replication_Service::instance());

    Request_Context_Repository().init(orb_.in());

    // get POAManager
    PortableServer::POAManager_var mgr = poa_->the_POAManager(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);

    TAO::Utils::PolicyList_Destroyer policy_list(2);

    persistent_poa_ =
      create_persistent_poa(poa_, mgr, "FTEC_Persistant_POA", policy_list ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);

    // Activate the Event channel implementation

    TAO_EC_Event_Channel_Attributes attr (persistent_poa_.in (),
      persistent_poa_.in ());


    TAO_FTEC_Event_Channel_Impl* ec;
    ACE_NEW_THROW_EX (ec,
      TAO_FTEC_Event_Channel_Impl (attr),
      CORBA::NO_MEMORY());

    this->ec_impl_ = ec;

    const PortableServer::ObjectId& object_id
      = FTRTEC::Identification_Service::instance()->object_id();

    PortableServer::ObjectId consumer_admin_object_id(object_id);
    consumer_admin_object_id[9]++;

    PortableServer::ObjectId supplier_admin_object_id(consumer_admin_object_id);
    supplier_admin_object_id[9]++;

    ec->activate(orb_,
                 supplier_admin_object_id,
                 consumer_admin_object_id
                 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);

    FtRtecEventChannelAdmin::EventChannel_var result;
    activate_object_with_id(result.out(), persistent_poa_, this, object_id
                ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);


    setup_object_group(this,
                       naming_context.in(),
                       membership,
                       result.in()
                       ACE_ENV_ARG_PARAMETER);
  return result._retn();
}


void TAO_FTEC_Event_Channel::set_listener(TAO_FTEC_Become_Primary_Listener* listener)
{
  GroupInfoPublisher::instance()->subscribe(listener);
}

void TAO_FTEC_Event_Channel::set_update (
        const FTRT::State & s
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , FTRT::InvalidUpdate
        , FTRT::OutOfSequence
      ))
{
    ec_impl_->set_update(s ACE_ENV_ARG_PARAMETER);
}

void TAO_FTEC_Event_Channel::oneway_set_update (
        const FTRT::State & s
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
    ec_impl_->set_update(s ACE_ENV_ARG_PARAMETER);
}


RtecEventChannelAdmin::ConsumerAdmin_ptr
TAO_FTEC_Event_Channel::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
{
    return ec_impl_->for_consumers(ACE_ENV_SINGLE_ARG_PARAMETER);
}

RtecEventChannelAdmin::SupplierAdmin_ptr
TAO_FTEC_Event_Channel::for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
{
    return ec_impl_->for_suppliers(ACE_ENV_SINGLE_ARG_PARAMETER);
}


void
TAO_FTEC_Event_Channel::set_state (const FTRT::State & s ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, FTRT::InvalidState))
{
  ec_impl_->set_state(s ACE_ENV_ARG_PARAMETER);
}

void
TAO_FTEC_Event_Channel::destroy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (Fault_Detector::instance())
    Fault_Detector::instance()->stop();

  ec_impl_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  this->orb_->shutdown ();
}

RtecEventChannelAdmin::Observer_Handle
TAO_FTEC_Event_Channel::append_observer (RtecEventChannelAdmin::Observer_ptr observer
                                ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
          CORBA::SystemException,
          RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
          RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER))
{
  /// we have yet to implement the replication of observers
  /// throw an exception for the moment
  ACE_THROW_RETURN(RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER(), 0);

  return this->ec_impl_->append_observer (observer ACE_ENV_ARG_PARAMETER);
}

void
TAO_FTEC_Event_Channel::remove_observer (RtecEventChannelAdmin::Observer_Handle handle
                                ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
          CORBA::SystemException,
          RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
          RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER))
{
  /// we have yet to implement the replication of observers
  /// throw an exception for the moment
  ACE_THROW(RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER());

  ec_impl_->remove_observer (handle ACE_ENV_ARG_PARAMETER);
}



CORBA::Boolean
TAO_FTEC_Event_Channel::start (
        FTRT::FaultListener_ptr listener,
        FTRT::Location_out location
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  return ec_impl_->start(listener, location ACE_ENV_ARG_PARAMETER);
}

void
TAO_FTEC_Event_Channel::create_group (
        const FTRT::ManagerInfoList & info_list,
        CORBA::ULong object_group_ref_version
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      , FTRT::PredecessorUnreachable
      ))
{
  ec_impl_->create_group(info_list, object_group_ref_version ACE_ENV_ARG_PARAMETER);
}

void
TAO_FTEC_Event_Channel::join_group (
        const FTRT::ManagerInfo & info
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ec_impl_->join_group(info ACE_ENV_ARG_PARAMETER);
}

void
TAO_FTEC_Event_Channel::add_member (
        const FTRT::ManagerInfo & info,
        CORBA::ULong object_group_ref_version
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ec_impl_->add_member(info, object_group_ref_version ACE_ENV_ARG_PARAMETER);
}

void
TAO_FTEC_Event_Channel::remove_member (
        const FTRT::Location & crashed_location,
        CORBA::ULong object_group_ref_version
        ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ec_impl_->remove_member(crashed_location,
                            object_group_ref_version
                ACE_ENV_ARG_PARAMETER);
}

void
TAO_FTEC_Event_Channel::replica_crashed (
        const FTRT::Location & location
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ec_impl_->replica_crashed(location ACE_ENV_ARG_PARAMETER);
}


  /// EventChannelFacade Interface

::FtRtecEventChannelAdmin::ObjectId *
TAO_FTEC_Event_Channel::connect_push_consumer (
        RtecEventComm::PushConsumer_ptr push_consumer,
        const RtecEventChannelAdmin::ConsumerQOS & qos
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , RtecEventChannelAdmin::TypeError
      ))
{
  return ec_impl_->connect_push_consumer(push_consumer, qos
                                         ACE_ENV_ARG_PARAMETER);
}


::FtRtecEventChannelAdmin::ObjectId *
TAO_FTEC_Event_Channel::connect_push_supplier (
        RtecEventComm::PushSupplier_ptr push_supplier,
        const RtecEventChannelAdmin::SupplierQOS & qos
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  return ec_impl_->connect_push_supplier(push_supplier, qos
                                         ACE_ENV_ARG_PARAMETER);

}

void
TAO_FTEC_Event_Channel::disconnect_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ec_impl_->disconnect_push_supplier(oid
                                     ACE_ENV_ARG_PARAMETER);
}

void
TAO_FTEC_Event_Channel::disconnect_push_consumer (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ))
{
  ec_impl_->disconnect_push_consumer(oid
                                     ACE_ENV_ARG_PARAMETER);
}

void
TAO_FTEC_Event_Channel::suspend_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , FtRtecEventComm::InvalidObjectID
      ))
{
  ec_impl_->suspend_push_supplier(oid
                        ACE_ENV_ARG_PARAMETER);
}

void
TAO_FTEC_Event_Channel::resume_push_supplier (
        const FtRtecEventChannelAdmin::ObjectId & oid
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , FtRtecEventComm::InvalidObjectID
      ))
{
  ec_impl_->resume_push_supplier(oid
                       ACE_ENV_ARG_PARAMETER);
}

void
TAO_FTEC_Event_Channel::push (
        const FtRtecEventChannelAdmin::ObjectId & oid,
        const RtecEventComm::EventSet & data
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        , FtRtecEventComm::InvalidObjectID
      ))
{
  ec_impl_->push(oid, data
       ACE_ENV_ARG_PARAMETER);
}

