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
#include "ace/pre.h"

#include "orbsvcs/RtecBaseC.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/Event_Utilities.h"
#include "rtec_cosec_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_CosEC_EventChannel_i;

class TAO_RTEC_COSEC_Export CosEC_ServantBase :
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
                     char *source_type_pairs
                     ACE_ENV_ARG_DECL);
  // This method creates a local scheduler, rtec and cosec.
  // The POA <poa> specified here is used when <activate> is called to
  // activate the contained servants.
  // The POA <thispoa> is used to activate this.

  int activate (ACE_ENV_SINGLE_ARG_DECL);
  // Activates the CosEC with <thispoa_> and friends with the <poa_>

  int activate (const char* servant_id ACE_ENV_ARG_DECL);
  // If the servant_id is not nil then it is used to supply the object id
  // for <this> servant.

  void deactivate (ACE_ENV_SINGLE_ARG_DECL);
  // Deactivates the CosEC and friends with the POA.

  // =  POA_CosEventChannelAdmin::EventChannel methods.
  virtual CosEventChannelAdmin::ConsumerAdmin_ptr for_consumers (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CosEventChannelAdmin::SupplierAdmin_ptr for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Destroys this Event Channel object.

 protected:
  // = RtEC creation, activation and deactivation methods.
  virtual POA_RtecEventChannelAdmin::EventChannel_ptr
  create_rtec (ACE_ENV_SINGLE_ARG_DECL);
  // Create a local rtec.

  virtual void activate_rtec (ACE_ENV_SINGLE_ARG_DECL);
  // Activates the rtec.

  virtual void deactivate_rtec (ACE_ENV_SINGLE_ARG_DECL);
  // Deactivates the rtec.

  // = CosEC creation, activation and deactivation methods.
  TAO_CosEC_EventChannel_i*
  create_cosec (ACE_ENV_SINGLE_ARG_DECL);
  // Create a local cosec.

  int activate_cosec (ACE_ENV_SINGLE_ARG_DECL);
  // Activates the cosec.

  void deactivate_cosec (ACE_ENV_SINGLE_ARG_DECL);
  // Deactivates the cosec.

  void init_SupplierQOS (RtecBase::handle_t supp_handle,
                         ACE_SupplierQOS_Factory &supplier_qos,
                         char *source_type_pairs);
  // Initialize the SupplierQOS Factory.


  void init_ConsumerQOS (RtecBase::handle_t cons_handle,
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


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_COSEVENT_UTILITIES_H */
