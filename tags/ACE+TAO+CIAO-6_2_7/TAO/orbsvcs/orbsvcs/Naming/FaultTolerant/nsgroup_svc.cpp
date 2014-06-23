//=============================================================================
/**
 *  @file nsgroup_svc.cpp
 *
 *  $Id$
 *
 *  This file implements nsgroup utility operations
 *
 *  @author Phillip LaBanca <labancap@ociweb.com>
 */
//=============================================================================

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Naming/FaultTolerant/nsgroup_svc.h"
#include "ace/OS_NS_strings.h"


NS_group_svc::NS_group_svc (void)
{
}

bool
NS_group_svc::determine_policy_string (
  const ACE_TCHAR *policy,
  FT_Naming::LoadBalancingStrategyValue& value)
{
  bool rc = false;

  if (ACE_OS::strcasecmp (policy, ACE_TEXT_CHAR_TO_TCHAR ("round")) == 0)
  {
    value = FT_Naming::ROUND_ROBIN;
    rc = true;
  }
  if (ACE_OS::strcasecmp (policy, ACE_TEXT_CHAR_TO_TCHAR ("random")) == 0)
  {
    value = FT_Naming::RANDOM;
    rc = true;
  }

  return rc;
}

int
NS_group_svc::set_orb( CORBA::ORB_ptr orb)
{

    this->orb_ = CORBA::ORB::_duplicate (orb);

    if (CORBA::is_nil (this->orb_.in ()))
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT (" (%P|%t) Unable to initialize the ")
                         ACE_TEXT ("ORB.\n")),
                        -1);
    return 0;
}

int
NS_group_svc::set_naming_manager( FT_Naming::NamingManager_ptr nm)
{

    this->naming_manager_ = FT_Naming::NamingManager::_duplicate (nm);

    if (CORBA::is_nil (this->naming_manager_.in ()))
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT (" (%P|%t) Invalid Naming Manager.\n")),
                        -1);
    return 0;
}

int
NS_group_svc::set_name_context( CosNaming::NamingContextExt_ptr nc)
{

    this->name_service_ = CosNaming::NamingContextExt::_duplicate (nc);

    if (CORBA::is_nil (this->name_service_.in ()))
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT (" (%P|%t) Invalid Name Context.\n")),
                        -1);
    return 0;
}

bool
NS_group_svc::group_exist (
  const ACE_TCHAR* group_name
)
{
  if (group_name == 0 )
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("group_exist args not provided\n")),
                      false);
  }

  try
    {
    PortableGroup::ObjectGroup_var group_var =
      this->naming_manager_->get_object_group_ref_from_name (
        ACE_TEXT_ALWAYS_CHAR (group_name));
  }
  catch (const PortableGroup::ObjectGroupNotFound&)
  {
    return false;
  }

  return true;
}

/**
 * The naming service shall provide a command line utility for creating
 * object groups. Adds the object group to to the load balancing service
 * with the specified selection policy. On Creation, an object group
 * contains no member objects.
 */

int
NS_group_svc::group_create (
  const ACE_TCHAR* group_name,
  const ACE_TCHAR* policy )
{

  if (group_name == 0 || policy == 0 )
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("group_create args not provided\n")),
                      -2);
  }

  /// Validate load balancing strategy policy string
  FT_Naming::LoadBalancingStrategyValue strategy;
  if (false == determine_policy_string (policy, strategy))
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%s is not a valid policy.\n"),
                       policy),
                      -2);
  }

  try
  {
    /// Verify that the group does not already exist
    /// Group names must be unique
    if ( true == group_exist (group_name))
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Group %s already exists\n"),
                         group_name),
                        -1);
    }

    PortableGroup::Criteria criteria (1);
    criteria.length (1);

    PortableGroup::Property &property = criteria[0];
    property.nam.length (1);

    property.nam[0].id = CORBA::string_dup (
      "org.omg.PortableGroup.MembershipStyle");

    PortableGroup::MembershipStyleValue msv = PortableGroup::MEMB_APP_CTRL;
    property.val <<= msv;

    CORBA::Object_var obj =
      this->naming_manager_->create_object_group (
        ACE_TEXT_ALWAYS_CHAR (group_name),
        strategy,
        criteria);

    if (CORBA::is_nil (obj.in ()))
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("\nUnable to create group %s.\n"),
                         group_name),
                        -1);
    }

  }
  catch (const CORBA::Exception&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to create group %s\n"),
                       group_name),
                      -1);
  }

  return 0;
}

