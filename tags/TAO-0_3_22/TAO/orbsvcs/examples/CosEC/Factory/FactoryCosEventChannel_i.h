// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/examples/CosEC/Factory
//
// = FILENAME
//     FactoryCosEventChannel_i.h
//
// = DESCRIPTION
//    Decorator for the CosEventChannel.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef FACTORYCOSEVENTCHANNEL_I_H
#define FACTORYCOSEVENTCHANNEL_I_H

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/Event_Utilities.h"

class ACE_Config_Scheduler;
class TAO_EC_Event_Channel;
class TAO_CosEC_EventChannel_i;

class FactoryCosEventChannel_i :
  public virtual POA_CosEventChannelAdmin::EventChannel ,
  public virtual TAO_RefCountServantBase
{
  // = TITLE
  //
  // = DESCRIPTION
  //
  //
 public:
  // = Initialization method.
  FactoryCosEventChannel_i (void);
  // Constructor.

  virtual ~FactoryCosEventChannel_i (void);
  // Destructor.

  int init (PortableServer::POA_ptr poa,
            CORBA::Environment &ACE_TRY_ENV);
  //
  // Returns -1 on error, 0 on success.

  int activate (ACE_CString& str_channel_id,
                CORBA::Environment &ACE_TRY_ENV);
  // Activates the CosEC and friends with the POA.

  // =  POA_CosEventChannelAdmin::EventChannel methods.
  virtual CosEventChannelAdmin::ConsumerAdmin_ptr for_consumers (CORBA::Environment &ACE_TRY_ENV);

  virtual CosEventChannelAdmin::SupplierAdmin_ptr for_suppliers (CORBA::Environment &ACE_TRY_ENV);

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV);
  // Destroys this Event Channel object.

 protected:
  ACE_Config_Scheduler* sched_servant_;
  // Scheduler used by the Rtec.

  PortableServer::POA_var poa_;
  // The poa that we use.

  TAO_EC_Event_Channel* ec_servant_;
  // The servant object of the Rtec.

  TAO_CosEC_EventChannel_i* cosec_servant_;
  // The servant object of the COS Event Channel.

  RtecScheduler::Scheduler_var scheduler_;
  // Ref to sched.

  RtecEventChannelAdmin::EventChannel_var rtec_;
  // Ref to the Rtec.

  CosEventChannelAdmin::EventChannel_var cosec_;
  // Ref to the cosec.

  ACE_ConsumerQOS_Factory consumer_qos_;
  // The Consumer QOS.

  ACE_SupplierQOS_Factory supplier_qos_;
  // The Supplier QOS.
};

#endif /* FACTORYCOSEVENTCHANNEL_I_H */
