
//=============================================================================
/**
 *  @file    local_server.h
 *
 *  $Id$
 *
 *   This class implements a simple server for the
 *   Nested Upcalls - MT Client test
 *
 *
 *  @author Michael Kircher
 */
//=============================================================================

#ifndef MT_CLIENT_TEST_MT_SERVER_H
#define MT_CLIENT_TEST_MT_SERVER_H

#include "MT_Object_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
class TAO_ORB_Manager;
TAO_END_VERSIONED_NAMESPACE_DECL

/**
 * @class MT_Server
 *
 * @brief This is the server for the mt_object
 *
 * See the README file for more information.
 */
class MT_Server
{

public:

  /// Default constructor
  MT_Server ();

  /// Destructor
  ~MT_Server (void);

  /// Initialize the MT_Server state - parsing arguments and ...
  int init (int argc,
            ACE_TCHAR **argv,
            TAO_ORB_Manager* orb_manager_ptr);

  /// Run the orb
  int run (void);

  /// Just run it briefly
  int run_ORB_briefly (void);

private:
  /// reading the IOR of mt object in
  int read_ior (ACE_TCHAR *filename);

  /// Parses the commandline arguments.
  int parse_args (void);

  /// Key of the object
  char *object_key_;

  /// File to output the IOR of the object A.
  FILE* ior_output_file_;

  /// The ORB manager
  TAO_ORB_Manager* orb_manager_ptr_;

  /// Implementation object
  MT_Object_i mT_Object_i_;

  /// reference for the distant MT Object
  MT_Object_var mT_Object_var_;

  /// Number of commandline arguments.
  int argc_;

  /// commandline arguments.
  ACE_TCHAR **argv_;

  /// number of times the server should call to the disted MT OBject
  unsigned int iterations_;

  /// IOR of our servant.
  CORBA::String_var str_;
};


/**
 * @class MT_Server_Task
 *
 * @brief Wrapper for the MT_Server to be an extra thread
 *
 * No big purpose, just wrapping
 */
class MT_Server_Task : public ACE_Task<ACE_SYNCH>
{
public:
  MT_Server_Task (ACE_Thread_Manager* thr_mgr_ptr,
                  int argc,
                  ACE_TCHAR **argv,
                  TAO_ORB_Manager* orb_manager_ptr);

  virtual int svc (void);

private:
  MT_Server mT_Server_;
  int argc_;
  ACE_TCHAR **argv_;
  TAO_ORB_Manager* orb_manager_ptr_;
};


#endif /* MT_CLIENT_TEST_MT_SERVER_H */