/**
 * The naming service shall provide a command line utility for binding an object
 * group to a path in the naming service.
 * Binds the specified object group to the specified path in the naming service.
 * When clients resolve that path, they tranparently obtain a member of the
 * specified object group.
 */
int
NS_group_svc::group_bind (
  const ACE_TCHAR* group_name,
  const ACE_TCHAR* path)
{

  if (group_name == 0 || path == 0)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("group_bind args not provided\n")),
                       -2);
  }

  try
  {

    PortableGroup::ObjectGroup_var group_var =
      this->naming_manager_->get_object_group_ref_from_name (
        ACE_TEXT_ALWAYS_CHAR(group_name));

    if (CORBA::is_nil (group_var.in()))
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Unable to get reference.\n")),
                         -1);

    CORBA::String_var str = CORBA::string_dup( ACE_TEXT_ALWAYS_CHAR (path) );
    CosNaming::Name_var name = this->name_service_->to_name ( str.in() );

    this->name_service_->rebind (name.in(), group_var.in());

  }
  catch (const CosNaming::NamingContextExt::InvalidName& ex){
      ex._tao_print_exception ("InvalidName Exception in group_bind");

      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("\n%s is invalid\n"),
                         path),
                        -1);
  }
  catch (const CosNaming::NamingContext::CannotProceed&){
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nCannot proceed with %s\n"),
                       path),
                      -1);
  }
  catch (const CosNaming::NamingContext::NotFound&){
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("\nUnable to find %s\n"),
                         path),
                        -1);
  }
  catch (const CORBA::SystemException& ex){

    ex._tao_print_exception ("SystemException Exception in group_bind");

    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to bind %s\n"),
                       path),
                      -1);
  }
  catch (const CORBA::Exception& ex){

    ex._tao_print_exception ("Exception in group_bind");

    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to bind %s\n"),
                       path),
                      -1);
  }
  return 0;
}

int
NS_group_svc::group_unbind (const ACE_TCHAR* path){
  if ( path == 0)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("group_unbind args not provided\n")),
                      -2);
  }

  try
    {

      CORBA::String_var str = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (path));
      CosNaming::Name_var name = this->name_service_->to_name ( str.in() );
      this->name_service_->unbind (name.in());

  }
  catch (const CosNaming::NamingContext::NotFound&){
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("\nUnable to find %s\n"),
                         path),
                        -1);
  }
  catch (const CosNaming::NamingContext::CannotProceed&){
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("\nCannot proceed with %s\n"),
                         path),
                         -1);
  }
  catch (const CosNaming::NamingContext::InvalidName&) {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("\n%s is invalid\n"),
                         path),
                         -1);
  }
  catch (const CORBA::SystemException& ex) {

      ex._tao_print_exception ("Exception in group_unbind");

      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("\nUnable to unbind %s\n"),
                         path),
                         -1);
  }
  catch (const CORBA::Exception& ex) {

      ex._tao_print_exception ("Exception in group_unbind");

      ORBSVCS_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("\nUnable to unbind %s\n"),
                         path),
                         -1);
  }
  return 0;
}

/**
 * The naming service shall provide a command line utility to display all
 * defined object groups within the naming service.
 * Displays all object groups that currently exist in the naming service.
 */
int
NS_group_svc::group_list (void)
{

  // KCS: The group list is independent of locations. I created a new operation in the
  // naming manager IDL to support requesting the group list - which is a list of names

  /// Display object group list for each load balancing strategy
  int rc = 0;
  if (display_load_policy_group (FT_Naming::ROUND_ROBIN,
                                 ACE_TEXT ("Round Robin")) < 0 )
  {
    rc = -1;
  }
  if (rc == 0 && display_load_policy_group (FT_Naming::RANDOM,
                                            ACE_TEXT ("Random")) < 0 )
  {
    rc = -1;
  }

  return rc;
}

