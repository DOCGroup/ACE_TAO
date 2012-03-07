/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    FT_ReplicationManagerFaultAnalyzer.cpp
 *
 *  $Id$
 *
 *  This file is part of TAO's implementation of Fault Tolerant CORBA.
 *
 *  @author Steve Totten <totten_s@ociweb.com>
 */
//=============================================================================

#include "FT_ReplicationManagerFaultAnalyzer.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/FT_NotifierC.h"
#include "orbsvcs/FT_ReplicationManager/FT_ReplicationManager.h"
#include "orbsvcs/FT_ReplicationManager/FT_FaultEventDescriptor.h"
#include "orbsvcs/PortableGroup/PG_Property_Utils.h"
#include "orbsvcs/PortableGroup/PG_Operators.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"
#include "tao/debug.h"
#include <iostream>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Constructor.
TAO::FT_ReplicationManagerFaultAnalyzer::FT_ReplicationManagerFaultAnalyzer (
  const TAO::FT_ReplicationManager * replication_manager)
  : replication_manager_ (
      const_cast<TAO::FT_ReplicationManager *> (replication_manager))
{
}

/// Destructor.
TAO::FT_ReplicationManagerFaultAnalyzer::~FT_ReplicationManagerFaultAnalyzer ()
{
}

// Validate the event to make sure it is one we can handle.
// If it is not an event we can handle, this function logs the error
// and returns -1.
int TAO::FT_ReplicationManagerFaultAnalyzer::validate_event_type (
  const CosNotification::StructuredEvent & event)
{
  // Delegate to base class.
  //@@ Visual C++ 6.0 won't compile this if I include the namespace name
  // on the base class.
  // return TAO::FT_DefaultFaultAnalyzer::validate_event_type (event);
  return FT_DefaultFaultAnalyzer::validate_event_type (event);
}

/// Analyze a fault event.
int TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event (
  const CosNotification::StructuredEvent & event)
{
  int result = 0;

  const CosNotification::FilterableEventBody & filterable =
    event.filterable_data;
  CORBA::ULong item_count = filterable.length ();
  if (TAO_debug_level > 6)
  {
    for (CORBA::ULong n_prop = 0; n_prop < item_count; ++n_prop)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT("TAO::FT_ReplicationManagerFaultAnalyzer::analyze_fault_event: ")
        ACE_TEXT("Property Name: <%C>\n"),
        filterable[n_prop].name.in()
      ));
    }
  }

  // Populate a TAO::FT_FaultEventDescriptor structure from the
  // properties in the event.
  TAO::FT_FaultEventDescriptor fault_event_desc;

  // Extract the location.
  if (result == 0)
  {
    result = this->get_location (
      filterable[1].value, fault_event_desc.location.out());
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

  if ((result == 0) && (item_count == 2))
  {
    // All objects at location failed.
    fault_event_desc.all_at_location_failed = 1;
  }

  if ((result == 0) && (item_count == 3))
  {
    // All objects of type at location failed.
    fault_event_desc.all_of_type_at_location_failed = 1;
    result = this->get_type_id (
      filterable[2].value, fault_event_desc.type_id.out());
  }

  if ((result == 0) && (item_count == 4))
  {
    // An object (replica) at a location failed.
    fault_event_desc.object_at_location_failed = 1;
    result = this->get_type_id (
      filterable[2].value, fault_event_desc.type_id.out());
    if (result == 0)
    {
      result = this->get_object_group_id (
        filterable[3].value, fault_event_desc.object_group_id);
    }
  }

  // A specific object at a location failed.
  if ((result == 0) && (fault_event_desc.object_at_location_failed == 1))
  {
    result = this->single_replica_failure (fault_event_desc);
  }

  // All objects at location failed.
  if ((result == 0) && (fault_event_desc.all_at_location_failed == 1))
  {
    result = this->location_failure (fault_event_desc);
  }

  // All objects of type at location failed.
  if ((result == 0) && (fault_event_desc.all_of_type_at_location_failed == 1))
  {
    result = this->type_failure (fault_event_desc);
  }

  // Debugging support.
  if (TAO_debug_level > 6)
  {
    fault_event_desc.dump ();
  }

  return result;
}

