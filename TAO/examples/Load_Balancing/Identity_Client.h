/**
 *  @file    Identity_Client.h
 *
 *  Code for Identity_Client, which is used in conjunction with
 *  Identity_Server to test/demonstrate the functionality of the
 *  Load Balancing service.
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */

#ifndef IDENTITY_CLIENT_H_
#define IDENTITY_CLIENT_H_

#include "tao/Utils/ORB_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Identity_Client
 *
 * Contacts the @c Object_Group_Factory in the Load Balancing Server
 * to obtain a reference to the type of the @c Object_Group
 * specified on the command line.  Then, queries the
 * @c Object_Group for its id and members, and prints that
 * information.  Finally, performs @c number_of_invocations_
 * @c Identity::get_name calls, performing @c Object_Group::resolve
 * before each @c get_name call in order to get the @c Identity
 * reference to use for the call.  (This provides an example of
 * fine-grained, i.e., per call,  Load Balancing among all the
 * @c Identity objects registered with the @c Object_Group for the
 * client's @c get_name calls.
 */
class Identity_Client
{
public:
  Identity_Client () = default;

  ~Identity_Client () = default;

  Identity_Client (Identity_Client const &) = delete;
  Identity_Client (Identity_Client &&) = delete;
  Identity_Client &operator= (Identity_Client const &) = delete;
  Identity_Client &operator= (Identity_Client &&) = delete;

  /// Initializes @c orb_manager_, and parses commandline arguments.
  int init (int argc, ACE_TCHAR *argv[]);

  /// See TITLE.
  int run ();

private:
  /// Parses the commandline arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// The ORB manager.
  TAO_ORB_Manager orb_manager_;

  /// The ior of the @c Object_Group_Factory object we shall use to
  /// to obtain an @c Object_Group object.
  ACE_TCHAR const *group_factory_ior_ {};

  /// Number of times to invoke @c get_name method on @c Identity
  /// objects.  The default value is 5.
  size_t number_of_invocations_ {5};

  /**
   * Flag indicating which @c Object_Group to use to obtain references
   * to @c Identity objects.  Random group should be used if the flag
   * is true, and round robin group otherwise.  Round robin is the
   * default.
   */
  bool use_random_ {};
};

#endif /* IDENTITY_CLIENT_H_ */