int
NS_group_svc::display_load_policy_group(
  FT_Naming::LoadBalancingStrategyValue strategy,
  const ACE_TCHAR *display_label) {

  if( display_label == 0 ) {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("display_load_policy_group args ")
                       ACE_TEXT ("not provided\n")),
                       -2);
  }

  try
  {

    FT_Naming::GroupNames_var list = this->naming_manager_->groups (strategy);

    ORBSVCS_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\n%s Load Balancing Groups\n"),
                display_label));

    if ( list->length () > 0 ) {

      for (unsigned int i = 0; i < list->length (); ++i)
      {
        CORBA::String_var s = CORBA::string_dup (list[i]);
        ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("  %C\n"),
                    s.in ()));
      }

    } else {
        ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("No %s Load Balancing Groups Registered\n"),
                    display_label));
    }

  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("Exception in group_list");

    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to get %s group list\n"),
                       display_label),
                      -1);
  }
  return 0;
}

/**
 * The naming service shall provide a command line utility to modify the load
 * balancing strategy for a specified object group.
 * Changes the selection algorithm for the specified object group. An object
 * group's selection algorithm determines how the naming service directs client
 * requests to object group members.
 */
int
NS_group_svc::group_modify (
  const ACE_TCHAR* group_name,
  const ACE_TCHAR* policy)
{
  if (group_name == 0 || policy == 0)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("group_modify args not provided\n")),
                      -2);
  }

  /// Validate load balancing strategy policy string
  FT_Naming::LoadBalancingStrategyValue strategy;
  if (false == determine_policy_string (policy, strategy))
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%s is not a valid policy.\n"),
                       policy),
                      -2);
  }

  try
  {
    this->naming_manager_->set_load_balancing_strategy (
                            ACE_TEXT_ALWAYS_CHAR (group_name),
                            strategy );
  }
  catch (const PortableGroup::ObjectGroupNotFound&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to find group %s\n"),
                       group_name),
                       -1);
  }
  catch (const CORBA::Exception&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to modify group %s\n"),
                       group_name),
                       -1);
  }

  return 0;
}

/**
 * The naming service shall provide a command line utility to remove a specified
 * object group from the naming service.
 * Removes the specified object group from the naming service.
 */
int
NS_group_svc::group_remove (const ACE_TCHAR* group_name)
{
  if (group_name == 0)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("group_remove args not provided\n")),
                       -2);
  }

  try
  {
    this->naming_manager_->delete_object_group (
      ACE_TEXT_ALWAYS_CHAR (group_name));
  }
  catch (const PortableGroup::ObjectGroupNotFound&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to find group %s\n"),
                       group_name),
                       -1);
  }
  catch (const CORBA::Exception&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to remove group %s\n"),
                       group_name),
                      -1);
  }

  return 0;
}

/**
 * The naming service shall provide a command line utility for adding object
 * references to an object group.
 * Adds an object to the specified object group. After being added, the object
 * is available for selection.
 */
int
NS_group_svc::member_add (
  const ACE_TCHAR* group_name,
  const ACE_TCHAR* location,
  const ACE_TCHAR* ior)
{
  if (group_name == 0 || location == 0 || ior == 0 )
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("member_add args not provided\n")),
                       -2);
  }

  try
  {
    PortableGroup::Location location_name;
    location_name.length (1);
    location_name[0].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (location));

    PortableGroup::ObjectGroup_var group_var =
      this->naming_manager_->get_object_group_ref_from_name (
        ACE_TEXT_ALWAYS_CHAR (group_name));

    CORBA::Object_var ior_var =
      this->orb_->string_to_object(ACE_TEXT_ALWAYS_CHAR (ior));

    if (CORBA::is_nil (ior_var.in ()))
      {
        ORBSVCS_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("\nInvalid member IOR provided.\n")),
                          -1);
      }

    group_var = this->naming_manager_->add_member (group_var.in(),
                                                   location_name,
                                                   ior_var.in());

  }
  catch (const PortableGroup::ObjectGroupNotFound&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to find group %s\n"),
                       group_name),
                      -1);
  }
  catch (const PortableGroup::ObjectNotAdded&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to add location %s to group %s\n"),
                       location, group_name),
                      -1);
  }
  catch (const CORBA::Exception&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to add location %s to group %s\n"),
                       location, group_name),
                      -1);
  }

  return 0;
}

