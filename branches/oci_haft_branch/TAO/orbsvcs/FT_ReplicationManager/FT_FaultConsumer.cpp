/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_FaultConsumer.cpp
 *
 *  $Id$
 *
 *  This file is part of TAO's implementation of Fault Tolerant CORBA.
 *  This file provides the implementation of the TAO::FT_FaultConsumer
 *  class.  The TAO::FT_FaultConsumer connects to the FaultNotifier to
 *  receive fault reports.  It interacts with the ReplicationManager
 *  to process fault reports (e.g., to set a new primary on an object
 *  group or to create a new member of an object group).
 *
 *  @author Steve Totten <totten_s@ociweb.com>
 */
//=============================================================================

#include "orbsvcs/FT_ReplicationManager/FT_FaultConsumer.h"
#include "orbsvcs/FT_ReplicationManagerC.h"
#include <tao/debug.h>

ACE_RCSID (FT_FaultConsumer,
           FT_FaultConsumer,
           "$Id$")

/// Default constructor.
TAO::FT_FaultConsumer::FT_FaultConsumer ()
  : poa_ (PortableServer::POA::_nil ())
  , fault_notifier_ (FT::FaultNotifier::_nil ())
  , replication_manager_ (FT::ReplicationManager::_nil ())
  , consumer_id_ (0)
  , consumer_ref_ (CosNotifyComm::StructuredPushConsumer::_nil ())
  , notifications_ (0)
{
}

/// Destructor.
TAO::FT_FaultConsumer::~FT_FaultConsumer ()
{
}

/**
* Connect to the FT::FaultNotifier.
* Note: We make the following assumptions about what the
* application will do:
* - Create an instance of this consumer class.
* - Obtain the object reference of the FaultNotifier to which this
*   consumer should connect.
* - Call this init() method, passing it the POA with which we
*   have been activated, the FaultNotifier, and ReplicationManager
*   object references.
*/
int TAO::FT_FaultConsumer::init (
  PortableServer::POA_ptr poa,
  FT::FaultNotifier_ptr fault_notifier,
  FT::ReplicationManager_ptr replication_manager
  ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (poa));
  ACE_ASSERT (!CORBA::is_nil (fault_notifier));
  ACE_ASSERT (!CORBA::is_nil (replication_manager));

  // Duplicate the object references passed in.
  this->poa_ =
    PortableServer::POA::_duplicate (poa);
  this->fault_notifier_ =
    FT::FaultNotifier::_duplicate (fault_notifier);
  this->replication_manager_ =
    FT::ReplicationManager::_duplicate (replication_manager);

  //@@ Should this init() method activate the consumer in the POA, or
  // should the application do that?
  // I don't think this object can activate itself because it doesn't
  // know the policies on the POA.  So, we assume the application has
  // already activated us.
  //@@ For now, let's try just activating it in the POA.

  // Activate this consumer in the POA.
  this->object_id_ = this->poa_->activate_object (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  CORBA::Object_var obj =
    this->poa_->id_to_reference (this->object_id_.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Narrow it to CosNotifyComm::StructuredPushConsumer.
  this->consumer_ref_ = CosNotifyComm::StructuredPushConsumer::_narrow (
    obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Subscribe to the FaultNotifier.
  CosNotifyFilter::Filter_var filter = CosNotifyFilter::Filter::_nil ();
  this->consumer_id_ = fault_notifier_->connect_structured_fault_consumer (
    this->consumer_ref_.in(), filter ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Success.
  return 0;
}

/**
* Clean house for process shut down.
* - Disconnect from FT::FaultNotifier.
* - Deactivate from the POA.
*/
int TAO::FT_FaultConsumer::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  // Disconnect from the FaultNotifier.
  this->fault_notifier_->disconnect_consumer (this->consumer_id_
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  //@@ Should this fini() method deactivate the consumer from the POA, or
  // should the application do that?
  //
  // Deactivate ourselves from the POA.
  this->poa_->deactivate_object (this->object_id_.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->consumer_ref_ = CosNotifyComm::StructuredPushConsumer::_nil ();

  // Success.
  return 0;
}

CosNotifyComm::StructuredPushConsumer_ptr
TAO::FT_FaultConsumer::consumer_ref ()
{
  return CosNotifyComm::StructuredPushConsumer::_duplicate (
      this->consumer_ref_);
}

size_t TAO::FT_FaultConsumer::notifications () const
{
  return this->notifications_;
}


////////////////
// CORBA methods

// Receive and process an incoming fault event from the Fault Notifier.
// First, we validate the event to make sure it is something we can
// handle.  Then, we analyze it.
// If it is not an event we can handle, we simply log the error and
// drop the event.
void TAO::FT_FaultConsumer::push_structured_event (
  const CosNotification::StructuredEvent &event
  ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((CORBA::SystemException, CosEventComm::Disconnected))
{
  // Debugging support.
  this->notifications_ += 1;
#if (TAO_DEBUG_LEVEL_NEEDED == 1)
  if (TAO_debug_level > 1)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
  {
    ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT (
        "TAO::FT_FaultConsumer::push_structured_event: "
        "Received Fault notification(%d):\n"),
      ACE_static_cast (unsigned int, this->notifications_)
    ));
  }

  int result = 0;

  // Make sure it is an event type we can handle.
  if (result == 0)
  {
    result = this->validate_event_type (event);
    ACE_CHECK;
    if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT (
          "TAO::FT_FaultConsumer::push_structured_event: "
          "Received invalid fault event type.\n")
      ));
    }
  }

  // Analyze the event.
  if (result == 0)
  {
    TAO::FT_FaultEventDescriptor fault_event_desc;
    result = this->analyze_fault_event (
      event,
      fault_event_desc
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
    if (result != 0)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT (
          "TAO::FT_FaultConsumer::push_structured_event: "
          "Could not analyze fault event.\n")
      ));
    }

    // Debugging support.
