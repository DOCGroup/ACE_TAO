// -*- C++ -*-

//=============================================================================
/**
 *  @file   LB_server.h
 *
 *  @author Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef LB_SERVER_H
#define LB_SERVER_H

#include "orbsvcs/CosLoadBalancingC.h"
#include "orbsvcs/PortableGroupC.h"
#include "tao/PortableServer/PortableServer.h"

struct LB_Server_Options
{
  ACE_TString ior_output_file { ACE_TEXT ("obj.ior") };
};

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

  LB_server (LB_server const &) = delete;
  LB_server &operator= (LB_server const &) = delete;
  LB_server (LB_server &&) = delete;
  LB_server &operator= (LB_server &&) = delete;

  /// destroys LoadManager, ORB and POA.
  int destroy ();

  /// start the ORB.
  int start_orb_and_poa ();

  /// Create the Object Group using the Load Manager Reference.
  int create_object_group (char const *loc_1, char const *loc_2);

  /// Run the ORB.
  int run ();

private:
  int write_ior_to_file (char const *ior);
  int parse_args (int argc, ACE_TCHAR *argv[]);

private:
  /// Load Manager
  CosLoadBalancing::LoadManager_var lm_;

  /// Object Group.
  CORBA::Object_var object_group_;

  /// factory id for the object group.
  PortableGroup::GenericFactory::FactoryCreationId_var fcid_;

  CORBA::ORB_var orb_;
  int argc_;
  // Borrowed from main for ORB_init.
  ACE_TCHAR **argv_;
  PortableServer::POA_var root_poa_;
  LB_Server_Options options_;
};

#endif /* LB_SERVER_H */
