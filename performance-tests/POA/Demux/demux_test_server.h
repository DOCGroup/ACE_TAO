//=============================================================================
/**
 *  @file    demux_test_server.h
 *
 *  $Id$
 *
 *  Server-side demux_test class
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef DOORS_DEMUX_TEST_SERVER_H
#define DOORS_DEMUX_TEST_SERVER_H

// @@ The timeprobes should be enabled with make probe=1 !!
// #define ACE_ENABLE_TIMEPROBES

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "tao/Timeprobe.h"
#include "tao/PortableServer/PortableServer.h"
#include "orbsvcs/Naming/Naming_Client.h"

#include "demux_test_i.h"
#include "demux_test_macros.h"

class Demux_Test_Server
{
public:
  /// ctor
  Demux_Test_Server (void);

  /// dtor
  ~Demux_Test_Server (void);

  /// Execute client code.
  int run (void);

  /// Initialize the communication endpoint with server.
  int init (int argc, ACE_TCHAR **argv);

  /// Calculates the statistics by reading different data values from results.dat.
  int calculate_avg_latency (void);

private:

  /// parse command line arguments (if any)
  int parse_args (void);

  /// initialize the naming service
  int init_naming_service (void);

  /// # of arguments on the command line.
  int argc_;

  /// arguments from command line.
  ACE_TCHAR **argv_;

  /// the ORB
  CORBA::ORB_var orb_;

  /// the root POA (we will not have any objects registered with this)
  PortableServer::POA_var root_poa_;

  /// The POA manager
  PortableServer::POAManager_var poa_mgr_;

  /// An array of child POAs with the next one being the child of the previous
  /// one
  PortableServer::POA_var child_poa_ [TAO_DEMUX_TEST_MAX_POAS];

  /// An instance of the name client used for resolving the factory
  /// objects.
  TAO_Naming_Client my_name_client_;

  //  Demux_Test_i demux_test_[TAO_DEMUX_TEST_MAX_POAS * TAO_DEMUX_TEST_MAX_OBJS];
  // The actual demux_test objects

  /// number of POAs
  CORBA::ULong num_POAs_;

  /// number of objects
  CORBA::ULong  num_objs_;

  /// file storing POA names
  FILE *poa_fp_;

  /// file for storing IORs
  FILE *ior_fp_;

  /// file for storing Servant Names.
  FILE *servant_fp_;

  /// Use the USER_ID policy for servants as opposed to System ID.
  int use_user_id_;

  /// Use the Transient POAs for servant activation as opposed
  /// to persistent POAs.
  int use_transient_poas_;

};


#endif /* DOORS_DEMUX_TEST_SERVER_H */
