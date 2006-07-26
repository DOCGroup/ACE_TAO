// -*- C++ -*-
// $Id$

#include "CosEvent_Utilities.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "EventChannel_i.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_string.h"

CosEC_ServantBase::CosEC_ServantBase (void)
  :poa_ (PortableServer::POA::_nil ()),
   rtec_servant_ (0),
   cosec_servant_ (0),
   rtec_ (RtecEventChannelAdmin::EventChannel::_nil ()),
   cosec_ (CosEventChannelAdmin::EventChannel::_nil ()),
   eventTypeIds_ (0),
   eventSourceIds_ (0),
   source_type_pairs_ (0)
{
  // No-Op.
}

CosEC_ServantBase::~CosEC_ServantBase (void)
{
  // No-Op.
#if 0
  ACE_DEBUG ((LM_DEBUG, "in cosec servant base %d \n", this));
#endif
}

void
CosEC_ServantBase::init (PortableServer::POA_ptr thispoa,
                         PortableServer::POA_ptr poa,
                         char *,
                         char *,
                         char *
                         ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (thispoa));
  ACE_ASSERT (!CORBA::is_nil (poa));

  // Save the POA refs.
  this->thispoa_ = PortableServer::POA::_duplicate (thispoa);
  this->poa_ = PortableServer::POA::_duplicate (poa);

  auto_ptr<POA_RtecEventChannelAdmin::EventChannel>
    auto_rtec_servant_ (this->create_rtec (ACE_ENV_SINGLE_ARG_PARAMETER));
  ACE_CHECK;

  auto_ptr<TAO_CosEC_EventChannel_i>
    auto_cosec_servant_ (this->create_cosec (ACE_ENV_SINGLE_ARG_PARAMETER));
  ACE_CHECK;

  // if all the servants were allocated then set the class pointers.
  this->rtec_servant_ = auto_rtec_servant_.release ();
  this->cosec_servant_ = auto_cosec_servant_.release ();
}

