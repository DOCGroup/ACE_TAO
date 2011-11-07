/* -*- C++ -*- */

//=============================================================================
/**
 *  @file     CosEvent_Utilities.h
 *
 *  $Id$
 *
 *  A few utility classes to make it easier to write EC applications.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_COSEVENT_UTILITIES_H
#define TAO_COSEVENT_UTILITIES_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecBaseC.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/Event_Utilities.h"
#include "rtec_cosec_export.h"

class TAO_CosEC_EventChannel_i;

class TAO_RTEC_COSEC_Export CosEC_ServantBase :
  public virtual POA_CosEventChannelAdmin::EventChannel
{
  // = TITLE
  //   A generic servant base class.
  // = DESCRIPTION
  //   This class contains all the member data and methods required to
  //   create and manage a CosEC servant.
  //   Clients can derive from this class and create servants.
 public:
  // = Initialization method.
  /// Constructor.
  CosEC_ServantBase (void);

  /// Destructor.
  virtual ~CosEC_ServantBase (void);

  /**
   * This method creates a local scheduler, rtec and cosec.
   * The POA <poa> specified here is used when <activate> is called to
   * activate the contained servants.
   * The POA <thispoa> is used to activate this.
   */
  virtual void init (PortableServer::POA_ptr thispoa,
                     PortableServer::POA_ptr poa,
                     ACE_TCHAR *eventTypeIds,
                     ACE_TCHAR *eventSourceIds,
                     ACE_TCHAR *source_type_pairs);

  /// Activates the CosEC with <thispoa_> and friends with the <poa_>
  int activate (void);

  /// If the servant_id is not nil then it is used to supply the object id
  /// for <this> servant.
  int activate (const char* servant_id);

  /// Deactivates the CosEC and friends with the POA.
  void deactivate (void);

  // =  POA_CosEventChannelAdmin::EventChannel methods.
  virtual CosEventChannelAdmin::ConsumerAdmin_ptr for_consumers (void);

  virtual CosEventChannelAdmin::SupplierAdmin_ptr for_suppliers (void);

  /// Destroys this Event Channel object.
  virtual void destroy (void);

 protected:
  // = RtEC creation, activation and deactivation methods.
  /// Create a local rtec.
  virtual POA_RtecEventChannelAdmin::EventChannel_ptr
  create_rtec (void);

  /// Activates the rtec.
  virtual void activate_rtec (void);

  /// Deactivates the rtec.
  virtual void deactivate_rtec (void);

  // = CosEC creation, activation and deactivation methods.
  /// Create a local cosec.
  TAO_CosEC_EventChannel_i*
  create_cosec (void);

  /// Activates the cosec.
  int activate_cosec (void);

  /// Deactivates the cosec.
  void deactivate_cosec (void);

  /// Initialize the SupplierQOS Factory.
  void init_SupplierQOS (RtecBase::handle_t supp_handle,
                         ACE_SupplierQOS_Factory &supplier_qos,
                         ACE_TCHAR *source_type_pairs);


  /// Initialize the ConsumerQOS Factory.
  void init_ConsumerQOS (RtecBase::handle_t cons_handle,
                         ACE_ConsumerQOS_Factory &consumer_qos,
                         ACE_TCHAR *eventTypeIds,
                         ACE_TCHAR *eventSourceIds);

  // = Protected Data members.

  /// The poa that we use to activate ourselves.
  PortableServer::POA_var thispoa_;

  /// The poa that we use to activate others
  PortableServer::POA_var poa_;

  /// The Event Channel servant.
  POA_RtecEventChannelAdmin::EventChannel_ptr rtec_servant_;

  /// The servant object of the COS Event Channel.
  TAO_CosEC_EventChannel_i *cosec_servant_;

  /// Ref to the Rtec.
  RtecEventChannelAdmin::EventChannel_var rtec_;

  /// Ref to the cosec.
  CosEventChannelAdmin::EventChannel_var cosec_;

  /// The Consumer QOS.
  ACE_ConsumerQOS_Factory consumer_qos_;

  /// The Supplier QOS.
  ACE_SupplierQOS_Factory supplier_qos_;

  /// The list of EventTypeIDs (for ConsumerQOS) seperated by spaces.
  /// e.g. "1 2 3 4"
  ACE_TCHAR *eventTypeIds_;

  /// The list of EventSourceIDs (for ConsumerQOS) seperated by spaces.
  /// e.g. "1 2 3 4"
  ACE_TCHAR *eventSourceIds_;

  /**
   * The pairs of Source and EventType Ids (for the SupplierQOS).
   * e.g "1 4 2 5 3 6" where (1,4) (2,5) and (3,6) from source id,
   * event id pairs.
   */
  ACE_TCHAR *source_type_pairs_;
};

#include /**/ "ace/post.h"
#endif /* TAO_COSEVENT_UTILITIES_H */
