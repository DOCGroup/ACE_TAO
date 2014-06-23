// -*- C++ -*-

//=============================================================================
/**
 *  @file   LB_server.h
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

#include "ace/Get_Opt.h"

/**
 * @class LB_server
 *
 * @brief Encapsulate the test in class.
 *
 * This is a class used to retrieve the LoadManager reference and
 * create the Object Group.  Servants add themselves to the object
 * group and the object group reference is published to the clients.
 * Clients then invoke the "servant operations" on the LoadManager
 * itself.  At this point the Load Balancing cycle starts.
 */
class LB_server
{
public:

  /// Constructor
  LB_server (int argc, ACE_TCHAR **argv);

  /// destroys LoadManager, ORB and POA.
  int destroy (void);

  /// start the ORB.
  int start_orb_and_poa (void);

  /// Create the Object Group using the Load Manager Reference.
  int create_object_group (const char *loc_1, const char *loc_2);

  /// Run the ORB.
  int run (void);

  /// for servants to register to the initialised ORB.
  CORBA::ORB_ptr orb (void);

private:

  int write_ior_to_file (const char *);
  int parse_args (int, ACE_TCHAR **);

private:

  /// Load Manager
  CosLoadBalancing::LoadManager_var lm_;

  /// Object Group.
  CORBA::Object_var object_group_;

  /// factory id for the object group.
  PortableGroup::GenericFactory::FactoryCreationId_var fcid_;

  CORBA::ORB_var orb_;
  int argc_;
  ACE_TCHAR **argv_;
  PortableServer::POA_var root_poa_;
  const ACE_TCHAR *ior_output_file_;
};
