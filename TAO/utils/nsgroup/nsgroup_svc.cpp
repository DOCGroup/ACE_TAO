//=============================================================================
/**
 *  @file nsgroup_svc.cpp
 *
 *  $Id$Id$
 *
 *  This file implements nsgroup command line utility operations
 *
 *  @author Phillip LaBanca <labancap@ociweb.com>
 */
//=============================================================================

#include "nsgroup_svc.h"
#include "ace/Get_Opt.h"
#include "ace/Service_Config.h"
#include "ace/OS_NS_strings.h"
#include <sstream>


NS_group_svc::NS_group_svc (int argc, ACE_TCHAR **argv)
  : argc_ (argc),
    argv_ (argv),
    nsgroup_cmd_(NS_group_svc::NSGROUP_NONE)
{
}

NS_group_svc::NSGROUP_COMMAND
NS_group_svc::parse_command_line (void)
{
  #if 0
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("parse_command_line::argc(%u)\n"), this->argc_));
  for( int i = 0; i < this->argc_; ++i){
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("parse_command_line::argv(%u:%s)\n"), i, this->argv_[i]));
  }
  #endif

  static const ACE_TCHAR options[] = ACE_TEXT("g:p:t:l:i:n:h");
  static const int skip_args     = 1;
  static const int report_errors = 0;
  static const int ordering      = ACE_Get_Opt::PERMUTE_ARGS;
  static const int long_only     = 1;

  ACE_Get_Opt get_opts (
    this->argc_,
    this->argv_,
    options,
    skip_args,
    report_errors,
    ordering,
    long_only
  );

  this->group_arg_ = 0;
  if (get_opts.long_option (ACE_TEXT ("group"),
                           'g',
                           ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to add long option 'g'\n")), NSGROUP_NONE);

  this->policy_arg_ = 0;
  if (get_opts.long_option (ACE_TEXT ("policy"),
                           'p',
                           ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to add long option 'p'\n")), NSGROUP_NONE);

  this->typeid_arg_ = 0;
  if (get_opts.long_option (ACE_TEXT ("type_id"),
                           't',
                           ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to add long option 't'\n")), NSGROUP_NONE);

  this->location_arg_ = 0;
  if (get_opts.long_option (ACE_TEXT ("location"),
                           'l',
                           ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to add long option 'l'\n")), NSGROUP_NONE);

  this->ior_arg_ = 0;
  if (get_opts.long_option (ACE_TEXT ("ior"),
                           'i',
                           ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to add long option 'i'\n")), NSGROUP_NONE);

  this->namepath_arg_ = 0;
  if (get_opts.long_option (ACE_TEXT ("namepath"),
                           'n',
                           ACE_Get_Opt::ARG_REQUIRED) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to add long option 'n'\n")), NSGROUP_NONE);

  if (get_opts.long_option (ACE_TEXT ("help"), 'h') != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Unable to add long option 'h'\n")), NSGROUP_NONE);

  int c;
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'g':  // group
        this->group_arg_ = get_opts.opt_arg ();
        break;
      case 'p':  // policy
        this->policy_arg_ = get_opts.opt_arg ();
        break;
      case 't':  // type_id
        this->typeid_arg_ = get_opts.opt_arg ();
        break;
      case 'l':  // location
        this->location_arg_ = get_opts.opt_arg ();
        break;
      case 'i': // ior
        this->ior_arg_ = get_opts.opt_arg ();
        break;
      case 'n': // namepath
        this->namepath_arg_ = get_opts.opt_arg ();
        break;
      case 'h':
        return NSGROUP_HELP;
      }

  // handle non-option arguments
  int non_option_arg_count = 0;
  for( int i = get_opts.opt_ind (); i < this->argc_; ++i)
  {

    non_option_arg_count++;

    //ACE_DEBUG ((LM_INFO, "Found non-option argument \"%s\"\n", this->argv_[i]));


    if( ACE_OS::strncmp (this->argv_[i],"group_create",
                        ACE_OS::strlen ("group_create")) == 0 ) {

      nsgroup_cmd_ =  NSGROUP_GROUP_CREATE;

    } else if(ACE_OS::strncmp (this->argv_[i], "group_bind",
                               ACE_OS::strlen ("group_bind")) == 0 ){

      nsgroup_cmd_ =  NSGROUP_GROUP_BIND;

    } else if(ACE_OS::strncmp (this->argv_[i], "group_modify",
                               ACE_OS::strlen ("group_modify")) == 0 ){

      nsgroup_cmd_ =  NSGROUP_GROUP_MODIFY;

    } else if(ACE_OS::strncmp (this->argv_[i], "group_list",
                               ACE_OS::strlen ("group_list")) == 0 ){

      nsgroup_cmd_ =  NSGROUP_GROUP_LIST;

    } else if(ACE_OS::strncmp (this->argv_[i], "group_remove",
                               ACE_OS::strlen ("group_remove")) == 0 ){

      nsgroup_cmd_ =  NSGROUP_GROUP_REMOVE;

    } else if(ACE_OS::strncmp (this->argv_[i], "member_list",
                               ACE_OS::strlen ("member_list")) == 0 ){

      nsgroup_cmd_ =  NSGROUP_MEMBER_LIST;

    } else if(ACE_OS::strncmp (this->argv_[i], "member_add",
                               ACE_OS::strlen ("member_add")) == 0 ){

      nsgroup_cmd_ =  NSGROUP_MEMBER_ADD;

    } else if(ACE_OS::strncmp (this->argv_[i], "member_remove",
                               ACE_OS::strlen ("member_remove")) == 0 ){

      nsgroup_cmd_ =  NSGROUP_MEMBER_REMOVE;

    } else if(ACE_OS::strncmp (this->argv_[i], "member_show",
                               ACE_OS::strlen ("member_show")) == 0 ){

      nsgroup_cmd_ =  NSGROUP_MEMBER_SHOW;

    } else {

      nsgroup_cmd_ =  NSGROUP_NONE;

    }
  }

  // The command should be the only non option argument
  if ( non_option_arg_count > 1 ) {
      nsgroup_cmd_ =  NSGROUP_NONE;
  }

  return nsgroup_cmd_;
}

int
NS_group_svc::run_cmd(void)
{
  int rc = -1;
  switch( parse_command_line() )
  {

    case NSGROUP_HELP:
      return show_usage();
    break;

    case NSGROUP_GROUP_CREATE:
      return group_create ( group_arg(), typeid_arg(), policy_arg() );
    break;

    case NSGROUP_GROUP_BIND:
      return group_bind ( group_arg(), namepath_arg() );
    break;

    case NSGROUP_GROUP_MODIFY:
      return group_modify ( group_arg(), policy_arg() );
    break;

    case NSGROUP_GROUP_LIST:
      return group_list ();
    break;

    case NSGROUP_GROUP_REMOVE:
      return group_remove ( group_arg() );
    break;

    case NSGROUP_MEMBER_LIST:
      return member_list ( group_arg() );
    break;

    case NSGROUP_MEMBER_ADD:
      return member_add ( group_arg(), location_arg(), ior_arg() );
    break;

    case NSGROUP_MEMBER_REMOVE:
      return member_remove ( group_arg(), location_arg() );
    break;

    case NSGROUP_MEMBER_SHOW:
      return member_show ( group_arg(), location_arg() );
    break;

    default:
      return rc;
    break;
  }

  return rc;
}

int
NS_group_svc::destroy (void)
{
  try
    {
      this->naming_manager_->delete_object (this->fcid_.in ());

      this->orb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Exception caught while destroying NS_group_svc\n");
      return -1;
    }
  return 1;

}

#if 0
void
NS_group_svc::determine_policy_string (const char *policy,
  CosLoadBalancing::StrategyInfo& si)
{
  if (ACE_OS::strcasecmp (policy, ACE_TEXT("round")) == 0)
      si.name = CORBA::string_dup ("RoundRobin");

  else if (ACE_OS::strcasecmp (policy, ACE_TEXT("rand")) == 0)
      si.name = CORBA::string_dup ("Random");

  else if (ACE_OS::strcasecmp (policy, ACE_TEXT("least")) == 0)
      si.name = CORBA::string_dup ("LeastLoaded");

  else
    {
      si.name = CORBA::string_dup ("Random");
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Unknown strategy, using Random\n")));
    }
}
#endif


int
NS_group_svc::start_orb (void)
{
  try
    {

#if 0
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("start_orb::argc(%u)\n"), this->argc_));
      for( int i = 0; i < this->argc_; ++i){
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("start_orb::argv(%u:%s)\n"), i, this->argv_[i]));
      }
#endif //

      // Initialise the ORB.
      this->orb_ = CORBA::ORB_init (this->argc_, this->argv_);
      if (CORBA::is_nil (this->orb_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                            " (%P|%t) Unable to initialize the ORB.\n"),
                            -1);

      //////////////////////////////////////////////////////////////////////////
      //
      //////////////////////////////////////////////////////////////////////////
      CORBA::Object_var naming_manager_object = this->orb_->resolve_initial_references ("NamingManager");
      this->naming_manager_ = FT::NamingManager::_narrow (naming_manager_object.in ());
      if (CORBA::is_nil (this->naming_manager_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                            " (%P|%t) Unable to get Naming Manager Reference\n"),
                            -1);
      //////////////////////////////////////////////////////////////////////////
      //
      //////////////////////////////////////////////////////////////////////////
      CORBA::Object_var naming_object =
        this->orb_->resolve_initial_references("NameService");

      this->name_service_ =
        CosNaming::NamingContext::_narrow (naming_object.in ());

      if (CORBA::is_nil (this->name_service_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get Name Service Reference\n"),
                          -1);
      //////////////////////////////////////////////////////////////////////////
      //
      //////////////////////////////////////////////////////////////////////////
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception raised initialising ORB");
      return -1;
    }

  return 1;

}

int
NS_group_svc::show_usage( void )
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Usage:\n")
              ACE_TEXT ("  %s\n")
              ACE_TEXT ("    group_create  -group <group> -policy <round | rand | least> -type_id <type_id> \n")
              ACE_TEXT ("    group_bind    -group <group> -namepath <path>\n")
              ACE_TEXT ("    group_modify  -group <group> -policy <round | rand | least> \n")
              ACE_TEXT ("    group_list\n")
              ACE_TEXT ("    group_remove  -group <group>\n")
              ACE_TEXT ("    member_list   -group <group>\n")
              ACE_TEXT ("    member_add    -group <group> -location <location> -ior <IOR>\n")
              ACE_TEXT ("    member_remove -group <group> -location <location>\n")
              ACE_TEXT ("    member_show   -group <group> -location <location>\n")
              ACE_TEXT ("    -help\n")
              ACE_TEXT ("\n"),
              this->argv_[0]));
    return 0;
}

/**
 * The naming service shall provide a command line utility for creating object groups.
 * Adds the object group to to the load balancing service with the specified
 * selection policy. On Creation, an object group contains no member objects.
 */

int
NS_group_svc::group_create (
  const char* group_name,
  const char* type_id,
  const char* policy )
{

  if ( group_name == 0 || type_id == 0 || policy == 0 )
  {
    return -1;
  }

  try
    {

      PortableGroup::Criteria criteria (1);
      criteria.length (1);

      PortableGroup::Property &property = criteria[0];
      property.nam.length (1);

      property.nam[0].id =
        CORBA::string_dup ("org.omg.PortableGroup.MembershipStyle");

      PortableGroup::MembershipStyleValue msv = PortableGroup::MEMB_APP_CTRL;
      property.val <<= msv;

      CORBA::Object_var obj = this->naming_manager_->create_object_group (group_name,
                                                                          type_id,
                                                                          criteria);

      if (CORBA::is_nil (obj.in ()))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                            " (%P|%t) Unable to create the group.\n"),
                            -1);
      }

      //PortableGroup::ObjectGroup_ptr object_group = PortableGroup::ObjectGroup::_narrow( obj.in() );
      //PortableGroup::ObjectGroupId group_id = this->naming_manager_->get_object_group_id( object_group );
      //ACE_DEBUG ((LM_DEBUG, "(%P|%t) nsgroup - group_create returned group_id(%u)\n", group_id));

#if 0
    // This subsection doesn't work due to TAO_PG_PropertyManager::set_properties_dynamically throws CORBA::NO_IMPLEMENT ()
      CosLoadBalancing::StrategyInfo strategy_info;
      determine_policy_string(policy, strategy_info);

      PortableGroup::Properties props (1);
      props.length (1);
      props[0].nam.length (1);
      props[0].nam[0].id = CORBA::string_dup ("org.omg.CosLoadBalancing.StrategyInfo");
      props[0].val <<= strategy_info;
      // TAO_PG_PropertyManager::set_properties_dynamically throws CORBA::NO_IMPLEMENT ()
      this->naming_manager_->set_properties_dynamically (obj.in(), props );
#endif

  }

  catch (const PortableGroup::ObjectGroupNotFound& ex)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Unable to find group(%s) \n", group_name));
      return -1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ( "Exception raised while creating object group");
      return -1;
    }

  return 0;
}