// Extract a string type_id from CORBA::Any.
// Caller owns the string returned via <type_id>.
int TAO::FT_ReplicationManagerFaultAnalyzer::get_type_id (
  const CORBA::Any& val, PortableGroup::TypeId_out type_id)
{
  const char* type_id_value;
  if ((val >>= type_id_value) == 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
      ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::get_type_id: ")
      ACE_TEXT("Could not extract TypeId value from any.\n")),
      -1);
  }

  // Make a deep copy of the TypeId string.
  type_id = CORBA::string_dup (type_id_value);
  return 0;
}

// Extract the ObjectGroupId from CORBA::Any.
int TAO::FT_ReplicationManagerFaultAnalyzer::get_object_group_id (
  const CORBA::Any& val, PortableGroup::ObjectGroupId& id)
{
  PortableGroup::ObjectGroupId temp_id = (PortableGroup::ObjectGroupId)0;
  if ((val >>= temp_id) == 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
      ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::get_object_group_id: ")
      ACE_TEXT("Could not extract ObjectGroupId value from any.\n")),
      -1);
  }
  id = temp_id;
  return 0;
}

int TAO::FT_ReplicationManagerFaultAnalyzer::get_location (
  const CORBA::Any& val, PortableGroup::Location_out location)
{
  const PortableGroup::Location* temp_loc;
  if ((val >>= temp_loc) == 0)
  {
    ACE_ERROR_RETURN ((LM_ERROR,
      ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::get_location: ")
      ACE_TEXT("Could not extract Location value from fault event.\n")),
      -1);
  }
  // Make a deep copy of the Location.
  ACE_NEW_RETURN (location, PortableGroup::Location (*temp_loc), -1);
  return 0;
}

//
//TODO: Use TAO::PG_Property_Set to get property values from properties
// instead of all these specific "get" functions.
//

// Get the MembershipStyle property.
int TAO::FT_ReplicationManagerFaultAnalyzer::get_membership_style (
  const PortableGroup::Properties & properties,
  PortableGroup::MembershipStyleValue & membership_style)
{
  PortableGroup::Name prop_name (1);
  prop_name.length (1);
  prop_name[0].id = CORBA::string_dup (FT::FT_MEMBERSHIP_STYLE);
  int result = 0;

  PortableGroup::Value value;
  if (TAO_PG::get_property_value (prop_name, properties, value)
    && ((value >>= membership_style) == 1))
  {
    if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT("TAO::FT_ReplicationManagerFaultAnalyzer::get_membership_style: ")
        ACE_TEXT("MembershipStyle is <%d>:\n"),
        membership_style
      ));
    }
  }
  else
  {
    result = -1;
  }

  return result;
}

int TAO::FT_ReplicationManagerFaultAnalyzer::get_replication_style (
  const PortableGroup::Properties & properties,
  FT::ReplicationStyleValue & replication_style)
{
  PortableGroup::Name prop_name (1);
  prop_name.length (1);
  prop_name[0].id = CORBA::string_dup (FT::FT_REPLICATION_STYLE);
  int result = 0;

  PortableGroup::Value value;
  if (TAO_PG::get_property_value (prop_name, properties, value)
    && ((value >>= replication_style) == 1))
  {
    if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::get_replication_style: ")
        ACE_TEXT ("ReplicationStyle is <%d>:\n"),
        replication_style
      ));
    }
  }
  else
  {
    result = -1;
  }

  return result;
}

int TAO::FT_ReplicationManagerFaultAnalyzer::get_minimum_number_members (
  const PortableGroup::Properties & properties,
  PortableGroup::MinimumNumberMembersValue & minimum_number_members)
{
  PortableGroup::Name prop_name (1);
  prop_name.length (1);
  prop_name[0].id = CORBA::string_dup (FT::FT_MINIMUM_NUMBER_MEMBERS);
  int result = 0;

  PortableGroup::Value value;
  if (TAO_PG::get_property_value (prop_name, properties, value)
    && ((value >>= minimum_number_members) == 1))
  {
    if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::get_minimum_number_members: ")
        ACE_TEXT ("MinimumNumberMembers is <%d>:\n"),
        minimum_number_members
      ));
    }
  }
  else
  {
    result = -1;
  }

  return result;
}

