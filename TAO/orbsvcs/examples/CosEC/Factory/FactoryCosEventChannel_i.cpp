// -*- C++ -*-
// $Id$

#include "FactoryCosEventChannel_i.h"

#include "ace/Auto_Ptr.h"

// @@ Pradeep: if you are using the new TAO_EC_Event_Channel you
//    *don't* need a scheduler.
//    Ohh, and you don't need to create the TAO_EC_Basic_Factory, I
//    have changed the EC to dynamically load the right factory,
//    configurable through the svc.conf file! We can discuss later
//    what is the right configuration for the real-time EC.
//    In general the code here looks a bit bulky, I apologize that I
//    cannot take a better look, please remind me to do so once I'm
//    back to work.

FactoryCosEventChannel_i::FactoryCosEventChannel_i (void)
  :poa_ (PortableServer::POA::_nil ()),
   ec_servant_ (0),
   cosec_servant_ (0),
   scheduler_ (RtecScheduler::Scheduler::_nil ()),
   rtec_ (RtecEventChannelAdmin::EventChannel::_nil ()),
   cosec_ (CosEventChannelAdmin::EventChannel::_nil ())
{
  // No-Op.
}

FactoryCosEventChannel_i::~FactoryCosEventChannel_i (void)
{
#if 0
  ACE_DEBUG ((LM_DEBUG,
              "In FactoryCosEventChannel_i %d dtor\n",
              this));
#endif
}

int
FactoryCosEventChannel_i::init (PortableServer::POA_ptr poa,
                                CORBA::Environment &ACE_TRY_ENV)
{
  ACE_ASSERT (poa != PortableServer::POA::_nil ());
  this->poa_ = PortableServer::POA::_duplicate (poa);

  // Create the Scheduler servant
  ACE_Config_Scheduler* _sched_servant;
  ACE_NEW_RETURN (_sched_servant,
                  ACE_Config_Scheduler (),
                  -1);
  auto_ptr<ACE_Config_Scheduler> auto_sched_servant_ (_sched_servant);

  // Create the RtEC servant.
  // @@ Pradeep: Please make sure that the POA policies are similar to
  //    the RootPOA, this is the POA used by the RTEC to activate its
  //    internal objects.
  TAO_EC_Event_Channel_Attributes attr (this->poa_.in (),
                                        this->poa_.in ());
  TAO_EC_Event_Channel* _ec_servant;
  ACE_NEW_RETURN (_ec_servant,
                  TAO_EC_Event_Channel (attr),
                  -1);
  auto_ptr<TAO_EC_Event_Channel> auto_ec_servant_ (_ec_servant);

  // Create the CosEC servant.
  TAO_CosEC_EventChannel_i* _cosec_servant;
  ACE_NEW_RETURN (_cosec_servant,
                  TAO_CosEC_EventChannel_i (),
                  -1);
  auto_ptr<TAO_CosEC_EventChannel_i> auto_cosec_servant_ (_cosec_servant);

  // if all the servants where allocated then set the class pointers.
  this->sched_servant_ = auto_sched_servant_.release ();
  this->ec_servant_ = auto_ec_servant_.release ();
  this->cosec_servant_ = auto_cosec_servant_.release ();

  return 0; // success.
}