/**
 * The naming service shall provide a command line utility for binding an object
 * group to a path in the naming service.
 * tao_nsgroup group_bind -group <group> <path>
 * Binds the specified object group to the specified path in the naming service.
 * When clients resolve that path, they tranparently obtain a member of the
 * specified object group.
 */
int
NS_group_svc::group_bind (
  const char* group,
  const char* path)
{

  if (group == 0 || path == 0)
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) nsgroup - group_bind args not provided.\n"));
    return -1;
  }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) group_bind(%s,%s) NOT IMPLEMENTED\n", group, path));
  return -1;
}

/**
 * The naming service shall provide a command line utility to display all
 * defined object groups within the naming service.
 * Displays all object groups that currently exist in the naming service.
 */
int
NS_group_svc::group_list (void)
{
  FT::GroupNames_var group_names =  this->naming_manager_->groups ();

  // KCS: The group list is independent of locations. I created a new operation in the
  // naming manager IDL to support requesting the group list - which is a list of names
  //  PortableGroup::ObjectGroups_var groups = this->naming_manager_->groups_at_location( the_location );
  //  ACE_DEBUG ((LM_DEBUG, "(%P|%t) nsgroup - groups at location:(%u)\n", group_names->length()));

  std::cout << "Round Robin Groups:" << std::endl;
  for (unsigned int i = 0; i < group_names->length (); ++i)
  {
    std::cout << "  " << (*group_names)[i] << std::endl;
  }

  return 0;
}