/**
 * The naming service shall provide a command line utility for displaying all
 * members (object references) for a specified object group.
 * Lists the members of the specified object group.
 */
int
NS_group_svc::member_list (const ACE_TCHAR* group_name)
{
  if (group_name == 0)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("member_list args not provided\n")),
                      -2);
  }

  try
  {
    PortableGroup::ObjectGroup_var group_var =
      this->naming_manager_->get_object_group_ref_from_name (
        ACE_TEXT_ALWAYS_CHAR (group_name));

    PortableGroup::Locations_var locations =
      this->naming_manager_->locations_of_members (group_var.in());

    for (unsigned int i = 0; i < locations->length(); ++i)
    {
      const PortableGroup::Location & loc = locations[i];
      if (loc.length() > 0) {
        ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%C\n"),
                    loc[0].id.in()));
      }
    }

  }
  catch (const PortableGroup::ObjectGroupNotFound&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to find group %s\n"),
                       group_name),
                      -1);
  }
  catch (const CORBA::Exception&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to list members for group %s\n"),
                       group_name),
                      -1);
  }

  return 0;
}

/**
 * The naming service shall provide a command line utility for removing object
 * references from an object group.
 * Removes the specified member object from the specified object group.
 */
int
NS_group_svc::member_remove (
  const ACE_TCHAR* group_name,
  const ACE_TCHAR* location)
{
  if (group_name == 0 || location == 0)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("member_remove args not provided\n")),
                      -2);
  }

  /**
   * Remove an object at a specific location from the given
   * ObjectGroup.  Deletion of application created objects must be
   * deleted by the application.  Objects created by the
   * infrastructure (load balancer) will be deleted by the
   * infrastructure.
   */

  try
  {
    PortableGroup::Location location_name;
    location_name.length (1);
    location_name[0].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (location));

    PortableGroup::ObjectGroup_var group_var =
      this->naming_manager_->get_object_group_ref_from_name (
        ACE_TEXT_ALWAYS_CHAR (group_name));

    group_var = this->naming_manager_->remove_member (group_var.in(),
                                                      location_name);
  }
  catch (const PortableGroup::ObjectGroupNotFound&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to find group %s\n"),
                       group_name),
                      -1);
  }
  catch (const PortableGroup::MemberNotFound&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to find member %s\n"),
                       location),
                      -1);
  }
  catch (const CORBA::Exception&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to remove member %s\n"),
                       location),
                      -1);
  }

  return 0;
}

/**
 * The naming service shall provide a command line utility to display an object
 * reference from a specified object group.
 * Displays the object reference that cooresponds to the specified member of an
 * object group.
 */
int
NS_group_svc::member_show (
  const ACE_TCHAR* group_name,
  const ACE_TCHAR* location)
{
  if (group_name == 0 || location == 0)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("member_show args not provided\n")),
                      -2);
  }

  //Get and display IOR for the member location
  try
  {
    PortableGroup::Location location_name (1);
    location_name.length (1);
    location_name[0].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (location));

    PortableGroup::ObjectGroup_var group_var =
      this->naming_manager_->get_object_group_ref_from_name (
        ACE_TEXT_ALWAYS_CHAR (group_name));

    CORBA::Object_var ior_var =
      this->naming_manager_->get_member_ref (group_var.in(), location_name);

    CORBA::String_var ior_string  =
      this->orb_->object_to_string (ior_var.in());

    ORBSVCS_DEBUG ((LM_DEBUG,
                ACE_TEXT ("%C\n"),
                ior_string.in()));

  }
  catch (const PortableGroup::ObjectGroupNotFound&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to find group %s\n"),
                       group_name),
                      -1);
  }
  catch (const PortableGroup::MemberNotFound&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to find member location %s\n"),
                       location),
                      -1);
  }
  catch (const CORBA::Exception&)
  {
    ORBSVCS_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("\nUnable to show member location %s\n"),
                       location),
                      -1);
  }

  return 0;
}
