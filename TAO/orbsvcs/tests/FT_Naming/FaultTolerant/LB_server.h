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


#include "orbsvcs/FT_NamingManagerC.h"
#include "orbsvcs/PortableGroupC.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/Naming/Naming_Client.h"

// Forward Declaration of the kind of servant we would like to handle
// by the server.
class Basic;

/**
 * @class LB_server
 *
 * @brief Encapsulate the test in class.
 *
 * TODO:  Fix description
 * This is a class used to retrieve the NamingManager reference and
 * create the Object Group.  Servants add themselves to the object
 * group and the object group reference is published to the clients.
 * Clients then invoke the "servant operations" on the NamingManager
 * itself.  At this point the Load Balancing cycle starts.
 */
class LB_server
{
public:

  /// Constructor
  LB_server (int argc, ACE_TCHAR **argv);

  /// destroys NamingManager, ORB and POA.
  int destroy (void);

  /// start the ORB.
  int start_orb_and_poa (void);

  /// Get the NamingManager Interface.
  int init (int argc, ACE_TCHAR **argv);

  /// Create the Object Group using the Load Manager Reference.
  /// Returns 0 on successful creation. Returns 1 if object
  /// group already exists. Returns -1 if object group cannot
  /// be created for some reason.
  int create_object_group (const char *group_name);

  /// register the servants with the object group.
  int register_servant (Basic *servant, const char *loc);

  /// remove a servant from the object group.
  int remove_servant (const char *loc);

  /// obtain the name service facade
  TAO_Naming_Client& name_svc ();

  /// for servants to register to the initialised ORB.
  CORBA::ORB_ptr orb (void);

  /// for servants to get the reference for object_group.
  CORBA::Object_ptr object_group (void);

  /// for servants to get the reference for object_group.
  FT_Naming::NamingManager_ptr naming_manager (void);

private:

    int parse_args (int, ACE_TCHAR **);

private:

  /// Load Manager
  FT_Naming::NamingManager_var naming_manager_;

  /// Object Group.
  CORBA::Object_var object_group_;

  /// factory id for the object group.
  PortableGroup::GenericFactory::FactoryCreationId_var fcid_;

  /// Used to force the creation of the object group only once.
  static int called_once_;

  /// write the IOR to a file so that it can be read later.
  int write_ior_to_file (const char *filename, const char *ior);

  CORBA::ORB_var orb_;
  int argc_;
  ACE_TCHAR **argv_;
  PortableServer::POA_var root_poa_;
  const ACE_TCHAR *ior_output_file_;
  TAO_Naming_Client name_svc_;
};