/**
 * The naming service shall provide a command line utility to modify the load
 * balancing strategy for a specified object group.
 * tao_nsgroup group_modify -policy <round | rand | least> <group>
 * Changes the selection algorithm for the specified object group. An object
 * group's selection algorithm determines how the naming service directs client
 * requests to object group members.
 */
int
NS_group_svc::group_modify (
  const char* group,
  const char* policy)
{
  ACE_UNUSED_ARG (group);
  ACE_UNUSED_ARG (policy);

  #if 0

  if (group == 0 || policy == 0)
  {
    return -1;
  }

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) nsgroup - group_modify(%s,%s)\n", group, policy));

  CosLoadBalancing::StrategyInfo strategy_info;
  determine_policy_string(policy, strategy_info);

  try
  {
    PortableGroup::Properties props (1);
    props.length (1);
    props[0].nam.length (1);
    props[0].nam[0].id = CORBA::string_dup ("org.omg.CosLoadBalancing.StrategyInfo");
    props[0].val <<= strategy_info;

    PortableGroup::ObjectGroup_ptr group_ptr = this->naming_manager_->get_object_group_ref_from_name (group);
    // TAO_PG_PropertyManager::set_properties_dynamically throws CORBA::NO_IMPLEMENT ()
    this->naming_manager_->set_properties_dynamically (group_ptr, props);
    CORBA::release (group_ptr);
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("NS_group_svc group_modify exception");
  }
  #endif

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) group_modify(%s,%s) NOT IMPLEMENTED\n", group, policy));
  return -1;

}