// @@ Pradeep: should we remove this? After all it is kept by CVS.
#if 0
int
FactoryCosEventChannel_i::activate (ACE_CString& str_channel_id,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_ASSERT (this->poa_ != PortableServer::POA::_nil ());

  // @@ Pradeep: don't use the same POA to activate the objects
  //    created by the factory and your internal objects: what if the
  //    user creates two event channels with names "foo" and
  //    "foo_Sched"?

  // Start the scheduler.
    PortableServer::ObjectId_var oid_sched =
    TAO_POA::string_to_ObjectId ((str_channel_id + "_Sched").c_str ());

  this->poa_->activate_object_with_id (oid_sched,
                                       this->sched_servant_,
                                       ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
  CORBA::Object_var obj_sched =
    this->poa_->id_to_reference (oid_sched,
                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);


  this->sched_servant_->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->scheduler_ =
    RtecScheduler::Scheduler::_narrow (obj_sched.in (),
                                       ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (ACE_Scheduler_Factory::server (this->scheduler_.in ()) == -1)
    return -1;

  //Activate the Rtec
  this->ec_servant_->activate (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Start the RtEC

  PortableServer::ObjectId_var oid_rtec =
    TAO_POA::string_to_ObjectId ((str_channel_id + "_RtEC").c_str ());

 this->poa_->activate_object_with_id (oid_rtec,
                                      this->ec_servant_,
                                      ACE_TRY_ENV);
 ACE_CHECK_RETURN (-1);

 CORBA::Object_var obj_rtec =
    this->poa_->id_to_reference (oid_rtec,
                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Give the ownership to the POA.
  this->ec_servant_->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->rtec_ =
    RtecEventChannelAdmin::EventChannel::_narrow (obj_rtec.in (),
                                                  ACE_TRY_ENV);

  ACE_CHECK_RETURN (-1);

  // Initialize the CosEC servant.
  RtecScheduler::handle_t supp_handle =
    this->scheduler_->create ("supplier",
                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->supplier_qos_.insert (1,
                              ACE_ES_EVENT_ANY,
                              supp_handle,
                              1);

  RtecScheduler::handle_t cons_handle =
    this->scheduler_->create ("consumer",
                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->consumer_qos_.start_disjunction_group ();
  this->consumer_qos_.insert_source (1, // default = 1
                                     cons_handle);

  const RtecEventChannelAdmin::ConsumerQOS &consumerqos =
    this->consumer_qos_.get_ConsumerQOS ();

  const RtecEventChannelAdmin::SupplierQOS &supplierqos =
    this->supplier_qos_.get_SupplierQOS ();

  if (this->cosec_servant_->init (consumerqos,
                                  supplierqos,
                                  this->rtec_.in (),
                                  ACE_TRY_ENV) != 0)
    return -1;
  ACE_CHECK_RETURN (-1);

    // Start the CosEC

  PortableServer::ObjectId_var oid_cosec =
   TAO_POA::string_to_ObjectId ((str_channel_id + "_CosEC").c_str ());

  this->poa_->activate_object_with_id (oid_cosec,
                                       this->cosec_servant_,
                                       ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var obj_cosec =
    this->poa_->id_to_reference (oid_cosec,
                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Give the ownership to the POA.
  this->cosec_servant_->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->cosec_ =
    CosEventChannelAdmin::EventChannel::_narrow (obj_cosec.in (),
                                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
}
#endif

int
FactoryCosEventChannel_i::activate (ACE_CString& str_channel_id,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  ACE_ASSERT (this->poa_ != PortableServer::POA::_nil ());

  this->scheduler_ = this->sched_servant_->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->sched_servant_->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  /*if (ACE_Scheduler_Factory::server (this->scheduler_.in ()) == -1)
    return -1;*/

  //Activate the Rtec
  this->ec_servant_->activate (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->rtec_ = this->ec_servant_->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

 // Give the ownership to the POA.
  this->ec_servant_->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

 // Initialize the CosEC servant.
  RtecScheduler::handle_t supp_handle =
    this->scheduler_->create ("supplier",
                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->supplier_qos_.insert (1,
                              ACE_ES_EVENT_ANY,
                              supp_handle,
                              1);

  RtecScheduler::handle_t cons_handle =
    this->scheduler_->create ("consumer",
                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  this->consumer_qos_.start_disjunction_group ();
  this->consumer_qos_.insert_source (1, // default = 1
                                     cons_handle);

  const RtecEventChannelAdmin::ConsumerQOS &consumerqos =
    this->consumer_qos_.get_ConsumerQOS ();

  const RtecEventChannelAdmin::SupplierQOS &supplierqos =
    this->supplier_qos_.get_SupplierQOS ();

  if (this->cosec_servant_->init (consumerqos,
                                  supplierqos,
                                  this->rtec_.in (),
                                  ACE_TRY_ENV) != 0)
    return -1;
  ACE_CHECK_RETURN (-1);

  // Start the CosEC
  this->cosec_ = this->cosec_servant_->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

 // Give the ownership to the POA.
  this->cosec_servant_->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);
}

CosEventChannelAdmin::ConsumerAdmin_ptr
FactoryCosEventChannel_i::for_consumers (CORBA::Environment &ACE_TRY_ENV)
{
  return this->cosec_->for_consumers (ACE_TRY_ENV);
}

CosEventChannelAdmin::SupplierAdmin_ptr
FactoryCosEventChannel_i::for_suppliers (CORBA::Environment &ACE_TRY_ENV)
{
  return this->cosec_->for_suppliers (ACE_TRY_ENV);
}

void
FactoryCosEventChannel_i::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  // Deactivate all those we control...

  // first the cosec.
  PortableServer::ObjectId* oid_cosec =
    this->poa_->servant_to_id (this->cosec_servant_,
                               ACE_TRY_ENV);
  ACE_CHECK;

  // deactivate from the poa.
  this->poa_->deactivate_object (*oid_cosec,
                                 ACE_TRY_ENV);
  ACE_CHECK;

  // next the rtec.
  PortableServer::ObjectId* oid_rtec =
    this->poa_->servant_to_id (this->ec_servant_,
                               ACE_TRY_ENV);
  ACE_CHECK;

  // deactivate from the poa.
  this->poa_->deactivate_object (*oid_rtec,
                                 ACE_TRY_ENV);
  ACE_CHECK;

  // lastly the scheduler.
 PortableServer::ObjectId* oid_sched =
    this->poa_->servant_to_id (this->sched_servant_,
                               ACE_TRY_ENV);
 ACE_CHECK;

 // deactivate from the poa.
  this->poa_->deactivate_object (*oid_sched,
                                 ACE_TRY_ENV);
  ACE_CHECK;
}
