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
  ACE_TString group_ior_file { ACE_TEXT ("group.ior") };
  ACE_TString factory1_ior { ACE_TEXT ("file://factory1.ior") };
  ACE_TString factory2_ior { ACE_TEXT ("file://factory2.ior") };
  ACE_TString factory3_ior { ACE_TEXT ("file://factory3.ior") };
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
  LB_server (int argc, ACE_TCHAR ** argv);

  LB_server (LB_server const &) = delete;
  LB_server &operator= (LB_server const &) = delete;
  LB_server (LB_server &&) = delete;
  LB_server &operator= (LB_server &&) = delete;

  /// destroys LoadManager, ORB and POA.
  int destroy ();

  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// start the ORB.
  int init ();

  /// Run the ORB.
  int run ();

private:
  /// Create the Object Group using the Load Manager Reference.
  int create_object_group ();

  int write_ior_to_file (char const *ior, ACE_TCHAR const *ior_file);

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
  LB_Server_Options options_;
};

#endif /* LB_SERVER_H */