int TAO::FT_ReplicationManagerFaultAnalyzer::get_initial_number_members (
  const PortableGroup::Properties & properties,
  PortableGroup::InitialNumberMembersValue & initial_number_members)
{
  PortableGroup::Name prop_name (1);
  prop_name.length (1);
  prop_name[0].id = CORBA::string_dup (FT::FT_INITIAL_NUMBER_MEMBERS);
  int result = 0;

  PortableGroup::Value value;
  if (TAO_PG::get_property_value (prop_name, properties, value)
    && ((value >>= initial_number_members) == 1))
  {
    if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::get_initial_number_members: ")
        ACE_TEXT ("InitialNumberMembers is <%d>:\n"),
        initial_number_members
      ));
    }
  }
  else
  {
    result = -1;
  }

  return result;
}

int TAO::FT_ReplicationManagerFaultAnalyzer::get_factories (
  const PortableGroup::Properties & properties,
  PortableGroup::FactoryInfos_out factories)
{
  PortableGroup::Name prop_name (1);
  prop_name.length (1);
  prop_name[0].id = CORBA::string_dup (FT::FT_FACTORIES);
  int result = 0;

  PortableGroup::FactoryInfos_var temp_factories;
  PortableGroup::Value value;
  if (TAO_PG::get_property_value (prop_name, properties, value) == 1)
  {
    if ((value >>= temp_factories) == 0)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::get_factories: ")
        ACE_TEXT ("Could not extract Factories from properties.\n")
      ));
      result = -1;
    }
    else
    {
      // Make a deep copy of the Factories.
      ACE_NEW_RETURN (factories, PortableGroup::FactoryInfos (temp_factories.in()), -1);
      result = 0;
    }
  }
  else
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::get_factories: ")
      ACE_TEXT ("Could not find Factories property.\n")
    ));
    result = -1;
  }
  return result;
}

int TAO::FT_ReplicationManagerFaultAnalyzer::is_primary_member (
  const PortableGroup::ObjectGroup_ptr iogr,
  const PortableGroup::Location & location,
  int & object_is_primary)
{

  // To determine if this was a primary that faulted:
  // Get the TagFTGroupTaggedComponent from the IOGR and search
  // for the primary, using the TAO_FT_IOGR_Property helper class.
  // Then, compare the TypeId and Location of the failed object with
  // those of the primary.  If they match, it was a primary fault.

  int result = 0;
  object_is_primary = 0;

  try
  {
    // Create an "empty" TAO_FT_IOGR_Property and use it to get the
    // tagged component.
    TAO_FT_IOGR_Property temp_ft_prop;
    FT::TagFTGroupTaggedComponent ft_group_tagged_component;
    CORBA::Boolean got_tagged_component =
      temp_ft_prop.get_tagged_component (
          iogr, ft_group_tagged_component);
    if (got_tagged_component)
    {
      // Create a new TAO_FT_IOGR_Property with the tagged
      // component.
      TAO_FT_IOGR_Property ft_prop (ft_group_tagged_component);

      // Check to see if a primary is set.
      CORBA::Boolean primary_is_set = ft_prop.is_primary_set (
        iogr);
      if (primary_is_set)
      {
        // Get the primary object.
        CORBA::Object_var primary_obj = ft_prop.get_primary (
          iogr);
        if (CORBA::is_nil (primary_obj.in()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
            ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::is_primary_member: ")
            ACE_TEXT ("Could not get primary IOR from IOGR.\n")),
            -1);
        }

        // Get the object reference of the failed member.
        CORBA::Object_var failed_obj =
          this->replication_manager_->get_member_ref (
            iogr, location);
        if (CORBA::is_nil (failed_obj.in()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
            ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::is_primary_member: ")
            ACE_TEXT ("Could not get IOR of failed member from IOGR.\n")),
            -1);
        }

        // Are the two object refs (primary and failed) equivalent?
        CORBA::Boolean equiv = primary_obj->_is_equivalent (
          failed_obj.in());
        if (equiv)
        {
          object_is_primary = 1;
          result = 0;
        }
      }
      else  // primary is not set
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::is_primary_member: ")
          ACE_TEXT ("Primary is not set on IOGR.\n")
        ));
        result = -1;
      }
    }
    else // could not get tagged component
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::is_primary_member: ")
        ACE_TEXT ("Could not get tagged component from IOGR.\n")
      ));
      result = -1;
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::is_primary_member: "));
    result = -1;
  }

  return result;
}