/**
 * The naming service shall provide a command line utility to remove a specified
 * object group from the naming service.
 * tao_nsgroup group_remove <group>
 * Removes the specified object group from the naming service.
 */
int
NS_group_svc::group_remove (const char* group_name)
{
  if (group_name == 0)
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) group_remove invalid group name argument\n"));
    return -1;
  }

  try
    {
      this->naming_manager_->delete_object_group (group_name);
    }
  catch (const PortableGroup::ObjectGroupNotFound& ex)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Unable to find group(%s) \n", group_name));
      return -1;
    }
  catch (const CORBA::Exception& ex)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P|%t) Unable to remove group(%s) \n", group_name));
      ex._tao_print_exception ("group_remove exception");
      return -1;
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
  const char* group,
  const char* location,
  const char* ior)
{
  if (group == 0 || location == 0 || ior == 0 )
  {
    return -1;
  }

  try
  {
    PortableGroup::Location location_name;
    location_name.length (1);
    location_name[0].id = CORBA::string_dup(location);

    PortableGroup::ObjectGroup_var group_var = this->naming_manager_->get_object_group_ref_from_name (group);
    CORBA::Object_var ior_var = this->orb_->string_to_object( CORBA::string_dup(ior) );

    this->naming_manager_->add_member (group_var.in(), location_name, ior_var.in());
  }
  catch (const PortableGroup::ObjectGroupNotFound& ex)
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Unable to find group(%s) \n", group));
    return -1;
  }
  catch (const PortableGroup::ObjectNotAdded& ex)
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Unable to add location(%s) to group(%s) \n", location, group));
    return -1;
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("member_add exception");
    return -1;
  }

  return 0;
}