#if (TAO_DEBUG_LEVEL_NEEDED == 1)
    if (TAO_debug_level > 6)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT("Getting the location as a string.\n")
      ));

      // Get the location as a string.
      ACE_CString loc_as_string;
      FT::Location_var & loc = fault_event_desc.location;

      for (CORBA::ULong li = 0; li < loc->length(); ++li)
      {
        if (li > 0) loc_as_string += "/";
        // Assume only the "id" field of the CosNaming::Name is used.
        loc_as_string += CORBA::string_dup (loc[li].id);
      }

      if (fault_event_desc.all_at_location_failed == 1)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT (
            "All objects at location <%s> failed.\n"),
            loc_as_string.c_str()
        ));
      }

      if (fault_event_desc.all_of_type_at_location_failed == 1)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT (
            "All objects of type <%s> at location <%s> failed.\n"),
            fault_event_desc.type_id.in(),
            loc_as_string.c_str()
        ));
      }

      if (fault_event_desc.object_at_location_failed == 1)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT (
            "Replica of type <%s> with ObjectGroupId <%Q> "
            "at location <%s> failed.\n"),
            fault_event_desc.type_id.in(),
            fault_event_desc.object_group_id,
            loc_as_string.c_str()
        ));
      }
    }
  }

  return;
}


void TAO::FT_FaultConsumer::offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
    ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((CORBA::SystemException, CosNotifyComm::InvalidEventType))
{
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT("TAO::FT_FaultConsumer::offer_change() call ignored.\n")
  ));
}

void TAO::FT_FaultConsumer::disconnect_structured_push_consumer (
    ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //TODO: For now, we are just ignoring the disconnect callback.
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT("TAO::FT_FaultConsumer::disconnect_structured_push_consumer() "
             "call ignored.\n")
    ));
}

//////////////////
// Private methods

