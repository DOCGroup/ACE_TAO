/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs
//
// = FILENAME
//     CosEvent_Utilities.h
//
// = DESCRIPTION
//    A few utility classes to make it easier to write EC applications.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_COSEVENT_UTILITIES_H
#define TAO_COSEVENT_UTILITIES_H

#include "orbsvcs/RtecSchedulerC.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/Event_Utilities.h"

class TAO_CosEC_EventChannel_i;

class CosEC_ServantBase :
  public virtual POA_CosEventChannelAdmin::EventChannel,
  public virtual PortableServer::RefCountServantBase
{
  // = TITLE
  //   A generic servant base class.
  // = DESCRIPTION
  //   This class contains all the member data and methods required to
  //   create and manage a CosEC servant.
  //   Clients can derive from this class and create servants.
 public:
  // = Initialization method.
  CosEC_ServantBase (void);
  // Constructor.

  virtual ~CosEC_ServantBase (void);
  // Destructor.

  virtual void init (PortableServer::POA_ptr thispoa,
                     PortableServer::POA_ptr poa,
                     char *eventTypeIds,
                     char *eventSourceIds,
                     char *source_type_pairs,
                     CORBA::Environment &ACE_TRY_ENV);
  // This method creates a local scheduler, rtec and cosec.
  // The POA <poa> specified here is used when <activate> is called to
  // activate the contained servants.
  // The POA <thispoa> is used to activate this.

  int activate (CORBA::Environment &ACE_TRY_ENV);
  // Activates the CosEC with <thispoa_> and friends with the <poa_>

  int activate (const char* servant_id, CORBA::Environment &ACE_TRY_ENV);
  // If the servant_id is not nil then it is used to supply the object id
  // for <this> servant.

  void deactivate (CORBA::Environment &ACE_TRY_ENV);
  // Deactivates the CosEC and friends with the POA.

  // =  POA_CosEventChannelAdmin::EventChannel methods.
  virtual CosEventChannelAdmin::ConsumerAdmin_ptr for_consumers (CORBA::Environment &ACE_TRY_ENV);

  virtual CosEventChannelAdmin::SupplierAdmin_ptr for_suppliers (CORBA::Environment &ACE_TRY_ENV);

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV);
  // Destroys this Event Channel object.

 protected:
  // = RtEC creation, activation and deactivation methods.
  virtual POA_RtecEventChannelAdmin::EventChannel_ptr
  create_rtec (CORBA::Environment &ACE_TRY_ENV);
  // Create a local rtec.

  virtual void activate_rtec (CORBA::Environment &ACE_TRY_ENV);
  // Activates the rtec.

  virtual void deactivate_rtec (CORBA::Environment &ACE_TRY_ENV);
  // Deactivates the rtec.

  // = CosEC creation, activation and deactivation methods.
  TAO_CosEC_EventChannel_i*
  create_cosec (CORBA::Environment &ACE_TRY_ENV);
  // Create a local cosec.

  int activate_cosec (CORBA::Environment &ACE_TRY_ENV);
  // Activates the cosec.

  void deactivate_cosec (CORBA::Environment &ACE_TRY_ENV);
  // Deactivates the cosec.

  void init_SupplierQOS (RtecScheduler::handle_t supp_handle,
                         ACE_SupplierQOS_Factory &supplier_qos,
                         char *source_type_pairs);
  // Initialize the SupplierQOS Factory.


  void init_ConsumerQOS (RtecScheduler::handle_t cons_handle,
                         ACE_ConsumerQOS_Factory &consumer_qos,
                         char *eventTypeIds,
                         char *eventSourceIds);
  // Initialize the ConsumerQOS Factory.

  // = Protected Data members.

  PortableServer::POA_var thispoa_;
  // The poa that we use to activate ourselves.

  PortableServer::POA_var poa_;
  // The poa that we use to activate others

  POA_RtecEventChannelAdmin::EventChannel_ptr rtec_servant_;
  // The Event Channel servant.

  TAO_CosEC_EventChannel_i *cosec_servant_;
  // The servant object of the COS Event Channel.

  RtecEventChannelAdmin::EventChannel_var rtec_;
  // Ref to the Rtec.

  CosEventChannelAdmin::EventChannel_var cosec_;
  // Ref to the cosec.

  ACE_ConsumerQOS_Factory consumer_qos_;
  // The Consumer QOS.

  ACE_SupplierQOS_Factory supplier_qos_;
  // The Supplier QOS.

  char *eventTypeIds_;
  // The list of EventTypeIDs (for ConsumerQOS) seperated by spaces.
  // e.g. "1 2 3 4"

  char *eventSourceIds_;
  // The list of EventSourceIDs (for ConsumerQOS) seperated by spaces.
  // e.g. "1 2 3 4"

  char *source_type_pairs_;
  // The pairs of Source and EventType Ids (for the SupplierQOS).
  // e.g "1 4 2 5 3 6" where (1,4) (2,5) and (3,6) from source id,
  // event id pairs.
};
#endif /* TAO_COSEVENT_UTILITIES_H */
