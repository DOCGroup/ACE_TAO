/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests
//
// = FILENAME
//    server.h
//
// = DESCRIPTION
//      This class facilitates the testing of Load Balancer.
//
// = AUTHORS
//      Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
//      Ossama Othman <ossama@doc.ece.uci.edu>
//
// ============================================================================

#include "orbsvcs/orbsvcs/CosLoadBalancingC.h"
#include "orbsvcs/orbsvcs/PortableGroupC.h"
#include "tao/PortableServer/PortableServer.h"

// Forward Declaration of the kind of servant we would like to handle
// by the server.
class Basic;

class LB_Basic_Test
{
  // This is a class used to retrieve the LoadManager reference
  // and create the Object Group.
  // Servants add themselves to the object group and the object group
  // reference is published to the clients.
  // Clients then invoke the "servant operations" on the LoadManager itself.
  // At this point the Load Balancing cycle starts.
public:

  LB_Basic_Test (int argc, char **argv);
  // Constructor

  ~LB_Basic_Test (void);
  // Destructor

  int start_orb_and_poa (void);
  // start the ORB.
  
  int init (int argc, char **argv);
  // Get the LoadManager Interface.

  int create_object_group (void);
  // Create the Object Group using the Load Manager Reference.

  int register_servant (Basic *servant, const char *loc);
  // register the servants with the object group.

  CORBA::ORB_ptr orb () { return this->orb_.in ();}
  // for servants to register to the initialised ORB.

private:

  CosLoadBalancing::LoadManager_var lm_;
  // Load Manager

  CORBA::Object_var object_group_;
  // Object Group.

  PortableGroup::GenericFactory::FactoryCreationId_var fcid_;
  // factory id for the object group.

  static int called_once_;
  // Used to force the creation of the object group only once.

  int write_ior_to_file (const char *);
  // write the IOR to a file so that it can be read later.

  CORBA::ORB_var orb_;
  int argc_;
  char **argv_;
  PortableServer::POA_var root_poa_;
};