// Handle a single replica failure.
int TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure (
  TAO::FT_FaultEventDescriptor & fault_event_desc)
{
  int result = 0;
  PortableGroup::ObjectGroup_var the_object_group = PortableGroup::ObjectGroup::_nil();
  PortableGroup::Properties_var properties;

  try
  {
    // Get the object group reference based on the ObjectGroupId.
    the_object_group =
      this->replication_manager_->get_object_group_ref_from_id (
        fault_event_desc.object_group_id);

    // This should not happen, but let us be safe.
    if (CORBA::is_nil (the_object_group.in()))
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: ")
        ACE_TEXT ("Could not get ObjectGroup reference from ObjectGroupId: <%Q>.\n"),
          fault_event_desc.object_group_id
      ));
      throw PortableGroup::ObjectGroupNotFound ();
    }

    // Get the properties associated with this ObjectGroup.
    properties = this->replication_manager_->get_properties (
      the_object_group.in());
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT (
        "TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: "));
    result = -1;
  }

  if (result == 0)
  {
    // Get the MembershipStyle property.
    PortableGroup::MembershipStyleValue membership_style;
    result = this->get_membership_style (properties.in(), membership_style);
    if (result != 0)
    {
      //@@ it seems a shame to fail here.  We should at least remove the failed replica from the group.
      ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: ")
        ACE_TEXT ("Could not extract MembershipStyle from properties on ")
        ACE_TEXT ("ObjectGroup with id <%Q>.\n"),
        fault_event_desc.object_group_id),
        -1);
    }
    else
    {
      fault_event_desc.membership_style = membership_style;
      if (TAO_debug_level > 6)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: ")
          ACE_TEXT ("MembershipStyleValue = <%d>"),
            fault_event_desc.membership_style
        ));
      }
    }

    // Get the ReplicationStyle property.
    FT::ReplicationStyleValue replication_style;
    result = this->get_replication_style (properties.in(), replication_style);
    if (result != 0)
    {
      //@@ it seems a shame to fail here.  We should at least remove the failed replica from the group.
      ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: ")
        ACE_TEXT ("Could not extract ReplicationStyle from properties on ")
        ACE_TEXT ("ObjectGroup with id <%Q>.\n"),
        fault_event_desc.object_group_id),
        -1);
    }
    else
    {
      fault_event_desc.replication_style = replication_style;
      if (TAO_debug_level > 6)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: ")
          ACE_TEXT ("ReplicationStyleValue = <%d>"),
            fault_event_desc.replication_style
        ));
      }
    }

    // Get the MinimumNumberMembers property.
    PortableGroup::MinimumNumberMembersValue minimum_number_members;
    result = this->get_minimum_number_members (
      properties.in(), minimum_number_members);
    if (result != 0)
    {
      // This is not a fatal error.  It may be App Controlled.
      result = 0;
      if (TAO_debug_level > 3)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: ")
          ACE_TEXT ("Could not extract MinimumNumberMembers from properties on ")
          ACE_TEXT ("ObjectGroup with id <%Q>.\n"),
          fault_event_desc.object_group_id));
      }
    }
    else
    {
      fault_event_desc.minimum_number_members = minimum_number_members;
      if (TAO_debug_level > 6)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: ")
          ACE_TEXT ("MinimumNumberMembers = <%d>"),
            fault_event_desc.minimum_number_members
        ));
      }
    }

    // Get the InitialNumberMembers property.
    PortableGroup::InitialNumberMembersValue initial_number_members;
    result = this->get_initial_number_members (
      properties.in(), initial_number_members);
    if (result != 0)
    {
      // This is not a fatal error.  It may be App Controlled.
      result = 0;
      if (TAO_debug_level > 3)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: ")
          ACE_TEXT ("Could not extract InitialNumberMembers from properties on ")
          ACE_TEXT ("ObjectGroup with id <%Q>.\n"),
          fault_event_desc.object_group_id));
      }
    }
    else
    {
      fault_event_desc.initial_number_members = initial_number_members;
      if (TAO_debug_level > 6)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: ")
          ACE_TEXT ("InitialNumberMembers = <%d>"),
            fault_event_desc.initial_number_members
        ));
      }
    }

    // Get the Factories property.
    result = this->get_factories (
      properties.in(),
      fault_event_desc.factories.out());
    if (result != 0)
    {
      // This is not a fatal error.  It may be App Controlled.
      result = 0;
      if (TAO_debug_level > 3)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: ")
          ACE_TEXT ("Could not extract Factories from properties on ")
          ACE_TEXT ("ObjectGroup with id <%Q>.\n"),
          fault_event_desc.object_group_id));
       }
    }
    else
    {
      if (TAO_debug_level > 6)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: ")
          ACE_TEXT ("Got Factories from properties on ")
          ACE_TEXT ("ObjectGroup with id <%Q>.\n"),
            fault_event_desc.object_group_id
        ));
      }
    }

  }

  // If the ReplicationStyle is COLD_PASSIVE, WARM_PASSIVE, or
  // SEMI_ACTIVE, we can see if it was the primary replica that
  // failed.
  if ((result == 0) &&
      (fault_event_desc.replication_style == FT::COLD_PASSIVE ||
       fault_event_desc.replication_style == FT::WARM_PASSIVE ||
       fault_event_desc.replication_style == FT::SEMI_ACTIVE))
  {
    if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: ")
        ACE_TEXT ("Checking to see if failed replica was the primary for ")
        ACE_TEXT ("ObjectGroup with id <%Q>.\n"),
          fault_event_desc.object_group_id
      ));
    }
    result = this->is_primary_member (
      the_object_group.in(),
      fault_event_desc.location.in(),
      fault_event_desc.object_is_primary);
  }

  // If the MembershipStyle is FT::MEMB_INF_CTRL (infrastructure
  // controlled) and the primary has faulted, establish a new primary.
  // We get back a new object group.
  if ((result == 0) &&
      (fault_event_desc.membership_style == FT::MEMB_INF_CTRL))
  {

    PortableGroup::ObjectGroup_var new_object_group;
    result = this->remove_failed_member (
        the_object_group.in(),
        fault_event_desc,
        new_object_group.out());
    if (result == 0)
    {
      the_object_group = new_object_group;
    }

    if (fault_event_desc.object_is_primary == 1)
    {
      if (TAO_debug_level > 6)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: ")
          ACE_TEXT ("Setting new primary for ")
          ACE_TEXT ("ObjectGroup with id <%Q>.\n"),
            fault_event_desc.object_group_id
        ));
      }
      result = this->set_new_primary (
        the_object_group.in(),
        fault_event_desc,
        new_object_group.out());
      if (result == 0)
      {
        the_object_group = new_object_group;
      }
    }
  }

