// -*- C++ -*-

//=============================================================================
/**
 *  @file    Identity_Client.h
 *
 *  Code for Identity_Client, which is used in conjunction with
 *  Identity_Server to test/demonstrate the functionality of the
 *  Load Balancing service.
 *
 *  @author Marina Spivak <marina@cs.wustl.edu> with modifications by Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef IDENTITY_CLIENT_H_
#define IDENTITY_CLIENT_H_

#include "tao/Utils/ORB_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Identity_Client
 =TITLE
 * Contacts the <Object_Group_Factory> in th Load Balancing Server
 * to obtain a reference to the type of the <Object_Group>
 * specified on the command line.  Then, queries the
 * <Object_Group> for its id and members, and prints that
 * information.  Finally, performs <number_of_invocations_>
 * <Identity::get_name> calls, performing <Object_Group::resolve>
 * before each <get_name> call in order to get the <Identity>
 * reference to use for the call.  (This provides an example of
 * fine-grained, i.e., per call,  Load Balancing among all the
 * <Identity> objects registered with the <Object_Group> for the
 * client's <get_name> calls.
 */
class Identity_Client
{
public:
  /// Default constructor.
  Identity_Client () = default;

  /// Destructor.
  ~Identity_Client () = default;

  Identity_Client (Identity_Client const &) = delete;
  Identity_Client (Identity_Client &&) = delete;
  Identity_Client &operator= (Identity_Client const &) = delete;
  Identity_Client &operator= (Identity_Client &&) = delete;

  /// Initializes <orb_manager_>, and parses commandline arguments.
  int init (int argc, ACE_TCHAR *argv[]);

  /// See TITLE.
  int run ();

private:
  /// Parses the commandline arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// The ORB manager.
  TAO_ORB_Manager orb_manager_;

  /// The ior of the <Object_Group_Factory> object we shall use to
  /// to obtain an <Object_Group> object.
  /// Borrowed from the command-line arguments.
  ACE_TCHAR const *group_factory_ior_ {};

  /// Number of times to invoke <get_name> method on <Identity>
  /// objects.  The default value is 5.
  size_t number_of_invocations_ {5};

  /**
   * Flag indicating which <Object_Group> to use to obtain references
   * to <Identity> objects. Random group is used when the flag is true,
   * and round robin otherwise. Round robin is the default.
   */
  bool use_random_ {};

  int iterations_ {};
};

#endif /* IDENTITY_CLIENT_H_ */
