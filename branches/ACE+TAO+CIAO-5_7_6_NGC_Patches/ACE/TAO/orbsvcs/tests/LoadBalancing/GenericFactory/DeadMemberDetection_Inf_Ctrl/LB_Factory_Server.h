// -*- C++ -*-

//=============================================================================
/**
 *  @file   LB_Factory_Server.h
 *
 *  $Id$
 *
 *  @author Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#include "orbsvcs/CosLoadBalancingC.h"
#include "orbsvcs/PortableGroupC.h"
#include "tao/PortableServer/PortableServer.h"

/**
 * @class LB_Factory_Server
 *
 * @brief Encapsulate the test in class.
 *
 * This is a class used to retrieve the LoadManager reference and
 * create the Object Group.  Servants add themselves to the object
 * group and the object group reference is published to the clients.
 * Clients then invoke the "servant operations" on the LoadManager
 * itself.  At this point the Load Balancing cycle starts.
 */
class LB_Factory_Server
{
public:

  /// Constructor
  LB_Factory_Server (int argc, ACE_TCHAR ** argv);

  /// destroys LoadManager, ORB and POA.
  int destroy (void);

  int parse_args (int& argc, ACE_TCHAR **& argv);

  /// start the ORB.
  int init (void);

  /// Run the ORB.
  int run (void);

  /// for servants to register to the initialised ORB.
  CORBA::ORB_ptr orb (void);

private:

  /// Create the Object Group using the Load Manager Reference.
  int create_object_group (const char *loc_1, const char *loc_2, const char *loc_3);

  int write_ior_to_file (const char * ior, const char * ior_file);

private:

  /// Load Manager
  CosLoadBalancing::LoadManager_var lm_;

  /// factory id for the object group.
  PortableGroup::GenericFactory::FactoryCreationId_var fcid_;

  CORBA::ORB_var orb_;
  int argc_;
  ACE_TCHAR ** argv_;
  PortableServer::POA_var root_poa_;
  int server_id_;
  ACE_TString factory_ior_file_;
};
