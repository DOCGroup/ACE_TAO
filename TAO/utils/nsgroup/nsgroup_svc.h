//==========================================================================
/**
 *  @file nsgroup_svc.h
 *
 *  $Id$
 *
 *  @author Phillip LaBanca <labancap@ociweb.com>
 */
//==========================================================================

#ifndef i_nsgroup_svc_h
#define i_nsgroup_svc_h

#include "orbsvcs/FT_NamingManagerC.h"

/**
 * @class NS_group_svc
 *
 * @brief Encapsulate the NS group operations in a class.
 *
 *
 */
class NS_group_svc
{
public:

  enum NSGROUP_COMMAND {
    NSGROUP_NONE,
    NSGROUP_HELP,
    NSGROUP_GROUP_CREATE,
    NSGROUP_GROUP_BIND,
    NSGROUP_GROUP_UNBIND,
    NSGROUP_GROUP_MODIFY,
    NSGROUP_GROUP_LIST,
    NSGROUP_GROUP_REMOVE,
    NSGROUP_MEMBER_LIST,
    NSGROUP_MEMBER_ADD,
    NSGROUP_MEMBER_REMOVE,
    NSGROUP_MEMBER_SHOW
  };

  /// Constructor
  NS_group_svc (void);

  /// Constructor
  NS_group_svc (int argc, ACE_TCHAR **argv);

  /// destroys LoadManager, ORB and POA.
  int destroy (void);

  /// start the ORB.
  int start_orb (void);

  /// parse command line, validate arguments and run the command
  int run_cmd (void);

  /// Display command line interface usage
  int show_usage( void );

  /// The naming service shall provide a command line utility for creating
  /// object groups.
  int group_create (const char* group, const char* policy);

  /// The naming service shall provide a command line utility for binding an
  /// object group to a path in the naming service.
  int group_bind (const char* group, const char* path);
  int group_unbind (const char* path);

  /// The naming service shall provide a command line utility to display all
  /// defined object groups within the naming service.
  int group_list (void);

  /// The naming service shall provide a command line utility to modify the load
  /// balancing strategy for a specified object group.
  int group_modify (const char* group, const char* policy);

  /// The naming service shall provide a command line utility to remove a
  /// specified object group from the naming service.
  int group_remove (const char* group);

  /// The naming service shall provide a command line utility for adding object
  /// references to an object group.
  int member_add (const char* group,
                  const char* location,
                  const char* ior);

  /// The naming service shall provide a command line utility for displaying all
  /// members (object references) for a specified object group.
  int member_list (const char* group);

  /// The naming service shall provide a command line utility for removing
  /// object references from an object group.
  int member_remove (const char* group, const char* location);

  /// The naming service shall provide a command line utility to display an
  /// object reference from a specified object group.
  int member_show (const char* group, const char* location);

  /// returns true if the specified object group name is found
  bool group_exist (const char* group_name);

  const char * group_arg(void) const { return group_arg_; }
  const char * policy_arg(void) const { return policy_arg_; }
  const char * location_arg(void) const { return location_arg_; }
  const char * ior_arg(void) const { return ior_arg_; }
  const char * namepath_arg(void) const { return namepath_arg_; }

  int set_orb( CORBA::ORB_ptr value);
  int set_naming_manager( FT_Naming::NamingManager_ptr value);
  int set_name_context( CosNaming::NamingContextExt_ptr value);

private:

  /// parse command line arguments
  NSGROUP_COMMAND parse_command_line (void);

  /// determine stategy based on policy string value default to ROUND_ROBIN
  FT_Naming::LoadBalancingStrategyValue determine_policy_string (const char *policy);

  int display_load_policy_group( FT_Naming::LoadBalancingStrategyValue strategy, const char *display_label);

private:

  FT_Naming::NamingManager_var naming_manager_;

  /// Name Service
  CosNaming::NamingContextExt_var name_service_;

  CORBA::ORB_var orb_;

  int argc_;
  ACE_TCHAR **argv_;

  /// parsed command result
  NSGROUP_COMMAND nsgroup_cmd_;

  /// parsed command line arguments
  const char *group_arg_;
  const char *policy_arg_;
  const char *typeid_arg_;
  const char *location_arg_;
  const char *ior_arg_;
  const char *namepath_arg_;
};

#endif