#if 0 // According to the FT CORBA specification, this will be handled by the ObjectGroupManager::remove_member method
  // If the MembershipStyle is FT::MEMB_INF_CTRL (infrastructure
  // controlled) and the number of remaining members is less than
  // the MinimumNumberMembers property, add new members.
  // We get back a new object group.
  if ((result == 0) &&
      (fault_event_desc.membership_style == FT::MEMB_INF_CTRL))
  {
    if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::single_replica_failure: ")
        ACE_TEXT ("Potentially adding new members to ")
        ACE_TEXT ("ObjectGroup with id <%Q>.\n"),
          fault_event_desc.object_group_id
      ));
    }
    result = this->add_members (
      the_object_group.in(),
      fault_event_desc,
      new_object_group.out());
    the_object_group = new_object_group;
  }
#endif
  return result;
}

int TAO::FT_ReplicationManagerFaultAnalyzer::remove_failed_member (
  PortableGroup::ObjectGroup_ptr iogr,
  TAO::FT_FaultEventDescriptor & fault_event_desc,
  PortableGroup::ObjectGroup_out new_iogr)
{
  int result = 0;
  new_iogr = PortableGroup::ObjectGroup::_nil ();

  try
  {
    // Remove the old primary member from the object group.
    PortableGroup::ObjectGroup_var temp_iogr =
      this->replication_manager_->remove_member (
        iogr,
        fault_event_desc.location.in());
    new_iogr = temp_iogr._retn ();
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      "TAO::FT_ReplicationManagerFaultAnalyzer::remove_failed_member: ");
    result = -1;
  }
  return result;
}