/**
 * The naming service shall provide a command line utility for displaying all
 * members (object references) for a specified object group.
 * Lists the members of the specified object group.
 */
int
NS_group_svc::member_list (const char* group)
{
  if (group == 0)
  {
    return -1;
  }

  try
  {
    PortableGroup::ObjectGroup_var group_var = this->naming_manager_->get_object_group_ref_from_name (group);
    PortableGroup::Locations_var locations = this->naming_manager_->locations_of_members (group_var.in());

    for (unsigned int i = 0; i < locations->length(); ++i)
    {
      const PortableGroup::Location & loc = locations[i];
      if (loc.length() > 0) {
        std::cout << loc[0].id.in() << std::endl;
      }
    }
  }
  catch (const PortableGroup::ObjectGroupNotFound& ex )
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Unable to find group(%s) \n", group));
    return -1;
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("member_list exception");
    return -1;
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
  const char* group,
  const char* location)
{
  if (group == 0 || location == 0)
  {
    return -1;
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
    location_name[0].id = CORBA::string_dup(location);

    PortableGroup::ObjectGroup_var group_var = this->naming_manager_->get_object_group_ref_from_name (group);
    this->naming_manager_->remove_member (group_var.in(), location_name);
  }
  catch (const PortableGroup::ObjectGroupNotFound& ex )
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Unable to find group(%s) \n", group));
    return -1;
  }
  catch (const PortableGroup::MemberNotFound& ex )
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Unable to find member(%s) \n", location));
    return -1;
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("member_remove exception");
    return -1;
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
  const char* group,
  const char* location)
{
  if (group == 0 || location == 0)
  {
    return -1;
  }

  //Get and display IOR for the member
  try
  {
    PortableGroup::Location location_name;
    location_name.length (1);
    location_name[0].id = CORBA::string_dup(location);

    PortableGroup::ObjectGroup_var group_var = this->naming_manager_->get_object_group_ref_from_name (group);
    CORBA::Object_var ior_var = this->naming_manager_->get_member_ref (group_var.in(), location_name);
    CORBA::String_var ior_string  = this->orb_->object_to_string( ior_var.in() );

    std::cout << ior_string.in() << std::endl;

    //ACE_DEBUG ((LM_DEBUG, "(%P|%t) member_show(%s,%s,%s)\n", group, location, ior_string.in() ));

  }
  catch (const PortableGroup::ObjectGroupNotFound& ex )
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Unable to find group(%s) \n", group));
    return -1;
  }
  catch (const PortableGroup::MemberNotFound& ex )
  {
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Unable to find member(%s) \n", location));
    return -1;
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception ("member_show exception");
    return -1;
  }

  return 0;
}
