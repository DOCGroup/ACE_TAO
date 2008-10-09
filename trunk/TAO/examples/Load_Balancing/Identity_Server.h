// $Id$
// -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Load_Balancing
//
// = FILENAME
//    Identity_Server.h
//
// = DESCRIPTION
//    Driver for identity server, which is used to test/demonstrate
//    the functionality of the Load Balancing service.
//
// = AUTHORS
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef IDENTITY_SERVER_H_
#define IDENTITY_SERVER_H_

#include "tao/Utils/ORB_Manager.h"
#include "Load_BalancerC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Identity_Server
{
  // =TITLE
  //   Contacts the <Object_Group_Factory> in the Load Balancing Server
  //   to create two <Object_Group>s, one round robin and one random.
  //   Then, creates a number of <Identity> objects and registers them with
  //   the created <Object_Group>s in a manner specified by the
  //   commandline arguments.
  //
public:

  Identity_Server (void);
  // Default constructor.

  ~Identity_Server (void);
  // Destructor.

  int init (int argc, ACE_TCHAR *argv[]);
  // Performs all the initializations necessary before going into the
  // ORB event loop.

  int run (void);
  // Run the server.

private:
  int parse_args (int argc, ACE_TCHAR *argv[]);
  // Parses the commandline arguments.

  void create_objects (size_t number_of_objects,
                       Load_Balancer::Object_Group_ptr group);
  // Creates the specified number of identity objects, and registers
  // each one with the provided <Object_Group>.  Identity servants are
  // given names
  //"Identity object 1" .... "Identity object <number_of_objects>".

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  const ACE_TCHAR *group_factory_ior_;
  // The ior of the <Object_Group_Factory> object we shall use to
  // create <Object_Group>s to load balance our <Identity> objects.

  size_t random_objects_;
  // Number of <Identity> objects to create for registering with
  // the random <Object_Group>.  The default value is 5.

  size_t rr_objects_;
  // Number of <Identity> objects to create for registering with the
  // round robin <Object_Group>.  The default value is 5.
};

#endif /* IDENTITY_SERVER_H_ */