// Choose a new primary member for the ObjectGroup.
// Sets <new_iogr> and returns 0 on success.
// Returns -1 on failure.
int TAO::FT_ReplicationManagerFaultAnalyzer::set_new_primary (
  PortableGroup::ObjectGroup_ptr iogr,
  TAO::FT_FaultEventDescriptor & fault_event_desc,
  PortableGroup::ObjectGroup_out new_iogr)
{
  int result = 0;
  new_iogr = PortableGroup::ObjectGroup::_nil ();

  try
  {
    // Get the locations of the remaining members of the object group.
    PortableGroup::Locations_var locations =
      this->replication_manager_->locations_of_members (
        iogr);

    // Choose the first location as our new primary location.
    if (locations->length() >= 1)
    {
      new_iogr = this->replication_manager_->set_primary_member (
        iogr,
        (*locations)[0]);
    }
    else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT ("TAO::FT_ReplicationManagerFaultAnalyzer::set_new_primary: ")
        ACE_TEXT ("No locations remaining in ObjectGroup with id <%Q>.\n"),
          fault_event_desc.object_group_id),
        -1);
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      "TAO::FT_ReplicationManagerFaultAnalyzer::set_new_primary: ");
    result = -1;
  }

  return result;
}

#if 0 // this is handled by the remove_member method
// While the number of members in the object group is less than
// the MinimumNumberMembers property, add new members.
// Sets <new_iogr> and returns 0 on success.
// Returns -1 on failure.
int TAO::FT_ReplicationManagerFaultAnalyzer::add_members (
  PortableGroup::ObjectGroup_ptr iogr,
  TAO::FT_FaultEventDescriptor & fault_event_desc,
  PortableGroup::ObjectGroup_out new_iogr)
{
  int result = 0;
  new_iogr = PortableGroup::ObjectGroup::_nil ();

  try
  {
    // Get current number of members in object group
    // (same as number of locations).
    PortableGroup::Locations_var locations =
      this->replication_manager_->locations_of_members (
        iogr);
    CORBA::ULong num_members = locations->length();

    // If it is less than the MinimumNumberMembers property, add
    // new members.
    if (num_members < fault_event_desc.minimum_number_members)
    {
      //@@ To create a member, we need to know the ObjectGroup,
      //   Location, TypeId, and Criteria.

      // Get the factory registry from the Replication Manager.
      PortableGroup::Criteria fake_criteria;
      PortableGroup::FactoryRegistry_var factory_registry =
        this->replication_manager_->get_factory_registry (
          fake_criteria);


      // @@ DLW SAYS: we need to find out the role played by this object
      // group so we can use the correct set of factories.
      // Get the list of factories for the type of the failed replica.
      CORBA::String_var type_id;
      PortableGroup::FactoryInfos_var factories_by_type =
          factory_registry->list_factories_by_role (
          fault_event_desc.type_id.in(), type_id);

      //
      // Build a set of locations of factories for this type that we
      // can use to create new members (i.e., at locations where
      // members do not currently exist).
      //
      FT_Location_Set valid_locations;

      // For each factory that can be used for this type...
      for (CORBA::ULong f=0; f<factories_by_type->length(); ++f)
      {
        // ...insert its location into valid_locations set.
        valid_locations.insert (factories_by_type[f].the_location);
      }

      // Now remove any locations where members already exist.
      for (CORBA::ULong m=0; m<num_members; ++m)
      {
        if (valid_locations.find (locations[m]))
          valid_locations.remove (locations[m]);
      }

      // The valid_locations set now contains all the factory
      // locations we can use to add members to this object group.
      // So, now we add new members until we reach
      // the value of the MinimumNumberMembers property.
      PortableGroup::Location_var good_location;
      for (FT_Location_Set::iterator iter (valid_locations);
           iter.next (good_location.out()) &&
           fault_event_desc.minimum_number_members > num_members;
           iter.advance(), ++num_members)
      {
        // Create a new member of the object group at this location.
        new_iogr = this->replication_manager_->create_member (
          iogr,
          good_location.in(),
          fault_event_desc.type_id.in(),
          fake_criteria);

        // Stop adding members when we reach the value of the
        // MinimumNumberMembers property.
        // if (num_members++ >= fault_event_desc.minimum_number_members)
            // break;
      }

    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      "TAO::FT_ReplicationManagerFaultAnalyzer::add_members: ");
    result = -1;
  }

  return result;
}
#endif // 0