int
CosEC_ServantBase::activate (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));
  ACE_ASSERT (!CORBA::is_nil (this->thispoa_.in ()));

  // Activate the Rtec
  this->activate_rtec (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Activate the CosEC
  int retval = this->activate_cosec (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  if (retval != 0)
    return -1;

   // Activate ourselves..
   // Note that the POA is <thispoa_>

   PortableServer::ObjectId_var oid =
     this->thispoa_->activate_object (this
                                      ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   this->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   CORBA::Object_var obj =
     this->thispoa_->id_to_reference (oid.in ()
                                      ACE_ENV_ARG_PARAMETER);
   ACE_CHECK_RETURN (-1);

   return 0; // success.
}

int
CosEC_ServantBase::activate (const char* servant_id
                             ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->poa_.in ()));
  ACE_ASSERT (!CORBA::is_nil (this->thispoa_.in ()));

  // Activate the Rtec
  this->activate_rtec (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Activate the CosEC
  int retval = this->activate_cosec (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  if (retval != 0)
    return -1;

  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId (servant_id);

  // Activate ourselves.
  // Note that the POA is <thispoa_>
  this->thispoa_->activate_object_with_id (oid.in (),
                                           this
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var obj =
    this->thispoa_->id_to_reference (oid.in ()
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0; // success.
}

void
CosEC_ServantBase::activate_rtec (ACE_ENV_SINGLE_ARG_DECL)
{
  // Activate the Rtec
  PortableServer::ObjectId_var oid =
    this->poa_->activate_object (this->rtec_servant_
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->rtec_servant_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var obj =
    this->poa_->id_to_reference (oid.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->rtec_ =
    RtecEventChannelAdmin::EventChannel::_narrow (obj.in ());
}

int
CosEC_ServantBase::activate_cosec (ACE_ENV_SINGLE_ARG_DECL)
{
 // Initialize the CosEC servant.
  RtecBase::handle_t supp_handle = 0;

  this->init_SupplierQOS (supp_handle,
                          this->supplier_qos_,
                          this->source_type_pairs_);

  RtecBase::handle_t cons_handle = 0;

  this->init_ConsumerQOS (cons_handle,
                          this->consumer_qos_,
                          this->eventTypeIds_,
                          this->eventSourceIds_);

  const RtecEventChannelAdmin::ConsumerQOS &consumerqos =
    this->consumer_qos_.get_ConsumerQOS ();

  const RtecEventChannelAdmin::SupplierQOS &supplierqos =
    this->supplier_qos_.get_SupplierQOS ();

  if (this->cosec_servant_->init (consumerqos,
                                  supplierqos,
                                  this->rtec_.in ()
                                  ACE_ENV_ARG_PARAMETER) != 0)
    return -1;
  ACE_CHECK_RETURN (-1);

  // Activate the CosEC
  PortableServer::ObjectId_var oid =
    this->poa_->activate_object (this->cosec_servant_
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->cosec_servant_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::Object_var obj =
    this->poa_->id_to_reference (oid.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->cosec_ =
    CosEventChannelAdmin::EventChannel::_narrow (obj.in ());
  return 0; // success
}

void
CosEC_ServantBase::deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  // Deactivate all those we control...
  this->deactivate_rtec (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->deactivate_cosec (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Finally we go away..
  PortableServer::ObjectId_var oid =
    this->thispoa_->servant_to_id (this
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // deactivate from the poa.
  this->thispoa_->deactivate_object (oid.in ()
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
CosEC_ServantBase::deactivate_rtec (ACE_ENV_SINGLE_ARG_DECL)
{
  // Deactivate the rtec.
  PortableServer::ObjectId_var oid =
    this->poa_->servant_to_id (this->rtec_servant_
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // deactivate from the poa.
  this->poa_->deactivate_object (oid.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
CosEC_ServantBase::deactivate_cosec (ACE_ENV_SINGLE_ARG_DECL)
{
  // Deactivate the cosec.
  PortableServer::ObjectId_var oid =
    this->poa_->servant_to_id (this->cosec_servant_
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // deactivate from the poa.
  this->poa_->deactivate_object (oid.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

CosEventChannelAdmin::ConsumerAdmin_ptr
CosEC_ServantBase::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->cosec_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::SupplierAdmin_ptr
CosEC_ServantBase::for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->cosec_->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
CosEC_ServantBase::destroy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Deactivate all the contained servants and ourselves.
  // The poa will "destroy" the ref counted servants.

  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

POA_RtecEventChannelAdmin::EventChannel_ptr
CosEC_ServantBase::create_rtec (ACE_ENV_SINGLE_ARG_DECL)
{
  // Create the RtEC servant.
  TAO_EC_Event_Channel_Attributes attr (this->poa_.in (),
                                        this->poa_.in ());
  TAO_EC_Event_Channel* _rtec_servant;
  ACE_NEW_THROW_EX (_rtec_servant,
                    TAO_EC_Event_Channel (attr),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return _rtec_servant;
}

TAO_CosEC_EventChannel_i*
CosEC_ServantBase::create_cosec (ACE_ENV_SINGLE_ARG_DECL)
{
  // Create the CosEC servant.
  TAO_CosEC_EventChannel_i* _cosec_servant;
  ACE_NEW_THROW_EX (_cosec_servant,
                    TAO_CosEC_EventChannel_i (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  return _cosec_servant;
}

void
CosEC_ServantBase::init_SupplierQOS (RtecBase::handle_t supp_handle,
                                     ACE_SupplierQOS_Factory &supplier_qos,
                                     char *source_type_pairs)
{
  // @@ Pradeep: It is very important that you make the type of
  // events generated by the CosEC an option.
  // I know this is not very well documented, but the type should
  // be >= ACE_ES_EVENT_UNDEFINED = 16
  // Something else: please make the EventSourceID for the
  // supplier also an option...

  const char *c = " "; // space
  char *tok = 0;

 // if nothing was specified on the command line use defaults..
  if (source_type_pairs == 0)
    supplier_qos.insert (1,
                         ACE_ES_EVENT_ANY,
                         supp_handle,
                         1);
  else // parse the event types..
    {
      tok = ACE_OS::strtok (source_type_pairs, c);
      if (tok == 0) // error
        {
          ACE_DEBUG ((LM_DEBUG, "error parsing source,event pairs for SupplierQOS, defaulting to source id = 1, eventid = ACE_ES_EVENT_ANY"));

          supplier_qos.insert (1,
                               ACE_ES_EVENT_ANY,
                               supp_handle,
                               1);
        }
      else
        // we just use 1 source-type pair in the event channel.
        // so scan for the 1st pair only.
        {
          int source_val = 0, type_val = 0;
          source_val = ACE_OS::atoi (tok);

          tok = ACE_OS::strtok (0, c);

          if (tok != 0)
            type_val = ACE_OS::atoi (tok);

          ACE_DEBUG ((LM_DEBUG, "supplier_qos::insert (%d, %d)\n",
                      source_val, type_val));

          // Setup the QOS params..
          supplier_qos.insert (source_val,
                               type_val,
                               supp_handle,
                               1);
        }
    }
}

void
CosEC_ServantBase::init_ConsumerQOS (RtecBase::handle_t cons_handle,
                                     ACE_ConsumerQOS_Factory &consumer_qos,
                                     char *eventTypeIds,
                                     char *eventSourceIds
                                     )
{
  // @@ Pradeep: ditto here, make the set of sources (and/or type)
  // a parameter, and make sure the user can specify multiple of
  // them (just call insert_source() or insert_type() in the
  // parse_args routine).

  const char *c = " "; // space
  char *tok = 0;

  consumer_qos.start_disjunction_group ();

  // insert the event ids first..

  // if nothing was specified on the command line use defaults..
  if (eventTypeIds == 0)
    {
      //consumer_qos.insert_type (ACE_ES_EVENT_ANY, // default
      //                             cons_handle);
      // @@ if i uncomment this line then the Rtec displays the message
      // "Consumer tried to register for allevents!  This is not implemented."
      // whenever a consumer tries to register with it.
    }
  else // parse the event types..
    {
      tok = ACE_OS::strtok (eventTypeIds, c);
      if (tok == 0) // error
        {
          ACE_DEBUG ((LM_DEBUG, "error parsing eventIds for ConsumerQOS, defaulting to 1"));
          consumer_qos.insert_type (ACE_ES_EVENT_ANY,
                                    cons_handle);
        }
      else
        do
          {
            int type_val = ACE_OS::atoi (tok);
            ACE_DEBUG ((LM_DEBUG, "consumer_qos::insert_type (%d)\n",
                        type_val));
            consumer_qos.insert_type (type_val,
                                      cons_handle);
            tok = ACE_OS::strtok (0, c);
          }
        while (tok != 0);
    }

  // repeat for source ids..

  // if nothing was specified on the command line use defaults..
  if (eventSourceIds == 0)
    consumer_qos.insert_source (1, // default = 1
                                cons_handle);
  else // parse the event types..
    {
      tok = ACE_OS::strtok (eventSourceIds, c);
      if (tok == 0) // error
        {
          ACE_DEBUG ((LM_DEBUG, "error parsing sourceIds for ConsumerQOS, defaulting to 1"));
          consumer_qos.insert_source (1, // default = 1
                                      cons_handle);
        }
      else
        do
          {
            int source_val = ACE_OS::atoi (tok);
            ACE_DEBUG ((LM_DEBUG, "consumer_qos::insert_source (%d)\n",
                        source_val));
            consumer_qos.insert_type (source_val,
                                      cons_handle);
            tok = ACE_OS::strtok (0, c);
          }
        while (tok != 0);
    }
}

