// $Id$
// -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Load_Balancing
//
// = FILENAME
//    Identity_Client.h
//
// = DESCRIPTION
//    Code for Identity_Client, which is used in conjunction with
//    Identity_Server to test/demonstrate the functionality of the
//    Load Balancing service.
//
// = AUTHORS
//    Marina Spivak <marina@cs.wustl.edu>
//    with modifications by Bala Natarajan <bala@cs.wustl.edu>
// ============================================================================

#ifndef IDENTITY_CLIENT_H_
#define IDENTITY_CLIENT_H_

#include "tao/PortableServer/ORB_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Identity_Client
{
  // =TITLE
  //   Contacts the <Object_Group_Factory> in th Load Balancing Server
  //   to obtain a reference to the type of the <Object_Group>
  //   specified on the command line.  Then, queries the
  //   <Object_Group> for its id and members, and prints that
  //   information.  Finally, performs <number_of_invocations_>
  //   <Identity::get_name> calls, performing <Object_Group::resolve>
  //   before each <get_name> call in order to get the <Identity>
  //   reference to use for the call.  (This provides an example of
  //   fine-grained, i.e., per call,  Load Balancing among all the
  //   <Identity> objects registered with the <Object_Group> for the
  //   client's <get_name> calls.
  //
public:

  Identity_Client (void);
  // Default constructor.

  ~Identity_Client (void);
  // Destructor.

  int init (int argc, char *argv[]);
  // Initializes <orb_manager_>, and parses commandline arguments.

  int run (ACE_ENV_SINGLE_ARG_DECL);
  // See TITLE.

private:
  int parse_args (int argc, char *argv[]);
  // Parses the commandline arguments.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  const char *group_factory_ior_;
  // The ior of the <Object_Group_Factory> object we shall use to
  // to obtain an <Object_Group> object.

  size_t number_of_invocations_;
  // Number of times to invoke <get_name> method on <Identity>
  // objects.  The default value is 5.

  size_t use_random_;
  // Flag indicating which <Object_Group> to use to obtain references
  // to <Identity> objects.  Random group should be used if the flag
  // is set to 1, and round robin group otherwise.  Round robin is the
  // default.

  int iterations_;
};

#endif /* IDENTITY_CLIENT_H_ */