// Handle a location failure.
int TAO::FT_ReplicationManagerFaultAnalyzer::location_failure (
  TAO::FT_FaultEventDescriptor & fault_event_desc)
{
  int result = 0;

  // To handle a location failure, we should:
  // - Unregister all the factories at that location.
  //   (We do this first so that we don't try to create a new replica
  //   at that location for any of the affected object groups.)
  // - Determine all the object groups that had members at that
  //   location.
  // - Handle each one of them as a single replica failure.

  try
  {
    // Get the factory registry from the Replication Manager.
    PortableGroup::Criteria fake_criteria;
    PortableGroup::FactoryRegistry_var factory_registry =
      this->replication_manager_->get_factory_registry (
        fake_criteria);

    // Unregister all factories at the failed location.
    factory_registry->unregister_factory_by_location (
      fault_event_desc.location.in());

    // Determine all the object groups that had members at that
    // location.
    PortableGroup::ObjectGroups_var object_groups_at_location =
      this->replication_manager_->groups_at_location (
      fault_event_desc.location.in());

    // Handle each one of them as a single replica failure.
    for (CORBA::ULong i=0;
         result==0 && i<object_groups_at_location->length();
         ++i)
    {
      // Get the object group id.
      fault_event_desc.object_group_id =
        this->replication_manager_->get_object_group_id (
          object_groups_at_location[i]);

      // Get type id of this object group.
      fault_event_desc.type_id =
        this->replication_manager_->type_id (
          object_groups_at_location[i]);

      // Handle it as a single replica failure.
      result = this->single_replica_failure (fault_event_desc);
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      "TAO::FT_ReplicationManagerFaultAnalyzer::location_failure: ");
    result = -1;
  }

  return result;
}

// Handle a type failure.
int TAO::FT_ReplicationManagerFaultAnalyzer::type_failure (
  TAO::FT_FaultEventDescriptor & fault_event_desc)
{
  int result = 0;

  // To handle a type failure, we should:
  // - Unregister the factory at the location of the failure
  //   that is associated with the failed type.
  //   (We do this first so that we don't try to create a new replica
  //   with that factory for any of the affected object groups.)
  // - Determine all the object groups that had members at that
  //   location of that type.
  // - Handle each one of them as a single replica failure.

  try
  {
    // Get the factory registry from the Replication Manager.
    PortableGroup::Criteria fake_criteria;
    PortableGroup::FactoryRegistry_var factory_registry =
      this->replication_manager_->get_factory_registry (
        fake_criteria);

    // Unregister the factory at the failed location associated with
    // the role.
    //@@ Using type_id as the role for now.
    factory_registry->unregister_factory (
      fault_event_desc.type_id.in(),
      fault_event_desc.location.in());

    // Get all the object groups that had members at that
    // location.
    PortableGroup::ObjectGroups_var object_groups_at_location =
      this->replication_manager_->groups_at_location (
      fault_event_desc.location.in());

    // For each one, if it was of the same type as the failed type,
    // handle it as a single replica failure.
    for (CORBA::ULong i=0;
         result==0 && i<object_groups_at_location->length();
         ++i)
    {
      // Get the object group id.
      fault_event_desc.object_group_id =
        this->replication_manager_->get_object_group_id (
          object_groups_at_location[i]);

      // Get type id of this object group.
      PortableGroup::TypeId_var type_id =
        this->replication_manager_->type_id (
          object_groups_at_location[i]);

      // If the type id is the same as the failed type id...
      if (ACE_OS::strcmp (type_id.in(), fault_event_desc.type_id.in()) == 0)
      {
        // Handle it as a single replica failure.
        result = this->single_replica_failure (fault_event_desc);
      }
    }
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      "TAO::FT_ReplicationManagerFaultAnalyzer::type_failure: ");
    result = -1;
  }

  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
