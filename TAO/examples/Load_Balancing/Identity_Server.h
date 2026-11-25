/**
 *  @file    Identity_Server.h
 *
 *  Driver for identity server, which is used to test/demonstrate
 *  the functionality of the Load Balancing service.
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */

#ifndef IDENTITY_SERVER_H_
#define IDENTITY_SERVER_H_

#include "tao/Utils/ORB_Manager.h"
#include "Load_BalancerC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Identity_Server
 *
 * Contacts the @c Object_Group_Factory in the Load Balancing Server
 * to create two @c Object_Group objects, one round robin and one random.
 * Then, creates a number of @c Identity objects and registers them with
 * the created @c Object_Group objects in a manner specified by the
 * commandline arguments.
 */
class Identity_Server
{
public:
  Identity_Server () = default;

  ~Identity_Server () = default;

  Identity_Server (Identity_Server const &) = delete;
  Identity_Server (Identity_Server &&) = delete;
  Identity_Server &operator= (Identity_Server const &) = delete;
  Identity_Server &operator= (Identity_Server &&) = delete;

  /// Performs all the initializations necessary before going into the
  /// ORB event loop.
  int init (int argc, ACE_TCHAR *argv[]);

  /// Run the server.
  int run ();

private:
  /// Parses the commandline arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /**
   * Creates the specified number of identity objects, and registers
   * each one with the provided @c Object_Group.  Identity servants are
   * given names
   *"Identity object 1" .... "Identity object @a number_of_objects".
   */
  void create_objects (size_t number_of_objects,
                       Load_Balancer::Object_Group_ptr group);

  /// The ORB manager.
  TAO_ORB_Manager orb_manager_;

  /// The ior of the @c Object_Group_Factory object we shall use to
  /// create @c Object_Group objects to load balance our @c Identity objects.
  ACE_TCHAR const *group_factory_ior_ {};

  /// Number of @c Identity objects to create for registering with
  /// the random @c Object_Group.  The default value is 5.
  size_t random_objects_ {5};

  /// Number of @c Identity objects to create for registering with the
  /// round robin @c Object_Group.  The default value is 5.
  size_t rr_objects_ {5};
};

#endif /* IDENTITY_SERVER_H_ */