// Validate the event to make sure it is one we can handle.
// If it is not an event we can handle, this function logs the error
// and returns -1.
int TAO::FT_FaultConsumer::validate_event_type (
  const CosNotification::StructuredEvent & event)
{
  int result = 0;

  // CORBA 3.0.2, section 23.4.5.1 states:
  //
  //   The fault management specification defines one event type:
  //   ObjectCrashFault. As the name suggests, this event is
  //   generated by a Fault Detector when it detects that an object
  //   has crashed.

  // So, the event header's event_type.domain_name must be "FT_CORBA"
  // and the event header's event_type.type_name must be "ObjectCrashFault".
  CORBA::String_var domain_name = CORBA::string_dup (
    event.header.fixed_header.event_type.domain_name);
  CORBA::String_var type_name = CORBA::string_dup (
      event.header.fixed_header.event_type.type_name);
  CORBA::String_var event_name = CORBA::string_dup (
      event.header.fixed_header.event_name);

  if (result == 0)
  {
    //TODO: Get rid of magic strings (though they come from the spec).
    if (ACE_OS::strcmp (domain_name.in(), "FT_CORBA") != 0 ||
        ACE_OS::strcmp (type_name.in(), "ObjectCrashFault") != 0)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT (
          "TAO::FT_FaultConsumer::validate_event_type: "
          "Received invalid event type.\n"
          "EventType domain: <%s>\n"
          "EventType type: <%s>\n"
          "EventName: <%s>\n"),
        domain_name.in(),
        type_name.in(),
        event_name.in()
      ));
      result = -1;
    }
  }

  // CORBA 3.0.2, section 23.4.5.1 also states:
  //
  //   The filterable_data part of the event body contains the
  //   identity of the crashed object as four name-value pairs: the
  //   fault tolerance domain identifier, the member’s location
  //   identifier, the repository identifier and the object group
  //   identifier. The Fault Notifier filters events based on the
  //   domain_name, the type_name, and the four identifiers.  All
  //   other fields of the structured event may be set to null.
  //
  //   The Fault Detector always sets the following fault event
  //   fields: domain_name, type_name, FTDomainId, and Location.
  //
  // So, at least "FTDomainId" and "Location" must be present:
  if (result == 0)
  {
    if (event.filterable_data.length () >= 2)
    {
      // Check for FTDomainId.
      if (ACE_OS::strcmp (
            event.filterable_data[0].name.in(), "FTDomainId") != 0)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT (
            "TAO::FT_FaultConsumer::validate_event_type: "
            "Received invalid structured event.\n"
            "filterable_data[0] must be \"FTDomainId\", not \"%s\"\n"),
          event.filterable_data[0].name.in()
        ));
        result = -1;
      }
      else if (ACE_OS::strcmp (
            event.filterable_data[1].name.in(), "Location") != 0)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT (
            "TAO::FT_FaultConsumer::validate_event_type: "
            "Received invalid structured event.\n"
            "filterable_data[1] must be \"Location\", not \"%s\"\n"),
          event.filterable_data[1].name.in()
        ));
        result = -1;
      }
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT (
          "TAO::FT_FaultConsumer::validate_event_type: "
          "Received invalid structured event.\n"
          "There must be at least two name/value pairs in "
          "the filterable_data field, for \"FTDomainId\" and \"Location\".\n")
      ));
      result = -1;
    }
  }

  return result;
}

