//==========================================================================
/**
 *  @file nsgroup_svc.h
 *
 *  @author Phillip LaBanca <labancap@ociweb.com>
 */
//==========================================================================

#ifndef i_nsgroup_svc_h
#define i_nsgroup_svc_h

#include "orbsvcs/FT_NamingManagerC.h"
#include /**/ "orbsvcs/Naming/FaultTolerant/ftnaming_intf_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


/**
 * @class NS_group_svc
 *
 * @brief Encapsulate the NS group operations in a class.
 *
 *
 */
class  TAO_FtNaming_Intf_Export NS_group_svc
{
public:
  /**
   *  Constructor
   */
  explicit NS_group_svc (bool quiet = false);

  NS_group_svc (NS_group_svc const &) = delete;
  NS_group_svc (NS_group_svc &&) = delete;
  NS_group_svc &operator= (NS_group_svc const &) = delete;
  NS_group_svc &operator= (NS_group_svc &&) = delete;

  /**
   * The naming service shall provide a command line utility for creating
   * object groups.
   *
   * @param group    group name
   * @param policy   round
   *
   * @return 0 on success, -1 on failure, -2 on bad argument.
   */
 int group_create (ACE_TCHAR const *group, ACE_TCHAR const *policy);

  /**
   *  The naming service shall provide a command line utility for binding an
   *  object group to a path in the naming service.
   *
   *  @param group    group name
   *  @param path     stringified name in the naming service
   *
   *  @return 0 on success, -1 on failure, -2 on bad argument.
   */
  int group_bind (ACE_TCHAR const *group, ACE_TCHAR const *path);

  /**
   *  @param group    group name
   *  @param path     stringified name in the naming service
   *
   *  @return 0 on success, -1 on failure, -2 on bad argument.
   */
  int group_unbind (ACE_TCHAR const *path);

  /**
   *  The naming service shall provide a command line utility to display all
   *  defined object groups within the naming service.
   *
   *  @return 0 on success, -1 on failure, -2 on bad argument.
   */
  int group_list () const;

  /**
   *  The naming service shall provide a command line utility to modify the load
   *  balancing strategy for a specified object group.
   *
   *  @param group    group name
   *  @param policy   round
   *
   *  @return 0 on success, -1 on failure, -2 on bad argument.
   */
  int group_modify (ACE_TCHAR const *group, ACE_TCHAR const *policy);

  /**
   *  The naming service shall provide a command line utility to remove a
   *  specified object group from the naming service.
   *
   *  @param group    group name
   *
   *  @return 0 on success, -1 on failure, -2 on bad argument.
   */
  int group_remove (ACE_TCHAR const *group);

  /**
   *  The naming service shall provide a command line utility for adding object
   *  references to an object group.
   *
   *  @param group    group name
   *  @param location member location string
   *  @param ior      member stringified ior
   *
   *  @return 0 on success, -1 on failure, -2 on bad argument.
   */
  int member_add (ACE_TCHAR const *group,
                  ACE_TCHAR const *location,
                  ACE_TCHAR const *ior);

  /**
   *  The naming service shall provide a command line utility for displaying all
   *  members (object references) for a specified object group.
   *
   *  @param group    group name
   *
   *  @return 0 on success, -1 on failure, -2 on bad argument.
   */
  int member_list (ACE_TCHAR const *group);

  /**
   *  The naming service shall provide a command line utility for removing
   *  object references from an object group.
   *
   *  @param group    group name
   *  @param location member location string
   *
   *  @return 0 on success, -1 on failure, -2 on bad argument.
   */
  int member_remove (ACE_TCHAR const *group, ACE_TCHAR const *location);

  /**
   *  The naming service shall provide a command line utility to display an
   *  object reference from a specified object group.
   *
   *  @param group    group name
   *  @param location member location string
   *
   *  @return zero for success; nonzero for failure.
   */
  int member_show (ACE_TCHAR const *group, ACE_TCHAR const *location);

  /**
   *  @param group    group name
   *
   *  @return true if the specified object group name is found
   */
  bool group_exist (ACE_TCHAR const *group_name) const;

  /**
   *  @return 0 on success, -1 on failure.
   */
  int set_orb( CORBA::ORB_ptr value);

  /**
   *  @return 0 on success, -1 on failure.
   */
  int set_naming_manager( FT_Naming::NamingManager_ptr value);

  /**
   *  @return 0 on success, -1 on failure.
   */
  int set_name_context( CosNaming::NamingContextExt_ptr value);

private:
  /**
   *  determine strategy based on policy string
   *
   *  @param const ACE_TCHAR *policy ["round"]
   *  @param FT_Naming::LoadBalancingStrategyValue& value
   *  @return true on success, false on failure.
   */
  static bool determine_policy_string (
    ACE_TCHAR const *policy,
    FT_Naming::LoadBalancingStrategyValue& value);

  /**
   *  @return 0 on success, -1 on failure.
   */
  int display_load_policy_group (
    FT_Naming::LoadBalancingStrategyValue strategy,
    ACE_TCHAR const *display_label) const;

private:
  FT_Naming::NamingManager_var naming_manager_;

  CosNaming::NamingContextExt_var name_service_;

  CORBA::ORB_var orb_;
  bool quiet_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
