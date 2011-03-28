
//=============================================================================
/**
 *  @file    client.h
 *
 *  $Id$
 *
 *  Test for multithreaded client using one global ORB (and Reactor)
 *
 *
 *  @author Michael Kircher
 */
//=============================================================================


#ifndef MT_CLIENT_H
#define MT_CLIENT_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "tao/ORB.h"
#include "MT_Client_TestC.h"

/**
 * @class MT_Client
 *
 * @brief A simple client talking to one server
 *
 * It is connecting to a server which holds the Object_A
 * or using the "-m" switch to a server holding the MT_Object
 */
class MT_Client
{

public:
  /// = Constructor and destructor.
  MT_Client (void);
  ~MT_Client (void);

  /// Execute client example code.
  int run (void);

  /// Initialize the client communication endpoint with server.
  int init (int argc,
            ACE_TCHAR **argv,
            int client_number);

private:
  /// reading the IOR of object A in
  int read_ior (ACE_TCHAR *filename);

  /// Parses the arguments passed on the command line.
  int parse_args (void);

  /// # of arguments on the command line.
  int argc_;

  /// arguments from command line.
  ACE_TCHAR **argv_;

  /// Key of the object
  char *object_key_;

  /// Flag to tell server to shutdown.
  int shutdown_;

  /// Remember our orb.
  CORBA::ORB_var orb_var_;

  /// pointer to the mt Object
  MT_Object_var mT_Object_var_;

  /// This is used to choose the server...
  int client_number_;

  /// number of loops
  unsigned long iterations_;

};

/**
 * @class MT_Client_Task
 *
 * @brief Wrapper for the MT_Client to be an extra thread
 *
 * No big purpose, just wrapping
 */
class MT_Client_Task : public ACE_Task<ACE_SYNCH>
{
public:
  MT_Client_Task (int argc, ACE_TCHAR **argv, int client_number);

  virtual int svc (void);

private:
  MT_Client mT_Client_;
  int argc_;
  ACE_TCHAR **argv_;

  /// To determine if we use the first or the second server.
  int client_number_;
};

#endif /* MT_CLIENT_H */