/// Analyze a fault event.
int TAO::FT_FaultConsumer::analyze_fault_event (
  const CosNotification::StructuredEvent & event,
  TAO::FT_FaultEventDescriptor & fault_event_desc
  ACE_ENV_ARG_DECL_WITH_DEFAULTS
  )
  ACE_THROW_SPEC ((CORBA::SystemException, CosEventComm::Disconnected))
{
  int result = 0;

  const CosNotification::FilterableEventBody & filterable =
    event.filterable_data;
  CORBA::ULong item_count = filterable.length ();
#if (TAO_DEBUG_LEVEL_NEEDED == 1)
  if (TAO_debug_level > 6)
#endif /* (TAO_DEBUG_LEVEL_NEEDED == 1) */
  {
    for (CORBA::ULong n_prop = 0; n_prop < item_count; ++n_prop)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT("TAO::FT_FaultConsumer::analyze_fault_event: "
                 "Property Name: <%s>\n"),
        filterable[n_prop].name.in()
      ));
    }
  }

  //
  // Populate the TAO::FT_FaultEventDescriptor structure from the
  // properties in the event.
  //

  // Extract the location.
  if (result == 0)
  {
    FT::Location* temp_loc;
    if ((filterable[1].value >>= temp_loc) == 0)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT (
          "TAO::FT_FaultConsumer::analyze_fault_event: "
          "Could not extract Location value from fault event.\n")
      ));
      result = -1;
    }
    else
    {
      // Make a deep copy of the Location.
      FT::Location* copy_loc = 0;
      ACE_NEW_RETURN (copy_loc, FT::Location (*temp_loc), -1);
      fault_event_desc.location = copy_loc;
    }
  }

  // CORBA 3.0.2, section 23.4.5.1 states:
  //
  //   The fault detector may or may not set the TypeId and
  //   ObjectGroupId fields with the following interpretations:
  //   - Neither is set if all objects at the given location have failed.
  //   - TypeId is set and ObjectGroupId is not set if all objects at
  //     the given location with the given type have failed.
  //   - Both are set if the member with the given ObjectGroupId at the
  //     given location has failed.
  if (result == 0)
  {
    // All objects at location failed.
    if (item_count == 2)
    {
      fault_event_desc.all_at_location_failed = 1;
    }

    // All objects of type at location failed.
    if (item_count == 3)
    {
      fault_event_desc.all_of_type_at_location_failed = 1;
      fault_event_desc.type_id = this->extract_type_id (filterable[2].value);
    }

    // An object (replica) at a location failed.
    if (item_count == 4)
    {
      fault_event_desc.object_at_location_failed = 1;
      fault_event_desc.type_id = this->extract_type_id (filterable[2].value);
      fault_event_desc.object_group_id =
        this->extract_object_group_id (filterable[3].value);
    }
  }

  //TODO: Finish the implementation of analyze_fault_event() to:
  // - if the ReplicationStyle of the object group is FT::SEMI_ACTIVE,
  //   and if the failed replica was the primary replica, set a new
  //   primary replica
  // - if the number of replicas falls below MinimumNumberReplicas
  //   property and MembershipStyle of the object group is FT::MEMB_INF_CTRL,
  //   create and add a new member.

  //Q: How do we get the ObjectGroup from the ObjectGroupId?
  //A: ??

  //Q: Then, how do we get the properties of the ObjectGroup to
  // figure out the ReplicationStyle, current primary,
  // MinimumNumberReplicas, etc.?
  //A: FT::PropertyManager::get_properties().

  // Assuming we have the ObjectGroup reference...
  FT::ObjectGroup_var the_object_group;

  // Get the properties associated with this ObjectGroup.
#if 0
  if (result == 0)
  {
    FT::Properties_var properties =
      this->replication_manager_->get_properties (
        the_object_group.in()
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
  }
#endif

  return result;
}

// Extract a string type_id from CORBA::Any.
// Caller owns the returned string.
char*
TAO::FT_FaultConsumer::extract_type_id (const CORBA::Any& val)
{
  const char* type_id_value;
  if ((val >>= type_id_value) == 0)
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT (
        "TAO::FT_FaultConsumer::extract_type_id: "
        "Could not extract TypeId value from any.\n")
    ));
    return 0;
  }

  return CORBA::string_dup (type_id_value);
}

// Extract the ObjectGroupId from CORBA::Any.
FT::ObjectGroupId
TAO::FT_FaultConsumer::extract_object_group_id (const CORBA::Any& val)
{
  FT::ObjectGroupId id = (FT::ObjectGroupId)0;
  if ((val >>= id) == 0)
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT (
        "TAO::FT_FaultConsumer::extract_object_group_id: "
        "Could not extract ObjectGroupId value from any.\n")
    ));
  }

  return id;
}

