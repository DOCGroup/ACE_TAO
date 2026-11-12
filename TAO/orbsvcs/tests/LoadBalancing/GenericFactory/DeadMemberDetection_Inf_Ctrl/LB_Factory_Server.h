// -*- C++ -*-

//=============================================================================
/**
 *  @file   LB_Factory_Server.h
 *
 *  @author Jaiganesh Balasubramanian <jai@doc.ece.uci.edu>
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef LB_FACTORY_SERVER_H
#define LB_FACTORY_SERVER_H

#include "orbsvcs/CosLoadBalancingC.h"
#include "orbsvcs/PortableGroupC.h"
#include "tao/PortableServer/PortableServer.h"

struct LB_Factory_Server_Options
{
  ACE_TString factory_ior_file;
  ACE_TString direct_object_ior_file { ACE_TEXT ("direct_obj2.ior") };
  int server_id {};
  int pong_delay {};
};

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

  LB_Factory_Server (LB_Factory_Server const &) = delete;
  LB_Factory_Server &operator= (LB_Factory_Server const &) = delete;
  LB_Factory_Server (LB_Factory_Server &&) = delete;
  LB_Factory_Server &operator= (LB_Factory_Server &&) = delete;

  /// destroys LoadManager, ORB and POA.
  int destroy ();

  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// start the ORB.
  int init ();

  /// Run the ORB.
  int run ();

private:
  /// Create the Object Group using the Load Manager Reference.
  int write_ior_to_file (char const *ior, ACE_TCHAR const *ior_file);

private:
  /// Load Manager
  CosLoadBalancing::LoadManager_var lm_;

  /// factory id for the object group.
  PortableGroup::GenericFactory::FactoryCreationId_var fcid_;

  CORBA::ORB_var orb_;
  int argc_;
  ACE_TCHAR **argv_;
  PortableServer::POA_var root_poa_;
  LB_Factory_Server_Options options_;
};

#endif /* LB_FACTORY_SERVER_H */
