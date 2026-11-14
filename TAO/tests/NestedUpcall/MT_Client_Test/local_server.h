
//=============================================================================
/**
 *  @file    local_server.h
 *
 *   This class implements a simple server for the
 *   Nested Upcalls - MT Client test
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

#include <string>
#include <vector>

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
  MT_Server () = default;

  ~MT_Server ();

  MT_Server (MT_Server const &) = delete;
  MT_Server (MT_Server &&) = delete;
  MT_Server &operator= (MT_Server const &) = delete;
  MT_Server &operator= (MT_Server &&) = delete;

  /// Initialize the MT_Server state - parsing arguments and ...
  int init (int argc,
            ACE_TCHAR **argv,
            TAO_ORB_Manager *orb_manager_ptr);

  /// Just run it briefly
  int run_ORB_briefly ();

private:
  /// reading the IOR of mt object in
  int read_ior (ACE_TCHAR const *filename);

  /// Parses the commandline arguments.
  int parse_args ();

  /// Key of the object
  std::string object_key_;

  /// ORB manager borrowed from the client main thread while this task runs.
  TAO_ORB_Manager *orb_manager_ptr_ {};

  /// Implementation object
  MT_Object_i mT_Object_i_;

  /// reference for the distant MT Object
  MT_Object_var mT_Object_var_;

  /// Number of commandline arguments.
  int argc_ {};

  /// Command-line arguments.
  std::vector<ACE_TCHAR *> argv_;

  /// number of times the server should call to the disted MT OBject
  unsigned int iterations_ {1};

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
  MT_Server_Task (ACE_Thread_Manager *thr_mgr_ptr,
                  int argc,
                  ACE_TCHAR **argv,
                  TAO_ORB_Manager *orb_manager_ptr);

  int svc () override;

  MT_Server_Task (MT_Server_Task const &) = delete;
  MT_Server_Task (MT_Server_Task &&) = delete;
  MT_Server_Task &operator= (MT_Server_Task const &) = delete;
  MT_Server_Task &operator= (MT_Server_Task &&) = delete;

private:
  MT_Server mT_Server_;
  int argc_ {};
  // argv_ and orb_manager_ptr_ are borrowed until svc() completes.
  ACE_TCHAR **argv_;
  TAO_ORB_Manager *orb_manager_ptr_ {};
};

#endif /* MT_CLIENT_TEST_MT_SERVER_H */
