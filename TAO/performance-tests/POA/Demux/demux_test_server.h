// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Demux
//
// = FILENAME
//    demux_test_server.h
//
//    Server-side demux_test class
//
// = AUTHOR
//
//    Aniruddha Gokhale
//
// ============================================================================

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
  Demux_Test_Server (void);
  // ctor

  ~Demux_Test_Server (void);
  // dtor

  int run (void);
  // Execute client code.

  int init (int argc, ACE_TCHAR **argv);
  // Initialize the communication endpoint with server.

  int calculate_avg_latency (void);
  // Calculates the statistics by reading different data values from results.dat.

private:

  int parse_args (void);
  // parse command line arguments (if any)

  int init_naming_service (void);
  // initialize the naming service

  int argc_;
  // # of arguments on the command line.

  ACE_TCHAR **argv_;
  // arguments from command line.

  CORBA::ORB_var orb_;
  // the ORB

  PortableServer::POA_var root_poa_;
  // the root POA (we will not have any objects registered with this)

  PortableServer::POAManager_var poa_mgr_;
  // The POA manager

  PortableServer::POA_var child_poa_ [TAO_DEMUX_TEST_MAX_POAS];
  // An array of child POAs with the next one being the child of the previous
  // one

  TAO_Naming_Client my_name_client_;
  // An instance of the name client used for resolving the factory
  // objects.

  //  Demux_Test_i demux_test_[TAO_DEMUX_TEST_MAX_POAS * TAO_DEMUX_TEST_MAX_OBJS];
  // The actual demux_test objects

  CORBA::ULong num_POAs_;
  // number of POAs

  CORBA::ULong  num_objs_;
  // number of objects

  FILE *poa_fp_;
  // file storing POA names

  FILE *ior_fp_;
  // file for storing IORs

  FILE *servant_fp_;
  // file for storing Servant Names.

  int use_user_id_;
  // Use the USER_ID policy for servants as opposed to System ID.

  int use_transient_poas_;
  // Use the Transient POAs for servant activation as opposed
  // to persistent POAs.

};


#endif /* DOORS_DEMUX_TEST_SERVER_H */
